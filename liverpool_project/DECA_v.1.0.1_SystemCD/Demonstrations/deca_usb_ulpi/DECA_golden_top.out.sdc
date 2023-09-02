## Generated SDC file "DECA_golden_top.out.sdc"

## Copyright (C) 1991-2014 Altera Corporation. All rights reserved.
## Your use of Altera Corporation's design tools, logic functions 
## and other software and tools, and its AMPP partner logic 
## functions, and any output files from any of the foregoing 
## (including device programming or simulation files), and any 
## associated documentation or information are expressly subject 
## to the terms and conditions of the Altera Program License 
## Subscription Agreement, the Altera Quartus II License Agreement,
## the Altera MegaCore Function License Agreement, or other 
## applicable license agreement, including, without limitation, 
## that your use is for the sole purpose of programming logic 
## devices manufactured by Altera and sold by Altera or its 
## authorized distributors.  Please refer to the applicable 
## agreement for further details.


## VENDOR  "Altera"
## PROGRAM "Quartus II"
## VERSION "Version 14.0.2 Build 209 09/17/2014 SJ Full Version"

## DATE    "Tue Oct 28 14:39:40 2014"

##
## DEVICE  "10M50DAF484C7G"
##


#**************************************************************
# Time Information
#**************************************************************

set_time_format -unit ns -decimal_places 3



#**************************************************************
# Create Clock
#**************************************************************

create_clock -name {altera_reserved_tck} -period 100.000 -waveform { 0.000 50.000 } [get_ports {altera_reserved_tck}]
create_clock -name {MAX10_CLK1_50} -period 20.000 -waveform { 0.000 10.000 } [get_ports {MAX10_CLK1_50}]
#create_clock -name {vir_osc_clk} -period 20.833 -waveform { 0.000 10.416 } 
create_clock -name {USB_CLKIN} -period 16.66 -waveform { 0.000 8.33 } [get_ports {USB_CLKIN}]

derive_pll_clocks
#**************************************************************
# Create Generated Clock
#**************************************************************

#create_generated_clock -name {u0|altpll_0|sd1|pll7|clk[0]} -source [get_pins {u0|altpll_0|sd1|pll7|inclk[0]}] -duty_cycle 50.000 -multiply_by 2 -master_clock {MAX10_CLK1_50} [get_pins {u0|altpll_0|sd1|pll7|clk[0]}] 
#create_generated_clock -name {u0|altpll_0|sd1|pll7|clk[2]} -source [get_pins {u0|altpll_0|sd1|pll7|inclk[0]}] -duty_cycle 50.000 -multiply_by 24 -divide_by 25 -master_clock {MAX10_CLK1_50} [get_pins {u0|altpll_0|sd1|pll7|clk[2]}] 


#**************************************************************
# Set Clock Latency
#**************************************************************



#**************************************************************
# Set Clock Uncertainty
#**************************************************************
#


#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {MAX10_CLK1_50}] -rise_to [get_clocks {MAX10_CLK1_50}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {MAX10_CLK1_50}] -fall_to [get_clocks {MAX10_CLK1_50}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {MAX10_CLK1_50}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -setup 0.070  
#set_clock_uncertainty -rise_from [get_clocks {MAX10_CLK1_50}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -hold 0.100  
#set_clock_uncertainty -rise_from [get_clocks {MAX10_CLK1_50}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -setup 0.070  
#set_clock_uncertainty -rise_from [get_clocks {MAX10_CLK1_50}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -hold 0.100  
#set_clock_uncertainty -fall_from [get_clocks {MAX10_CLK1_50}] -rise_to [get_clocks {MAX10_CLK1_50}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {MAX10_CLK1_50}] -fall_to [get_clocks {MAX10_CLK1_50}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {MAX10_CLK1_50}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -setup 0.070  
#set_clock_uncertainty -fall_from [get_clocks {MAX10_CLK1_50}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -hold 0.100  
#set_clock_uncertainty -fall_from [get_clocks {MAX10_CLK1_50}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -setup 0.070  
#set_clock_uncertainty -fall_from [get_clocks {MAX10_CLK1_50}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -hold 0.100  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -rise_to [get_clocks {MAX10_CLK1_50}] -setup 0.100  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -rise_to [get_clocks {MAX10_CLK1_50}] -hold 0.070  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -fall_to [get_clocks {MAX10_CLK1_50}] -setup 0.100  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -fall_to [get_clocks {MAX10_CLK1_50}] -hold 0.070  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[2]}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -rise_to [get_clocks {MAX10_CLK1_50}] -setup 0.100  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -rise_to [get_clocks {MAX10_CLK1_50}] -hold 0.070  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -fall_to [get_clocks {MAX10_CLK1_50}] -setup 0.100  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -fall_to [get_clocks {MAX10_CLK1_50}] -hold 0.070  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -rise_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}] -fall_to [get_clocks {u0|altpll_0|sd1|pll7|clk[0]}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {altera_reserved_tck}] -rise_to [get_clocks {altera_reserved_tck}]  0.020  
#set_clock_uncertainty -rise_from [get_clocks {altera_reserved_tck}] -fall_to [get_clocks {altera_reserved_tck}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {altera_reserved_tck}] -rise_to [get_clocks {altera_reserved_tck}]  0.020  
#set_clock_uncertainty -fall_from [get_clocks {altera_reserved_tck}] -fall_to [get_clocks {altera_reserved_tck}]  0.020  


