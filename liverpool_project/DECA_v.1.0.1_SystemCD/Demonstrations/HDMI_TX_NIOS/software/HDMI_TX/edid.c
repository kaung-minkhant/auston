/*
 * edid.c
 *
 *  Created on: 2013/6/24
 *      Author: User
 */

#include "common.h"
#include "i2c.h"
#include "system.h"
#include "encoder.h"
#include "edid.h"

// ---------

#ifndef u8
#define u8 unsigned char
#endif

#ifndef u16
#define u16 unsigned short
#endif

#ifndef u32
#define u32 unsigned int
#endif

// ---------

#if 0
static float edid_bin10_to_frac(u16 bin)
{
    float v = 0.0;

    if (bin & 0x200)
        v += 0.5f;
    if (bin & 0x100)
        v += 0.25f;
    if (bin & 0x080)
        v += 0.125f;
    if (bin & 0x040)
        v += 0.0625f;
    if (bin & 0x020)
        v += 0.03125f;
    if (bin & 0x010)
        v += 0.01563f;
    if (bin & 0x008)
        v += 0.00781f;
    if (bin & 0x004)
        v += 0.00391f;
    if (bin & 0x002)
        v += 0.001953125f;
    if (bin & 0x001)
        v += 0.0009765625f;

    return v;
}
#endif

int edid_decode_cea(struct base_edid *edid, const unsigned char *edid_bin, int ext_idx)
{
	u8 *ext_bin = (u8 *)&edid_bin[128 * (ext_idx + 1)];
	u8 *c, *hv, *dtd = 0;
	int i;

	struct ext_data *ext_data = &edid->ext_data[ext_idx];
	struct edid_cea *cea = &ext_data->cea_ext;

	c = &ext_bin[1];

	// version
	if (*c != 0x03)
		return -1;	// todo: support v1 / v2

	ext_data->ext_tag = EDID_EXT_TAG_CEA;
	cea->rev = *c;

	// obtain offset to detail timing descriptor
	c = &ext_bin[2];
	if (*c > 4)
		dtd = &ext_bin[*c];

	cea->off_dtd = c[0];
	cea->nr_dtd = c[1] & 0x0f;

	cea->flags.underscan = ((c[1] & 0x80) > 0) ? 1 : 0;
	cea->flags.audio = ((c[1] & 0x40) > 0) ? 1 : 0;
	cea->flags.ycbcr444 = ((c[1] & 0x20) > 0) ? 1 : 0;
	cea->flags.ycbcr422 = ((c[1] & 0x10) > 0) ? 1 : 0;

	// data block collection
	c = &ext_bin[4];

	cea->nr_dbcs = 0;
	while (c < dtd) {
		switch (*c >> 5) {
		case CEA_DB_AUDIO:
			cea->dbc_tags[cea->nr_dbcs++] = CEA_DB_AUDIO;
			cea->db_audio.nr_sad = (int)(*c & 0x1f) / 3;
			for (i = 0; i < cea->db_audio.nr_sad; ++i) {
				u8 *sad_bin = &c[1+3*i];
				cea->db_audio.sad[i].format = (int)((*sad_bin >> 3) & 0xf);
				cea->db_audio.sad[i].channels = (int)(*sad_bin & 0x7) + 1;
				cea->db_audio.sad[i].freq = (int)(sad_bin[1] & 0x7f);

				if (cea->db_audio.sad[i].format == CEA_SAD_FMT_LPCM) {
					cea->db_audio.sad[i].resolution = (int)(sad_bin[2] & 0x07);
				} else if (cea->db_audio.sad[i].format != 0) {
					if (cea->db_audio.sad[i].format <= CEA_SAD_FMT_ATRAC) {
						cea->db_audio.sad[i].bitrate = (int)sad_bin[2] * 8;
					}
				}
			}
			c += (*c & 0x1f) + 1;
			break;

		case CEA_DB_VIDEO:
			cea->dbc_tags[cea->nr_dbcs++] = CEA_DB_VIDEO;
			cea->db_video.nr_svd = (int)(*c & 0x1f);
			for (i = 0; i < cea->db_video.nr_svd; ++i) {
				cea->db_video.svd[i].native = c[1 + i] & 0x80 ? 1 : 0;
				cea->db_video.svd[i].vic = (int)(c[1 + i] & 0x7f);
			}
			c += (*c & 0x1f) + 1;
			break;

		case CEA_DB_VENDOR:
			cea->dbc_tags[cea->nr_dbcs++] = CEA_DB_VENDOR;

			if ((c[1] != 0x03) || (c[2] != 0x0c) || (c[3] != 0x00)) {
				cea->db_vsdb_hdmi.ieee_reg_id = 0;
				c += (*c & 0x1f) + 1;
				break;
			}

			cea->db_vsdb_hdmi.ext_len = (int)(*c & 0x1f);
			cea->db_vsdb_hdmi.ieee_reg_id = (c[3] << 16) | (c[2] << 8) | c[1];

			cea->db_vsdb_hdmi.addr_a = (int)(c[4] >> 4);
			cea->db_vsdb_hdmi.addr_b = (int)(c[4] & 0x0f);
			cea->db_vsdb_hdmi.addr_c = (int)(c[5] >> 4);
			cea->db_vsdb_hdmi.addr_d = (int)(c[5] & 0x0f);

			if ((*c & 0x1f) >= 6) {
				cea->db_vsdb_hdmi.ai = (c[6] & 0x80) ? 1 : 0;
				cea->db_vsdb_hdmi.bpp48 = (c[6] & 0x40) ? 1 : 0;
				cea->db_vsdb_hdmi.bpp36 = (c[6] & 0x20) ? 1 : 0;
				cea->db_vsdb_hdmi.bpp30 = (c[6] & 0x10) ? 1 : 0;
				cea->db_vsdb_hdmi.ycbcr444 = (c[6] & 0x08) ? 1 : 0;
				cea->db_vsdb_hdmi.dvi_dual = (c[6] & 0x01) ? 1 : 0;
			} else {
				cea->db_vsdb_hdmi.ai = 0;
				cea->db_vsdb_hdmi.bpp48 = 0;
				cea->db_vsdb_hdmi.bpp36 = 0;
				cea->db_vsdb_hdmi.bpp30 = 0;
				cea->db_vsdb_hdmi.ycbcr444 = 0;
				cea->db_vsdb_hdmi.dvi_dual = 0;
			}

			if ((*c & 0x1f) >= 7) {
				cea->db_vsdb_hdmi.max_tmds_clk = c[7] * 5;
			} else {
				cea->db_vsdb_hdmi.max_tmds_clk = 0;
			}

			cea->db_vsdb_hdmi.p_latency = 0;
			cea->db_vsdb_hdmi.i_latency = 0;
			cea->db_vsdb_hdmi.hdmi_video = 0;

			cea->db_vsdb_hdmi.vid_latency_p = 0;
			cea->db_vsdb_hdmi.aud_latency_p = 0;
			cea->db_vsdb_hdmi.vid_latency_i = 0;
			cea->db_vsdb_hdmi.aud_latency_i = 0;

			if ((*c & 0x1f) >= 8) {
				u8 c8 = c[8];
				hv = &c[9];
				if (c8 & 0x80) {
					cea->db_vsdb_hdmi.p_latency = 1;
					cea->db_vsdb_hdmi.vid_latency_p = (hv[0] - 1) * 2;  // (ms)
					cea->db_vsdb_hdmi.aud_latency_p = (hv[1] - 1) * 2;  // (ms)
					hv += 2;
				}
				if (c8 & 0x40) {
					cea->db_vsdb_hdmi.i_latency = 1;
					cea->db_vsdb_hdmi.vid_latency_i = (hv[0] - 1) * 2;  // (ms)
					cea->db_vsdb_hdmi.aud_latency_i = (hv[1] - 1) * 2;  // (ms)
					hv += 2;
				}
				if (c8 & 0x20) {
					cea->db_vsdb_hdmi.hdmi_video = 1;
				}
			}

			if (cea->db_vsdb_hdmi.hdmi_video > 0) {
				cea->db_vsdb_hdmi.present_3d = hv[0] & 0x80 ? 1 : 0;
				cea->db_vsdb_hdmi.multi_3d = (hv[0] >> 5) & 0x03;
				cea->db_vsdb_hdmi.image_size = (hv[0] >> 3) & 0x03;
				cea->db_vsdb_hdmi.hdmi_xx_len = hv[1] >> 5;
				cea->db_vsdb_hdmi.hdmi_3d_len = hv[1] & 0x1f;
			} else {
				cea->db_vsdb_hdmi.present_3d = 0;
				cea->db_vsdb_hdmi.multi_3d = 0;
				cea->db_vsdb_hdmi.image_size = 0;
				cea->db_vsdb_hdmi.hdmi_xx_len = 0;
				cea->db_vsdb_hdmi.hdmi_3d_len = 0;
			}

			if (cea->db_vsdb_hdmi.hdmi_3d_len > 0) {
				u8 *d = &hv[2 + cea->db_vsdb_hdmi.hdmi_xx_len];  // data pointer starting from 3D_Structure_ALL_15...8
				u8 *vic = 0;	// data pointer starting from 2D_VIC_Order_1 + 3D_Structure_1
				int vic_l = 0;  // remaining 3D size starting from 2D_VIC_Order_1 + 3D_Structure_1

				if (cea->db_vsdb_hdmi.multi_3d == 2) {
					cea->db_vsdb_hdmi.struct_all_3d = (d[0] << 8) | d[1];
					cea->db_vsdb_hdmi.mask_3d = (d[2] << 8) | d[3];
					vic = &d[4];
					vic_l = cea->db_vsdb_hdmi.hdmi_3d_len - 4;
				} else if (cea->db_vsdb_hdmi.multi_3d == 1) {
					cea->db_vsdb_hdmi.struct_all_3d = (d[0] << 8) | d[1];
					cea->db_vsdb_hdmi.mask_3d = 0;
					vic = &d[2];
					vic_l = cea->db_vsdb_hdmi.hdmi_3d_len - 2;
				} else {
					cea->db_vsdb_hdmi.struct_all_3d = 0;
					cea->db_vsdb_hdmi.mask_3d = 0;
				}

				cea->db_vsdb_hdmi.nr_vic_3d = 0;
				while (vic_l > 0) {
					cea->db_vsdb_hdmi.vic_3d[cea->db_vsdb_hdmi.nr_vic_3d].vic_2d = vic[0] >> 4;
					cea->db_vsdb_hdmi.vic_3d[cea->db_vsdb_hdmi.nr_vic_3d].struct_3d = vic[0] & 0x0f;
					if (cea->db_vsdb_hdmi.vic_3d[cea->db_vsdb_hdmi.nr_vic_3d].struct_3d >= 0x08) {
						cea->db_vsdb_hdmi.vic_3d[cea->db_vsdb_hdmi.nr_vic_3d].detail_3d = vic[1] >> 4;
						vic += 2;
						vic_l -= 2;
					} else {
						cea->db_vsdb_hdmi.vic_3d[cea->db_vsdb_hdmi.nr_vic_3d].detail_3d = 0;
						++vic;
						--vic_l;
					}
					++cea->db_vsdb_hdmi.nr_vic_3d;
				}
			} else {
				cea->db_vsdb_hdmi.struct_all_3d = 0;
				cea->db_vsdb_hdmi.mask_3d = 0;
			}

			c += (*c & 0x1f) + 1;
			break;

		case CEA_DB_SPEAKER:
			cea->dbc_tags[cea->nr_dbcs++] = CEA_DB_SPEAKER;
			cea->db_speaker.designation = (int)(c[1] & 0x7f);
			c += (*c & 0x1f) + 1;
			break;

		case CEA_DB_DTC:
			cea->dbc_tags[cea->nr_dbcs++] = CEA_DB_DTC;
			c += (*c & 0x1f) + 1;
			break;

		case CEA_DB_EXTENDED:
			switch (c[1]) {
			case CEA_DBE_VIDEO_CAP:
				cea->dbe_tags[cea->nr_dbcs] = CEA_DBE_VIDEO_CAP;
				cea->db_vcdb.quant_range_sel = (c[2] & 0x40) ? 1 : 0;
				cea->db_vcdb.pt_scan_behavior = (int)((c[2] >> 4) & 0x03);
				cea->db_vcdb.it_scan_behavior = (int)((c[2] >> 2) & 0x03);
				cea->db_vcdb.ce_scan_behavior = (int)(c[2] & 0x03);
				break;
			case CEA_DBE_VIDEO_VENDOR:
				cea->dbe_tags[cea->nr_dbcs] = CEA_DBE_VIDEO_VENDOR;
				break;
			case CEA_DBE_VIDEO_DDI:
				cea->dbe_tags[cea->nr_dbcs] = CEA_DBE_VIDEO_DDI;
				break;
			case CEA_DBE_VIDEO_VESA:
				cea->dbe_tags[cea->nr_dbcs] = CEA_DBE_VIDEO_VESA;
				break;
			case CEA_DBE_VIDEO_HDMI:
				cea->dbe_tags[cea->nr_dbcs] = CEA_DBE_VIDEO_HDMI;
				break;
			case CEA_DBE_COLORIMETRY:
				cea->dbe_tags[cea->nr_dbcs] = CEA_DBE_COLORIMETRY;
				cea->db_colorimetry.xvYCC = (int)(c[2] & 0x03);
				cea->db_colorimetry.metadata = (int)(c[3] & 0x07);
				break;
			case CEA_DBE_AUDIO_MISC:
				cea->dbe_tags[cea->nr_dbcs] = CEA_DBE_AUDIO_MISC;
				break;
			case CEA_DBE_AUDIO_VENDOR:
				cea->dbe_tags[cea->nr_dbcs] = CEA_DBE_AUDIO_VENDOR;
				break;
			case CEA_DBE_AUDIO_HDMI:
				cea->dbe_tags[cea->nr_dbcs] = CEA_DBE_AUDIO_HDMI;
				break;
			}

			cea->dbc_tags[cea->nr_dbcs++] = CEA_DB_EXTENDED;

			c += (*c & 0x1f) + 1;
			break;

		default:
			c = dtd;
			break;
		}
	}

	// detail timing descriptor
	if (dtd == 0)
		return 0;

	for (i = 0; i < (int)cea->nr_dtd; ++i) {
		c = &dtd[18 * i];

        cea->detailed_timing[i].pixel_clock = (((int)c[1] << 8) | (int)c[0]) * 10000;
        cea->detailed_timing[i].width = (((int)c[4] << 4) & 0xf00) | (int)c[2];
        cea->detailed_timing[i].height = (((int)c[7] << 4) & 0xf00) | (int)c[5];
        cea->detailed_timing[i].h_blank = (((int)c[4] << 8) & 0xf00) | (int)c[3];
        cea->detailed_timing[i].v_blank = (((int)c[7] << 8) & 0xf00) | (int)c[6];
        cea->detailed_timing[i].h_fp = (((int)c[11] << 2) & 0x300) | (int)c[8];
        cea->detailed_timing[i].v_fp = (((int)c[11] << 2) & 0x30) | ((int)c[10] >> 4);
        cea->detailed_timing[i].h_sync = (((int)c[11] << 4) & 0x300) | (int)c[9];
        cea->detailed_timing[i].v_sync = (((int)c[11] << 4) & 0x30) | ((int)c[10] & 0x0f);
        cea->detailed_timing[i].h_size = (((int)c[14] << 4) & 0xf00) | (int)c[12];
        cea->detailed_timing[i].v_size = (((int)c[14] << 8) & 0xf00) | (int)c[13];
        cea->detailed_timing[i].border_lr = (int)c[15];
        cea->detailed_timing[i].border_tb = (int)c[16];

        cea->detailed_timing[i].interlaced = (c[17] & 0x80) ? 1 : 0;

        cea->detailed_timing[i].stereo = STEREO_NONE;
		if ((c[17] & 0x61) == 0x20)
			cea->detailed_timing[i].stereo |= STEREO_FIELD_SEQUENTIAL_RIGHT_SYNC;
		if ((c[17] & 0x61) == 0x40)
			cea->detailed_timing[i].stereo |= STEREO_FIELD_SEQUENTIAL_LEFT_SYNC;
		if ((c[17] & 0x61) == 0x21)
			cea->detailed_timing[i].stereo |= STEREO_2WAY_INTERLEAVE_RIGHT_EVEN;
		if ((c[17] & 0x61) == 0x41)
			cea->detailed_timing[i].stereo |= STEREO_2WAY_INTERLEAVE_LEFT_EVEN;
		if ((c[17] & 0x61) == 0x60)
			cea->detailed_timing[i].stereo |= STEREO_4WAY_INTERLEAVE;
		if ((c[17] & 0x61) == 0x61)
			cea->detailed_timing[i].stereo |= STEREO_SIDE_BY_SIDE_INTERLEAVE;

		cea->detailed_timing[i].sync = 0;
        if (c[17] & 0x10) {
            if (c[17] & 0x08) {
            	cea->detailed_timing[i].sync |= D_SEPERATE_SYNC;
            	if (c[17] & 0x04)
                	cea->detailed_timing[i].sync |= D_SEPERATE_VSYNC_P;
            	else
            		cea->detailed_timing[i].sync |= D_SEPERATE_VSYNC_N;
            } else {
            	cea->detailed_timing[i].sync |= D_COMPOSITE_SYNC;
            	if (c[17] & 0x04)
                	cea->detailed_timing[i].sync |= D_COMPOSITE_SERRATION;
            }
        } else {
        	if (c[17] & 0x80) {
        		cea->detailed_timing[i].sync |= A_BIPOLAR_COMPOSITE_SYNC;
        		if (c[17] & 0x40) {
            		cea->detailed_timing[i].sync |= A_BIPOLAR_SERRATION;
            		if (c[17] & 0x20)
                		cea->detailed_timing[i].sync |= A_BIPOLAR_SERRATION_SYNC_RGB;
            		else
                		cea->detailed_timing[i].sync |= A_BIPOLAR_SERRATION_SYNC_G;
        		}
        	} else {
        		cea->detailed_timing[i].sync |= A_COMPOSITE_SYNC;
        	}
        }
	}

	return 0;
}

