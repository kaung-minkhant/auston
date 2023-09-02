module CAP_CTRL ( 
   input  RESET_N , 
   input  CLK_50 ,
	output CAP_SENSE_I2C_SCL,   
	inout  CAP_SENSE_I2C_SDA,   
//----SET IN --- 	
   input        SW_ACTIVE_POLARITY,
   input  [15:0]SW_PROX_EN,
   input  [15:0]SW_PROX_TOUCH_TH0,
   input  [15:0]SW_PROX_TOUCH_TH1,

//----Test or ST-BUS --- 
	output reg [15:0]Device_ID  , 
	output reg [15:0]Family_ID  ,        
	output reg [15:0]Device_REV ,
	output reg [15:0]Button_STAT,
	output reg [15:0]Prox_STAT , 
	output reg [15:0]Prox_EN, 
   output reg [15:0]PROX_TOUCH_TH0,
   output reg [15:0]PROX_TOUCH_TH1,	
   output reg [15:0]BASE_THRESHOLD0,	
   output reg [15:0]BASE_THRESHOLD1,
	output     [15:0]F_STAT,
	output reg [15:0]PROX_POSITIVE_TH0,
	
	//test
	output reg  CLK_400K ,
   output reg  I2C_LO0P,
   output reg [7:0] ST ,
   output reg [7:0] CNT,
	output           W_WORD_END ,
   output reg       W_WORD_GO ,
	
	output [7:0] WORD_ST,
	output [7:0] WORD_CNT,
	output [7:0] WORD_BYTE	,
   output [15:0]R_DATA,
	output SDAI_W 
	
	);
assign F_STAT[0] =  Button_STAT[0] ;
assign F_STAT[1] =  Button_STAT[1] ;
	
//-- I2C clock 400k generater 
reg  [31:0]C_DELAY ;
always @(posedge CLK_50 ) begin 
  if ( C_DELAY > 125/2 ) begin  
    CLK_400K<=~CLK_400K ; C_DELAY <= 0 ; 
  end 
  else C_DELAY <=C_DELAY +1 ; 
end
  
//======== Main-ST =======
//==Pointer NUM== 
parameter SLAVE_ADDR            =8'h6e;  
//-WRITE-
parameter P_LATCHED_BUTTON_STAT =8'hac;  
parameter P_PROX_EN             =8'h26;
parameter P_PROX_TOUCH_TH0      =8'h2a;
parameter P_PROX_TOUCH_TH1      =8'h2c;
parameter P_LATCHED_PROX_STAT   =8'haf;
parameter P_PROX_CFG            =8'h27;
parameter P_BASE_THRESHOLD0     =8'h0c;
parameter P_BASE_THRESHOLD1     =8'h0d;
parameter P_PROX_POSITIVE_TH0   =8'h35;


//-READ-
parameter P_DEVICE_ID           =8'h90;
parameter P_FAMILY_ID           =8'h8f;
parameter P_DEVICE_REV          =8'h92;
parameter P_BUTTON_STAT         =8'haa; 
parameter P_PROX_STAT           =8'hae;

reg [7:0] DELY ;

always @(negedge RESET_N or posedge CLK_400K )begin 
if (!RESET_N  ) ST <=0;
else 
case (ST)
0: begin 
   ST<=30; //Config Reg
	W_POINTER_GO <=1;
   R_GO  <=1 ;		 
	W_WORD_GO <=1;
   end
//<----------------READ -------	
1: begin 
   ST<=2; 
	end	
2: begin 
	     if ( CNT==5)  W_POINTER_REG <= P_DEVICE_ID;   
	else if ( CNT==1)  W_POINTER_REG <= P_FAMILY_ID;     
	else if ( CNT==2)  W_POINTER_REG <= P_DEVICE_REV;  
	else if ( CNT==3)  W_POINTER_REG <=  P_BUTTON_STAT; 
	else if ( CNT==4)  W_POINTER_REG <=  P_PROX_STAT ;
   else if ( CNT==0)  W_POINTER_REG <=  P_PROX_EN	;
   else if ( CNT==6)  W_POINTER_REG <=  P_PROX_TOUCH_TH0;
   else if ( CNT==7)  W_POINTER_REG <=  P_PROX_TOUCH_TH1;
   else if ( CNT==8)  W_POINTER_REG <=  P_BASE_THRESHOLD0;
   else if ( CNT==9)  W_POINTER_REG <=  P_BASE_THRESHOLD1;
	else if ( CNT==10) W_POINTER_REG <=  P_PROX_POSITIVE_TH0 ;
	if ( W_POINTER_END ) begin  W_POINTER_GO  <=0; ST<=3 ; DELY<=0;  end
	end                // Write ID pointer 
