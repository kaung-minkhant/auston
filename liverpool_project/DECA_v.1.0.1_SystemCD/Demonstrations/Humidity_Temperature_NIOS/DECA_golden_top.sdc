#**************************************************************
# Create Clock
#**************************************************************

create_clock -period 100 [get_ports ADC_CLK_10]
create_clock -period 20 [get_ports MAX10_CLK1_50]
create_clock -period 20 [get_ports MAX10_CLK2_50]



#**************************************************************
# Create Generated Clock
#**************************************************************
derive_pll_clocks






#**************************************************************
# Set Clock Latency
#**************************************************************


#**************************************************************
# Set Clock Uncertainty
#**************************************************************
derive_clock_uncertainty



#**************************************************************
# Set Input Delay
#**************************************************************



#**************************************************************
# Set Output Delay
#**************************************************************



#**************************************************************
# Set Clock Groups
##**************************************************************

#**************************************************************
# Set False Path
#**************************************************************

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



#**************************************************************
# Set Load
#**************************************************************