void edid_print_cea(struct base_edid *edid, int ext_idx)
{
	u8 *c, *dtd = 0;
	int i, t;

	struct ext_data *ext_data = &edid->ext_data[ext_idx];
	struct edid_cea *cea = &ext_data->cea_ext;

	printf("\n[%02d] extension block -- CEA-Extension Revision-%02d ...\n\n", ext_idx, cea->rev);

	printf("  number of DBCs (data block collections)      : %d\n", cea->nr_dbcs);
	printf("  number of DTDs (detailed timing descriptors) : %d\n", cea->nr_dtd);
	printf("  offset to DTD                                : %d\n", cea->off_dtd);
	printf("  monitor underscans IT formats                : %s\n", cea->flags.underscan ? "yes" : "no");
	printf("  supports audio                               : %s\n", cea->flags.audio ? "yes" : "no");
	printf("  supports YCbCr 4:4:4 in addition to RGB      : %s\n", cea->flags.ycbcr444 ? "yes" : "no");
	printf("  supports YCbCr 4:2:2 in addition to RGB      : %s\n", cea->flags.ycbcr422 ? "yes" : "no");

	printf("\n  data block collections\n  ---------------------------------\n");

	// data block collection
	for (t = 0; t < cea->nr_dbcs; ++t) {

		switch (cea->dbc_tags[t]) {
		case CEA_DB_AUDIO:
			printf("\n  [%02d] audio data block ...\n", t);
			printf("       number of SADs (short audio descriptor) : %d\n", cea->db_audio.nr_sad);

			for (i = 0; i < cea->db_audio.nr_sad; ++i) {

				const static char *sad_format[] = {
					"unknown (0)",
					"Linear PCM",
					"AC-3",
					"MPEG1 (Layer 1 & 2)",
					"MP3 (MPEG1 Layer3)",
					"MPEG2",
					"AAC",
					"DTS",
					"ATRAC",
					"One Bit Audio",
					"Dolby Digital +",
					"DTS_HD",
					"MAT (MLP)",
					"DST",
					"WMA Pro",
					"unknown (15)",
				};

				printf("       %02d) format      : %s\n", i, sad_format[cea->db_audio.sad[i].format]);
				printf("           channels    : %d\n", cea->db_audio.sad[i].channels);
				printf("           frequency   : 192K)%d 176.4k)%d 96k)%d 88.2k)%d 48k)%d 44.1k)%d 32k)%d\n",
					cea->db_audio.sad[i].freq & CEA_SAD_SF_192K ? 1 : 0,
					cea->db_audio.sad[i].freq & CEA_SAD_SF_176P4K ? 1 : 0,
					cea->db_audio.sad[i].freq & CEA_SAD_SF_96K ? 1 : 0,
					cea->db_audio.sad[i].freq & CEA_SAD_SF_88P2K ? 1 : 0,
					cea->db_audio.sad[i].freq & CEA_SAD_SF_48K ? 1 : 0,
					cea->db_audio.sad[i].freq & CEA_SAD_SF_44P1K ? 1 : 0,
					cea->db_audio.sad[i].freq & CEA_SAD_SF_32K ? 1 : 0);

				switch (cea->db_audio.sad[i].format) {
				case CEA_SAD_FMT_LPCM:
					printf("           resolution  : 24-bit)%d 20-bit)%d 16-bit)%d\n",
						cea->db_audio.sad[i].resolution & 0x04 ? 1 : 0,
						cea->db_audio.sad[i].resolution & 0x02 ? 1 : 0,
						cea->db_audio.sad[i].resolution & 0x01 ? 1 : 0);
					break;
				case CEA_SAD_FMT_AC3:
				case CEA_SAD_FMT_MPEG1:
				case CEA_SAD_FMT_MP3:
				case CEA_SAD_FMT_MPEG2:
				case CEA_SAD_FMT_AAC:
				case CEA_SAD_FMT_DTS:
				case CEA_SAD_FMT_ATRAC:
					printf("           bitrate max : %d (kHz)\n", cea->db_audio.sad[i].bitrate);
					break;
				//case CEA_SAD_FMT_ONEBIT:
				//case CEA_SAD_FMT_DOLBY_DIGITAL:
				//case CEA_SAD_FMT_DTS_HD:
				//case CEA_SAD_FMT_MAT:
				//case CEA_SAD_FMT_DST:
				//case CEA_SAD_FMT_WMA_PRO:
				//	break;
				}
			}
			break;

		case CEA_DB_VIDEO:
		{
			static const char *vic_table[] = {
				"unknown",									// 0
				"640x480p       @59.94/60 Hz (4:3)",
				"720x480p       @59.94/60 Hz (4:3)",
				"720x480p       @59.94/60 Hz (16:9)",
				"1280x720p      @59.94/60 Hz (16:9)",
				"1920x1080i     @59.94/60 Hz (16:9)",			// 5
				"720(1440)x480i @59.94/60 Hz (4:3)",
				"720(1440)x480i @59.94/60 Hz (16:9)",
				"720(1440)x240p @59.94/60 Hz (4:3)",
				"720(1440)x240p @59.94/60 Hz (16:9)",
				"2880x480i      @59.94/60 Hz (4:3)",				// 10
				"2880x480i      @59.94/60 Hz (16:9)",
				"2880x240p      @59.94/60 Hz (4:3)",
				"2880x240p      @59.94/60 Hz (16:9)",
				"1440x480p      @59.94/60 Hz (4:3)",
				"1440x480p      @59.94/60 Hz (16:9)",			// 15
				"1920x1080p     @59.94/60 Hz (16:9)",
				"720x576p       @50 Hz (4:3)",
				"720x576p       @50 Hz (16:9)",
				"1280x720p      @50 Hz (16:9)",
				"1920x1080i     @50 Hz (16:9)",					// 20
				"720(1440)x576i @50 Hz (4:3)",
				"720(1440)x576i @50 Hz (16:9)",
				"720(1440)x288p @50 Hz (4:3)",
				"720(1440)x288p @50 Hz (16:9)",
				"2880x576i      @50 Hz (4:3)",					// 25
				"2880x576i      @50 Hz (16:9)",
				"2880x288p      @50 Hz (4:3)",
				"2880x288p      @50 Hz (16:9)",
				"1440x576p      @50 Hz (4:3)",
				"1440x576p      @50 Hz (16:9)",					// 30
				"1920x1080p     @50 Hz (16:9)",
				"1920x1080p     @23.97/24 Hz (16:9)",
				"1920x1080p     @25 Hz (16:9)",
				"1920x1080p     @29.97/30 Hz (16:9)",
				"2880x480p      @59.94/60 Hz (4:3)",				// 35
				"2880x480p      @59.94/60 Hz (16:9)",
				"2880x576p      @50 Hz (4:3)",
				"2880x576p      @50 Hz (16:9)",
				"1920x1080i     @50 Hz (16:9)",
				"1920x1080i     @100 Hz (16:9)",				// 40
				"1280x720p      @100 Hz (16:9)",
				"720x576p       @100 Hz (4:3)",
				"720x576p       @100 Hz (16:9)",
				"720(1440)x576i @100 Hz (4:3)",
				"720(1440)x576i @100 Hz (16:9)",			// 45
				"1920x1080i     @119.88/120 Hz (16:9)",
				"1280x720p      @119.88/120 Hz (16:9)",
				"720x480p       @119.88/120 Hz (4:3)",
				"720x480p       @119.88/120 Hz (16:9)",
				"720(1440)x480i @119.88/120 Hz (4:3)",				// 50
				"720(1440)x480i @119.88/120 Hz (16:9)",
				"720x576p       @200 Hz (4:3)",
				"720x576p       @200 Hz (16:9)",
				"720(1440)x576i @200 Hz (4:3)",
				"720(1440)x576i @200 Hz (16:9)",				// 55
				"720x480p       @239.76/240 Hz (4:3)",
				"720x480p       @239.76/240 Hz (16:9)",
				"720(1440)x480i @239.76/240 Hz (4:3)",
				"720(1440)x480i @239.76/240 Hz (16:9)",
			};

			printf("\n  [%02d] video data block ...\n", t);
			printf("       number of SVDs (short video descriptor) : %d\n", cea->db_video.nr_svd);

			for (i = 0; i < cea->db_video.nr_svd; ++i) {
				if (cea->db_video.svd[i].vic < 60)
					printf("       %02d) VIC-%02d %s %s\n", i, cea->db_video.svd[i].vic, vic_table[cea->db_video.svd[i].vic], cea->db_video.svd[i].native ? "(native)" : " ");
				else
					printf("       %02d) VIC-%02d unknown %s\n", i, cea->db_video.svd[i].vic, cea->db_video.svd[i].native ? "(native)" : " ");
			}
			break;
		}
		case CEA_DB_VENDOR:
			if (cea->db_vsdb_hdmi.ieee_reg_id != 0x000c03) {
				printf("\n  [%02d] VSDB vendor specific data block ...\n", t);
				break;
			}

			printf("\n  [%02d] HDMI VSDB vendor specific data block ...\n", t);
			printf("       source physical address         : %d.%d.%d.%d\n",
				cea->db_vsdb_hdmi.addr_a, cea->db_vsdb_hdmi.addr_b,
				cea->db_vsdb_hdmi.addr_c, cea->db_vsdb_hdmi.addr_d);

			if (cea->db_vsdb_hdmi.ext_len >= 6) {
				printf("       support ACP ISRC1 ISRC2 packets : %s\n", cea->db_vsdb_hdmi.ai ? "yes" : "no");
				printf("       support 48 bpp                  : %s\n", cea->db_vsdb_hdmi.bpp48 ? "yes" : "no");
				printf("       support 36 bpp                  : %s\n", cea->db_vsdb_hdmi.bpp36 ? "yes" : "no");
				printf("       support 30 bpp                  : %s\n", cea->db_vsdb_hdmi.bpp30 ? "yes" : "no");
				printf("       support YCbCr 4:4:4 deep color  : %s\n", cea->db_vsdb_hdmi.ycbcr444 ? "yes" : "no");
				printf("       support DVI dual-link           : %s\n", cea->db_vsdb_hdmi.dvi_dual ? "yes" : "no");
			}

			if (cea->db_vsdb_hdmi.ext_len >= 7) {
				printf("       max TMDS clock                  : %d (MHz)\n", cea->db_vsdb_hdmi.max_tmds_clk);
			}

			if (cea->db_vsdb_hdmi.p_latency != 0) {
				printf("       video latency (progressive)     : %d (ms)\n", cea->db_vsdb_hdmi.vid_latency_p);
				printf("       audio latency (progressive)     : %d (ms)\n", cea->db_vsdb_hdmi.aud_latency_p);
			}

			if (cea->db_vsdb_hdmi.i_latency != 0) {
				printf("       video latency (interlaced)      : %d (ms)\n", cea->db_vsdb_hdmi.vid_latency_i);
				printf("       audio latency (interlaced)      : %d (ms)\n", cea->db_vsdb_hdmi.aud_latency_i);
			}

			printf("       HDMI video fields               : %s\n", cea->db_vsdb_hdmi.hdmi_video ? "yes" : "no");
			printf("       HDMI 3D video fields            : %s\n", cea->db_vsdb_hdmi.present_3d ? "yes" : "no");

			if (cea->db_vsdb_hdmi.hdmi_video != 0) {
				switch (cea->db_vsdb_hdmi.multi_3d) {
				case 0:
					printf("         3D-Multi_present              : 2b00 (no 3D_Structure_ALL_15..0, no 3D_MASK_15..0)\n");
					break;
				case 1:
					printf("         3D-Multi_present              : 2b01 (with 3D_Structure_ALL_15..0, no 3D_MASK_15..0)\n");
					break;
				case 2:
					printf("         3D-Multi_present              : 2b10 (with 3D_Structure_ALL_15..0, with 3D_MASK_15..0)\n");
					break;
				case 3:
					printf("         3D-Multi_present              : 2b11 (no 3D_Structure_ALL_15..0, no 3D_MASK_15..0)\n");
					break;
				}

				if ((cea->db_vsdb_hdmi.multi_3d == 1) || (cea->db_vsdb_hdmi.multi_3d == 2)) {
					if (cea->db_vsdb_hdmi.struct_all_3d == 0) {
						printf("         3D_Structure_ALL              : none\n");
					} else {
						printf("         3D_Structure_ALL              : ");
						if (cea->db_vsdb_hdmi.struct_all_3d & 0x01)
							printf("frame_packing ");
						if (cea->db_vsdb_hdmi.struct_all_3d & 0x40)
							printf("top-and-bottom ");
						if (cea->db_vsdb_hdmi.struct_all_3d & 0x100)
							printf("side-by-side(half h_subsampling)");
						printf("\n");
					}
				}

				if (cea->db_vsdb_hdmi.multi_3d == 2) {
					printf("         3D_MASK                       : 0x%04x\n", cea->db_vsdb_hdmi.mask_3d);
				}

				switch (cea->db_vsdb_hdmi.image_size) {
				case 0:
					printf("         image size extra properties   : none\n");
					break;
				case 1:
					printf("         image size extra properties   : exact aspect ratio only\n");
					break;
				case 2:
					printf("         image size extra properties   : size correct (1cm nearest rounded)\n");
					break;
				case 3:
					printf("         image size extra properties   : size correct (div by 5cm nearest rounded)\n");
					break;
				}

				for (i = 0; i < cea->db_vsdb_hdmi.nr_vic_3d; ++ i) {

					printf("         [%02d] 2D_VIC_Order)%02d ", i, cea->db_vsdb_hdmi.vic_3d[i].vic_2d);

					switch (cea->db_vsdb_hdmi.vic_3d[i].struct_3d) {
					case 0:
						printf("3D_Structure)frame_packing ");
						break;
					case 1:
						printf("3D_Structure)field_alternative ");
						break;
					case 2:
						printf("3D_Structure)line_alternative ");
						break;
					case 3:
						printf("3D_Structure)side-by-side(full) ");
						break;
					case 4:
						printf("3D_Structure)L+depth ");
						break;
					case 5:
						printf("3D_Structure)L+depth+graphics+graphics_depth ");
						break;
					case 6:
						printf("3D_Structure)top-and-bottom ");
						break;
					case 8:
						printf("3D_Structure)side-by-side(half) ");
						break;
					default:
						printf("3D_Structure)unknown ");
						break;
					}

					if (cea->db_vsdb_hdmi.vic_3d[i].struct_3d == 0x08) {
						switch (cea->db_vsdb_hdmi.vic_3d[i].detail_3d) {
						case 0:
							printf("3D_Detail)h_sub_sampling+four_quincunx_matrix");
							break;
						case 1:
							printf("3D_Detail)h_sub_sampling");
							break;
						case 6:
							printf("3D_Detail)all_4_sub_sampling_position");
							break;
						case 7:
							printf("3D_Detail)odd_left+odd_right");
							break;
						case 8:
							printf("3D_Detail)odd_left+even_right");
							break;
						case 9:
							printf("3D_Detail)even_left+odd_right");
							break;
						case 10:
							printf("3D_Detail)even_left+even_right");
							break;
						}
					}

					printf("\n");
				}
			}

			break;

		case CEA_DB_SPEAKER:
			printf("\n  [%02d] speaker allocation data block ...\n", t);

			printf("       RLC/RRC : %s\n", cea->db_speaker.designation & CEA_SPK_RLC_RRC ? "yes" : "no");
			printf("       FLC/FRC : %s\n", cea->db_speaker.designation & CEA_SPK_FLC_FRC ? "yes" : "no");
			printf("       RC      : %s\n", cea->db_speaker.designation & CEA_SPK_RC ? "yes" : "no");
			printf("       RL/RR   : %s\n", cea->db_speaker.designation & CEA_SPK_RL_RR ? "yes" : "no");
			printf("       FC      : %s\n", cea->db_speaker.designation & CEA_SPK_FC ? "yes" : "no");
			printf("       LFE     : %s\n", cea->db_speaker.designation & CEA_SPK_LFE ? "yes" : "no");
			printf("       FL/FR   : %s\n", cea->db_speaker.designation & CEA_SPK_FL_FR ? "yes" : "no");

			break;

		case CEA_DB_DTC:
			printf("\n  [%02d] VESA DTC data block ... (todo)\n", t);
			break;

		case CEA_DB_EXTENDED:
			switch (cea->dbe_tags[t]) {
			case CEA_DBE_VIDEO_CAP:
				printf("\n  [%02d] video capability data block (ext) ...\n", t);

				printf("       quantization range selectable : %s\n", cea->db_vcdb.quant_range_sel ? "yes" : "no");

				switch (cea->db_vcdb.pt_scan_behavior) {
				case 0:
					printf("       PT scan behavior              : none\n");
					break;
				case 1:
					printf("       PT scan behavior              : overscanned always\n");
					break;
				case 2:
					printf("       PT scan behavior              : underscanned always\n");
					break;
				case 3:
					printf("       PT scan behavior              : supports both over/under-scan\n");
					break;
				}

				switch (cea->db_vcdb.it_scan_behavior) {
				case 0:
					printf("       IT scan behavior              : none\n");
					break;
				case 1:
					printf("       IT scan behavior              : overscanned always\n");
					break;
				case 2:
					printf("       IT scan behavior              : underscanned always\n");
					break;
				case 3:
					printf("       IT scan behavior              : supports both over/under-scan\n");
					break;
				}

				switch (cea->db_vcdb.ce_scan_behavior) {
				case 0:
					printf("       CE scan behavior              : none\n");
					break;
				case 1:
					printf("       CE scan behavior              : overscanned always\n");
					break;
				case 2:
					printf("       CE scan behavior              : underscanned always\n");
					break;
				case 3:
					printf("       CE scan behavior              : supports both over/under-scan\n");
					break;
				}
				break;
			case CEA_DBE_VIDEO_VENDOR:
				printf("\n  [%02d] vendor-specific video data block (ext) ... (todo)\n", t);
				break;
			case CEA_DBE_VIDEO_DDI:
				printf("\n  [%02d] VESA video display device information data block (ext) ... (todo)\n", t);
				break;
			case CEA_DBE_VIDEO_VESA:
				printf("\n  [%02d] VESA video data block (ext) ... (todo)\n", t);
				break;
			case CEA_DBE_VIDEO_HDMI:
				printf("\n  [%02d] HDMI video data block (ext) ... (todo)\n", t);
				break;
			case CEA_DBE_COLORIMETRY:
				printf("\n  [%02d] colorimetry data block (ext) ... (todo)\n", t);
				cea->db_colorimetry.xvYCC = (int)(c[2] & 0x03);
				cea->db_colorimetry.metadata = (int)(c[3] & 0x07);
				break;
			case CEA_DBE_AUDIO_MISC:
				printf("\n  [%02d] CEA misc audio fields (ext) ... (todo)\n", t);
				break;
			case CEA_DBE_AUDIO_VENDOR:
				printf("\n  [%02d] vendor-specific audio data block (ext) ... (todo)\n", t);
				break;
			case CEA_DBE_AUDIO_HDMI:
				printf("\n  [%02d] HDMI audio data block (ext) ... (todo)\n", t);
				break;
			}

			break;

		default:
			printf("\n  [%02d] unknown data block (%d).\n", t, cea->dbc_tags[t]);
			break;
		}
	}

	if (cea->nr_dtd != 0)
		printf("\n  detailed timing descriptors\n  ---------------------------------\n");

	// detail timing descriptor
	for (i = 0; i < (int)cea->nr_dtd; ++i) {

		printf("\n  [%d] DTD -- detailed timing descriptor\n", i);

		printf("      pixel clock   : %d (Hz)\n", cea->detailed_timing[i].pixel_clock);
		printf("      h addressible : %d (pixels)\n", cea->detailed_timing[i].width);
		printf("        blank       : %d (pixels)\n", cea->detailed_timing[i].h_blank);
		printf("        fp          : %d (pixels)\n", cea->detailed_timing[i].h_fp);
		printf("        sync        : %d (pixels)\n", cea->detailed_timing[i].h_sync);
		printf("        bp          : %d (pixels)\n", cea->detailed_timing[i].h_blank - cea->detailed_timing[i].h_fp - cea->detailed_timing[i].h_sync);
		printf("        size        : %d (mm)\n", cea->detailed_timing[i].h_size);
		printf("      v addressible : %d (lines)\n", cea->detailed_timing[i].height);
		printf("        blank       : %d (lines)\n", cea->detailed_timing[i].v_blank);
		printf("        fp          : %d (lines)\n", cea->detailed_timing[i].v_fp);
		printf("        sync        : %d (lines)\n", cea->detailed_timing[i].v_sync);
		printf("        bp          : %d (lines)\n", cea->detailed_timing[i].v_blank - cea->detailed_timing[i].v_fp - cea->detailed_timing[i].v_sync);
		printf("        size        : %d (mm)\n", cea->detailed_timing[i].v_size);
		printf("      border (L&R)  : %d (pixels)\n", cea->detailed_timing[i].border_lr);
		printf("      border (T&B)  : %d (lines)\n", cea->detailed_timing[i].border_tb);
		printf("      interlaced    : %d\n", cea->detailed_timing[i].interlaced);

		if (cea->detailed_timing[i].stereo == STEREO_NONE) {
			printf("      stereo        : none\n");
		} else {
    		printf("      stereo        : ");

    		if (cea->detailed_timing[i].stereo & STEREO_FIELD_SEQUENTIAL_RIGHT_SYNC)
        		printf("field_seq_right ");
    		if (cea->detailed_timing[i].stereo & STEREO_FIELD_SEQUENTIAL_LEFT_SYNC)
        		printf("field_seq_left ");
    		if (cea->detailed_timing[i].stereo & STEREO_2WAY_INTERLEAVE_RIGHT_EVEN)
        		printf("2way_interleave_right ");
    		if (cea->detailed_timing[i].stereo & STEREO_2WAY_INTERLEAVE_LEFT_EVEN)
        		printf("2way_interleave_left ");
    		if (cea->detailed_timing[i].stereo & STEREO_4WAY_INTERLEAVE)
        		printf("4way_interleave ");
    		if (cea->detailed_timing[i].stereo & STEREO_SIDE_BY_SIDE_INTERLEAVE)
        		printf("side-by-side ");

    		printf("\n");
		}

		if (cea->detailed_timing[i].sync & A_COMPOSITE_SYNC) {

			printf("      analog sync   : composite");

    		if (cea->detailed_timing[i].sync & A_COMPOSITE_SYNC)
        		printf(" bipolar");

    		if (cea->detailed_timing[i].sync & A_BIPOLAR_SERRATION) {
        		printf(" with_serrations");
    			if (cea->detailed_timing[i].sync & A_BIPOLAR_SERRATION_SYNC_G)
					printf(" syn_on_green");
				if (cea->detailed_timing[i].sync & A_BIPOLAR_SERRATION_SYNC_RGB)
					printf(" syn_on_rgb");
    		} else {
        		printf(" without_serrations");
    		}

			printf("\n");

		} else if (cea->detailed_timing[i].sync & D_COMPOSITE_SYNC) {

			printf("      digital sync  : composite");

    		if (cea->detailed_timing[i].sync & D_COMPOSITE_SERRATION)
        		printf(" with_serrations");
    		else
    			printf(" without_serrations");

    		printf("\n");

		} else if (cea->detailed_timing[i].sync & D_SEPERATE_SYNC) {

			printf("      digital sync  : seperate");

			if (cea->detailed_timing[i].sync & D_SEPERATE_VSYNC_N)
				printf(" vsync_n");
			else
				printf(" vsync_p");

			if (cea->detailed_timing[i].sync & D_SEPERATE_HSYNC_N)
				printf(" hsync_n");
			else
				printf(" hsync_p");

    		printf("\n");
		}
	}
}