3: begin 
    DELY  <=DELY +1;
    if ( DELY ==2 ) begin 

   W_POINTER_GO  <=1;
   ST<=4 ; 
		 end

	end       
4: begin 
   if  ( W_POINTER_END ) ST<=5 ; 	
	end              
5: begin ST<=6 ; end //delay
	
//read DATA 		 
6: begin 
	if ( R_END ) begin  R_GO  <=0; ST<=7 ; DELY<=0; end
	end                
7: begin 
    DELY  <=DELY +1;
    if ( DELY ==2 ) begin 	 
    R_GO  <=1;
    ST<=8 ; 
	 end
	 
	end       
8: begin 
   ST<=9 ; 
	end       
	
9: begin 
   if  ( R_END ) begin 
	       if ( CNT==5) Device_ID  <=R_DATA ; 
	  else if ( CNT==1) Family_ID  <=R_DATA ; 
	  else if ( CNT==2) Device_REV <=R_DATA ;   
	  else if ( CNT==3) Button_STAT<=(SW_ACTIVE_POLARITY)?R_DATA:16'hffff^R_DATA ;   
	  else if ( CNT==4) Prox_STAT  <=(SW_ACTIVE_POLARITY)?R_DATA:16'hffff^R_DATA ;      
	  else if ( CNT==0) Prox_EN    <=R_DATA ; 
     else if ( CNT==6) PROX_TOUCH_TH0<=R_DATA ; 
     else if ( CNT==7) PROX_TOUCH_TH1<=R_DATA ; 
     else if ( CNT==8) BASE_THRESHOLD0<=R_DATA ; 
     else if ( CNT==9) BASE_THRESHOLD1<=R_DATA ; 
     else if ( CNT==10)PROX_POSITIVE_TH0<=R_DATA ;	  

	  ST<=10 ; 	
	  CNT<=CNT+1 ;
	 end
	end              
10: begin      
     if (CNT ==11) begin CNT = 0 ;  ST<=30; end
	  else ST<=1; 
	  W_POINTER_GO <=1;
     R_GO         <=1 ;		 
	  W_WORD_GO    <=1; 
	 end 
//<----------------------------------READ-----------------------	 
 
//<----------------------------------WRITE WORD-----------------
30: begin 
    ST<=31; 
	 CNT<=0 ; 
    end	
