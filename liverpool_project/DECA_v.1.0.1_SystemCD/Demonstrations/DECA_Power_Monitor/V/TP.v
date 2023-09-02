module TP	 (
	input 		SYS_CLK ,
	input [7:0]	ST ,
	input [7:0]	CNT ,
	input 		RESET_N ,
	input [15:0]R_DATA, 
   input [15:0]Configuration,
   input [15:0]Calibration  ,  
   input [15:0]Mask_Enable  ,  
   input [15:0]Alert_Limit  , 
   input [15:0]Shunt_Voltage,
   input [15:0]Bus_Voltage,  
   input [15:0]Power,        
   input [15:0]Current,
   input [15:0]Die_ID,  
	input 		W_WORD_END,
	input 		W_WORD_GO ,
	input 		PMONITOR_I2C_SCL,
	input 		PMONITOR_I2C_SDA,
	input  [7:0]WORD_ST,
	input  [7:0]WORD_CNT,
	input  [7:0]WORD_BYTE
);
endmodule