int edid_decode_extensions(struct base_edid *edid, const unsigned char *edid_bin)
{
	const u8 *c = edid_bin;
	int i, nr_ext;

	nr_ext = (int)c[0x7e];

#if 1
	// for simplicity, we now support only storing 8 extension data in the edid struct
	if (nr_ext > 1)
		nr_ext = 1;
#endif

	for (i = 0; i < nr_ext; ++i) {

		c = &edid_bin[128 + 128 * i];

		switch (c[0]) {
		case 0x01:  // 0x01 (LCD Timings)
			// todo ...
			break;

		case EDID_EXT_TAG_CEA:  // 0x02 CEA-EXT (CEA-861 Series Extension)
			if (edid_decode_cea(edid, edid_bin, i) != 0)
				return -1;
			break;

		case 0x10:  // 0x10 VTB-EXT (Video Timing Block Extension)
			// todo ...
			break;

		case 0x20:  // 0x20 (EDID 2.0 Extension)
			// todo ...
			break;

		case 0x30:  // 0x30 (Color Information Type 0 Extension)
			// todo ...
			break;

		case 0x40:  // 0x40 DI-EXT (Display Information Extension / DVI Feature Data)
			// todo ...
			break;

		case 0x50:  // 0x50 LS-EXT (Localized String Extension / Touch Screen Data)
			// todo ...
			break;

		case 0x60:  // 0x60 DPVL-EXT (Digital Packet Video Link Extension)
			// todo ...
			break;

		case 0xf0:  // 0xf0 (Extension Block Map)
			// extension map at block 1
			// todo ...
			break;

		case 0xff:  // 0xff (define by display manufacturer)
			break;
		}
	}

	return 0;
}

