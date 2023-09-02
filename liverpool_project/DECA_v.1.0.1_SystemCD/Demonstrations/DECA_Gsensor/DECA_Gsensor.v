
//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

module DECA_Gsensor(

	//////////// CLOCK //////////
	input 		          		ADC_CLK_10,
	input 		          		MAX10_CLK1_50,
	input 		          		MAX10_CLK2_50,

	//////////// KEY //////////
	input 		     [1:0]		KEY,

	//////////// LED //////////
	output		     [7:0]		LED,

	//////////// G-Sensor //////////   for SPI
	output		          		G_SENSOR_CS_n, // SPI is LOW
	input 		          		G_SENSOR_INT1,  
	input 		          		G_SENSOR_INT2,
	inout 		          		G_SENSOR_SCLK, 
	inout 		          		G_SENSOR_SDI,  // data to G-SENSOR  
	inout 		          		G_SENSOR_SDO,  // data from G-SENSOR  

	//////////// SW //////////
	input 		     [1:0]		SW
);


//=======================================================
//  REG/WIRE declarations
//=======================================================

wire [15:0]   OUT_X ; 
wire [15:0]   OUT_Y ; 
wire [15:0]   OUT_Z ; 
wire [7:0]    WHO_AM_I ;
wire  [16:0]  s_data_x  ;  
wire  [16:0]  data_x ;
wire  [7:0]   led_out ;
wire          reset_n ; 
wire          DATA_RDY ; 

//=======================================================
//  Structural coding
//=======================================================
//---- reset  --- 
assign  reset_n = KEY [0]; 

//---G-Sensor for SPI Controller ----  
SPI_CTL spi(
 .DATA_RDY (DATA_RDY),
 .RESET_N  (reset_n ), 
 .CLK_50   (MAX10_CLK1_50), 
 .OUT_X    (OUT_X   ) ,
 .OUT_Y    (OUT_Y   ) ,
 .OUT_Z    (OUT_Z   ) ,
 .WHO_AM_I (WHO_AM_I) ,
 .CS     (G_SENSOR_CS_n),
 .SCLK   (G_SENSOR_SCLK ) ,
 .DIN    (G_SENSOR_SDI),
 .DO     (G_SENSOR_SDO)
 );  

//OUT_X : two’s complement left-justified.
//s_data_x :Shift OUT_X to positive number
assign s_data_x      = 32768  +   OUT_X ;
assign data_x [11:0] = s_data_x[15:4] ; 

//-----LED Processor----
led_driver u_led_driver	(
    .iRSTN    ( reset_n),
    .iCLK     ( MAX10_CLK1_50 ),
    .iDIG     ( data_x[10:1]),
    .iG_INT2  ( DATA_RDY ) ,
    .oLED     ( led_out),
    .fine_tune( ~KEY[0] )  );

//--LED Output--	 
assign LED[7:0] = 8'hff ^ {led_out[0],led_out[1],led_out[2],led_out[3],led_out[4],led_out[5],led_out[6],led_out[7] }  ;



endmodule
