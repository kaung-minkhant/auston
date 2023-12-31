
//=======================================================
//  This code is generated by Terasic System Builder
//=======================================================

module DECA_Humidity_Temperature_Measurement(

	//////////// CLOCK //////////
	input 		          		ADC_CLK_10,
	input 		          		MAX10_CLK1_50,
	input 		          		MAX10_CLK2_50,

	//////////// KEY //////////
	input 		     [1:0]		KEY,

	//////////// LED //////////
	output		     [7:0]		LED,

	//////////// Humidity and Temperature Sensor //////////
	input 		          		RH_TEMP_DRDY_n,
	output		          		RH_TEMP_I2C_SCL,
	inout 		          		RH_TEMP_I2C_SDA,

	//////////// SW //////////
	input 		     [1:0]		SW
);

//=======================================================
//  REG/WIRE declarations
//=======================================================

//----Humidity_Temperature Set ---	
wire [15:0] CONFIGURATION ;

//----Humidity_Temperature Result---	
wire [15:0] DEVICE_ID ; 
wire [15:0] MANUFACTURER_ID ;
wire [15:0] TEMP_CURRENT ;   
wire [15:0] RH_CURRENT ;

//---FOR SYSTEM RESET --	
wire RESET_N ; 

//---I2C CLOCK --	
wire CLK_400K ;

//---FOR CLOCK TEST --	
wire CK50_1_1HZ ; 
wire CK50_2_1HZ ; 
wire AD_1HZ ; 
wire I2C_1HZ ; 

//---FOR SIGNALTAPEII--	
wire [7:0] ST ;
wire [7:0] CNT;
wire       W_WORD_END ; 
wire       W_WORD_GO ;
wire [7:0] WORD_ST;
wire [7:0] WORD_CNT;
wire [7:0] WORD_BYTE;	
wire [15:0]R_DATA ;

//=======================================================
//  Structural coding
//=======================================================


//---- RESET ---
assign RESET_N =KEY[0];

//---- Power Monitor IC Connfiguration ---
assign CONFIGURATION =  16'h1000;
	
//----Humidity_Temperature ---	
RH_TEMP ctl( 
   //--SYSTEM--   
	.RESET_N 			(RESET_N),
   .CLK_50           (MAX10_CLK1_50),
	//--IC SIDE-- 
	.RH_TEMP_DRDY_n   (RH_TEMP_DRDY_n),   
	.RH_TEMP_I2C_SCL  (RH_TEMP_I2C_SCL),
	.RH_TEMP_I2C_SDA  (RH_TEMP_I2C_SDA),  
	//<Configuration>
	.Configuration    (CONFIGURATION  ),
	//<Temperature>
	.Temperature      (TEMP_CURRENT   ), 
	//<Humidity>
	.Humidity         (RH_CURRENT     ), 
	//<ID>
	.DeviceID         (DEVICE_ID      ),
	.ManufacturerID   (MANUFACTURER_ID),
	
	//---FOR TEST--
	.CLK_400K  (CLK_400K),
	.I2C_LO0P  (I2C_LO0P ),	
	.ST        (ST),
   .CNT       (CNT) ,
	.W_WORD_END(W_WORD_END)  ,
   .W_WORD_GO (W_WORD_GO ) ,
   .WORD_ST   (WORD_ST  ),
   .WORD_CNT  (WORD_CNT ),
   .WORD_BYTE (WORD_BYTE),
   .R_DATA    (R_DATA)	
	);
	
//---FOR SIGNALTAPEII--		
TP	 t (
	.I2C_SCL    (RH_TEMP_I2C_SCL),
	.I2C_SDA    (RH_TEMP_I2C_SDA),  
   .R_DATA     (R_DATA),
   .WORD_ST    (WORD_ST  ),
   .WORD_CNT   (WORD_CNT  ),
   .WORD_BYTE  (WORD_BYTE),
   .W_WORD_END (W_WORD_END ) ,
   .W_WORD_GO  (W_WORD_GO ),
   .RESET_N    (RESET_N) ,
   .SYS_CLK    (CLK_400K) ,
   .ST         (ST ),
   .CNT        (CNT),	
	.Configuration  (CONFIGURATION[15:0] ),
	.Temperature    (TEMP_CURRENT [15:0]  ),
	.Humidity       (RH_CURRENT [15:0]  ), 
	.Temperature_S  ((TEMP_CURRENT*165)/65536 -40 ),
	.Humidity_S     ((RH_CURRENT *100) /65536 ),      
	.DeviceID       (DEVICE_ID),
	.ManufacturerID (MANUFACTURER_ID),
	.RH_TEMP_DRDY_n (RH_TEMP_DRDY_n)
);

//---CLOCK TEST--
//CLOCKMEM C2  (  .CLK  (MAX10_CLK1_50) , .CLK_FREQ (50000000), . CK_1HZ (CK50_1_1HZ )  ) ; //50MHZ
//CLOCKMEM C3  (  .CLK  (MAX10_CLK2_50) , .CLK_FREQ (50000000), . CK_1HZ (CK50_2_1HZ )  ) ; //50MHZ
//CLOCKMEM C4  (  .CLK  (ADC_CLK_10)    , .CLK_FREQ (10000000), . CK_1HZ (AD_1HZ)  ) ;      //10MHZ
//CLOCKMEM C5  (  .CLK  (CLK_400K)      , .CLK_FREQ (400000)  , . CK_1HZ (I2C_1HZ )  ) ;    //400KHZ

//-- OUTPUT LED --
assign LED [7:0]  = 8'hff ^ {  I2C_1HZ, AD_1HZ,  CK50_2_1HZ , CK50_1_1HZ   } ;

endmodule