#**************************************************************
# Set Input Delay
#**************************************************************

set_input_delay -add_delay  -clock [get_clocks {altera_reserved_tck}]  20.000 [get_ports {altera_reserved_tdi}]
set_input_delay -add_delay  -clock [get_clocks {altera_reserved_tck}]  20.000 [get_ports {altera_reserved_tms}]


#**************************************************************
# Set Output Delay
#**************************************************************

set_output_delay -add_delay  -clock [get_clocks {altera_reserved_tck}]  20.000 [get_ports {altera_reserved_tdo}]


#**************************************************************
# Set Clock Groups
#**************************************************************

set_clock_groups -asynchronous -group [get_clocks {altera_reserved_tck}] 
set_clock_groups -asynchronous -group [get_clocks [get_clocks {altera_reserved_tck}]] -group [get_clocks [get_clocks {osc_clk}] cc_soc|the_sys_pll|sd1|pll7|clk[0] vir_osc_clk] -group [get_clocks {cc_soc|the_sys_pll|sd1|pll7|clk[1]}] -group [get_clocks {cc_soc|the_sys_pll|sd1|pll7|clk[2]}] -group [get_clocks {u20_sr_clk_out}] 


#**************************************************************
# Set False Path
#**************************************************************

#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|jupdate}] -to [get_registers {*|alt_jtag_atlantic:*|jupdate1*}]
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|rdata[*]}] -to [get_registers {*|alt_jtag_atlantic*|td_shift[*]}]
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|read}] -to [get_registers {*|alt_jtag_atlantic:*|read1*}]
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|read_req}] 
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|rvalid}] -to [get_registers {*|alt_jtag_atlantic*|td_shift[*]}]
#set_false_path -from [get_registers {*|t_dav}] -to [get_registers {*|alt_jtag_atlantic:*|tck_t_dav}]
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|user_saw_rvalid}] -to [get_registers {*|alt_jtag_atlantic:*|rvalid0*}]
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|wdata[*]}] -to [get_registers *]
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|write}] -to [get_registers {*|alt_jtag_atlantic:*|write1*}]
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|write_stalled}] -to [get_registers {*|alt_jtag_atlantic:*|t_ena*}]
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|write_stalled}] -to [get_registers {*|alt_jtag_atlantic:*|t_pause*}]
#set_false_path -from [get_registers {*|alt_jtag_atlantic:*|write_valid}] 
#set_false_path -from [get_registers {*altera_avalon_st_clock_crosser:*|in_data_buffer*}] -to [get_registers {*altera_avalon_st_clock_crosser:*|out_data_buffer*}]
#set_false_path -to [get_keepers {*altera_std_synchronizer:*|din_s1}]
#set_false_path -to [get_pins -nocase -compatibility_mode {*|alt_rst_sync_uq1|altera_reset_synchronizer_int_chain*|clrn}]
#set_false_path -from [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_nios2_oci_break:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci_break|break_readreg*}] -to [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck|*sr*}]
#set_false_path -from [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_nios2_oci_debug:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci_debug|*resetlatch}] -to [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck|*sr[33]}]
#set_false_path -from [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_nios2_oci_debug:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci_debug|monitor_ready}] -to [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck|*sr[0]}]
#set_false_path -from [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_nios2_oci_debug:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci_debug|monitor_error}] -to [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck|*sr[34]}]
#set_false_path -from [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_nios2_ocimem:the_usb20sr_refdes_cpu_qsys_gen2_nios2_ocimem|*MonDReg*}] -to [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck|*sr*}]
#set_false_path -from [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_tck|*sr*}] -to [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_sysclk:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_sysclk|*jdo*}]
#set_false_path -from [get_keepers {sld_hub:*|irf_reg*}] -to [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_wrapper|usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_sysclk:the_usb20sr_refdes_cpu_qsys_gen2_jtag_debug_slave_sysclk|ir*}]
#set_false_path -from [get_keepers {sld_hub:*|sld_shadow_jsm:shadow_jsm|state[1]}] -to [get_keepers {*usb20sr_refdes_cpu_qsys_gen2:*|usb20sr_refdes_cpu_qsys_gen2_nios2_oci:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci|usb20sr_refdes_cpu_qsys_gen2_nios2_oci_debug:the_usb20sr_refdes_cpu_qsys_gen2_nios2_oci_debug|monitor_go}]


#**************************************************************
# Set Multicycle Path
#**************************************************************



#**************************************************************
# Set Maximum Delay
#**************************************************************



#**************************************************************
# Set Minimum Delay
#**************************************************************



#**************************************************************
# Set Input Transition
#**************************************************************