31: begin 
             if ( CNT==0) {W_WORD_REG[7:0] ,W_WORD_DATA[15:0]} <= { P_PROX_EN[7:0] ,SW_PROX_EN[7:0] ,SW_PROX_EN[7:0] } ;
	     else if ( CNT==1) {W_WORD_REG[7:0] ,W_WORD_DATA[15:0]} <= { P_LATCHED_BUTTON_STAT[7:0] ,16'h0}; 		  
		  else if ( CNT==2) {W_WORD_REG[7:0] ,W_WORD_DATA[15:0]} <= { P_PROX_TOUCH_TH0[7:0],SW_PROX_TOUCH_TH0[15:0]};
		  else if ( CNT==3) {W_WORD_REG[7:0] ,W_WORD_DATA[15:0]} <= { P_PROX_TOUCH_TH1[7:0],SW_PROX_TOUCH_TH1[15:0]};     
		  else if ( CNT==4) {W_WORD_REG[7:0] ,W_WORD_DATA[15:0]} <= { P_LATCHED_PROX_STAT,16'h0 };  
		  else if ( CNT==5) {W_WORD_REG[7:0] ,W_WORD_DATA[15:0]} <= { P_PROX_CFG  ,16'hffff  } ; 
		  else if ( CNT==6) {W_WORD_REG[7:0] ,W_WORD_DATA[15:0]} <= { P_BASE_THRESHOLD0,16'h0000 };
		  else if ( CNT==7) {W_WORD_REG[7:0] ,W_WORD_DATA[15:0]} <= { P_BASE_THRESHOLD1,16'h0000 };
		  else if ( CNT==8) {W_WORD_REG[7:0] ,W_WORD_DATA[15:0]} <= { P_PROX_POSITIVE_TH0,16'h0000 } ;
		
	if (  W_WORD_END ) begin  W_WORD_GO  <=0; ST<=32 ;  DELY<=0;  end
	end                // Write ID pointer 
32: begin 
    DELY  <=DELY +1;
    if ( DELY ==2 ) begin 
    W_WORD_GO  <=1;
    ST<=33 ; 
	 end
	 
	end       
33: begin 
    ST<=34 ; 
	end       	
34: begin 
     if  ( W_WORD_END )  begin 
			ST<=35 ; 	
			CNT<=CNT+1 ;
	  end
	end              
35: begin 
    if (CNT ==9)  begin ST<=1 ;  CNT <=0; I2C_LO0P <= ~I2C_LO0P ; end 
	 else  ST<=31 ; 	 
	 end 
endcase 
end
//<-----------------------------MAIN-ST END ------------------------------------------
//I2C-BUS
wire   SDAO ; 
assign CAP_SENSE_I2C_SCL = W_POINTER_SCL  & R_SCL   & W_WORD_SCL;
assign SDAO              = W_POINTER_SDAO & R_SDAO  & W_WORD_SDAO;
assign CAP_SENSE_I2C_SDA = (SDAO)?1'bz :1'b0 ; 



//==== I2C WRITE WORD ===

wire   W_WORD_SCL ; 
reg    [7:0]  W_WORD_REG ; 
reg    [15:0] W_WORD_DATA;

wire   W_WORD_SDAO ;  

I2C_WRITE_WORD  wrd(
   .RESET_N (RESET_N),
	.PT_CK        (CLK_400K),
	.GO           (W_WORD_GO),
	.POINTER      (W_WORD_REG),
   .WDATA16	     (W_WORD_DATA),
	.SLAVE_ADDRESS(SLAVE_ADDR),
	.SDAI  (CAP_SENSE_I2C_SDA),
	.SDAO  (W_WORD_SDAO),
	.SCLO  (W_WORD_SCL ),
	.END_OK(W_WORD_END),
	//--for test 
	.ST  (WORD_ST ),
	.CNT (WORD_CNT),
	.BYTE(WORD_BYTE),
	.ACK_OK(),
	.SDAI_W (SDAI_W )
	
);

//==== I2C WRITE POINTER ===

wire   W_POINTER_SCL ; 
wire   W_POINTER_END ; 
reg    W_POINTER_GO ; 
reg    [7:0] W_POINTER_REG ; 
wire   W_POINTER_SDAO ;  

I2C_WRITE_POINTER  wpt(
   .RESET_N (RESET_N),
	.PT_CK        (CLK_400K),
	.GO           (W_POINTER_GO),
	.POINTER      (W_POINTER_REG),
	.SLAVE_ADDRESS(SLAVE_ADDR),
	.SDAI  (CAP_SENSE_I2C_SDA),
	.SDAO  (W_POINTER_SDAO),
	.SCLO  (W_POINTER_SCL ),
	.END_OK(W_POINTER_END),
	//--for test 
	.ST (),
	.ACK_OK(),
	.CNT (),
	.BYTE()  	
);


//==== I2C READ ===

wire R_SCL; 
wire R_END; 
reg  R_GO; 
wire R_SDAO;  
wire [15:0]R_DATA_;

//Swap Hi-8 and Low-8
assign     R_DATA = {R_DATA_[7:0], R_DATA_[15:8]} ; 

I2C_READ rd(
   .RESET_N (RESET_N),
	.PT_CK        (CLK_400K),
	.GO           (R_GO),
	.SLAVE_ADDRESS(SLAVE_ADDR),
	.SDAI  (CAP_SENSE_I2C_SDA),
	.SDAO  (R_SDAO),
	.SCLO  (R_SCL),
	.END_OK(R_END),
	.DATA16(R_DATA_),
	//--for test 
	.ST    (),
	.ACK_OK(),
	.CNT   (),
	.BYTE  ()  	
);
	
endmodule
	