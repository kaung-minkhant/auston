
//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

module DECA_User_IO(

	//////////// CLOCK //////////
	input 		          		ADC_CLK_10,
	input 		          		MAX10_CLK1_50,
	input 		          		MAX10_CLK2_50,

	//////////// KEY //////////
	input 		     [1:0]		KEY,

	//////////// LED //////////
	output		     [7:0]		LED,

	//////////// CapSense Button //////////
	inout 		          		CAP_SENSE_I2C_SCL,
	inout 		          		CAP_SENSE_I2C_SDA,

	//////////// SW //////////
	input 		     [1:0]		SW
);




//=======================================================
//  REG/WIRE declarations
//=======================================================
wire CK50_1_1HZ ; 
wire CK50_2_1HZ ; 
wire button_0,button_1 ;

//=======================================================
//  Structural coding
//=======================================================

//---CLOCK TEST--
CLOCKMEM C2  (  .CLK  (MAX10_CLK1_50) , .CLK_FREQ (50000000), . CK_1HZ (CK50_1_1HZ )  ) ;
CLOCKMEM C3  (  .CLK  (MAX10_CLK2_50) , .CLK_FREQ (50000000), . CK_1HZ (CK50_2_1HZ )  ) ;


//---CAPSENSE--
CAPSENSE_BUTTON ip (
    .clk        (MAX10_CLK1_50), // 50 MHz
    .reset_n    (1),
    .low_active (1), 
    .button_0   (button_0),
    .button_1   (button_1),
    // export
    .i2c_scl    (CAP_SENSE_I2C_SCL),
    .i2c_sda    (CAP_SENSE_I2C_SDA)
);

//--OUTPUT LED --
assign  LED = { 6'h3f ^ { button_1, button_0 ,CK50_1_1HZ ,CK50_2_1HZ,  SW[1:0] } , KEY[1:0] } ; 
 
endmodule