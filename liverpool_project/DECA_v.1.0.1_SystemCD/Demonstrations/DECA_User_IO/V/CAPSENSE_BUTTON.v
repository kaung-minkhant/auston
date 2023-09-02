module CAPSENSE_BUTTON(
    input clk, // 50 MHz
    input reset_n,
    input low_active, // button_x is low-active?

    output button_0,
    output button_1,

    // export
    output i2c_scl,
    inout  i2c_sda
);

CAP_CTRL rt1( 
   .RESET_N (reset_n),
   .CLK_50  (clk),
	.CAP_SENSE_I2C_SCL(i2c_scl),
	.CAP_SENSE_I2C_SDA(i2c_sda),	
	.SW_ACTIVE_POLARITY(low_active),
	.F_STAT            ( { button_1 , button_0 }   ) ,
   .SW_PROX_EN        (0) ,
	
	);

endmodule


