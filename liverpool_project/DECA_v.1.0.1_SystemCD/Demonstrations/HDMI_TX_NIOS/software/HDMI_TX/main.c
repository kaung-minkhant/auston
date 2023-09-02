/*
 * DECA_hdmi local test code
 *
 */

#include <stdio.h>

#include "common.h"
#include "i2c.h"
#include "system.h"

#include "encoder.h"
#include "adv7513.h"
#include "edid.h"

#define ADV7513_SALVE_ADDR7		0x39										// 0x39)PD-low 0x3D)PD-high
#define ADV7513_SALVE_ADDR8		((alt_u8)(ADV7513_SALVE_ADDR7) << 1)		// 0x72)PD-low 0x7A)PD-high

#define ADV7513_EDID_ADDR8		0x7e

static int bin_dump(const alt_u8 *buf, int len)
{
	int r, c;
	const alt_u8 *p = buf;

	int rows = len / 16;
	int cols = len % 16;

	if (len <= 0)
		return 0;

	for (r = 0; r < rows; ++r, p += 16) {
		printf("%04x | %02x %02x %02x %02x %02x %02x %02x %02x - ",
				r * 16, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

		printf("%02x %02x %02x %02x %02x %02x %02x %02x\n",
				p[8], p[9], p[10], p[11], p[12], p[13], p[14], p[15]);
	}

	if (cols > 0) {
		printf("%04x | ", (rows - 1) * 16);
		for (c = 0; c < cols; ++c, ++p) {
			if (c == 8) {
				printf("- %02x ", *p);
			} else {
				printf("%02x ", *p);
			}
		}
		printf("\n");
	}

	return len;
}

#define HDMI_TX_INT_MASK			0x01

// WARNING: for simplicity, we use potentially cpu-blocking codes in this ISR ...
//  - use printf() (normally you should not)
//  - cope with hot-plug tasks (usually it should be deferred to application level rather than in interrupt level)
static void adv7513_int_handler(void *context)
{
	struct hdmi_encoder *ec = (struct hdmi_encoder *)context;
	alt_u32 irq_status;
	alt_u8 regv;
	struct adv7513_int_mask int_mask;
	struct adv7513_int_status int_status;
	int r;

	// get system irq status
	irq_status = IORD_ALTERA_AVALON_PIO_EDGE_CAP(HDMI_TX_INT_BASE) & HDMI_TX_INT_MASK;
	//if (irq_status == 0)
	//	return;

	// clear system irq flag
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(HDMI_TX_INT_BASE, 0x00);

	// reload adv7513 int mask value
	adv7513_long_to_int_mask(&int_mask, ec->int_mask);

	// get adv7513 int status
	r = reg_read(ec, 0x93, &regv);
	if (r != 0)
		return;
	int_status.reg93 = regv;

	r = reg_read(ec, 0x96, &regv);
	if (r != 0)
		return;
	int_status.reg96 = regv;

	r = reg_read(ec, 0x97, &regv);
	if (r != 0)
		return;
	int_status.reg97 = regv;

	// disable adv7513 int
	reg_write(ec, 0x92, 0);
	reg_write(ec, 0x94, 0);
	reg_write(ec, 0x95, 0);

	// clear adv7513 int status
	if (int_status.reg93 != 0) {
		reg_write(ec, 0x93, 0xff);
	}

	if (int_status.reg96 != 0) {
		reg_write(ec, 0x96, 0xff);
	}

	if (int_status.reg97 != 0) {
		reg_write(ec, 0x97, 0xff);
	}

	// -------------------------------------------------------
	// handle adv7513 interrupt events
	// WARNING: this should be an application level task ...
	//
	if (int_status.reg96_bits.monitor_sense || int_status.reg96_bits.hpd) {

		alt_u8 edid[256];
		int dvi_mode = 0;
		int pwr_state = 0;
		int pwron_retry_count = 200;
		int m_sense_retry_count = 30;

		while (m_sense_retry_count-- > 0) {
			r = reg_read(ec, ADV7513_REG_STATUS, &regv);
			if (r != 0)
				goto __unplugged_iret;

			if ((regv & ((ADV7513_STATUS_HPD) | (ADV7513_STATUS_MONITOR_SENSE))) == ((ADV7513_STATUS_HPD) | (ADV7513_STATUS_MONITOR_SENSE)))
				break;
		}

		if ((regv & ((ADV7513_STATUS_HPD) | (ADV7513_STATUS_MONITOR_SENSE))) != ((ADV7513_STATUS_HPD) | (ADV7513_STATUS_MONITOR_SENSE)))
			goto __unplugged_iret;

		// now we have both HPD and monitor-sense signal asserted

		// power up the hdmi-encoder ...
		r = adv7513_do_hpd_powerup(ec);
		if (r != 0)
			goto __unplugged_iret;

		// readback edid data
		r = adv7513_edid_read(ec, edid, 0);
		if (r != 0)
			dvi_mode = 1;

		if (edid[0x7e] == 0)
			dvi_mode = 1;

		// configure the hdmi-encoder ...
		while (pwron_retry_count > 0) {
			r = adv7513_chip_init(ec, dvi_mode);
			if (r != 0)
				goto __unplugged_iret;

			r = adv7513_power_state(ec, &pwr_state);
			if (r != 0)
				goto __unplugged_iret;

			if (pwr_state != 0)
				break;
			--pwron_retry_count;
		}

		r = adv7513_power_state(ec, &pwr_state);
		if (r != 0)
			goto __unplugged_iret;

		if (pwr_state == 0)
			goto __unplugged_iret;

		goto __plugged_iret;
	}

__unplugged_iret:

	// enable interrupts
	reg_write(ec, 0x92, int_mask.reg92);
	reg_write(ec, 0x94, int_mask.reg94);
	reg_write(ec, 0x95, int_mask.reg95);

	return;

__plugged_iret:

	// enable interrupts
	reg_write(ec, 0x92, int_mask.reg92);
	reg_write(ec, 0x94, int_mask.reg94 & 0xbf);  // disable sending monitor-sense interrupt since it is already connected
	reg_write(ec, 0x95, int_mask.reg95);

	return;
}

int adv7513_int_setup(struct hdmi_encoder *ec, const struct adv7513_int_mask *mask, alt_isr_func isr)
{
	int r = 0;

	//printf("adv7513_int_setup()\n");

	ec->int_mask = adv7513_int_mask_to_long(mask);

	if (r != 0)
		return -1;

	// disable HDMI_TX_INT
	alt_ic_irq_disable(HDMI_TX_INT_IRQ_INTERRUPT_CONTROLLER_ID, HDMI_TX_INT_IRQ);

	r = reg_write(ec, 0x92, 0);
	if (r != 0)
		return -1;

	r = reg_write(ec, 0x94, 0);
	if (r != 0)
		return -1;

	r = reg_write(ec, 0x95, 0);
	if (r != 0)
		return -1;

	// clear int flags
	IOWR_ALTERA_AVALON_PIO_EDGE_CAP(HDMI_TX_INT_BASE, 0x00);

	r = reg_write(ec, 0x93, 0xff);
	if (r != 0)
		return -1;

	r = reg_write(ec, 0x96, 0xff);
	if (r != 0)
		return -1;

	r = reg_write(ec, 0x97, 0xff);
	if (r != 0)
		return -1;

	// enable interrupt
	if (ec->int_mask > 0) {
		// register isr
		if (isr) {
			// register interrupt handler
			r = alt_ic_isr_register(HDMI_TX_INT_IRQ_INTERRUPT_CONTROLLER_ID, HDMI_TX_INT_IRQ, isr, ec, NULL);
			if (r != 0)
				return -1;
		}

		// enable interrupts
		r = reg_write(ec, 0x92, mask->reg92);
		if (r != 0)
			return -1;

		r = reg_write(ec, 0x94, mask->reg94);
		if (r != 0)
			return -1;

		r = reg_write(ec, 0x95, mask->reg95);
		if (r != 0)
			return -1;

		IOWR_ALTERA_AVALON_PIO_IRQ_MASK(HDMI_TX_INT_BASE, HDMI_TX_INT_MASK); // 0x01: enable bit0

		alt_ic_irq_enable(HDMI_TX_INT_IRQ_INTERRUPT_CONTROLLER_ID, HDMI_TX_INT_IRQ);
	}

	return 0;
}

static void cmd_help()
{
	printf("  d         perform register-space dump of the ADV7513 transmitter\n");
	printf("  e         display EDID raw data of currently connected monitor\n");
	printf("  e p       display EDID raw data of currently connected monitor\n");
	printf("            and decode it in the human readable format\n");
	printf("  m         display current VIC and mode info (not exactly accurate)\n");
	printf("  o         power off the HDMI transmitter\n");
	printf("  i         power up the HDMI transmitter and initialize it in HDMI mode\n");
	printf("  v         power up the HDMI transmitter and initialize it in DVI mode\n");
	printf("  r hh      readback HDMI transmitter register at address hh (2-digit hex)\n");
	printf("  w hh vv   write HDMI transmitter register value vv (2-digit hex) to address hh\n");
	printf("            (note that hh should be exactly a 2-digit hex number)\n");
	printf("  a         audio on \n");
	printf("  b         audio off \n");
}

struct hdmi_encoder encoder = {
	.slave_addr = ADV7513_SALVE_ADDR8,
	.edid_addr = ADV7513_EDID_ADDR8,
	.hpd = 0,
	.chip_id = {0},
	.chip_rev = {0},
};

alt_u8 edid[256];

int main(int argc, const char *argv[])
{
	int r = 0, rr = 0;

	struct adv7513_int_mask ie_mask;

	memset(&ie_mask, 0, sizeof(ie_mask));

	printf("\n===== DECA adv7513 hdmi transmitter demo =====\n");

	r = adv7513_chip_identify(&encoder);
	if (r != 0) {
		printf("[err] failed to do chip-identification! (%d)\n", r);
		rr = -1;
		goto _safe_exit;
	}

	if ((encoder.chip_id[0] == 0x11) && (encoder.chip_id[1] == 0x75)) {
		printf("encoder chip : ADV%02x%02x rev: 0x%02x\n", encoder.chip_id[1], encoder.chip_id[0], encoder.chip_rev[0]);
	} else {
		printf("encoder chip : unknown (id: 0x%02x 0x%02x rev: 0x%02x)\n", encoder.chip_id[0], encoder.chip_id[1], encoder.chip_rev[0]);
	}

	ie_mask.reg94_bits.hpd = 1;
	ie_mask.reg94_bits.monitor_sense = 1;

	r = adv7513_int_setup(&encoder, &ie_mask, adv7513_int_handler);
	if (r != 0) {
		printf("[err] failed to do HDMI_TX_INT interrupt setup! (%d)\n", r);
		rr = -1;
		goto _safe_exit;
	} else {
		printf("success to setup HDMI_TX_INT interrupt handler.\n");
	}


	adv7513_kick_up(&encoder);

	printf("\noperation hints ...\n");
	printf(" - please wait for monitor sync ...\n");
	printf("   the pattern will be auto sent out to your monitor.\n");
	printf(" - type \"h\" in the command line prompt to see available commands.\n");

	printf("\na breif summary of commands for quick start up ...\n");
	printf("  d         perform register-space dump of the ADV7513 transmitter\n");
	printf("  e         display EDID raw data of currently connected monitor\n");
	printf("  e p       display EDID raw data of currently connected monitor\n");
	printf("            and decode it in the human readable format\n");
	printf("  m         display current VIC and mode info (not exactly accurate)\n");
	printf("  o         power off the HDMI transmitter\n");
	printf("  i         power up the HDMI transmitter and initialize it in HDMI mode\n");
	printf("  a         audio on \n");
	printf("  b         audio off \n");

	// interactive command line mode -- used in local develop-testing
	while (1) {

		char cmd[80] = {0};

		printf("\ncommand (h for help) > ");

		fgets(cmd, 256, stdin);

		if (cmd[0] == 'd') {
			alt_u8 regs[256];

			r = adv7513_regmap_dump(&encoder, regs, 0);
			if (r != 0) {
				printf("[err] failed to read HDMI encoder regs! (%d)\n", r);
			}

			printf("\n\n");
			bin_dump(regs, 256);
		}

		if (cmd[0] == 'e') {
			r = adv7513_edid_read(&encoder, edid, 0);
			if (r != 0) {
				printf("[err] failed to read edid data! (%d)\n", r);
			}

			printf("\n\n");
			bin_dump(edid, 256);

			printf("\n");
			if (cmd[2] == 'p') {
				struct base_edid edid_info;

				r = edid_decode_v14(&edid_info, edid);
				if (r != 0) {
					printf("[err] failed to decode edid data! (%d)\n", r);
				} else {
					edid_print_v14(&edid_info);
				}
			}
		}

		if (cmd[0] == 'i') {
			printf("\n\n");

			r = adv7513_do_hpd_powerup(&encoder);
			if (r != 0) {
				printf("[err] failed to do HDMI transmitter power up! (%d)", r);
			} else {
				printf("[hpd] done HDMI transmitter power up.\n");
			}

			r = adv7513_chip_init(&encoder, 0);
			if (r != 0) {
				printf("[err] failed to do HDMI transmitter initialization! (%d)\n", r);
			} else {
				printf("[hpd] done HDMI transmitter initialization.\n");
			}
		}

		if (cmd[0] == 'v') {
			printf("\n\n");

			r = adv7513_do_hpd_powerup(&encoder);
			if (r != 0) {
				printf("[err] failed to do HDMI transmitter power up! (%d)\n", r);
			} else {
				printf("[hpd] done HDMI transmitter power up.\n");
			}

			r = adv7513_chip_init(&encoder, 1);
			if (r != 0) {
				printf("[err] failed to do HDMI transmitter initialization! (%d)\n", r);
			} else {
				printf("[hpd] done HDMI transmitter initialization.\n");
			}
		}

		if (cmd[0] == 'm') {
			int vic;
			const char *mode = 0;

			r = adv7513_mode_state(&encoder, &vic, &mode);
			if (r != 0) {
				printf("[err] failed to read HDMI transmitter regs! (%d)\n", r);
			}

			printf("\n\ncurrent mode : VIC)%d %s\n", vic, mode);
		}

		if (cmd[0] == 'r') {
			alt_u8 regv = 0;
			alt_u8 reg = 0;

			reg = (alt_u8)strtoul(&cmd[2], 0, 16);

			printf("\n\n");
			if (reg_read(&encoder, reg, &regv) == 0) {
				printf("[%02x] %02x\n", reg, regv);
			} else {
				printf("[err] i2c error!\n");
			}
		}

		if (cmd[0] == 'w') {
			alt_u8 regv = 0;
			alt_u8 reg = 0;
			char arg1[8] = {0};
			char arg2[8] = {0};

			strncpy(arg1, &cmd[2], 2);
			strncpy(arg2, &cmd[5], 2);

			reg = (alt_u8)strtoul(arg1, 0, 16);
			regv = (alt_u8)strtoul(arg2, 0, 16);

			printf("\n\n");
			if (reg_write(&encoder, reg, regv) == 0) {
				printf("[%02x] <- %02x (done)\n", reg, regv);
			} else {
				printf("[%02x] <- %02x (i2c error)\n", reg, regv);
			}
		}

		if (cmd[0] == 'o') {
			printf("\n\n");

			r = adv7513_do_hpd_powerdown(&encoder);
			if (r != 0) {
				printf("[err] failed to do HDMI transmitter power off! (%d)\n", r);
			} else {
				printf("[hpd] done HDMI transmitter power off setting.\n");
			}

			//usleep(100000);
		}
		if (cmd[0] == 'a') {
					printf("\n\n");

					r = adv7513_audio_on(&encoder);
					if (r != 0) {
						printf("[err] failed to do HDMI transmitter power off! (%d)\n", r);
					} else {
						printf("[hpd] done HDMI transmitter power on setting.\n");
					}

					//usleep(100000);
				}
		if (cmd[0] == 'b') {
					printf("\n\n");

					r = adv7513_audio_off(&encoder);
					if (r != 0) {
						printf("[err] failed to do HDMI transmitter power off! (%d)\n", r);
					} else {
						printf("[hpd] done HDMI transmitter power off setting.\n");
					}

					//usleep(100000);
				}
		if (cmd[0] == 'h') {
			printf("\n\n");
			cmd_help();
		}
	}

_safe_exit:

	printf("\nprogram exit!\n");

	return rr;
}
