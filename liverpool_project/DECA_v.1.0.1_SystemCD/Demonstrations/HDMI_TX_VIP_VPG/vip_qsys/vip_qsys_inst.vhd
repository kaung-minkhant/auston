	component vip_qsys is
		port (
			alt_vip_cl_cvo_0_clocked_video_vid_clk       : in  std_logic                     := 'X'; -- vid_clk
			alt_vip_cl_cvo_0_clocked_video_vid_data      : out std_logic_vector(23 downto 0);        -- vid_data
			alt_vip_cl_cvo_0_clocked_video_underflow     : out std_logic;                            -- underflow
			alt_vip_cl_cvo_0_clocked_video_vid_datavalid : out std_logic;                            -- vid_datavalid
			alt_vip_cl_cvo_0_clocked_video_vid_v_sync    : out std_logic;                            -- vid_v_sync
			alt_vip_cl_cvo_0_clocked_video_vid_h_sync    : out std_logic;                            -- vid_h_sync
			alt_vip_cl_cvo_0_clocked_video_vid_f         : out std_logic;                            -- vid_f
			alt_vip_cl_cvo_0_clocked_video_vid_h         : out std_logic;                            -- vid_h
			alt_vip_cl_cvo_0_clocked_video_vid_v         : out std_logic;                            -- vid_v
			altpll_qsys_c1_clk                           : out std_logic;                            -- clk
			clk_clk                                      : in  std_logic                     := 'X'; -- clk
			reset_reset_n                                : in  std_logic                     := 'X'  -- reset_n
		);
	end component vip_qsys;

	u0 : component vip_qsys
		port map (
			alt_vip_cl_cvo_0_clocked_video_vid_clk       => CONNECTED_TO_alt_vip_cl_cvo_0_clocked_video_vid_clk,       -- alt_vip_cl_cvo_0_clocked_video.vid_clk
			alt_vip_cl_cvo_0_clocked_video_vid_data      => CONNECTED_TO_alt_vip_cl_cvo_0_clocked_video_vid_data,      --                               .vid_data
			alt_vip_cl_cvo_0_clocked_video_underflow     => CONNECTED_TO_alt_vip_cl_cvo_0_clocked_video_underflow,     --                               .underflow
			alt_vip_cl_cvo_0_clocked_video_vid_datavalid => CONNECTED_TO_alt_vip_cl_cvo_0_clocked_video_vid_datavalid, --                               .vid_datavalid
			alt_vip_cl_cvo_0_clocked_video_vid_v_sync    => CONNECTED_TO_alt_vip_cl_cvo_0_clocked_video_vid_v_sync,    --                               .vid_v_sync
			alt_vip_cl_cvo_0_clocked_video_vid_h_sync    => CONNECTED_TO_alt_vip_cl_cvo_0_clocked_video_vid_h_sync,    --                               .vid_h_sync
			alt_vip_cl_cvo_0_clocked_video_vid_f         => CONNECTED_TO_alt_vip_cl_cvo_0_clocked_video_vid_f,         --                               .vid_f
			alt_vip_cl_cvo_0_clocked_video_vid_h         => CONNECTED_TO_alt_vip_cl_cvo_0_clocked_video_vid_h,         --                               .vid_h
			alt_vip_cl_cvo_0_clocked_video_vid_v         => CONNECTED_TO_alt_vip_cl_cvo_0_clocked_video_vid_v,         --                               .vid_v
			altpll_qsys_c1_clk                           => CONNECTED_TO_altpll_qsys_c1_clk,                           --                 altpll_qsys_c1.clk
			clk_clk                                      => CONNECTED_TO_clk_clk,                                      --                            clk.clk
			reset_reset_n                                => CONNECTED_TO_reset_reset_n                                 --                          reset.reset_n
		);

