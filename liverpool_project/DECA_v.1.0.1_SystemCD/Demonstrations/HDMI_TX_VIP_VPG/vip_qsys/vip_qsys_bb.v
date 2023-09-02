
module vip_qsys (
	alt_vip_cl_cvo_0_clocked_video_vid_clk,
	alt_vip_cl_cvo_0_clocked_video_vid_data,
	alt_vip_cl_cvo_0_clocked_video_underflow,
	alt_vip_cl_cvo_0_clocked_video_vid_datavalid,
	alt_vip_cl_cvo_0_clocked_video_vid_v_sync,
	alt_vip_cl_cvo_0_clocked_video_vid_h_sync,
	alt_vip_cl_cvo_0_clocked_video_vid_f,
	alt_vip_cl_cvo_0_clocked_video_vid_h,
	alt_vip_cl_cvo_0_clocked_video_vid_v,
	altpll_qsys_c1_clk,
	clk_clk,
	reset_reset_n);	

	input		alt_vip_cl_cvo_0_clocked_video_vid_clk;
	output	[23:0]	alt_vip_cl_cvo_0_clocked_video_vid_data;
	output		alt_vip_cl_cvo_0_clocked_video_underflow;
	output		alt_vip_cl_cvo_0_clocked_video_vid_datavalid;
	output		alt_vip_cl_cvo_0_clocked_video_vid_v_sync;
	output		alt_vip_cl_cvo_0_clocked_video_vid_h_sync;
	output		alt_vip_cl_cvo_0_clocked_video_vid_f;
	output		alt_vip_cl_cvo_0_clocked_video_vid_h;
	output		alt_vip_cl_cvo_0_clocked_video_vid_v;
	output		altpll_qsys_c1_clk;
	input		clk_clk;
	input		reset_reset_n;
endmodule