int edid_decode_v14(struct base_edid *edid, const unsigned char *edid_bin)
{
	static const u8 header[] = { 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00 };

	const u8 *c = edid_bin;
    u8 checksum = 0;
    int i = 0, s = 0;

    // header
    if (memcmp(c, header, 8) != 0)
        return -1;

    // checksum
    for (s = 0; s < 127; ++s)
        checksum += c[s];

    if ((u8)(checksum + c[127]) != 0x00)
    	return -1;

    edid->checksum = c[127];

    // vid/pid
    c = &edid_bin[8];

    edid->id_mfg[0] = 0x40 + ((c[0] >> 2) & 0x1f);
    edid->id_mfg[1] = 0x40 + (((c[0] << 3) | (c[1] >> 5)) & 0x1f);
    edid->id_mfg[2] = 0x40 + (c[1] & 0x1f);
    edid->id_mfg[3] = 0;

    c = &edid_bin[0x0a];
    edid->id_prod = (c[1] << 8) | c[0];

    c = &edid_bin[0x0c];
    edid->id_sn = (c[3] << 24) | (c[2] << 16) | (c[1] << 8) | c[0];

    c = &edid_bin[0x10];

	edid->mfg_week = 0;
	edid->mfg_year = 0;
	edid->model_year = 0;

    if ((c[0] >= 0x01) && (c[0] <= 0x36))
    	edid->mfg_week = c[0];

    if (c[0] == 0xff)
    	edid->model_year = 1990 + c[1];
    else
    	edid->mfg_year = 1990 + c[1];

    // edid version
    c = &edid_bin[0x12];

    edid->ver_mj = c[0];
    edid->ver_mn = c[1];

    // basic display parameters & features
    c = &edid_bin[0x14];

    if (*c & 0x80) {
        edid->input.digital_if = 1;

        switch ((*c >> 4) & 0x7) {
        case 1:
            edid->input.digital.depth = VID_6BITS;
            break;
        case 2:
            edid->input.digital.depth = VID_8BITS;
            break;
        case 3:
            edid->input.digital.depth = VID_10BITS;
            break;
        case 4:
            edid->input.digital.depth = VID_12BITS;
            break;
        case 5:
            edid->input.digital.depth = VID_14BITS;
            break;
        case 6:
            edid->input.digital.depth = VID_16BITS;
            break;
        case 0:
        default:
            edid->input.digital.depth = VID_NOT_DEFINED;
            break;
        }

        switch (*c & 0xf) {
        case 1:
            edid->input.digital.interface = VII_DVI;
            break;
        case 2:
            edid->input.digital.interface = VII_HDMI_A;
            break;
        case 3:
            edid->input.digital.interface = VII_HDMI_B;
            break;
        case 4:
            edid->input.digital.interface = VII_MDDI;
            break;
        case 5:
            edid->input.digital.interface = VII_DISPLAYPORT;
            break;
        case 0:
        default:
            edid->input.digital.interface = VII_NOT_DEFINED;
            break;
        }

    } else {

        edid->input.digital_if = 0;

        switch ((*c >> 5) & 0x3) {
        case 0:
            edid->input.analog.level = VIL_0P700_0P300_1P000;
            break;
        case 1:
            edid->input.analog.level = VIL_0P714_0P286_1P000;
            break;
        case 2:
            edid->input.analog.level = VIL_1P000_0P400_1P400;
            break;
        case 3:
            edid->input.analog.level = VIL_0P700_0P000_0P700;
            break;
        }

        if (*c & 0x10)
            edid->input.analog.setup = VIS_BLANK_TO_BLACK_PEDESTAL;
        else
            edid->input.analog.setup = VIS_BLANK_LEVEL_EQ_BLACK_LEVEL;

        edid->input.analog.sync_type = 0;
        if (*c & 0x08)
        	edid->input.analog.sync_type |= VIS_SEPERATE_H_V_SYNC;
        if (*c & 0x04)
        	edid->input.analog.sync_type |= VIS_COMPOSITE_H_SYNC;
        if (*c & 0x02)
        	edid->input.analog.sync_type |= VIS_COMPOSITE_G_VIDEO;

        if (*c & 0x01)
        	edid->input.analog.serrations = VISR_VSYNC;
        else
        	edid->input.analog.serrations = 0;
    }

    c = &edid_bin[0x15];

    edid->screen.size_defined = 0;
    edid->screen.size.horizontal = 0;
    edid->screen.size.vertical = 0;
    edid->screen.aspect_defined = 0;
    edid->screen.aspect.landscape = 0.0f;
    edid->screen.aspect.portrait = 0.0f;

    if (c[0] != 0) {
        if (c[1] != 0) {
        	edid->screen.size_defined = 1;
        	edid->screen.size.horizontal = c[0];
        	edid->screen.size.vertical = c[1];
        } else {
        	edid->screen.aspect_defined = 1;
        	edid->screen.aspect.landscape = ((float)c[0] + 99.0f) * 0.01f;
        }
    } else if (c[1] != 0) {
    	edid->screen.aspect_defined = 2;
    	edid->screen.aspect.portrait = 100.0f / ((float)c[1] + 99.0f);
    }

    // gamma
    c = &edid_bin[0x17];

    if (*c < 0xff)
        edid->gamma = ((float)(*c) + 100.0f) / 100.0f;
    else
        edid->gamma = 0.0f;  // defined in an extension block

    // feature
    c = &edid_bin[0x18];

    edid->feature = 0;
    if (*c & 0x80)
    	edid->feature |= FS_PM_STANDBY;
    if (*c & 0x40)
    	edid->feature |= FS_PM_SUSPEND;
    if (*c & 0x20)
    	edid->feature |= FS_PM_ACTIVE_OFF;

    if (edid->input.digital_if) {
        switch ((*c >> 3) & 0x03) {
        case 0:
        	edid->feature |= FS_CEF_RGB444;
            break;
        case 1:
        	edid->feature |= (FS_CEF_RGB444 | FS_CEF_YCBCR444);
            break;
        case 2:
        	edid->feature |= (FS_CEF_RGB444 | FS_CEF_YCBCR422);
            break;
        case 3:
        	edid->feature |= (FS_CEF_RGB444 | FS_CEF_YCBCR444 | FS_CEF_YCBCR422);
            break;
        }
    } else {
        switch ((*c >> 3) & 0x03) {
        case 0:
        	edid->feature |= FS_DCT_MONO_GRAY;
            break;
        case 1:
        	edid->feature |= FS_DCT_RGB;
            break;
        case 2:
        	edid->feature |= FS_DCT_NON_RGB;
            break;
        }
    }

    if (*c & 0x04)
    	edid->feature |= FS_OF_SRGB_DEFAULT;
    if (*c & 0x02)
    	edid->feature |= FS_OF_NATIVE_PREFERRED;
    if (*c & 0x01)
    	edid->feature |= FS_OF_CONTINUOUS_FREQ;

    // chromaticity coordinates
    c = &edid_bin[0x19];

    edid->chromaticity.r_x = ((u16)c[2] << 2) | (((u16)c[0] >> 6) & 0x3);
    edid->chromaticity.r_y = ((u16)c[3] << 2) | (((u16)c[0] >> 4) & 0x3);
    edid->chromaticity.g_x = ((u16)c[4] << 2) | (((u16)c[0] >> 2) & 0x3);
    edid->chromaticity.g_y = ((u16)c[5] << 2) | ((u16)c[0] & 0x3);
    edid->chromaticity.b_x = ((u16)c[6] << 2) | (((u16)c[1] >> 6) & 0x3);
    edid->chromaticity.b_y = ((u16)c[7] << 2) | (((u16)c[1] >> 4) & 0x3);
    edid->chromaticity.w_x = ((u16)c[8] << 2) | (((u16)c[1] >> 2) & 0x3);
    edid->chromaticity.w_y = ((u16)c[9] << 2) | ((u16)c[1] & 0x3);

    // established timings I/II
    c = &edid_bin[0x23];

	eb_establish_timing(&edid->timing_1_2[0], 720, 400, 70, c[0] & 0x80);
	eb_establish_timing(&edid->timing_1_2[1], 720, 400, 88, c[0] & 0x40);
	eb_establish_timing(&edid->timing_1_2[2], 640, 480, 60, c[0] & 0x20);
	eb_establish_timing(&edid->timing_1_2[3], 640, 480, 67, c[0] & 0x10);
	eb_establish_timing(&edid->timing_1_2[4], 640, 480, 72, c[0] & 0x08);
	eb_establish_timing(&edid->timing_1_2[5], 640, 480, 75, c[0] & 0x04);
	eb_establish_timing(&edid->timing_1_2[6], 800, 600, 56, c[0] & 0x02);
	eb_establish_timing(&edid->timing_1_2[7], 800, 600, 60, c[0] & 0x01);
	eb_establish_timing(&edid->timing_1_2[8], 800, 600, 72, c[1] & 0x80);
	eb_establish_timing(&edid->timing_1_2[9], 800, 600, 75, c[1] & 0x40);
	eb_establish_timing(&edid->timing_1_2[10], 823, 624, 75, c[1] & 0x20);
	eb_establish_timing_i(&edid->timing_1_2[11], 1024, 768, 87, c[1] & 0x10);
	eb_establish_timing(&edid->timing_1_2[12], 1024, 768, 60, c[1] & 0x08);
	eb_establish_timing(&edid->timing_1_2[13], 1024, 768, 70, c[1] & 0x04);
	eb_establish_timing(&edid->timing_1_2[14], 1024, 768, 75, c[1] & 0x02);
	eb_establish_timing(&edid->timing_1_2[15], 1280, 1024, 75, c[1] & 0x01);
	eb_establish_timing(&edid->timing_1_2[16], 1152, 870, 75, c[2] & 0x80);

    // standard timings
    c = &edid_bin[0x26];

    for (i = 0; i < 8; ++i, c += 2) {
        if (*c == 0)
        	continue;

        edid->standard_timings[i].horizontal = (c[0] + 31) * 8;

		switch ((c[1] >> 6) & 0x3) {
		case 0:
			edid->standard_timings[i].aspect_ratio = ST_AR_16_10;
			break;
		case 1:
			edid->standard_timings[i].aspect_ratio = ST_AR_4_3;
			break;
		case 2:
			edid->standard_timings[i].aspect_ratio = ST_AR_5_4;
			break;
		case 3:
			edid->standard_timings[i].aspect_ratio = ST_AR_16_9;
			break;
		}

		edid->standard_timings[i].refresh_rate = (c[1] & 0x3f) + 60;
    }

    // 18-byte descriptors x 4
    for (i = 0; i < 4; ++i) {

    	c = &edid_bin[0x36 + 18 * i];

        if ((c[0] != 0) || (c[1] != 0)) {
            // detailed timing definition (preferred timing if is the first descriptor)

        	edid->descriptors[i].type = DT_DETAILED_TIMING;

            edid->descriptors[i].detailed_timing.pixel_clock = (((int)c[1] << 8) | (int)c[0]) * 10000;
            edid->descriptors[i].detailed_timing.width = (((int)c[4] << 4) & 0xf00) | (int)c[2];
            edid->descriptors[i].detailed_timing.height = (((int)c[7] << 4) & 0xf00) | (int)c[5];
            edid->descriptors[i].detailed_timing.h_blank = (((int)c[4] << 8) & 0xf00) | (int)c[3];
            edid->descriptors[i].detailed_timing.v_blank = (((int)c[7] << 8) & 0xf00) | (int)c[6];
            edid->descriptors[i].detailed_timing.h_fp = (((int)c[11] << 2) & 0x300) | (int)c[8];
            edid->descriptors[i].detailed_timing.v_fp = (((int)c[11] << 2) & 0x30) | ((int)c[10] >> 4);
            edid->descriptors[i].detailed_timing.h_sync = (((int)c[11] << 4) & 0x300) | (int)c[9];
            edid->descriptors[i].detailed_timing.v_sync = (((int)c[11] << 4) & 0x30) | ((int)c[10] & 0x0f);
            edid->descriptors[i].detailed_timing.h_size = (((int)c[14] << 4) & 0xf00) | (int)c[12];
            edid->descriptors[i].detailed_timing.v_size = (((int)c[14] << 8) & 0xf00) | (int)c[13];
            edid->descriptors[i].detailed_timing.border_lr = (int)c[15];
            edid->descriptors[i].detailed_timing.border_tb = (int)c[16];

            edid->descriptors[i].detailed_timing.interlaced = (c[17] & 0x80) ? 1 : 0;

            edid->descriptors[i].detailed_timing.stereo = STEREO_NONE;
			if ((c[17] & 0x61) == 0x20)
				edid->descriptors[i].detailed_timing.stereo |= STEREO_FIELD_SEQUENTIAL_RIGHT_SYNC;
			if ((c[17] & 0x61) == 0x40)
				edid->descriptors[i].detailed_timing.stereo |= STEREO_FIELD_SEQUENTIAL_LEFT_SYNC;
			if ((c[17] & 0x61) == 0x21)
				edid->descriptors[i].detailed_timing.stereo |= STEREO_2WAY_INTERLEAVE_RIGHT_EVEN;
			if ((c[17] & 0x61) == 0x41)
				edid->descriptors[i].detailed_timing.stereo |= STEREO_2WAY_INTERLEAVE_LEFT_EVEN;
			if ((c[17] & 0x61) == 0x60)
				edid->descriptors[i].detailed_timing.stereo |= STEREO_4WAY_INTERLEAVE;
			if ((c[17] & 0x61) == 0x61)
				edid->descriptors[i].detailed_timing.stereo |= STEREO_SIDE_BY_SIDE_INTERLEAVE;

			edid->descriptors[i].detailed_timing.sync = 0;
            if (c[17] & 0x10) {
                if (c[17] & 0x08) {
                	edid->descriptors[i].detailed_timing.sync |= D_SEPERATE_SYNC;

                	if (c[17] & 0x04)
                    	edid->descriptors[i].detailed_timing.sync |= D_SEPERATE_VSYNC_P;
                	else
                		edid->descriptors[i].detailed_timing.sync |= D_SEPERATE_VSYNC_N;

                	if (c[17] & 0x02)
                    	edid->descriptors[i].detailed_timing.sync |= D_SEPERATE_HSYNC_P;
                	else
                		edid->descriptors[i].detailed_timing.sync |= D_SEPERATE_HSYNC_N;
                } else {
                	edid->descriptors[i].detailed_timing.sync |= D_COMPOSITE_SYNC;
                	if (c[17] & 0x04)
                    	edid->descriptors[i].detailed_timing.sync |= D_COMPOSITE_SERRATION;
                }
            } else {
            	if (c[17] & 0x80) {
            		edid->descriptors[i].detailed_timing.sync |= A_BIPOLAR_COMPOSITE_SYNC;
            		if (c[17] & 0x40) {
                		edid->descriptors[i].detailed_timing.sync |= A_BIPOLAR_SERRATION;
                		if (c[17] & 0x20)
                    		edid->descriptors[i].detailed_timing.sync |= A_BIPOLAR_SERRATION_SYNC_RGB;
                		else
                    		edid->descriptors[i].detailed_timing.sync |= A_BIPOLAR_SERRATION_SYNC_G;
            		}
            	} else {
            		edid->descriptors[i].detailed_timing.sync |= A_COMPOSITE_SYNC;
            	}
            }

        } else {
        	// display descriptor

        	edid->descriptors[i].type = DT_DISPLAY_DESCRIPTOR;

        	switch (c[3]) {  // tag
            case 0xff:
            	edid->descriptors[i].tag = DTAG_PRODUCT_SERIAL;
            	edid->descriptors[i].serial[18] = 0;
            	memcpy(edid->descriptors[i].serial, &c[5], 18-5);
                for (s = 0; s < 18-5; ++s) {
                    if (edid->descriptors[i].serial[s] == 0x0a) {
                    	edid->descriptors[i].serial[s] = 0;
                    }
                }
                break;

            case 0xfe:
            	edid->descriptors[i].tag = DTAG_DATA_STRING;
            	edid->descriptors[i].string[18] = 0;
            	memcpy(edid->descriptors[i].string, &c[5], 18-5);
                for (s = 0; s < 18-5; ++s) {
                    if (edid->descriptors[i].string[s] == 0x0a) {
                    	edid->descriptors[i].string[s] = 0;
                    }
                }
                break;

            case 0xfd:
            	edid->descriptors[i].tag = DTAG_RANGE_LIMIT;

            	edid->descriptors[i].range_limits.h_offset = HRO_ZERO;

            	switch (c[4] & 0xfc) {
                case 0x08:
                	edid->descriptors[i].range_limits.h_offset = HRO_MAX_255_MIN_ZERO;
                    break;
                case 0x0c:
                	edid->descriptors[i].range_limits.h_offset = HRO_MAX_255_MIN_255;
                    break;
                }

            	edid->descriptors[i].range_limits.v_offset = VRO_ZERO;

                switch (c[4] & 0xf3) {
                case 0x02:
                	edid->descriptors[i].range_limits.v_offset = VRO_MAX_255_MIN_ZERO;
                    break;
                case 0x03:
                	edid->descriptors[i].range_limits.v_offset = VRO_MAX_255_MIN_255;
                    break;
                }

                if (c[5] != 0) {
					if ((c[4] & 0x03) == 0x03)
						edid->descriptors[i].range_limits.v_min = c[5] + 255;
					else
						edid->descriptors[i].range_limits.v_min = c[5];
                } else {
                	edid->descriptors[i].range_limits.v_min = 0;
                }

                if (c[6] != 0) {
					if ((c[4] & 0x02) == 0x02)
						edid->descriptors[i].range_limits.v_max = c[6] + 255;
					else
						edid->descriptors[i].range_limits.v_max = c[6];
                } else {
                	edid->descriptors[i].range_limits.v_max = 0;
                }

                if (c[7] != 0) {
					if ((c[4] & 0x0c) == 0x0c)
						edid->descriptors[i].range_limits.h_min = c[7] + 255;
					else
						edid->descriptors[i].range_limits.h_min = c[7];
                } else {
                	edid->descriptors[i].range_limits.h_min = 0;
                }

                if (c[8] != 0) {
					if ((c[4] & 0x08) == 0x08)
						edid->descriptors[i].range_limits.h_max = c[8] + 255;
					else
						edid->descriptors[i].range_limits.h_max = c[8];
                } else {
                	edid->descriptors[i].range_limits.h_max = 0;
                }

                edid->descriptors[i].range_limits.p_max = c[9] * 10;

                // additional timing support flags
            	edid->descriptors[i].range_limits.timing_flags = 0;

            	if ((c[10] == 0) && (edid->feature & FS_OF_CONTINUOUS_FREQ)) {

            		edid->descriptors[i].range_limits.timing_flags |= TF_DEFAULT_GTF;

            	} else if ((c[10] == 0x04) && (edid->feature & FS_OF_CONTINUOUS_FREQ)) {

            		edid->descriptors[i].range_limits.timing_flags |= TF_CVT;

            		edid->descriptors[i].range_limits.cvt.ver_mj = c[11] >> 4;
            		edid->descriptors[i].range_limits.cvt.ver_mn = c[11] & 0x0f;

            		edid->descriptors[i].range_limits.cvt.extra_pclk = (float)((u32)c[12] >> 2) * 0.25;
                    if (c[13] != 0)
                    	edid->descriptors[i].range_limits.cvt.line_max = ((((u32)c[12] & 0x03) << 8) | (u32)c[13]) * 8;
                    else
                    	edid->descriptors[i].range_limits.cvt.line_max = 0;

                    edid->descriptors[i].range_limits.cvt.aspect_ratios = 0;
                    if (c[14] & 0x80)
                    	edid->descriptors[i].range_limits.cvt.aspect_ratios |= CVT_AR_4_3;
                    if (c[14] & 0x40)
                    	edid->descriptors[i].range_limits.cvt.aspect_ratios |= CVT_AR_16_9;
                    if (c[14] & 0x20)
                    	edid->descriptors[i].range_limits.cvt.aspect_ratios |= CVT_AR_16_10;
                    if (c[14] & 0x10)
                    	edid->descriptors[i].range_limits.cvt.aspect_ratios |= CVT_AR_5_4;
                    if (c[14] & 0x08)
                    	edid->descriptors[i].range_limits.cvt.aspect_ratios |= CVT_AR_15_9;

                    switch (c[15] & 0xe0) {
                    case 0x00:
                    	edid->descriptors[i].range_limits.cvt.preferred_ar = CVT_AR_4_3;
                        break;
                    case 0x20:
                    	edid->descriptors[i].range_limits.cvt.preferred_ar = CVT_AR_16_9;
                        break;
                    case 0x40:
                    	edid->descriptors[i].range_limits.cvt.preferred_ar = CVT_AR_16_10;
                        break;
                    case 0x60:
                    	edid->descriptors[i].range_limits.cvt.preferred_ar = CVT_AR_5_4;
                        break;
                    case 0x80:
                    	edid->descriptors[i].range_limits.cvt.preferred_ar = CVT_AR_15_9;
                        break;
                    default:
                    	edid->descriptors[i].range_limits.cvt.preferred_ar = 0;
                        break;
                    }

                    edid->descriptors[i].range_limits.cvt.blanking = 0;
					if (c[15] & 0x08)
						edid->descriptors[i].range_limits.cvt.blanking |= CVT_STANDARD_BLANKING;
					if (c[15] & 0x10)
						edid->descriptors[i].range_limits.cvt.blanking |= CVT_REDUCED_BLANKING;

                    edid->descriptors[i].range_limits.cvt.scaling = 0;
					if (c[16] & 0x80)
	                    edid->descriptors[i].range_limits.cvt.scaling |= CVT_H_SHRINK;
					if (c[16] & 0x40)
	                    edid->descriptors[i].range_limits.cvt.scaling |= CVT_H_STRETCH;
					if (c[16] & 0x20)
	                    edid->descriptors[i].range_limits.cvt.scaling |= CVT_V_SHRINK;
					if (c[16] & 0x10)
	                    edid->descriptors[i].range_limits.cvt.scaling |= CVT_V_STRETCH;

					edid->descriptors[i].range_limits.cvt.v_refresh = c[17];

                } else if (c[10] == 0x01) {
                	edid->descriptors[i].range_limits.timing_flags |= TF_RANGE_LIMITS;
                } else if (c[10] == 0x02) {
                	edid->descriptors[i].range_limits.timing_flags |= TF_SECOND_GTF;

                	edid->descriptors[i].range_limits.gtf_2nd_curve.break_freq = c[12] * 2;
                	edid->descriptors[i].range_limits.gtf_2nd_curve.C = c[13] / 2;
                	edid->descriptors[i].range_limits.gtf_2nd_curve.M = ((u32)c[15] << 8) | (u32)c[14];
                	edid->descriptors[i].range_limits.gtf_2nd_curve.K = c[16];
                	edid->descriptors[i].range_limits.gtf_2nd_curve.J = c[17] / 2;
                }

                break;

            case 0xfc:
            	edid->descriptors[i].tag = DTAG_PRODUCT_NAME;
            	edid->descriptors[i].prod_name[18] = 0;
            	memcpy(edid->descriptors[i].prod_name, &c[5], 18-5);
                for (s = 0; s < 18-5; ++s) {
                    if (edid->descriptors[i].prod_name[s] == 0x0a) {
                    	edid->descriptors[i].prod_name[s] = 0;
                    }
                }
                break;

            case 0xfb:
            	edid->descriptors[i].tag = DTAG_COLOR_POINT;

				edid->descriptors[i].white_point[0].index = c[5];
				edid->descriptors[i].white_point[0].x = ((u16)c[7] << 2) | (((u16)c[6] >> 2) & 0x3);
				edid->descriptors[i].white_point[0].y = ((u16)c[8] << 2) | ((u16)c[6] & 0x3);
				if (c[9] < 0xff)
					edid->descriptors[i].white_point[0].gamma = ((float)c[9] + 100.0f) * 0.01f;
				else
					edid->descriptors[i].white_point[0].gamma = 0.0f;

				edid->descriptors[i].white_point[1].index = c[10];
				edid->descriptors[i].white_point[1].x = ((u16)c[12] << 2) | (((u16)c[11] >> 2) & 0x3);
				edid->descriptors[i].white_point[1].y = ((u16)c[13] << 2) | ((u16)c[11] & 0x3);
				if (c[14] < 0xff)
					edid->descriptors[i].white_point[1].gamma = ((float)c[14] + 100.0f) * 0.01f;
				else
					edid->descriptors[i].white_point[1].gamma = 0.0f;

                break;

            case 0xfa:
            	edid->descriptors[i].tag = DTAG_STANDARD_TIMING;

                for (s = 0; s < 6; ++s) {
            		u8 *d = (u8 *)&c[5 + 2 * s];

            		if (*d == 0)
                    	continue;

            		edid->descriptors[i].extra_standard_timings[s].horizontal = (d[0] + 31) * 8;

            		switch ((d[1] >> 6) & 0x3) {
            		case 0:
            			edid->descriptors[i].extra_standard_timings[s].aspect_ratio = ST_AR_16_10;
            			break;
            		case 1:
            			edid->descriptors[i].extra_standard_timings[s].aspect_ratio = ST_AR_4_3;
            			break;
            		case 2:
            			edid->descriptors[i].extra_standard_timings[s].aspect_ratio = ST_AR_5_4;
            			break;
            		case 3:
            			edid->descriptors[i].extra_standard_timings[s].aspect_ratio = ST_AR_16_9;
            			break;
            		}

            		edid->descriptors[i].extra_standard_timings[s].refresh_rate = (d[1] & 0x3f) + 60;
                }

                break;

            case 0xf9:
            	// display color management data
            	edid->descriptors[i].tag = DTAG_DCM_DATA;

            	edid->descriptors[i].dcm.ver = c[5];
            	edid->descriptors[i].dcm.r_a2 = ((u32)c[9] << 8) | (u32)c[8];
            	edid->descriptors[i].dcm.r_a3 = ((u32)c[7] << 8) | (u32)c[6];
            	edid->descriptors[i].dcm.g_a2 = ((u32)c[13] << 8) | (u32)c[12];
            	edid->descriptors[i].dcm.g_a3 = ((u32)c[11] << 8) | (u32)c[10];
            	edid->descriptors[i].dcm.b_a2 = ((u32)c[17] << 8) | (u32)c[16];
            	edid->descriptors[i].dcm.b_a3 = ((u32)c[15] << 8) | (u32)c[14];

            	break;

            case 0xf8:
            	edid->descriptors[i].tag = DTAG_CVT_TIMING;

                for (s = 0; s < 4; ++s) {
                	u8 *d = (u8 *)&c[6 + s * 3];

                	edid->descriptors[i].cvt[s].lines = (((u32)d[1] & 0xf0) << 4) | (u32)d[0];

                	switch (d[1] & 0x0c) {
                    case 0x00:
                    	edid->descriptors[i].cvt[s].aspect |= CVT_AR_4_3;
                        break;
                    case 0x04:
                    	edid->descriptors[i].cvt[s].aspect |= CVT_AR_16_9;
                        break;
                    case 0x08:
                    	edid->descriptors[i].cvt[s].aspect |= CVT_AR_16_10;
                        break;
                    case 0x0c:
                    	edid->descriptors[i].cvt[s].aspect |= CVT_AR_15_9;
                        break;
                    }

                	switch (d[2] & 0x60) {
                    case 0x00:
                    	edid->descriptors[i].cvt[s].preferred_v_rate |= CVT_V_RATE_50HZ;
                        break;
                    case 0x20:
                    	edid->descriptors[i].cvt[s].preferred_v_rate |= CVT_V_RATE_60HZ;
                        break;
                    case 0x40:
                    	edid->descriptors[i].cvt[s].preferred_v_rate |= CVT_V_RATE_75HZ;
                        break;
                    case 0x60:
                    	edid->descriptors[i].cvt[s].preferred_v_rate |= CVT_V_RATE_85HZ;
                        break;
                    }

                	edid->descriptors[i].cvt[s].supported_v_rate_blank = 0;

                    if (d[2] & 0x10)
                    	edid->descriptors[i].cvt[s].supported_v_rate_blank |= (CVT_V_RATE_50HZ | CVT_STANDARD_BLANKING);
                    if (d[2] & 0x08)
                    	edid->descriptors[i].cvt[s].supported_v_rate_blank |= (CVT_V_RATE_60HZ | CVT_STANDARD_BLANKING);
                    if (d[2] & 0x04)
                    	edid->descriptors[i].cvt[s].supported_v_rate_blank |= (CVT_V_RATE_75HZ | CVT_STANDARD_BLANKING);
                    if (d[2] & 0x02)
                    	edid->descriptors[i].cvt[s].supported_v_rate_blank |= (CVT_V_RATE_85HZ | CVT_STANDARD_BLANKING);
                    if (d[2] & 0x01)
                    	edid->descriptors[i].cvt[s].supported_v_rate_blank |= (CVT_V_RATE_60HZ | CVT_REDUCED_BLANKING);
                }

                break;

            case 0xf7:
            	edid->descriptors[i].tag = DTAG_ESTABLISHED_TIMING_III;

            	eb_establish_timing(&edid->descriptors[i].timing_3[0], 640, 350, 85, c[6] & 0x80);
				eb_establish_timing(&edid->descriptors[i].timing_3[1], 640, 400, 85, c[6] & 0x40);
				eb_establish_timing(&edid->descriptors[i].timing_3[2], 720, 400, 85, c[6] & 0x20);
				eb_establish_timing(&edid->descriptors[i].timing_3[3], 640, 480, 85, c[6] & 0x10);
				eb_establish_timing(&edid->descriptors[i].timing_3[4], 848, 480, 60, c[6] & 0x08);
				eb_establish_timing(&edid->descriptors[i].timing_3[5], 800, 600, 85, c[6] & 0x04);
				eb_establish_timing(&edid->descriptors[i].timing_3[6], 1024, 768, 85, c[6] & 0x02);
				eb_establish_timing(&edid->descriptors[i].timing_3[7], 1152, 864, 75, c[6] & 0x01);

				eb_establish_timing_rb(&edid->descriptors[i].timing_3[0 + 1 * 8], 1280, 768, 60, c[7] & 0x80);
				eb_establish_timing(&edid->descriptors[i].timing_3[1 + 1 * 8], 1280, 768, 60, c[7] & 0x40);
				eb_establish_timing(&edid->descriptors[i].timing_3[2 + 1 * 8], 1280, 768, 75, c[7] & 0x20);
				eb_establish_timing(&edid->descriptors[i].timing_3[3 + 1 * 8], 1280, 768, 85, c[7] & 0x10);
				eb_establish_timing(&edid->descriptors[i].timing_3[4 + 1 * 8], 1280, 960, 60, c[7] & 0x08);
				eb_establish_timing(&edid->descriptors[i].timing_3[5 + 1 * 8], 1280, 960, 85, c[7] & 0x04);
				eb_establish_timing(&edid->descriptors[i].timing_3[6 + 1 * 8], 1280, 1024, 60, c[7] & 0x02);
				eb_establish_timing(&edid->descriptors[i].timing_3[7 + 1 * 8], 1280, 1024, 85, c[7] & 0x01);

				eb_establish_timing(&edid->descriptors[i].timing_3[0 + 2 * 8], 1360, 768, 60, c[8] & 0x80);
				eb_establish_timing_rb(&edid->descriptors[i].timing_3[1 + 2 * 8], 1440, 900, 60, c[8] & 0x40);
				eb_establish_timing(&edid->descriptors[i].timing_3[2 + 2 * 8], 1440, 900, 60, c[8] & 0x20);
				eb_establish_timing(&edid->descriptors[i].timing_3[3 + 2 * 8], 1440, 900, 75, c[8] & 0x10);
				eb_establish_timing(&edid->descriptors[i].timing_3[4 + 2 * 8], 1440, 900, 85, c[8] & 0x08);
				eb_establish_timing_rb(&edid->descriptors[i].timing_3[5 + 2 * 8], 1400, 1050, 60, c[8] & 0x04);
				eb_establish_timing(&edid->descriptors[i].timing_3[6 + 2 * 8], 1400, 1050, 60, c[8] & 0x02);
				eb_establish_timing(&edid->descriptors[i].timing_3[7 + 2 * 8], 1400, 1050, 75, c[8] & 0x01);

                eb_establish_timing(&edid->descriptors[i].timing_3[0 + 3 * 8], 1400, 1050, 85, c[9] & 0x80);
				eb_establish_timing(&edid->descriptors[i].timing_3[1 + 3 * 8], 1680, 1050, 60, c[9] & 0x40);
				eb_establish_timing(&edid->descriptors[i].timing_3[2 + 3 * 8], 1680, 1050, 60, c[9] & 0x20);
				eb_establish_timing(&edid->descriptors[i].timing_3[3 + 3 * 8], 1680, 1050, 75, c[9] & 0x10);
				eb_establish_timing(&edid->descriptors[i].timing_3[4 + 3 * 8], 1680, 1050, 85, c[9] & 0x08);
				eb_establish_timing(&edid->descriptors[i].timing_3[5 + 3 * 8], 1600, 1200, 60, c[9] & 0x04);
				eb_establish_timing(&edid->descriptors[i].timing_3[6 + 3 * 8], 1600, 1200, 65, c[9] & 0x02);
				eb_establish_timing(&edid->descriptors[i].timing_3[7 + 3 * 8], 1600, 1200, 70, c[9] & 0x01);

               	eb_establish_timing(&edid->descriptors[i].timing_3[0 + 4 * 8], 1600, 1200, 75, c[10] & 0x80);
				eb_establish_timing(&edid->descriptors[i].timing_3[1 + 4 * 8], 1600, 1200, 85, c[10] & 0x40);
				eb_establish_timing(&edid->descriptors[i].timing_3[2 + 4 * 8], 1792, 1344, 60, c[10] & 0x20);
				eb_establish_timing(&edid->descriptors[i].timing_3[3 + 4 * 8], 1792, 1344, 75, c[10] & 0x10);
				eb_establish_timing(&edid->descriptors[i].timing_3[4 + 4 * 8], 1856, 1392, 60, c[10] & 0x08);
				eb_establish_timing(&edid->descriptors[i].timing_3[5 + 4 * 8], 1856, 1392, 75, c[10] & 0x04);
				eb_establish_timing_rb(&edid->descriptors[i].timing_3[6 + 4 * 8], 1920, 1200, 60, c[10] & 0x02);
				eb_establish_timing(&edid->descriptors[i].timing_3[7 + 4 * 8], 1920, 1200, 60, c[10] & 0x01);

				eb_establish_timing(&edid->descriptors[i].timing_3[0 + 5 * 8], 1920, 1200, 75, c[11] & 0x80);
				eb_establish_timing(&edid->descriptors[i].timing_3[1 + 5 * 8], 1920, 1200, 85, c[11] & 0x40);
				eb_establish_timing(&edid->descriptors[i].timing_3[2 + 5 * 8], 1920, 1440, 65, c[11] & 0x20);
				eb_establish_timing(&edid->descriptors[i].timing_3[3 + 5 * 8], 1920, 1440, 75, c[11] & 0x10);
                break;

            case 0x10:
            	edid->descriptors[i].tag = DTAG_DUMMY;
                break;
            default:
                break;
            }
        }
    }

    // extension flag & checksum
    c = &edid_bin[0x7e];

    edid->extensions = *c;

    if (edid->extensions != 0)
    	edid_decode_extensions(edid, edid_bin);

    return 0;
}

