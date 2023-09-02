#include <stdio.h>
#include <unistd.h>
#include "altera_modular_adc_sequencer_regs.h"
#include "altera_modular_adc_sample_store_regs.h"
#include "altera_modular_adc.h"


int main()
{
	int value_0, value_1, Slot=0/* 0: ANAIN1_SMA(J18) adc channel */;
	float Vout_0, Vin_0, Vout_1, Vin_1;
	printf("SMA ADC Demo\n");

	IOWR(MODULAR_ADC_0_SEQUENCER_CSR_BASE, 0x00, 0x00); // Continued ADC
    IOWR(MODULAR_ADC_0_SEQUENCER_CSR_BASE, 0x00, 0x01); // run

	IOWR(MODULAR_ADC_1_SEQUENCER_CSR_BASE, 0x00, 0x00); // Continued ADC
    IOWR(MODULAR_ADC_1_SEQUENCER_CSR_BASE, 0x00, 0x01); // run

	usleep(10*1000);

    while(1){
    	value_0 = IORD(MODULAR_ADC_0_SAMPLE_STORE_CSR_BASE, Slot) & 0xFFF; // 12-bit data
    	value_1 = IORD(MODULAR_ADC_1_SAMPLE_STORE_CSR_BASE, Slot) & 0xFFF; // 12-bit data

    	// SMA input,calculate input voltage
    	//  Vout=(6.25+Vin)/5
    	//  Vin = 5xVout - 6.25

    	Vout_0 = (float)value_0/4095.0*2.5; // 0~4095 mapping to 0~2.5V
    	Vin_0 = 5.0* Vout_0 - 6.25;

    	Vout_1 = (float)value_1/4095.0*2.5; // 0~4095 mapping to 0~2.5V
    	Vin_1 = 5.0* Vout_1 - 6.25;

    	printf("Vin_0=%dmV, Vout_0=%dmV(%xh). Vin_1=%dmV, Vout_1=%dmV(%xh)\r\n", (int)(Vin_0*1000.0), (int)(Vout_0*1000.0), value_0, (int)(Vin_1*1000.0), (int)(Vout_1*1000.0), value_1);
    	//printf("adc value=%xh\r\n", value);

    	//
    	usleep(10*1000); // 0.01 second
    }


  return 0;
}
