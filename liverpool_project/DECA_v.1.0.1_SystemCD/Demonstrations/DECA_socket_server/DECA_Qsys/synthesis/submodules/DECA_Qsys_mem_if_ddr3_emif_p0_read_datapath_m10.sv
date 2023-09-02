// (C) 2001-2015 Altera Corporation. All rights reserved.
// Your use of Altera Corporation's design tools, logic functions and other 
// software and tools, and its AMPP partner logic functions, and any output 
// files any of the foregoing (including device programming or simulation 
// files), and any associated documentation or information are expressly subject 
// to the terms and conditions of the Altera Program License Subscription 
// Agreement, Altera MegaCore Function License Agreement, or other applicable 
// license agreement, including, without limitation, that your use is for the 
// sole purpose of programming logic devices manufactured by Altera and sold by 
// Altera or its authorized distributors.  Please refer to the applicable 
// agreement for further details.


// ******************************************************************************************************************************** 
// File name: read_datapath.sv
// The read datapath is responsible for read data resynchronization from the memory clock domain to the AFI clock domain.
// It contains 1 FIFO per DQS group for read valid prediction and 1 FIFO per DQS group for read data synchronization.
// ******************************************************************************************************************************** 

`timescale 1 ps / 1 ps

(* altera_attribute = "-name AUTO_SHIFT_REGISTER_RECOGNITION OFF" *)

// altera message_off 10036 10030 10858
module DECA_Qsys_mem_if_ddr3_emif_p0_read_datapath_m10(
    pll_afi_clk,
    reset_n_afi_clk,
    read_capture_clk_hr_dq,
    rdata_hr,
    afi_rdata_en,
    afi_rdata,
    afi_rdata_valid,
    seq_read_fifo_reset,
    seq_read_latency_counter,
    seq_read_increment_vfifo
);

// ********************************************************************************************************************************
// BEGIN PARAMETER SECTION
// All parameters default to "" will have their values passed in from higher level wrapper with the controller and driver

parameter DEVICE_FAMILY = "";

// PHY-Memory Interface
parameter MEM_ADDRESS_WIDTH     = "";
parameter MEM_DM_WIDTH          = "";
parameter MEM_CONTROL_WIDTH     = "";
parameter MEM_DQ_WIDTH          = "";
parameter MEM_READ_DQS_WIDTH    = "";
parameter MEM_WRITE_DQS_WIDTH   = "";

// PHY-Controller (AFI) Interface
parameter AFI_ADDRESS_WIDTH         = "";
parameter AFI_DATA_MASK_WIDTH       = "";
parameter AFI_CONTROL_WIDTH         = "";
parameter AFI_DATA_WIDTH            = "";
parameter AFI_DQS_WIDTH             = "";
parameter AFI_RATE_RATIO            = "";
parameter NUM_OF_DQDQS              = "";
parameter DQDQS_DATA_WIDTH          = "";
parameter MAX_READ_LATENCY          = "";
parameter MAX_LATENCY_COUNT_WIDTH   = "";
parameter DEVICE_WIDTH              = 1;
parameter MEM_T_RL                  = "";

// Local parameters
localparam DDIO_PHY_DQ_WIDTH = AFI_DATA_WIDTH/NUM_OF_DQDQS;


// END PARAMETER SECTION
// ******************************************************************************************************************************** 

input                                       reset_n_afi_clk;
input                                       pll_afi_clk;
input           [AFI_DATA_WIDTH-1:0]        rdata_hr;

input           [MEM_READ_DQS_WIDTH-1:0]    seq_read_fifo_reset; // reset from sequencer to read and write pointers of the data resynchronization FIFO
input           [MEM_READ_DQS_WIDTH-1:0]    seq_read_increment_vfifo;
input           [MAX_LATENCY_COUNT_WIDTH-1:0] seq_read_latency_counter;
input           [MEM_READ_DQS_WIDTH-1:0]    read_capture_clk_hr_dq;


input           [AFI_RATE_RATIO-1:0]        afi_rdata_en;

output          [AFI_DATA_WIDTH-1:0]        afi_rdata;
output          [AFI_RATE_RATIO-1:0]        afi_rdata_valid;

logic           [AFI_DATA_WIDTH-1:0]        rdata_reorder;
logic           [NUM_OF_DQDQS-1 : 0]        vfifo_output;

reg             [MEM_READ_DQS_WIDTH-1:0]    read_enable_from_lfifo;
wire            [MEM_READ_DQS_WIDTH-1:0]    rdata_fifo_full;
wire            [MEM_READ_DQS_WIDTH-1:0]    lfifo_full;

// VFIFO read increment per capture clock

// VFIFO pointer

logic [MAX_READ_LATENCY-1:0] vfifo;

always_ff @ (posedge pll_afi_clk, negedge reset_n_afi_clk)
    begin: vfifo_shifter
        if (!reset_n_afi_clk)
            vfifo <= 0;
        else
            vfifo <= {vfifo[MAX_READ_LATENCY-2:0], afi_rdata_en[0]};
    end

logic [2:0] vfifo_pointer;

always_ff @ (posedge pll_afi_clk, negedge reset_n_afi_clk)
    begin: vfifo_pointer_gen
        if (!reset_n_afi_clk)
            vfifo_pointer <= 0;
        else if (seq_read_increment_vfifo[0])
            vfifo_pointer <= vfifo_pointer + 1'b1;
    end

localparam VFIFO_OFFSET = MAX_READ_LATENCY - 8;
always_ff @ (posedge pll_afi_clk, negedge reset_n_afi_clk)
    begin: vfifo_rden
        if (!reset_n_afi_clk)
            vfifo_output <= 0;
        else
            vfifo_output <= vfifo[vfifo_pointer+VFIFO_OFFSET];
    end

logic [3:0] reset_counter;
logic	write_req;
logic	read_req;

always_ff @ (posedge pll_afi_clk, negedge reset_n_afi_clk)
    begin
        if (!reset_n_afi_clk)
			begin
				write_req	<=	0;
				read_req		<=	0;
				reset_counter	<=	'0;
			end
		else
			begin
				if (!(&reset_counter))
					reset_counter	<=	reset_counter + 1'b1;
				
				if (reset_counter > 5)
					write_req	<=	1;
				
				if (reset_counter > 7)
					read_req		<=	1;
			end
	end

// HR rdata clock cross into afi_clk domain

wire [DDIO_PHY_DQ_WIDTH-1:0] read_fifo_output_per_dqs;

    dcfifo    rdata_fifo (
                .data (rdata_hr),
                .rdclk (pll_afi_clk),
                .rdreq (read_req),
                .wrclk (read_capture_clk_hr_dq[0]),
                .wrreq (write_req),
                .q (rdata_reorder),
                .rdempty (),
                .wrfull (),
                .aclr (~reset_n_afi_clk),
                .rdfull (),
                .rdusedw (),
                .wrempty (),
                .wrusedw ());
    defparam
        rdata_fifo.intended_device_family = DEVICE_FAMILY,
        rdata_fifo.lpm_numwords = 8,
        rdata_fifo.lpm_showahead = "OFF",
        rdata_fifo.lpm_type = "dcfifo",
        rdata_fifo.lpm_width = AFI_DATA_WIDTH,
        rdata_fifo.lpm_widthu = 3,
        rdata_fifo.overflow_checking = "ON",
        rdata_fifo.rdsync_delaypipe = 3,
        rdata_fifo.underflow_checking = "ON",
        rdata_fifo.use_eab = "ON",
        rdata_fifo.wrsync_delaypipe = 3;

// Read data reorder
generate
genvar i;
for (i=0; i<NUM_OF_DQDQS; i=i+1)
begin: rdata_per_dq_group

        assign afi_rdata[DQDQS_DATA_WIDTH*(i+1+0*NUM_OF_DQDQS)-1 : DQDQS_DATA_WIDTH*(i+0*NUM_OF_DQDQS)] = rdata_reorder[DQDQS_DATA_WIDTH*(i*4+1)-1 : DQDQS_DATA_WIDTH*(i*4+0)];
        assign afi_rdata[DQDQS_DATA_WIDTH*(i+1+1*NUM_OF_DQDQS)-1 : DQDQS_DATA_WIDTH*(i+1*NUM_OF_DQDQS)] = rdata_reorder[DQDQS_DATA_WIDTH*(i*4+2)-1 : DQDQS_DATA_WIDTH*(i*4+1)];
        assign afi_rdata[DQDQS_DATA_WIDTH*(i+1+2*NUM_OF_DQDQS)-1 : DQDQS_DATA_WIDTH*(i+2*NUM_OF_DQDQS)] = rdata_reorder[DQDQS_DATA_WIDTH*(i*4+3)-1 : DQDQS_DATA_WIDTH*(i*4+2)];
        assign afi_rdata[DQDQS_DATA_WIDTH*(i+1+3*NUM_OF_DQDQS)-1 : DQDQS_DATA_WIDTH*(i+3*NUM_OF_DQDQS)] = rdata_reorder[DQDQS_DATA_WIDTH*(i*4+4)-1 : DQDQS_DATA_WIDTH*(i*4+3)];

end
endgenerate

assign afi_rdata_valid = {2{vfifo_output}};


endmodule