void edid_print_v14(struct base_edid *edid)
{
	int i;

	printf("Base EDID Information ...\n\n");
	printf("vendor id    : %s\n", edid->id_mfg);
	printf("product code : %04x\n", edid->id_prod);
	printf("serial       : %08x\n", edid->id_sn);
	printf("mfg date     : year %04d week %02d\n", edid->mfg_year, edid->mfg_week);

	if (edid->model_year > 0)
		printf("model year   : %04d\n", edid->model_year);

	printf("EDID version : %d.%d\n", edid->ver_mj, edid->ver_mn);
	printf("extensions   : %d\n", edid->extensions);

	if (edid->input.digital_if != 0) {
		printf("\ndigital input interface ...\n");

		switch (edid->input.digital.depth) {
		case 1:
			printf("  color depth : 6 bpp\n");
			break;
		case 2:
			printf("  color depth : 8 bpp\n");
			break;
		case 3:
			printf("  color depth : 10 bpp\n");
			break;
		case 4:
			printf("  color depth : 12 bpp\n");
			break;
		case 5:
			printf("  color depth : 14 bpp\n");
			break;
		case 6:
			printf("  color depth : 16 bpp\n");
			break;
		default:
			printf("  color depth : undefined (%d)\n", edid->input.digital.depth);
			break;
		}

		switch (edid->input.digital.interface) {
		case 1:
			printf("  interface   : DVI\n");
			break;
		case 2:
			printf("  interface   : HDMI-a\n");
			break;
		case 3:
			printf("  interface   : HDMI-b\n");
			break;
		case 4:
			printf("  interface   : MDDI\n");
			break;
		case 5:
			printf("  interface   : DisplayPort\n");
			break;
		default:
			printf("  interface   : undefined (%d)\n", edid->input.digital.interface);
			break;
		}

	} else {
		printf("\nanalog input interface ...\n");

		switch (edid->input.analog.level) {
		case VIL_0P700_0P300_1P000:
			printf("  signal level    : 0.700 : 0.300 : 1.000 Vpp\n");
			break;
		case VIL_0P714_0P286_1P000:
			printf("  signal level    : 0.714 : 0.286 : 1.000 Vpp\n");
			break;
		case VIL_1P000_0P400_1P400:
			printf("  signal level    : 1.000 : 0.400 : 1.400 Vpp\n");
			break;
		case VIL_0P700_0P000_0P700:
			printf("  signal level    : 0.700 : 0.000 : 0.700 Vpp\n");
			break;
		}

		switch (edid->input.analog.setup) {
		case VIS_BLANK_LEVEL_EQ_BLACK_LEVEL:
			printf("  video setup     : blank level = black level\n");
			break;
		case VIS_BLANK_TO_BLACK_PEDESTAL:
			printf("  video setup     : blank-to-black setup or pedestal\n");
			break;
		}

		printf("  sync types      : seperate_sync_h_v(%d) composite_sync_h(%d) composite_sync_green(%d)\n",
			(edid->input.analog.sync_type & VIS_SEPERATE_H_V_SYNC) ? 1 : 0,
			(edid->input.analog.sync_type & VIS_COMPOSITE_H_SYNC) ? 1 : 0,
			(edid->input.analog.sync_type & VIS_COMPOSITE_G_VIDEO) ? 1 : 0);

		printf("  vsync serration : %s\n", (edid->input.analog.serrations & VISR_VSYNC) ? "supported" : "not supported");
	}

	if (edid->screen.size_defined) {
		printf("\nscreen size  : horizontal) %d (cm)  vertical) %d (cm)\n", edid->screen.size.horizontal, edid->screen.size.vertical);
	} else if (edid->screen.aspect_defined == 1) {
		printf("\naspect ratio : landscape) %.2f\n", edid->screen.aspect.landscape);
	} else if (edid->screen.aspect_defined == 2) {
		printf("\naspect ratio : portrait) %.2f\n", edid->screen.aspect.portrait);
	}

	printf("\ngamma        : %.2f\n", edid->gamma);

	printf("\nfeatures ...\n");
	printf("  [PM] standby mode                     : %s\n", (edid->feature & FS_PM_STANDBY) ? "yes" : "no");
	printf("  [PM] suspend mode                     : %s\n", (edid->feature & FS_PM_SUSPEND) ? "yes" : "no");
	printf("  [PM] active off                       : %s\n", (edid->feature & FS_PM_ACTIVE_OFF) ? "yes" : "no");

	printf("  [DI] supported color encoding formats : ");
	if (edid->feature & FS_CEF_RGB444)
		printf("RGB_4:4:4 ");
	if (edid->feature & FS_CEF_YCBCR444)
		printf("YCbCr_4:4:4 ");
	if (edid->feature & FS_CEF_YCBCR422)
		printf("YCbCr_4:2:2 ");
	printf("\n");

	if (edid->feature & FS_DCT_MONO_GRAY)
		printf("  [AI] display color type               : monochrome or grayscale display\n");
	if (edid->feature & FS_DCT_RGB)
		printf("  [AI] display color type               : RGB\n");
	if (edid->feature & FS_DCT_NON_RGB)
		printf("  [AI] display color type               : non-RGB\n");

	printf("  [OF] sRGB default                     : %s\n", (edid->feature & FS_OF_SRGB_DEFAULT) ? "yes" : "no");
	printf("  [OF] native preferred timing          : %s\n", (edid->feature & FS_OF_NATIVE_PREFERRED) ? "yes" : "no");
	printf("  [OF] continuous freq                  : %s\n", (edid->feature & FS_OF_CONTINUOUS_FREQ) ? "yes" : "no");

	printf("\nestablished timing ...\n");
	for (i = 0; i < 17; ++i) {
		if (edid->timing_1_2[i].valid)
			printf("  [%02d] %03d x %03d @ %02d Hz %s\n", i, edid->timing_1_2[i].width, edid->timing_1_2[i].height, edid->timing_1_2[i].refresh, edid->timing_1_2[i].interlaced ? "(I)" : " ");
	}

	printf("\nstandard timing ...\n");
	for (i = 0; i < 8; ++i) {
		if (edid->standard_timings[i].refresh_rate) {
			printf("  [%02d] %03d @ %02d Hz ", i, edid->standard_timings[i].horizontal, edid->standard_timings[i].refresh_rate);
			switch (edid->standard_timings[i].aspect_ratio) {
			case ST_AR_16_10:
				printf("16:10\n");
				break;
			case ST_AR_4_3:
				printf("4:3\n");
				break;
			case ST_AR_5_4:
				printf("5:4\n");
				break;
			case ST_AR_16_9:
				printf("16:9\n");
				break;
			default:
				printf("\n");
				break;
			}
		}
	}

	printf("\ndescriptors ...\n");
	for (i = 0; i < 4; ++i) {
    	if (edid->descriptors[i].type == DT_DETAILED_TIMING) {
    		printf("\n  [%d] DTD -- detailed timing descriptor\n", i);

    		printf("      pixel clock   : %d (Hz)\n", edid->descriptors[i].detailed_timing.pixel_clock);
    		printf("      h addressible : %d (pixels)\n", edid->descriptors[i].detailed_timing.width);
    		printf("        blank       : %d (pixels)\n", edid->descriptors[i].detailed_timing.h_blank);
    		printf("        fp          : %d (pixels)\n", edid->descriptors[i].detailed_timing.h_fp);
    		printf("        sync        : %d (pixels)\n", edid->descriptors[i].detailed_timing.h_sync);
    		printf("        bp          : %d (pixels)\n", edid->descriptors[i].detailed_timing.h_blank - edid->descriptors[i].detailed_timing.h_fp - edid->descriptors[i].detailed_timing.h_sync);
    		printf("        size        : %d (mm)\n", edid->descriptors[i].detailed_timing.h_size);
    		printf("      v addressible : %d (lines)\n", edid->descriptors[i].detailed_timing.height);
    		printf("        blank       : %d (lines)\n", edid->descriptors[i].detailed_timing.v_blank);
    		printf("        fp          : %d (lines)\n", edid->descriptors[i].detailed_timing.v_fp);
    		printf("        sync        : %d (lines)\n", edid->descriptors[i].detailed_timing.v_sync);
    		printf("        bp          : %d (lines)\n", edid->descriptors[i].detailed_timing.v_blank - edid->descriptors[i].detailed_timing.v_fp - edid->descriptors[i].detailed_timing.v_sync);
    		printf("        size        : %d (mm)\n", edid->descriptors[i].detailed_timing.v_size);
    		printf("      border (L&R)  : %d (pixels)\n", edid->descriptors[i].detailed_timing.border_lr);
    		printf("      border (T&B)  : %d (lines)\n", edid->descriptors[i].detailed_timing.border_tb);
    		printf("      interlaced    : %d\n", edid->descriptors[i].detailed_timing.interlaced);

    		if (edid->descriptors[i].detailed_timing.stereo == STEREO_NONE) {
    			printf("      stereo        : none\n");
    		} else {
        		printf("      stereo        : ");

        		if (edid->descriptors[i].detailed_timing.stereo & STEREO_FIELD_SEQUENTIAL_RIGHT_SYNC)
            		printf("field_seq_right ");
        		if (edid->descriptors[i].detailed_timing.stereo & STEREO_FIELD_SEQUENTIAL_LEFT_SYNC)
            		printf("field_seq_left ");
        		if (edid->descriptors[i].detailed_timing.stereo & STEREO_2WAY_INTERLEAVE_RIGHT_EVEN)
            		printf("2way_interleave_right ");
        		if (edid->descriptors[i].detailed_timing.stereo & STEREO_2WAY_INTERLEAVE_LEFT_EVEN)
            		printf("2way_interleave_left ");
        		if (edid->descriptors[i].detailed_timing.stereo & STEREO_4WAY_INTERLEAVE)
            		printf("4way_interleave ");
        		if (edid->descriptors[i].detailed_timing.stereo & STEREO_SIDE_BY_SIDE_INTERLEAVE)
            		printf("side-by-side ");

        		printf("\n");
    		}

    		if (edid->descriptors[i].detailed_timing.sync & A_COMPOSITE_SYNC) {

    			printf("      analog sync   : composite");

        		if (edid->descriptors[i].detailed_timing.sync & A_COMPOSITE_SYNC)
            		printf(" bipolar");

        		if (edid->descriptors[i].detailed_timing.sync & A_BIPOLAR_SERRATION) {
            		printf(" with_serrations");
        			if (edid->descriptors[i].detailed_timing.sync & A_BIPOLAR_SERRATION_SYNC_G)
						printf(" syn_on_green");
					if (edid->descriptors[i].detailed_timing.sync & A_BIPOLAR_SERRATION_SYNC_RGB)
						printf(" syn_on_rgb");
        		} else {
            		printf(" without_serrations");
        		}

				printf("\n");

    		} else if (edid->descriptors[i].detailed_timing.sync & D_COMPOSITE_SYNC) {

    			printf("      digital sync  : composite");

        		if (edid->descriptors[i].detailed_timing.sync & D_COMPOSITE_SERRATION)
            		printf(" with_serrations");
        		else
        			printf(" without_serrations");

        		printf("\n");

    		} else if (edid->descriptors[i].detailed_timing.sync & D_SEPERATE_SYNC) {

    			printf("      digital sync  : seperate");

				if (edid->descriptors[i].detailed_timing.sync & D_SEPERATE_VSYNC_N)
					printf(" vsync_n");
				else
					printf(" vsync_p");

				if (edid->descriptors[i].detailed_timing.sync & D_SEPERATE_HSYNC_N)
					printf(" hsync_n");
				else
					printf(" hsync_p");

        		printf("\n");
    		}

    	} else if (edid->descriptors[i].type == DT_DISPLAY_DESCRIPTOR) {

    		printf("\n  [%d] DD -- display descriptor\n", i);

        	switch (edid->descriptors[i].tag) {  // tag
            case DTAG_PRODUCT_SERIAL:
            	printf("      product serial : %s\n", edid->descriptors[i].serial);
                break;
            case DTAG_DATA_STRING:
            	printf("      data string : %s\n", edid->descriptors[i].string);
                break;
            case DTAG_RANGE_LIMIT:
            	edid->descriptors[i].tag = DTAG_RANGE_LIMIT;

        		printf("      range limits and additional timing ...\n");

        		printf("      horizonatal rate minimum : %d (kHz)\n", edid->descriptors[i].range_limits.h_min);
        		printf("      horizontal rate maximum  : %d (KHz)\n", edid->descriptors[i].range_limits.h_max);

        		switch (edid->descriptors[i].range_limits.h_offset) {
            	case HRO_ZERO:
            		printf("      horizontal rate offset   : no offset\n");
            		break;
            	case HRO_MAX_255_MIN_ZERO:
            		printf("      horizontal rate offset   : max)+255kHz\n");
            		break;
            	case HRO_MAX_255_MIN_255:
            		printf("      horizontal rate offset   : max)+255kHz min)+255kHz\n");
            		break;
            	}

        		printf("      vertical rate minimum    : %d (Hz)\n", edid->descriptors[i].range_limits.v_min);
        		printf("      vertical rate maximum    : %d (Hz)\n", edid->descriptors[i].range_limits.v_max);

        		switch (edid->descriptors[i].range_limits.v_offset) {
            	case VRO_ZERO:
            		printf("      vertical rate offset     : no offset\n");
            		break;
            	case VRO_MAX_255_MIN_ZERO:
            		printf("      vertical rate offset     : max)+255Hz\n");
            		break;
            	case VRO_MAX_255_MIN_255:
            		printf("      vertical rate offset     : max)+255Hz && min)+255Hz\n");
            		break;
            	}

        		printf("      pixel clock maximum      : %d (MHz)\n", edid->descriptors[i].range_limits.p_max);

                // additional timing support flags
            	if (edid->descriptors[i].range_limits.timing_flags == 0) {
            		printf("      additional timing flags  : none\n");
            	} else {
            		printf("      additional timing flags  : ");

            		if (edid->descriptors[i].range_limits.timing_flags & TF_DEFAULT_GTF)
                		printf("default_GTF ");
            		if (edid->descriptors[i].range_limits.timing_flags & TF_RANGE_LIMITS)
                		printf("none ");
            		if (edid->descriptors[i].range_limits.timing_flags & TF_SECOND_GTF)
                		printf("second_GTF ");
            		if (edid->descriptors[i].range_limits.timing_flags & TF_CVT)
                		printf("CVT ");

            		printf("\n");
            	}

                break;

            case DTAG_PRODUCT_NAME:
            	printf("      data string : %s\n", edid->descriptors[i].prod_name);
                break;
            case DTAG_COLOR_POINT:
            	printf("      color point ...\n");

            	printf("        white point 0 : index)%d x)%d y)%d gamma)%.2f\n",
            		edid->descriptors[i].white_point[0].index,
					edid->descriptors[i].white_point[0].x,
					edid->descriptors[i].white_point[0].y,
					edid->descriptors[i].white_point[0].gamma);

            	printf("        white point 1 : index)%d x)%d y)%d gamma)%.2f\n",
            		edid->descriptors[i].white_point[1].index,
					edid->descriptors[i].white_point[1].x,
					edid->descriptors[i].white_point[1].y,
					edid->descriptors[i].white_point[1].gamma);

                break;

            case DTAG_STANDARD_TIMING:
            {
            	int j;

            	printf("      extra standard timing ...\n");

            	for (j = 0; j < 6; ++j) {
            		if (edid->descriptors[i].extra_standard_timings[j].refresh_rate) {
            			printf("  [%02d] %03d @ %02d Hz ", j + 9, edid->descriptors[i].extra_standard_timings[j].horizontal, edid->descriptors[i].extra_standard_timings[j].refresh_rate);
            			switch (edid->descriptors[i].extra_standard_timings[j].aspect_ratio) {
            			case ST_AR_16_10:
            				printf("16:10\n");
            				break;
            			case ST_AR_4_3:
            				printf("4:3\n");
            				break;
            			case ST_AR_5_4:
            				printf("5:4\n");
            				break;
            			case ST_AR_16_9:
            				printf("16:9\n");
            				break;
            			default:
            				printf("\n");
            				break;
            			}
            		}
            	}

                break;
            }

            case DTAG_DCM_DATA:
            	// display color management data
            	printf("      DCM -- display color management data ... (skipped)\n");
            	break;

            case DTAG_CVT_TIMING:
            	printf("      CVT -- coordinated video timing ... (skipped)\n");
                break;
            case DTAG_ESTABLISHED_TIMING_III:
            {
            	int j ;

            	printf("      established timing 3 ...\n");

            	for (j = 0; j < 17; ++j) {
            		if (edid->descriptors[i].timing_3[j].valid)
            			printf("  [%02d] %03d x %03d @ %02d Hz %s %s\n", i, edid->descriptors[i].timing_3[j].width, edid->descriptors[i].timing_3[j].height, edid->descriptors[i].timing_3[j].refresh, edid->descriptors[i].timing_3[j].interlaced ? "I" : " ", edid->descriptors[i].timing_3[j].rb ? "RB" : " ");
            	}

            	break;
            }
            case DTAG_DUMMY:
                break;
            default:
                break;
            }
    	}
	}

	for (i = 0; i < 8; ++i) {
		switch (edid->ext_data[i].ext_tag) {
		case EDID_EXT_TAG_CEA:
			edid_print_cea(edid, i);
			break;
		}
	}
}
