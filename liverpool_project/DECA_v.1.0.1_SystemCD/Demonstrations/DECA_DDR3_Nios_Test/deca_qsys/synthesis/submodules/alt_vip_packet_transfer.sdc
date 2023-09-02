# (C) 2001-2015 Altera Corporation. All rights reserved.
# Your use of Altera Corporation's design tools, logic functions and other 
# software and tools, and its AMPP partner logic functions, and any output 
# files any of the foregoing (including device programming or simulation 
# files), and any associated documentation or information are expressly subject 
# to the terms and conditions of the Altera Program License Subscription 
# Agreement, Altera MegaCore Function License Agreement, or other applicable 
# license agreement, including, without limitation, that your use is for the 
# sole purpose of programming logic devices manufactured by Altera and sold by 
# Altera or its authorized distributors.  Please refer to the applicable 
# agreement for further details.


# ==================================================================
# Write proc

# Clock crossing register
set_false_path -from [get_keepers {*packet_transfer*write_proc_instance|*mem_ctr_crosser|din_reg*}] -to [get_keepers {*packet_transfer*write_proc_instance|*mem_ctr_crosser|delay_line[0]*}]
set_false_path -from [get_keepers {*packet_transfer*write_proc_instance|*unload_ack}] -to [get_keepers {*packet_transfer*write_proc_instance|*unload_ack_crosser|delay_line[0]*}]
set_false_path -from [get_keepers {*packet_transfer*write_proc_instance|*unload_req}] -to [get_keepers {*packet_transfer*write_proc_instance|*unload_req_crosser|delay_line[0]*}]

# Ack-syn path
set_false_path -from [get_keepers {*packet_transfer*write_proc_instance|*unload_buffered*}] -to [get_keepers {*packet_transfer*write_proc_instance|*_mm_output*}]


# ==================================================================
# Read proc

# Clock crossing register

set_false_path -from [get_keepers {*packet_transfer*read_proc_instance|*read_syn_buf*}] -to [get_keepers {*packet_transfer*read_proc_instance|*read_syn_crosser|delay_line[0]*}]
set_false_path -from [get_keepers {*packet_transfer*read_proc_instance|*read_ack_buf*}] -to [get_keepers {*packet_transfer*read_proc_instance|*read_ack_crosser|delay_line[0]*}]

# Ack-syn path
set_false_path -from [get_keepers {*packet_transfer*read_proc_instance|*current_ctxt_burst_size*}] -to [get_keepers *packet_transfer*read_proc_instance\|*mm_msg_queue*scfifo*porta_datain*]
set_false_path -from [get_keepers {*packet_transfer*read_proc_instance|*ctxt_expecting_valid_ptr*}] -to [get_keepers *packet_transfer*read_proc_instance\|*mm_msg_queue*scfifo*porta_datain*]


set_false_path -from [get_keepers {*packet_transfer*read_proc_instance|*current_ctxt_burst_size*
                                   *packet_transfer*read_proc_instance|*current_ctxt_id*
                                   *packet_transfer*read_proc_instance|*period_expecting_valid_ptr*
                                   *packet_transfer*read_proc_instance|*context_expecting_valid_ptr*
                                   *packet_transfer*read_proc_instance|*csel_expecting_valid_ptr*
                                   *packet_transfer*read_proc_instance|*ctxt_expecting_valid_ptr*
                                  }] -to [get_keepers {*packet_transfer*read_proc_instance|*mm_msg_din*
                                    *packet_transfer*read_proc_instance|*mm_msg_bypass_reg*
                                    *packet_transfer*read_proc_instance|*mm_msg_queue*porta_datain_reg*
                                  }]

set_false_path -from [get_keepers {*packet_transfer*read_proc_instance|*csel_target_addr*
                                   *packet_transfer*read_proc_instance|*context_target_addr*
                                   *packet_transfer*read_proc_instance|*ctxt_target_addr*
                                   *packet_transfer*read_proc_instance|*period_targer_addr_start*
                                  }] -to [get_keepers {*read_proc_instance|*mm_target_addr*}]
                                  
                                  
set_false_path -from [get_keepers {*packet_transfer*read_proc_instance|*current_ctxt_burst_size*}] -to [get_keepers {*read_proc_instance|*mm_burst_size*}]
#**************************************************************