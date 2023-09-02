module TP	 (
   input 		SYS_CLK ,
   input [7:0]	ST ,
   input [7:0]	CNT ,
   input 		RESET_N ,
   input [15:0]R_DATA,
	input 		RH_TEMP_DRDY_n,
   input [15:0]Configuration , 
	input [15:0]Temperature, 	
	input [15:0]Humidity,    	
   input [13:0]Temperature_S ,
	input [13:0]Humidity_S ,      	
	input [15:0]DeviceID,
	input [15:0]ManufacturerID,
   input       W_WORD_END,
   input       W_WORD_GO ,
   input       I2C_SCL,  
   input       I2C_SDA,  
   input [7:0] WORD_ST,
   input [7:0] WORD_CNT,
   input [7:0] WORD_BYTE
);
endmodule
