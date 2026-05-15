#ifndef PRJ007_DDR_MAP__H
#define PRJ007_DDR_MAP__H
//Msg
#ifndef DDR_MAN
#define DDR_MAN         1
#define IO_BASE         0xb0000000
#endif

//DDR AR/W QOS
#define QOS_BASE        (IO_BASE + 0x03000000)

//Vreg
#ifndef DDR_BASE
#define DDR_BASE        (IO_BASE + 0x03010000)
#endif
#ifndef DDRC_BASE
#define DDRC_BASE       (IO_BASE + 0x03010000)
#endif

//#ifndef DDRCAHB_BASE
//#define DDRCAHB_BASE     (IO_BASE + 0x034f0000)
//#endif

#ifndef DDRCAPB_BASE
#define DDRCAPB_BASE     (IO_BASE + 0x03012000)
#endif

#ifndef DDRPAPB_BASE
#define DDRPAPB_BASE     (IO_BASE + 0x03011000)
#endif

#define DDR_LOAD      0
#define SEL_DDR3
#define DDR_CHIP_16Bit_Width
#define DWC_INNO_DDRPHY
    //----  APB DDRC register map ----//
#define    MSTR          (DDRCAPB_BASE + 0x000)
#define    STAT          (DDRCAPB_BASE + 0x004)
#define    MRCTRL0       (DDRCAPB_BASE + 0x010)
#define    MRCTRL1       (DDRCAPB_BASE + 0x014)
#define    MRSTAT        (DDRCAPB_BASE + 0x018)
#define    DERATEEN      (DDRCAPB_BASE + 0x020)
#define    DERATEINT     (DDRCAPB_BASE + 0x024)
#define    DERATECTL     (DDRCAPB_BASE + 0x02c)
#define    PWRCTL        (DDRCAPB_BASE + 0x030)
#define    PWRTMG        (DDRCAPB_BASE + 0x034)
#define    HWLPCTL       (DDRCAPB_BASE + 0x038)
#define    RFSHCTL0      (DDRCAPB_BASE + 0x050)
#define    RFSHCTL3      (DDRCAPB_BASE + 0x060)
#define    RFSHTMG       (DDRCAPB_BASE + 0x064)
#define    CRCPARCTL0    (DDRCAPB_BASE + 0x0c0)
#define    CRCPARSTAT    (DDRCAPB_BASE + 0x0cc)
#define    INIT0         (DDRCAPB_BASE + 0x0d0)
#define    INIT1         (DDRCAPB_BASE + 0x0d4)
#define    INIT2         (DDRCAPB_BASE + 0x0d8)
#define    INIT3         (DDRCAPB_BASE + 0x0dc)
#define    INIT4         (DDRCAPB_BASE + 0x0e0)
#define    INIT5         (DDRCAPB_BASE + 0x0e4)
#define    DIMMCTL       (DDRCAPB_BASE + 0x0f0)
#define    DRAMTMG0      (DDRCAPB_BASE + 0x100)
#define    DRAMTMG1      (DDRCAPB_BASE + 0x104)
#define    DRAMTMG2      (DDRCAPB_BASE + 0x108)
#define    DRAMTMG3      (DDRCAPB_BASE + 0x10c)
#define    DRAMTMG4      (DDRCAPB_BASE + 0x110)
#define    DRAMTMG5      (DDRCAPB_BASE + 0x114)
#define    DRAMTMG6      (DDRCAPB_BASE + 0x118)
#define    DRAMTMG7      (DDRCAPB_BASE + 0x11c)
#define    DRAMTMG8      (DDRCAPB_BASE + 0x120)
#define    DRAMTMG14     (DDRCAPB_BASE + 0x138)
#define    DRAMTMG15     (DDRCAPB_BASE + 0x13c)
#define    ZQCTL0        (DDRCAPB_BASE + 0x180)
#define    ZQCTL1        (DDRCAPB_BASE + 0x184)
#define    ZQCTL2        (DDRCAPB_BASE + 0x188)
#define    ZQSTAT        (DDRCAPB_BASE + 0x18c)
#define    DFITMG0       (DDRCAPB_BASE + 0x190)
#define    DFITMG1       (DDRCAPB_BASE + 0x194)
#define    DFILPCFG0     (DDRCAPB_BASE + 0x198)
#define    DFIUPD0       (DDRCAPB_BASE + 0x1a0)
#define    DFIUPD1       (DDRCAPB_BASE + 0x1a4)
#define    DFIUPD2       (DDRCAPB_BASE + 0x1a8)
#define    DFIMISC       (DDRCAPB_BASE + 0x1b0)
#define    DFISTAT       (DDRCAPB_BASE + 0x1bc)
#define    DFIPHYMSTR    (DDRCAPB_BASE + 0x1c4)
#define    ADDRMAP1      (DDRCAPB_BASE + 0x204)
#define    ADDRMAP2      (DDRCAPB_BASE + 0x208)
#define    ADDRMAP3      (DDRCAPB_BASE + 0x20c)
#define    ADDRMAP4      (DDRCAPB_BASE + 0x210)
#define    ADDRMAP5      (DDRCAPB_BASE + 0x214)
#define    ADDRMAP6      (DDRCAPB_BASE + 0x218)
#define    ADDRMAP9      (DDRCAPB_BASE + 0x224)
#define    ADDRMAP10     (DDRCAPB_BASE + 0x228)
#define    ADDRMAP11     (DDRCAPB_BASE + 0x22c)
#define    ODTCFG        (DDRCAPB_BASE + 0x240)
#define    ODTMAP        (DDRCAPB_BASE + 0x244)
#define    SCHED         (DDRCAPB_BASE + 0x250)
#define    SCHED1        (DDRCAPB_BASE + 0x254)
#define    PERFHPR1      (DDRCAPB_BASE + 0x25c)
#define    PERFLPR1      (DDRCAPB_BASE + 0x264)
#define    PERFWR1       (DDRCAPB_BASE + 0x26c)
#define    DBG0          (DDRCAPB_BASE + 0x300)
#define    DBG1          (DDRCAPB_BASE + 0x304)
#define    DBGCAM        (DDRCAPB_BASE + 0x308)
#define    DBGCMD        (DDRCAPB_BASE + 0x30c)
#define    DBGSTAT       (DDRCAPB_BASE + 0x310)
#define    SWCTL         (DDRCAPB_BASE + 0x320)
#define    SWSTAT        (DDRCAPB_BASE + 0x324)
#define    SWCTLSTATIC   (DDRCAPB_BASE + 0x328)
#define    POISONCFG     (DDRCAPB_BASE + 0x36c)
#define    POISONSTAT    (DDRCAPB_BASE + 0x370)
#define    DERATESTAT    (DDRCAPB_BASE + 0x3f0)

#define    PSTAT         (DDRCAPB_BASE + 0x3fc)
#define    PCCFG         (DDRCAPB_BASE + 0x400)
#define    PCFGR0        (DDRCAPB_BASE + 0x404)
#define    PCFGW0        (DDRCAPB_BASE + 0x408)
#define    PCFGQOS00     (DDRCAPB_BASE + 0x494)
#define    PCFGR1        (DDRCAPB_BASE + 0x4b4)
#define    PCFGW1        (DDRCAPB_BASE + 0x4b8)
#define    PCFGQOS01     (DDRCAPB_BASE + 0x544)
#define    PCFGR2        (DDRCAPB_BASE + 0x564)
#define    PCFGW2        (DDRCAPB_BASE + 0x568)
#define    PCFGQOS02     (DDRCAPB_BASE + 0x5f4)
#define    PCFGR3        (DDRCAPB_BASE + 0x614)
#define    PCFGW3        (DDRCAPB_BASE + 0x618)
#define    PCFGQOS03     (DDRCAPB_BASE + 0x6a4)
#define    PCFGR4        (DDRCAPB_BASE + 0x6c4)
#define    PCFGW4        (DDRCAPB_BASE + 0x6c8)
#define    PCFGQOS04     (DDRCAPB_BASE + 0x754)
#define    PCFGR5        (DDRCAPB_BASE + 0x774)
#define    PCFGW5        (DDRCAPB_BASE + 0x778)
#define    PCFGC5        (DDRCAPB_BASE + 0x77c)
#define    PCFGR6        (DDRCAPB_BASE + 0x824)
#define    PCFGW6        (DDRCAPB_BASE + 0x828)
#define    PCFGQOS06     (DDRCAPB_BASE + 0x8b4)
#define    PCTRL0        (DDRCAPB_BASE + 0x490)
#define    PCTRL1        (DDRCAPB_BASE + 0x540)
#define    PCTRL2        (DDRCAPB_BASE + 0x5f0)
#define    PCTRL3        (DDRCAPB_BASE + 0x6a0)
#define    PCTRL4        (DDRCAPB_BASE + 0x750)
#define    PCTRL5        (DDRCAPB_BASE + 0x800)
#define    PCTRL6        (DDRCAPB_BASE + 0x8b0)

    //----  APB INNO PHY register map ----//
// RW REGISTER LIST
#define reg_dm_invalid_value                              (DDRPAPB_BASE + 0x000), 18, 18
#define reg_dq_invalid_value                              (DDRPAPB_BASE + 0x000), 17, 17
#define reg_channel_en                                    (DDRPAPB_BASE + 0x000), 16, 8
#define reg_burst_sel                                     (DDRPAPB_BASE + 0x000), 7 , 7
#define mem_select_t                                      (DDRPAPB_BASE + 0x000), 6 , 4
#define soft_reset1                                       (DDRPAPB_BASE + 0x000), 2 , 2
#define soft_reset0                                       (DDRPAPB_BASE + 0x000), 1 , 1
#define soft_reset                                        (DDRPAPB_BASE + 0x000), 0 , 0
#define reg_wl_loadmode                                   (DDRPAPB_BASE + 0x004), 31, 16
#define reg_wl_dqs_start_point                            (DDRPAPB_BASE + 0x004), 15, 8
#define reg_wlcs_sel                                      (DDRPAPB_BASE + 0x004), 7 , 6
#define reg_wl_bypass                                     (DDRPAPB_BASE + 0x004), 5 , 5
#define reg_wl_enable                                     (DDRPAPB_BASE + 0x004), 4 , 4
#define reg_calcs_sel                                     (DDRPAPB_BASE + 0x004), 3 , 2
#define reg_calib_bypass                                  (DDRPAPB_BASE + 0x004), 1 , 1
#define reg_start_calib                                   (DDRPAPB_BASE + 0x004), 0 , 0
#define AL_FRE_OP0                                        (DDRPAPB_BASE + 0x008), 29, 24
#define AL_FRE_OP1                                        (DDRPAPB_BASE + 0x008), 21, 16
#define AL_FRE_OP2                                        (DDRPAPB_BASE + 0x008), 13, 8
#define AL_FRE_OP3                                        (DDRPAPB_BASE + 0x008), 5 , 0
#define CL_FRE_OP0                                        (DDRPAPB_BASE + 0x00c), 29, 24
#define CL_FRE_OP1                                        (DDRPAPB_BASE + 0x00c), 21, 16
#define CL_FRE_OP2                                        (DDRPAPB_BASE + 0x00c), 13, 8
#define CL_FRE_OP3                                        (DDRPAPB_BASE + 0x00c), 5 , 0
#define CWL_FRE_OP0                                       (DDRPAPB_BASE + 0x010), 29, 24
#define CWL_FRE_OP1                                       (DDRPAPB_BASE + 0x010), 21, 16
#define CWL_FRE_OP2                                       (DDRPAPB_BASE + 0x010), 13, 8
#define CWL_FRE_OP3                                       (DDRPAPB_BASE + 0x010), 5 , 0
#define reg_fb1xclk_invdelaysel_dqcmd                     (DDRPAPB_BASE + 0x014), 29, 24
#define reg_h4xclk_invdelaysel_dqcmd                      (DDRPAPB_BASE + 0x014), 20, 16
#define reg_h4xclkdqs_invdelaysel_dqcmd                   (DDRPAPB_BASE + 0x014), 12, 8
#define reg_h1xclk_invdelaysel_dqcmd                      (DDRPAPB_BASE + 0x014), 4 , 0
#define reg_cmd_ph90en_bp                                 (DDRPAPB_BASE + 0x018), 31, 0
#define reg_pllpostdiv_fsp3                               (DDRPAPB_BASE + 0x01c), 30, 28
#define reg_pllpostdiven_fsp3                             (DDRPAPB_BASE + 0x01c), 27, 27
#define reg_pllcpi_bias_fsp3                              (DDRPAPB_BASE + 0x01c), 26, 24
#define reg_pllpostdiv_fsp2                               (DDRPAPB_BASE + 0x01c), 22, 20
#define reg_pllpostdiven_fsp2                             (DDRPAPB_BASE + 0x01c), 19, 19
#define reg_pllcpi_bias_fsp2                              (DDRPAPB_BASE + 0x01c), 18, 16
#define reg_pllpostdiv_fsp1                               (DDRPAPB_BASE + 0x01c), 14, 12
#define reg_pllpostdiven_fsp1                             (DDRPAPB_BASE + 0x01c), 11, 11
#define reg_pllcpi_bias_fsp1                              (DDRPAPB_BASE + 0x01c), 10, 8
#define reg_pllpostdiv_fsp0                               (DDRPAPB_BASE + 0x01c), 6 , 4
#define reg_pllpostdiven_fsp0                             (DDRPAPB_BASE + 0x01c), 3 , 3
#define reg_pllcpi_bias_fsp0                              (DDRPAPB_BASE + 0x01c), 2 , 0
#define reg_bist_init_done                                (DDRPAPB_BASE + 0x020), 31, 31
#define reg_bist_init_done_bypass                         (DDRPAPB_BASE + 0x020), 30, 30
#define reg_catrain_init_from_bist                        (DDRPAPB_BASE + 0x020), 29, 29
#define reg_cmd_bist_err_inject                           (DDRPAPB_BASE + 0x020), 27, 27
#define reg_cmd_2t_mode_t                                 (DDRPAPB_BASE + 0x020), 26, 26
#define reg_oscen                                         (DDRPAPB_BASE + 0x020), 25, 25
#define reg_vt_comp_bp                                    (DDRPAPB_BASE + 0x020), 23, 23
#define reg_cmdout_mux                                    (DDRPAPB_BASE + 0x020), 22, 22
#define ph90en_bp                                         (DDRPAPB_BASE + 0x020), 21, 21
#define ph90en_bp_dq                                      (DDRPAPB_BASE + 0x020), 20, 20
#define reg_rden_bypass                                   (DDRPAPB_BASE + 0x020), 19, 19
#define reg_rden_delay                                    (DDRPAPB_BASE + 0x020), 18, 16
#define reg_phy_long_txenb                                (DDRPAPB_BASE + 0x020), 15, 15
#define reg_rdodt_bypass                                  (DDRPAPB_BASE + 0x020), 14, 14
#define reg_rxodt_st_bypass                               (DDRPAPB_BASE + 0x020), 13, 13
#define reg_rxodt_stdelay                                 (DDRPAPB_BASE + 0x020), 11, 8
#define reg_rxodt_length                                  (DDRPAPB_BASE + 0x020), 7 , 4
#define reg_rxodt_start_point                             (DDRPAPB_BASE + 0x020), 3 , 0
#define reg_ddrc_tzqinit                                  (DDRPAPB_BASE + 0x024), 31, 24
#define reg_ddrc_tzqlat                                   (DDRPAPB_BASE + 0x024), 23, 16
#define reg_ddrc_treset_l_x1024                           (DDRPAPB_BASE + 0x024), 15, 8
#define reg_ddrc_treset_h_x1024                           (DDRPAPB_BASE + 0x024), 7 , 0
#define reg_ddrc_tckeh                                    (DDRPAPB_BASE + 0x028), 31, 24
#define reg_cat_ca_then_cs                                (DDRPAPB_BASE + 0x028), 23, 23
#define reg_cat_clear                                     (DDRPAPB_BASE + 0x028), 22, 22
#define reg_cat_rank_num                                  (DDRPAPB_BASE + 0x028), 19, 18
#define reg_cat_channel_num                               (DDRPAPB_BASE + 0x028), 17, 16
#define reg_clk_div_cnt                                   (DDRPAPB_BASE + 0x028), 12, 8
#define reg_cat_bp_rank_sel                               (DDRPAPB_BASE + 0x028), 7 , 6
#define reg_cat_bp_cmd_send                               (DDRPAPB_BASE + 0x028), 5 , 5
#define reg_cat_bp_mode                                   (DDRPAPB_BASE + 0x028), 4 , 4
#define reg_cat_bp_en                                     (DDRPAPB_BASE + 0x028), 3 , 3
#define reg_cat_bp_start                                  (DDRPAPB_BASE + 0x028), 2 , 2
#define reg_cat_start                                     (DDRPAPB_BASE + 0x028), 1 , 1
#define reg_cat_enable                                    (DDRPAPB_BASE + 0x028), 0 , 0
#define reg_txcbt                                         (DDRPAPB_BASE + 0x02c), 28, 25
#define reg_tadr                                          (DDRPAPB_BASE + 0x02c), 23, 20
#define reg_tckelck                                       (DDRPAPB_BASE + 0x02c), 18, 15
#define reg_tdstrain                                      (DDRPAPB_BASE + 0x02c), 13, 10
#define reg_tmrw                                          (DDRPAPB_BASE + 0x02c), 8 , 5
#define reg_tcacd                                         (DDRPAPB_BASE + 0x02c), 4 , 0
#define reg_phy_div_value                                 (DDRPAPB_BASE + 0x030), 28, 25
#define reg_tvrefca_long                                  (DDRPAPB_BASE + 0x030), 24, 16
#define reg_tcaent                                        (DDRPAPB_BASE + 0x030), 15, 8
#define reg_tfc                                           (DDRPAPB_BASE + 0x030), 7 , 0
#define reg_mr1                                           (DDRPAPB_BASE + 0x034), 31, 24
#define reg_mr2                                           (DDRPAPB_BASE + 0x034), 23, 16
#define reg_mr3                                           (DDRPAPB_BASE + 0x034), 15, 8
#define reg_mr11                                          (DDRPAPB_BASE + 0x034), 7 , 0
#define reg_cat_cs_right_scan_steps                       (DDRPAPB_BASE + 0x038), 31, 24
#define reg_mr13                                          (DDRPAPB_BASE + 0x038), 23, 16
#define reg_mr14                                          (DDRPAPB_BASE + 0x038), 15, 8
#define reg_mr22_rank0                                    (DDRPAPB_BASE + 0x038), 7 , 0
#define reg_ca_vref_update                                (DDRPAPB_BASE + 0x03c), 31, 31
#define reg_cat_vref_scan_disable                         (DDRPAPB_BASE + 0x03c), 30, 30
#define reg_cat_vref_scan_max                             (DDRPAPB_BASE + 0x03c), 29, 24
#define reg_cat_vref_scan_min                             (DDRPAPB_BASE + 0x03c), 21, 16
#define reg_cha_cat_vref_bp_value                         (DDRPAPB_BASE + 0x03c), 14, 8
#define reg_chb_cat_vref_bp_value                         (DDRPAPB_BASE + 0x03c), 6 , 0
#define reg_cs_perbit_skew_offest_fsp0                    (DDRPAPB_BASE + 0x040), 31, 28
#define reg_cs_perbit_skew_offest_fsp1                    (DDRPAPB_BASE + 0x040), 27, 24
#define reg_cs_perbit_skew_offest_fsp2                    (DDRPAPB_BASE + 0x040), 23, 20
#define reg_cs_perbit_skew_offest_fsp3                    (DDRPAPB_BASE + 0x040), 19, 16
#define reg_lpddr4_ca_odt                                 (DDRPAPB_BASE + 0x040), 13, 12
#define reg_lpddr4_ca_odt_sel                             (DDRPAPB_BASE + 0x040), 11, 11
#define reg_cat_vref_scan_steps                           (DDRPAPB_BASE + 0x040), 10, 8
#define reg_cs_pwc_disable                                (DDRPAPB_BASE + 0x040), 7 , 7
#define reg_cat_skip_fspy                                 (DDRPAPB_BASE + 0x040), 6 , 6
#define reg_cat_fspy_rank                                 (DDRPAPB_BASE + 0x040), 5 , 4
#define reg_cat_fc_pd_en                                  (DDRPAPB_BASE + 0x040), 3 , 3
#define reg_cat_skip_cs_train                             (DDRPAPB_BASE + 0x040), 2 , 2
#define reg_cmd_perbit_skew_bp                            (DDRPAPB_BASE + 0x040), 1 , 1
#define reg_ca_perbit_skew_update                         (DDRPAPB_BASE + 0x040), 0 , 0
#define reg_cat_cke_mode                                  (DDRPAPB_BASE + 0x044), 25, 25
#define reg_cat_ck_cke_odt_fix_perbit_skew                (DDRPAPB_BASE + 0x044), 24, 24
#define reg_cat_ca_scan_max                               (DDRPAPB_BASE + 0x044), 23, 16
#define reg_cat_ca_train_value                            (DDRPAPB_BASE + 0x044), 13, 8
#define reg_cat_cs_train_value                            (DDRPAPB_BASE + 0x044), 5 , 0
#define reg_cha_cat_cs_check_value                        (DDRPAPB_BASE + 0x048), 29, 24
#define reg_chb_cat_cs_check_value                        (DDRPAPB_BASE + 0x048), 21, 16
#define reg_cha_cat_ca_check_value                        (DDRPAPB_BASE + 0x048), 13, 8
#define reg_chb_cat_ca_check_value                        (DDRPAPB_BASE + 0x048), 5 , 0
#define reg_freq_choose_op_t                              (DDRPAPB_BASE + 0x04c), 31, 30
#define reg_lpddr4_rd_preamble                            (DDRPAPB_BASE + 0x04c), 29, 29
#define reg_wl_freq_update                                (DDRPAPB_BASE + 0x04c), 28, 28
#define reg_calib_freq_update                             (DDRPAPB_BASE + 0x04c), 27, 27
#define reg_cmd_abutobsmodeen                             (DDRPAPB_BASE + 0x04c), 26, 26
#define reg_sdram_vref_update                             (DDRPAPB_BASE + 0x04c), 25, 25
#define reg_calib_mode_sel                                (DDRPAPB_BASE + 0x04c), 24, 24
#define reg_lpddr4_cmd_gap_for_diff_rank                  (DDRPAPB_BASE + 0x04c), 22, 20
#define reg_b0_cmdobsmuxsel                               (DDRPAPB_BASE + 0x04c), 19, 16
#define reg_b0_obsdataen                                  (DDRPAPB_BASE + 0x04c), 15, 15
#define bist_calibst                                      (DDRPAPB_BASE + 0x04c), 14, 14
#define reg_fiford_delay                                  (DDRPAPB_BASE + 0x04c), 13, 13
#define bist_ck_select                                    (DDRPAPB_BASE + 0x04c), 12, 12
#define reg_mch_odt                                       (DDRPAPB_BASE + 0x04c), 11, 11
#define mux_sync_sel                                      (DDRPAPB_BASE + 0x04c), 10, 10
#define reg_sync_en                                       (DDRPAPB_BASE + 0x04c), 9 , 9
#define reg_rdptr_delay                                   (DDRPAPB_BASE + 0x04c), 8 , 8
#define reg_scr_rcvmodsel                                 (DDRPAPB_BASE + 0x04c), 7 , 7
#define reg_scr_sckdimm0dis                               (DDRPAPB_BASE + 0x04c), 6 , 6
#define reg_scr_sdhsclkb_pos1neg0sel                      (DDRPAPB_BASE + 0x04c), 5 , 5
#define reg_scr_sdhsclk_pos1neg0sel                       (DDRPAPB_BASE + 0x04c), 4 , 4
#define reg_scr_odttrien                                  (DDRPAPB_BASE + 0x04c), 3 , 3
#define reg_scr_csbtrien                                  (DDRPAPB_BASE + 0x04c), 2 , 2
#define reg_scr_cmdtrien                                  (DDRPAPB_BASE + 0x04c), 1 , 1
#define reg_scr_cketrirnk0                                (DDRPAPB_BASE + 0x04c), 0 , 0
#define reg_cat_cs_left_scan_steps                        (DDRPAPB_BASE + 0x050), 31, 24
#define reg_io_highz_dis                                  (DDRPAPB_BASE + 0x050), 20, 20
#define reg_cmd_delay_one_ui                              (DDRPAPB_BASE + 0x050), 19, 19
#define cmd_bypassen                                      (DDRPAPB_BASE + 0x050), 18, 18
#define reg_cmd_iobufact_bp                               (DDRPAPB_BASE + 0x050), 17, 17
#define bypassen                                          (DDRPAPB_BASE + 0x050), 16, 8
#define reg_mpr_cnt                                       (DDRPAPB_BASE + 0x050), 7 , 0
#define reg_lp3_odt1_mask                                 (DDRPAPB_BASE + 0x054), 31, 30
#define reg_lp3_odt0_mask                                 (DDRPAPB_BASE + 0x054), 29, 28
#define reg_freq_choose_wr_t                              (DDRPAPB_BASE + 0x054), 27, 26
#define reg_cmd_invdelay_lp_en                            (DDRPAPB_BASE + 0x054), 25, 25
#define reg_dfi_clk_gate_bp                               (DDRPAPB_BASE + 0x054), 24, 24
#define reg_max_rdvalue                                   (DDRPAPB_BASE + 0x054), 23, 16
#define reg_calib_timeout                                 (DDRPAPB_BASE + 0x054), 15, 0
#define reg_pvt_comp_dis                                  (DDRPAPB_BASE + 0x058), 29, 29
#define reg_tx_lock_code_bp_en                            (DDRPAPB_BASE + 0x058), 28, 28
#define reg_rx_lock_code_bp_en                            (DDRPAPB_BASE + 0x058), 27, 27
#define reg_mdll_update_cnt_clear                         (DDRPAPB_BASE + 0x058), 26, 26
#define reg_phy_sdram_initial                             (DDRPAPB_BASE + 0x058), 25, 25
#define reg_cat_rxodt_en                                  (DDRPAPB_BASE + 0x058), 15, 15
#define reg_rx_calib_use_rdc_cmd                          (DDRPAPB_BASE + 0x058), 14, 14
#define reg_wrank_dig_1xdly                               (DDRPAPB_BASE + 0x058), 13, 11
#define reg_rrankdly_4x_dec                               (DDRPAPB_BASE + 0x058), 10, 10
#define reg_rdrank_4xdly                                  (DDRPAPB_BASE + 0x058), 9 , 8
#define reg_rdrank_delay_bp                               (DDRPAPB_BASE + 0x058), 7 , 7
#define reg_rdrank_1xdly                                  (DDRPAPB_BASE + 0x058), 6 , 4
#define reg_wrrank_1xdly                                  (DDRPAPB_BASE + 0x058), 2 , 0
#define reg_mdll_chg_margin                               (DDRPAPB_BASE + 0x05c), 31, 24
#define reg_pvt_comp_req_wait_cnt                         (DDRPAPB_BASE + 0x05c), 23, 16
#define reg_tx_lock_code_bp_value                         (DDRPAPB_BASE + 0x05c), 15, 8
#define reg_rx_lock_code_bp_value                         (DDRPAPB_BASE + 0x05c), 7 , 0
#define reg_cmd0_wrap_sel                                 (DDRPAPB_BASE + 0x060), 29, 25
#define reg_cmd1_wrap_sel                                 (DDRPAPB_BASE + 0x060), 24, 20
#define reg_cmd2_wrap_sel                                 (DDRPAPB_BASE + 0x060), 19, 15
#define reg_cmd3_wrap_sel                                 (DDRPAPB_BASE + 0x060), 14, 10
#define reg_cmd4_wrap_sel                                 (DDRPAPB_BASE + 0x060), 9 , 5
#define reg_cmd5_wrap_sel                                 (DDRPAPB_BASE + 0x060), 4 , 0
#define reg_cmd6_wrap_sel                                 (DDRPAPB_BASE + 0x064), 29, 25
#define reg_cmd7_wrap_sel                                 (DDRPAPB_BASE + 0x064), 24, 20
#define reg_cmd8_wrap_sel                                 (DDRPAPB_BASE + 0x064), 19, 15
#define reg_cmd9_wrap_sel                                 (DDRPAPB_BASE + 0x064), 14, 10
#define reg_cmd10_wrap_sel                                (DDRPAPB_BASE + 0x064), 9 , 5
#define reg_cmd11_wrap_sel                                (DDRPAPB_BASE + 0x064), 4 , 0
#define reg_cmd12_wrap_sel                                (DDRPAPB_BASE + 0x068), 29, 25
#define reg_cmd13_wrap_sel                                (DDRPAPB_BASE + 0x068), 24, 20
#define reg_cmd14_wrap_sel                                (DDRPAPB_BASE + 0x068), 19, 15
#define reg_cmd15_wrap_sel                                (DDRPAPB_BASE + 0x068), 14, 10
#define reg_cmd16_wrap_sel                                (DDRPAPB_BASE + 0x068), 9 , 5
#define reg_cmd17_wrap_sel                                (DDRPAPB_BASE + 0x068), 4 , 0
#define reg_cmd18_wrap_sel                                (DDRPAPB_BASE + 0x06c), 29, 25
#define reg_cmd19_wrap_sel                                (DDRPAPB_BASE + 0x06c), 24, 20
#define reg_cmd20_wrap_sel                                (DDRPAPB_BASE + 0x06c), 19, 15
#define reg_cmd21_wrap_sel                                (DDRPAPB_BASE + 0x06c), 14, 10
#define reg_cmd22_wrap_sel                                (DDRPAPB_BASE + 0x06c), 9 , 5
#define reg_cmd23_wrap_sel                                (DDRPAPB_BASE + 0x06c), 4 , 0
#define reg_cmd24_wrap_sel                                (DDRPAPB_BASE + 0x070), 29, 25
#define reg_cmd25_wrap_sel                                (DDRPAPB_BASE + 0x070), 24, 20
#define reg_cmd26_wrap_sel                                (DDRPAPB_BASE + 0x070), 19, 15
#define reg_cmd27_wrap_sel                                (DDRPAPB_BASE + 0x070), 14, 10
#define reg_cmd28_wrap_sel                                (DDRPAPB_BASE + 0x070), 9 , 5
#define reg_cmd29_wrap_sel                                (DDRPAPB_BASE + 0x070), 4 , 0
#define reg_cmd30_wrap_sel                                (DDRPAPB_BASE + 0x074), 29, 25
#define reg_mr22_rank1                                    (DDRPAPB_BASE + 0x078), 11, 4
#define reg_byte8_wrap_sel                                (DDRPAPB_BASE + 0x078), 3 , 0
#define reg_byte7_wrap_sel                                (DDRPAPB_BASE + 0x07c), 31, 28
#define reg_byte6_wrap_sel                                (DDRPAPB_BASE + 0x07c), 27, 24
#define reg_byte5_wrap_sel                                (DDRPAPB_BASE + 0x07c), 23, 20
#define reg_byte4_wrap_sel                                (DDRPAPB_BASE + 0x07c), 19, 16
#define reg_byte3_wrap_sel                                (DDRPAPB_BASE + 0x07c), 15, 12
#define reg_byte2_wrap_sel                                (DDRPAPB_BASE + 0x07c), 11, 8
#define reg_byte1_wrap_sel                                (DDRPAPB_BASE + 0x07c), 7 , 4
#define reg_byte0_wrap_sel                                (DDRPAPB_BASE + 0x07c), 3 , 0
#define reg_cke_ck_cmd_pad_t                              (DDRPAPB_BASE + 0x080), 31, 0
#define reg_pllfbdiv_dqcmd                                (DDRPAPB_BASE + 0x084), 24, 16
#define reg_pllprediv_dqcmd                               (DDRPAPB_BASE + 0x084), 8 , 4
#define reg_pllpostdiven_ls                               (DDRPAPB_BASE + 0x084), 3 , 3
#define reg_pllpostdiv_ls                                 (DDRPAPB_BASE + 0x084), 2 , 0
#define reg_invdelaysel_osc                               (DDRPAPB_BASE + 0x088), 27, 20
#define reg_plltestsel_dqcmd                              (DDRPAPB_BASE + 0x088), 19, 18
#define reg_pllgvco_bias_dqcmd                            (DDRPAPB_BASE + 0x088), 17, 16
#define reg_pllcpi_bias_ls                                (DDRPAPB_BASE + 0x088), 14, 12
#define reg_pllcpp_bias_dqcmd                             (DDRPAPB_BASE + 0x088), 10, 8
#define reg_plltestouten_dqcmd                            (DDRPAPB_BASE + 0x088), 7 , 7
#define reg_lockenb_dqcmd                                 (DDRPAPB_BASE + 0x088), 6 , 6
#define reg_pllrstbsel_dqcmd                              (DDRPAPB_BASE + 0x088), 5 , 5
#define reg_pllref_clk_byp_dqcmd                          (DDRPAPB_BASE + 0x088), 4 , 4
#define reg_ssc_rstn                                      (DDRPAPB_BASE + 0x088), 3 , 3
#define reg_pllincz_dqcmd                                 (DDRPAPB_BASE + 0x088), 2 , 2
#define reg_pllclkouten_dqcmd_t                           (DDRPAPB_BASE + 0x088), 1 , 1
#define reg_pllpd_dqcmd_t                                 (DDRPAPB_BASE + 0x088), 0 , 0
#define reg_hclk_train_sel                                (DDRPAPB_BASE + 0x08c), 31, 31
#define reg_hclk_bist_sel                                 (DDRPAPB_BASE + 0x08c), 30, 30
#define reg_hclk_zqcalib_sel                              (DDRPAPB_BASE + 0x08c), 29, 29
#define reg_hclk_byte7_sel                                (DDRPAPB_BASE + 0x08c), 28, 28
#define reg_hclk_byte6_sel                                (DDRPAPB_BASE + 0x08c), 27, 27
#define reg_hclk_byte5_sel                                (DDRPAPB_BASE + 0x08c), 26, 26
#define reg_hclk_byte4_sel                                (DDRPAPB_BASE + 0x08c), 25, 25
#define reg_hclk_byte3_sel                                (DDRPAPB_BASE + 0x08c), 24, 24
#define reg_hclk_byte2_sel                                (DDRPAPB_BASE + 0x08c), 23, 23
#define reg_hclk_byte1_sel                                (DDRPAPB_BASE + 0x08c), 22, 22
#define reg_hclk_byte0_sel                                (DDRPAPB_BASE + 0x08c), 21, 21
#define reg_hclk_byte_sel                                 (DDRPAPB_BASE + 0x08c), 20, 20
#define reg_hclk_ca_sel                                   (DDRPAPB_BASE + 0x08c), 19, 19
#define reg_train_reg_update_en                           (DDRPAPB_BASE + 0x08c), 18, 18
#define reg_dqclken_t                                     (DDRPAPB_BASE + 0x08c), 17, 17
#define reg_outclken                                      (DDRPAPB_BASE + 0x08c), 16, 16
#define reg_lp_bypass                                     (DDRPAPB_BASE + 0x08c), 15, 15
#define reg_deep_lp_en                                    (DDRPAPB_BASE + 0x08c), 14, 14
#define reg_lp_wakeup_sel                                 (DDRPAPB_BASE + 0x08c), 13, 13
#define reg_lp_vref_ctrl_en                               (DDRPAPB_BASE + 0x08c), 12, 12
#define reg_lp_wakeup_threhold                            (DDRPAPB_BASE + 0x08c), 11, 8
#define reg_lp_io_dis_ctrl                                (DDRPAPB_BASE + 0x08c), 7 , 6
#define reg_lp_dq_clk_ctrl_en                             (DDRPAPB_BASE + 0x08c), 5 , 5
#define reg_lp_dig_rst_ctrl_en                            (DDRPAPB_BASE + 0x08c), 4 , 4
#define reg_lp_spll_clktree_ctrl_en                       (DDRPAPB_BASE + 0x08c), 3 , 3
#define reg_lp_dig_clk_ctrl_en                            (DDRPAPB_BASE + 0x08c), 2 , 2
#define reg_lp_io_ctrl_en                                 (DDRPAPB_BASE + 0x08c), 1 , 1
#define reg_lp_pllpd_ctrl_en                              (DDRPAPB_BASE + 0x08c), 0 , 0
#define reg_hclk_byte8_sel                                (DDRPAPB_BASE + 0x090), 31, 31
#define reg_lp_stvalue                                    (DDRPAPB_BASE + 0x090), 23, 20
#define reg_lp_ackvalue                                   (DDRPAPB_BASE + 0x090), 19, 16
#define reg_wait_cnt                                      (DDRPAPB_BASE + 0x090), 15, 0
#define reg_zqcali_type_sel                               (DDRPAPB_BASE + 0x094), 30, 30
#define reg_lpddr4x_zqcal                                 (DDRPAPB_BASE + 0x094), 29, 29
#define reg_zq_chg_interval                               (DDRPAPB_BASE + 0x094), 28, 20
#define reg_pu_interval                                   (DDRPAPB_BASE + 0x094), 18, 10
#define reg_pd_zqcali                                     (DDRPAPB_BASE + 0x094), 3 , 3
#define reg_zqcali_clear                                  (DDRPAPB_BASE + 0x094), 2 , 2
#define reg_zqcali_bypass                                 (DDRPAPB_BASE + 0x094), 1 , 1
#define reg_zqcali_en                                     (DDRPAPB_BASE + 0x094), 0 , 0
#define reg_drvpd_zqcali_vref_sel                         (DDRPAPB_BASE + 0x098), 15, 8
#define reg_drvpu_zqcali_vref_sel                         (DDRPAPB_BASE + 0x098), 7 , 0
#define reg_odtlegpu_zqcali                               (DDRPAPB_BASE + 0x09c), 28, 24
#define reg_odtlegpd_zqcali                               (DDRPAPB_BASE + 0x09c), 20, 16
#define reg_odtpd_zqcali_vref_sel                         (DDRPAPB_BASE + 0x09c), 15, 8
#define reg_odtpu_zqcali_vref_sel                         (DDRPAPB_BASE + 0x09c), 7 , 0
#define reg_drvlegpu_zqcali                               (DDRPAPB_BASE + 0x0a0), 28, 24
#define reg_drvlegpd_zqcali                               (DDRPAPB_BASE + 0x0a0), 20, 16
#define reg_ddrphy_rtrain_en                              (DDRPAPB_BASE + 0x0a4), 31, 31
#define reg_rd_train_dq_scan_max                          (DDRPAPB_BASE + 0x0a4), 30, 24
#define reg_rd_train_dqs_scan_max                         (DDRPAPB_BASE + 0x0a4), 22, 16
#define reg_rd_train_perdef_en                            (DDRPAPB_BASE + 0x0a4), 12, 12
#define reg_train_vref_en                                 (DDRPAPB_BASE + 0x0a4), 11, 11
#define reg_ddr4_dbi                                      (DDRPAPB_BASE + 0x0a4), 10, 10
#define reg_rdtrain_cs_sel                                (DDRPAPB_BASE + 0x0a4), 9 , 8
#define reg_rx_vref_value_update                          (DDRPAPB_BASE + 0x0a4), 7 , 7
#define reg_rd_train_dqs_range_bypass                     (DDRPAPB_BASE + 0x0a4), 6 , 6
#define reg_bypass_rd_train_cmd_start_en                  (DDRPAPB_BASE + 0x0a4), 5 , 5
#define reg_bypass_rd_train_en                            (DDRPAPB_BASE + 0x0a4), 4 , 4
#define reg_rd_train_check_value_en                       (DDRPAPB_BASE + 0x0a4), 3 , 3
#define reg_rd_train_freq_update                          (DDRPAPB_BASE + 0x0a4), 2 , 2
#define reg_dqs_rd_train_en                               (DDRPAPB_BASE + 0x0a4), 1 , 1
#define reg_dq_rd_train_en                                (DDRPAPB_BASE + 0x0a4), 0 , 0
#define reg_lpddr4_mr15_value                             (DDRPAPB_BASE + 0x0a8), 31, 24
#define reg_lpddr4_mr20_value                             (DDRPAPB_BASE + 0x0a8), 23, 16
#define reg_lpddr4_mr32_value                             (DDRPAPB_BASE + 0x0a8), 15, 8
#define reg_lpddr4_mr40_value                             (DDRPAPB_BASE + 0x0a8), 7 , 0
#define reg_ddr4_mr4_value                                (DDRPAPB_BASE + 0x0ac), 31, 16
#define reg_ddr4_preamble_cnt                             (DDRPAPB_BASE + 0x0ac), 15, 8
#define reg_ddr4_mr3                                      (DDRPAPB_BASE + 0x0ac), 7 , 0
#define reg_dm_wr_train_en                                (DDRPAPB_BASE + 0x0b0), 25, 25
#define reg_wrtrain_lpddr4_vref_range                     (DDRPAPB_BASE + 0x0b0), 24, 24
#define reg_pbit_deskew_offset_for_lpddr4                 (DDRPAPB_BASE + 0x0b0), 23, 15
#define reg_dqs_wr_train_en                               (DDRPAPB_BASE + 0x0b0), 9 , 9
#define reg_wrtrain_check_data_value_random_gen           (DDRPAPB_BASE + 0x0b0), 8 , 8
#define reg_wrtrain_cs_sel                                (DDRPAPB_BASE + 0x0b0), 7 , 6
#define reg_wr_train_rst                                  (DDRPAPB_BASE + 0x0b0), 5 , 5
#define reg_wr_train_dqs_default_bypass                   (DDRPAPB_BASE + 0x0b0), 4 , 4
#define reg_wr_train_dqs_range_bypass                     (DDRPAPB_BASE + 0x0b0), 3 , 3
#define reg_wr_train_freq_update                          (DDRPAPB_BASE + 0x0b0), 2 , 2
#define reg_dq_wr_train_en                                (DDRPAPB_BASE + 0x0b0), 1 , 1
#define reg_dq_wr_train_auto                              (DDRPAPB_BASE + 0x0b0), 0 , 0
#define reg_wr_train_ba_addr                              (DDRPAPB_BASE + 0x0b4), 30, 28
#define reg_wr_train_col_addr                             (DDRPAPB_BASE + 0x0b4), 25, 16
#define reg_wr_train_row_addr                             (DDRPAPB_BASE + 0x0b4), 15, 0
#define reg_phy_trefi                                     (DDRPAPB_BASE + 0x0b8), 31, 18
#define reg_phy_trfc                                      (DDRPAPB_BASE + 0x0b8), 17, 8
#define reg_max_refi_cnt                                  (DDRPAPB_BASE + 0x0b8), 7 , 4
#define reg_phy_max_refi_enable                           (DDRPAPB_BASE + 0x0b8), 1 , 1
#define reg_phy_refresh_en                                (DDRPAPB_BASE + 0x0b8), 0 , 0
#define reg_a0_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 31, 31
#define reg_a1_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 30, 30
#define reg_a2_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 29, 29
#define reg_a3_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 28, 28
#define reg_a4_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 27, 27
#define reg_a5_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 26, 26
#define reg_a6_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 25, 25
#define reg_a7_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 24, 24
#define reg_a8_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 23, 23
#define reg_a9_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 22, 22
#define reg_a10_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 21, 21
#define reg_a11_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 20, 20
#define reg_a12_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 19, 19
#define reg_a13_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 18, 18
#define reg_a14_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 17, 17
#define reg_a15_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 16, 16
#define reg_a16_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 15, 15
#define reg_a17_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 14, 14
#define reg_actn_lp4x_en                                  (DDRPAPB_BASE + 0x0bc), 13, 13
#define reg_ba0_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 12, 12
#define reg_ba1_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 11, 11
#define reg_bg0_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 10, 10
#define reg_bg1_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 9 , 9
#define reg_ck_lp4x_en                                    (DDRPAPB_BASE + 0x0bc), 8 , 8
#define reg_ckb_lp4x_en                                   (DDRPAPB_BASE + 0x0bc), 7 , 7
#define reg_cke0_lp4x_en                                  (DDRPAPB_BASE + 0x0bc), 6 , 6
#define reg_csb0_lp4x_en                                  (DDRPAPB_BASE + 0x0bc), 5 , 5
#define reg_odt0_lp4x_en                                  (DDRPAPB_BASE + 0x0bc), 4 , 4
#define reg_csb1_lp4x_en                                  (DDRPAPB_BASE + 0x0bc), 3 , 3
#define reg_odt1_lp4x_en                                  (DDRPAPB_BASE + 0x0bc), 2 , 2
#define reg_resetn_lp4x_en                                (DDRPAPB_BASE + 0x0bc), 1 , 1
#define reg_cmd_ca_enb_lp4                                (DDRPAPB_BASE + 0x0bc), 0 , 0
#define reg_a17_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 31, 31
#define reg_a16_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 30, 30
#define reg_a15_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 29, 29
#define reg_a14_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 28, 28
#define reg_a13_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 27, 27
#define reg_a12_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 26, 26
#define reg_a11_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 25, 25
#define reg_a10_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 24, 24
#define reg_a9_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 23, 23
#define reg_a8_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 22, 22
#define reg_a7_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 21, 21
#define reg_a6_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 20, 20
#define reg_a5_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 19, 19
#define reg_a4_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 18, 18
#define reg_a3_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 17, 17
#define reg_a2_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 16, 16
#define reg_a1_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 15, 15
#define reg_a0_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 14, 14
#define reg_actn_pvt_comp_en                              (DDRPAPB_BASE + 0x0c0), 13, 13
#define reg_ba0_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 12, 12
#define reg_ba1_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 11, 11
#define reg_bg0_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 10, 10
#define reg_bg1_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 9 , 9
#define reg_ck_pvt_comp_en                                (DDRPAPB_BASE + 0x0c0), 8 , 8
#define reg_ckb_pvt_comp_en                               (DDRPAPB_BASE + 0x0c0), 7 , 7
#define reg_cke0_pvt_comp_en                              (DDRPAPB_BASE + 0x0c0), 6 , 6
#define reg_csb0_pvt_comp_en                              (DDRPAPB_BASE + 0x0c0), 5 , 5
#define reg_odt0_pvt_comp_en                              (DDRPAPB_BASE + 0x0c0), 4 , 4
#define reg_cke1_pvt_comp_en                              (DDRPAPB_BASE + 0x0c0), 3 , 3
#define reg_csb1_pvt_comp_en                              (DDRPAPB_BASE + 0x0c0), 2 , 2
#define reg_odt1_pvt_comp_en                              (DDRPAPB_BASE + 0x0c0), 1 , 1
#define reg_resetn_pvt_comp_en                            (DDRPAPB_BASE + 0x0c0), 0 , 0
#define reg_cmd_drv_zqcalib_en                            (DDRPAPB_BASE + 0x0c4), 17, 17
#define reg_cmd_fbsel_reg                                 (DDRPAPB_BASE + 0x0c4), 16, 16
#define reg_cmd_fben_reg                                  (DDRPAPB_BASE + 0x0c4), 15, 15
#define reg_cmd_abutweakpub_reg                           (DDRPAPB_BASE + 0x0c4), 13, 13
#define reg_cmd_abutslewpu_reg                            (DDRPAPB_BASE + 0x0c4), 12, 8
#define reg_cmd_abutweakpd_reg                            (DDRPAPB_BASE + 0x0c4), 5 , 5
#define reg_cmd_abutslewpd_reg                            (DDRPAPB_BASE + 0x0c4), 4 , 0

#define reg_lp_clken_csb0_a_en_reg                        (DDRPAPB_BASE + 0x0c8), 30, 30    // CSB0 clock enable
#define reg_lp_clken_csb0_en_reg                          (DDRPAPB_BASE + 0x0c8), 29, 29    // CSB0_A clock enable
#define reg_cmd_abutnrcomp_reg                            (DDRPAPB_BASE + 0x0c8), 28, 24    // The pull-down resistance of CMD except CK.
#define reg_cmd_abutprcomp_reg                            (DDRPAPB_BASE + 0x0c8), 20, 16    // The pull-up resistance of CMD except CK.
#define reg_cmd_abutnrcomp_ck0_reg                        (DDRPAPB_BASE + 0x0c8), 12, 8     // The pull-down resistance of CK.
#define reg_cmd_abutprcomp_ck0_reg                        (DDRPAPB_BASE + 0x0c8), 4 , 0     // The pull-up resistance of CK.

#define reg_cmd_abutnrcomp4_reg                           (DDRPAPB_BASE + 0x0c8), 28, 28
#define reg_cmd_abutnrcomp3_reg                           (DDRPAPB_BASE + 0x0c8), 27, 27
#define reg_cmd_abutnrcomp2_reg                           (DDRPAPB_BASE + 0x0c8), 26, 26
#define reg_cmd_abutnrcomp1_reg                           (DDRPAPB_BASE + 0x0c8), 25, 25
#define reg_cmd_abutnrcomp0_reg                           (DDRPAPB_BASE + 0x0c8), 24, 24

#define reg_cmd_abutprcomp4_reg                           (DDRPAPB_BASE + 0x0c8), 20, 20
#define reg_cmd_abutprcomp3_reg                           (DDRPAPB_BASE + 0x0c8), 19, 19
#define reg_cmd_abutprcomp2_reg                           (DDRPAPB_BASE + 0x0c8), 18, 18
#define reg_cmd_abutprcomp1_reg                           (DDRPAPB_BASE + 0x0c8), 17, 17
#define reg_cmd_abutprcomp0_reg                           (DDRPAPB_BASE + 0x0c8), 16, 16

#define reg_cmd_abutnrcomp4_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 12, 12
#define reg_cmd_abutnrcomp3_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 11, 11
#define reg_cmd_abutnrcomp2_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 10, 10
#define reg_cmd_abutnrcomp1_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 9 , 9
#define reg_cmd_abutnrcomp0_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 8 , 8

#define reg_cmd_abutprcomp4_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 4 , 4
#define reg_cmd_abutprcomp3_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 3 , 3
#define reg_cmd_abutprcomp2_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 2 , 2
#define reg_cmd_abutprcomp1_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 1 , 1
#define reg_cmd_abutprcomp0_ck0_reg                       (DDRPAPB_BASE + 0x0c8), 0 , 0

#define reg_ram_vref1_margsel_reg                         (DDRPAPB_BASE + 0x0cc), 24, 16
#define reg_cmd_abutnrcomp4_special_reg                   (DDRPAPB_BASE + 0x0cc), 12, 12
#define reg_cmd_abutnrcomp3_special_reg                   (DDRPAPB_BASE + 0x0cc), 11, 11
#define reg_cmd_abutnrcomp2_special_reg                   (DDRPAPB_BASE + 0x0cc), 10, 10
#define reg_cmd_abutnrcomp1_special_reg                   (DDRPAPB_BASE + 0x0cc), 9 , 9
#define reg_cmd_abutnrcomp0_special_reg                   (DDRPAPB_BASE + 0x0cc), 8 , 8
#define reg_cmd_abutprcomp4_special_reg                   (DDRPAPB_BASE + 0x0cc), 4 , 4
#define reg_cmd_abutprcomp3_special_reg                   (DDRPAPB_BASE + 0x0cc), 3 , 3
#define reg_cmd_abutprcomp2_special_reg                   (DDRPAPB_BASE + 0x0cc), 2 , 2
#define reg_cmd_abutprcomp1_special_reg                   (DDRPAPB_BASE + 0x0cc), 1 , 1
#define reg_cmd_abutprcomp0_special_reg                   (DDRPAPB_BASE + 0x0cc), 0 , 0
#define reg_a0_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d0), 31, 24
#define reg_a1_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d0), 23, 16
#define reg_a2_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d0), 15, 8
#define reg_a3_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d0), 7 , 0
#define reg_a4_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d4), 31, 24
#define reg_a5_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d4), 23, 16
#define reg_a6_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d4), 15, 8
#define reg_a7_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d4), 7 , 0
#define reg_a8_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d8), 31, 24
#define reg_a9_invdelaysel_bp                             (DDRPAPB_BASE + 0x0d8), 23, 16
#define reg_a10_invdelaysel_bp                            (DDRPAPB_BASE + 0x0d8), 15, 8
#define reg_a11_invdelaysel_bp                            (DDRPAPB_BASE + 0x0d8), 7 , 0
#define reg_a12_invdelaysel_bp                            (DDRPAPB_BASE + 0x0dc), 31, 24
#define reg_a13_invdelaysel_bp                            (DDRPAPB_BASE + 0x0dc), 23, 16
#define reg_a14_invdelaysel_bp                            (DDRPAPB_BASE + 0x0dc), 15, 8
#define reg_a15_invdelaysel_bp                            (DDRPAPB_BASE + 0x0dc), 7 , 0
#define reg_a16_invdelaysel_bp                            (DDRPAPB_BASE + 0x0e0), 31, 24
#define reg_a17_invdelaysel_bp                            (DDRPAPB_BASE + 0x0e0), 23, 16
#define reg_ba0_invdelaysel_bp                            (DDRPAPB_BASE + 0x0e0), 15, 8
#define reg_ba1_invdelaysel_bp                            (DDRPAPB_BASE + 0x0e0), 7 , 0
#define reg_bg0_invdelaysel_bp                            (DDRPAPB_BASE + 0x0e4), 31, 24
#define reg_bg1_invdelaysel_bp                            (DDRPAPB_BASE + 0x0e4), 23, 16
#define reg_cke0_invdelaysel_bp                           (DDRPAPB_BASE + 0x0e4), 15, 8
#define reg_cke1_invdelaysel_bp                           (DDRPAPB_BASE + 0x0e4), 7 , 0
#define reg_ckb_invdelaysel_bp                            (DDRPAPB_BASE + 0x0e8), 31, 24
#define reg_ck_invdelaysel_bp                             (DDRPAPB_BASE + 0x0e8), 23, 16
#define reg_odt0_invdelaysel_bp                           (DDRPAPB_BASE + 0x0e8), 15, 8
#define reg_odt1_invdelaysel_bp                           (DDRPAPB_BASE + 0x0e8), 7 , 0
#define reg_csb0_invdelaysel_bp                           (DDRPAPB_BASE + 0x0ec), 31, 24
#define reg_csb1_invdelaysel_bp                           (DDRPAPB_BASE + 0x0ec), 23, 16
#define reg_resetn_invdelaysel_bp                         (DDRPAPB_BASE + 0x0ec), 15, 8
#define reg_actn_invdelaysel_bp                           (DDRPAPB_BASE + 0x0ec), 7 , 0
#define group1_dq0_train_check_data_value0                (DDRPAPB_BASE + 0x0f0), 31, 24
#define group1_dq0_train_check_data_value1                (DDRPAPB_BASE + 0x0f0), 23, 16
#define group1_dq0_train_check_data_value2                (DDRPAPB_BASE + 0x0f0), 15, 8
#define group1_dq0_train_check_data_value3                (DDRPAPB_BASE + 0x0f0), 7 , 0
#define group1_dq0_train_check_data_value4                (DDRPAPB_BASE + 0x0f4), 31, 24
#define group1_dq0_train_check_data_value5                (DDRPAPB_BASE + 0x0f4), 23, 16
#define group1_dq0_train_check_data_value6                (DDRPAPB_BASE + 0x0f4), 15, 8
#define group1_dq0_train_check_data_value7                (DDRPAPB_BASE + 0x0f4), 7 , 0
#define group1_dq0_train_check_data_value8                (DDRPAPB_BASE + 0x0f8), 15, 8
#define group1_dq0_train_check_data_value9                (DDRPAPB_BASE + 0x0f8), 7 , 0
#define group1_dq1_train_check_data_value0                (DDRPAPB_BASE + 0x0fc), 31, 24
#define group1_dq1_train_check_data_value1                (DDRPAPB_BASE + 0x0fc), 23, 16
#define group1_dq1_train_check_data_value2                (DDRPAPB_BASE + 0x0fc), 15, 8
#define group1_dq1_train_check_data_value3                (DDRPAPB_BASE + 0x0fc), 7 , 0
#define group1_dq1_train_check_data_value4                (DDRPAPB_BASE + 0x100), 31, 24
#define group1_dq1_train_check_data_value5                (DDRPAPB_BASE + 0x100), 23, 16
#define group1_dq1_train_check_data_value6                (DDRPAPB_BASE + 0x100), 15, 8
#define group1_dq1_train_check_data_value7                (DDRPAPB_BASE + 0x100), 7 , 0
#define group1_dq1_train_check_data_value8                (DDRPAPB_BASE + 0x104), 15, 8
#define group1_dq1_train_check_data_value9                (DDRPAPB_BASE + 0x104), 7 , 0
#define group1_dq2_train_check_data_value0                (DDRPAPB_BASE + 0x108), 31, 24
#define group1_dq2_train_check_data_value1                (DDRPAPB_BASE + 0x108), 23, 16
#define group1_dq2_train_check_data_value2                (DDRPAPB_BASE + 0x108), 15, 8
#define group1_dq2_train_check_data_value3                (DDRPAPB_BASE + 0x108), 7 , 0
#define group1_dq2_train_check_data_value4                (DDRPAPB_BASE + 0x10c), 31, 24
#define group1_dq2_train_check_data_value5                (DDRPAPB_BASE + 0x10c), 23, 16
#define group1_dq2_train_check_data_value6                (DDRPAPB_BASE + 0x10c), 15, 8
#define group1_dq2_train_check_data_value7                (DDRPAPB_BASE + 0x10c), 7 , 0
#define group1_dq2_train_check_data_value8                (DDRPAPB_BASE + 0x110), 15, 8
#define group1_dq2_train_check_data_value9                (DDRPAPB_BASE + 0x110), 7 , 0
#define group1_dq3_train_check_data_value0                (DDRPAPB_BASE + 0x114), 31, 24
#define group1_dq3_train_check_data_value1                (DDRPAPB_BASE + 0x114), 23, 16
#define group1_dq3_train_check_data_value2                (DDRPAPB_BASE + 0x114), 15, 8
#define group1_dq3_train_check_data_value3                (DDRPAPB_BASE + 0x114), 7 , 0
#define group1_dq3_train_check_data_value4                (DDRPAPB_BASE + 0x118), 31, 24
#define group1_dq3_train_check_data_value5                (DDRPAPB_BASE + 0x118), 23, 16
#define group1_dq3_train_check_data_value6                (DDRPAPB_BASE + 0x118), 15, 8
#define group1_dq3_train_check_data_value7                (DDRPAPB_BASE + 0x118), 7 , 0
#define group1_dq3_train_check_data_value8                (DDRPAPB_BASE + 0x11c), 15, 8
#define group1_dq3_train_check_data_value9                (DDRPAPB_BASE + 0x11c), 7 , 0
#define group1_dq4_train_check_data_value0                (DDRPAPB_BASE + 0x120), 31, 24
#define group1_dq4_train_check_data_value1                (DDRPAPB_BASE + 0x120), 23, 16
#define group1_dq4_train_check_data_value2                (DDRPAPB_BASE + 0x120), 15, 8
#define group1_dq4_train_check_data_value3                (DDRPAPB_BASE + 0x120), 7 , 0
#define group1_dq4_train_check_data_value4                (DDRPAPB_BASE + 0x124), 31, 24
#define group1_dq4_train_check_data_value5                (DDRPAPB_BASE + 0x124), 23, 16
#define group1_dq4_train_check_data_value6                (DDRPAPB_BASE + 0x124), 15, 8
#define group1_dq4_train_check_data_value7                (DDRPAPB_BASE + 0x124), 7 , 0
#define group1_dq4_train_check_data_value8                (DDRPAPB_BASE + 0x128), 15, 8
#define group1_dq4_train_check_data_value9                (DDRPAPB_BASE + 0x128), 7 , 0
#define group1_dq5_train_check_data_value0                (DDRPAPB_BASE + 0x12c), 31, 24
#define group1_dq5_train_check_data_value1                (DDRPAPB_BASE + 0x12c), 23, 16
#define group1_dq5_train_check_data_value2                (DDRPAPB_BASE + 0x12c), 15, 8
#define group1_dq5_train_check_data_value3                (DDRPAPB_BASE + 0x12c), 7 , 0
#define group1_dq5_train_check_data_value4                (DDRPAPB_BASE + 0x130), 31, 24
#define group1_dq5_train_check_data_value5                (DDRPAPB_BASE + 0x130), 23, 16
#define group1_dq5_train_check_data_value6                (DDRPAPB_BASE + 0x130), 15, 8
#define group1_dq5_train_check_data_value7                (DDRPAPB_BASE + 0x130), 7 , 0
#define group1_dq5_train_check_data_value8                (DDRPAPB_BASE + 0x134), 15, 8
#define group1_dq5_train_check_data_value9                (DDRPAPB_BASE + 0x134), 7 , 0
#define group1_dq6_train_check_data_value0                (DDRPAPB_BASE + 0x138), 31, 24
#define group1_dq6_train_check_data_value1                (DDRPAPB_BASE + 0x138), 23, 16
#define group1_dq6_train_check_data_value2                (DDRPAPB_BASE + 0x138), 15, 8
#define group1_dq6_train_check_data_value3                (DDRPAPB_BASE + 0x138), 7 , 0
#define group1_dq6_train_check_data_value4                (DDRPAPB_BASE + 0x13c), 31, 24
#define group1_dq6_train_check_data_value5                (DDRPAPB_BASE + 0x13c), 23, 16
#define group1_dq6_train_check_data_value6                (DDRPAPB_BASE + 0x13c), 15, 8
#define group1_dq6_train_check_data_value7                (DDRPAPB_BASE + 0x13c), 7 , 0
#define group1_dq6_train_check_data_value8                (DDRPAPB_BASE + 0x140), 15, 8
#define group1_dq6_train_check_data_value9                (DDRPAPB_BASE + 0x140), 7 , 0
#define group1_dq7_train_check_data_value0                (DDRPAPB_BASE + 0x144), 31, 24
#define group1_dq7_train_check_data_value1                (DDRPAPB_BASE + 0x144), 23, 16
#define group1_dq7_train_check_data_value2                (DDRPAPB_BASE + 0x144), 15, 8
#define group1_dq7_train_check_data_value3                (DDRPAPB_BASE + 0x144), 7 , 0
#define group1_dq7_train_check_data_value4                (DDRPAPB_BASE + 0x148), 31, 24
#define group1_dq7_train_check_data_value5                (DDRPAPB_BASE + 0x148), 23, 16
#define group1_dq7_train_check_data_value6                (DDRPAPB_BASE + 0x148), 15, 8
#define group1_dq7_train_check_data_value7                (DDRPAPB_BASE + 0x148), 7 , 0
#define reg_wrtrain_odt_advance                           (DDRPAPB_BASE + 0x14c), 20, 20
#define reg_wrtrain_odt_keep                              (DDRPAPB_BASE + 0x14c), 19, 16
#define group1_dq7_train_check_data_value8                (DDRPAPB_BASE + 0x14c), 15, 8
#define group1_dq7_train_check_data_value9                (DDRPAPB_BASE + 0x14c), 7 , 0
#define reg_wrtrain_vref_wait_vref_cnt_50ns               (DDRPAPB_BASE + 0x150), 31, 24
#define reg_train_vref_step_min                           (DDRPAPB_BASE + 0x150), 23, 21
#define reg_train_vref_step_max                           (DDRPAPB_BASE + 0x150), 20, 16
#define reg_cmd_invdelaysel_sel                           (DDRPAPB_BASE + 0x150), 15, 10
#define reg_rdtrain_wait_vref_valid_cnt                   (DDRPAPB_BASE + 0x150), 9 , 0

#if defined(CONFIG_PRJ008)
#define reg_ram_vrefca_pd                                 (DDRPAPB_BASE + 0x154), 28, 28
#define reg_ram_vrefdq_pd                                 (DDRPAPB_BASE + 0x154), 27, 27
#endif

#define reg_ddrphy_trp                                    (DDRPAPB_BASE + 0x154), 26, 19
#define reg_wl_dqs_lock_point                             (DDRPAPB_BASE + 0x154), 18, 13
#define reg_freq_choose_wr_bypass                         (DDRPAPB_BASE + 0x154), 12, 12
#define reg_data_path_clk_gate_dly                        (DDRPAPB_BASE + 0x154), 11, 6
#define reg_data_path_clk_gate_dly_bp                     (DDRPAPB_BASE + 0x154), 5 , 5
#define reg_freq_choose_op_bypass                         (DDRPAPB_BASE + 0x154), 4 , 4
#define reg_all_freq_train_finish                         (DDRPAPB_BASE + 0x154), 3 , 3
#define reg_pll_lock_bypass                               (DDRPAPB_BASE + 0x154), 2 , 2
#define reg_pllpd_bypass                                  (DDRPAPB_BASE + 0x154), 1 , 1
#define reg_lpddr4_write_postamble_sel                    (DDRPAPB_BASE + 0x154), 0 , 0
#define group1_dm_train_check_data_value0                 (DDRPAPB_BASE + 0x1cc), 31, 24
#define group1_dm_train_check_data_value1                 (DDRPAPB_BASE + 0x1cc), 23, 16
#define group1_dm_train_check_data_value2                 (DDRPAPB_BASE + 0x1cc), 15, 8
#define group1_dm_train_check_data_value3                 (DDRPAPB_BASE + 0x1cc), 7 , 0
#define group1_dm_train_check_data_value4                 (DDRPAPB_BASE + 0x1e0), 31, 24
#define group1_dm_train_check_data_value5                 (DDRPAPB_BASE + 0x1e0), 23, 16
#define group1_dm_train_check_data_value6                 (DDRPAPB_BASE + 0x1e0), 15, 8
#define group1_dm_train_check_data_value7                 (DDRPAPB_BASE + 0x1e0), 7 , 0
#define reg_lp4_odt_bypass_value                          (DDRPAPB_BASE + 0x1e4), 31, 28
#define reg_lp4_odt_bypass_en                             (DDRPAPB_BASE + 0x1e4), 27, 27
#define reg_wrtrain_vref_scan_min                         (DDRPAPB_BASE + 0x1e4), 26, 20
#define group1_dm_train_check_data_value8                 (DDRPAPB_BASE + 0x1e4), 15, 8
#define group1_dm_train_check_data_value9                 (DDRPAPB_BASE + 0x1e4), 7 , 0
#define reg_cmd_2t_mode_value                             (DDRPAPB_BASE + 0x1f4), 31, 0
#define reg_dfx_pvt_cmp_cnt_clr                           (DDRPAPB_BASE + 0x1f8), 25, 25
#define reg_dfx_pvt_data_sel                              (DDRPAPB_BASE + 0x1f8), 24, 17
#define reg_rtrain_rank_num                               (DDRPAPB_BASE + 0x1f8), 16, 13
#define reg_rtrain_chk_done_prd                           (DDRPAPB_BASE + 0x1f8), 12, 5
#define reg_rtrain_cnt_clear                              (DDRPAPB_BASE + 0x1f8), 4 , 4
#define reg_pvt_compensation_cnt_clear                    (DDRPAPB_BASE + 0x1f8), 3 , 3
#define reg_ddrphy_pvt_rtrain_en                          (DDRPAPB_BASE + 0x1f8), 1 , 1
#define reg_ddrphy_wdqsoff_length                         (DDRPAPB_BASE + 0x1fc), 12, 7
#define reg_ddrphy_wdqson_length                          (DDRPAPB_BASE + 0x1fc), 6 , 1
#define reg_ddrphy_wdqs_crtl_mode2_en                     (DDRPAPB_BASE + 0x1fc), 0 , 0
// RO REGISTER LIST
#define train_all_step_done                               (DDRPAPB_BASE + 0x158), 7 , 7
#define train_step1_delay_done                            (DDRPAPB_BASE + 0x158), 6 , 6
#define train_step2_vref_done                             (DDRPAPB_BASE + 0x158), 5 , 5
#define train_step3_delay_done                            (DDRPAPB_BASE + 0x158), 4 , 4
#define train_step1_error                                 (DDRPAPB_BASE + 0x158), 3 , 3
#define train_step2_error                                 (DDRPAPB_BASE + 0x158), 2 , 2
#define train_step3_error                                 (DDRPAPB_BASE + 0x158), 1 , 1
#define train_true_done                                   (DDRPAPB_BASE + 0x158), 0 , 0
#define pvt_comp_cs_to_reg                                (DDRPAPB_BASE + 0x15c), 14, 8
#define pvt_comp_req_wait_time_out_to_reg                 (DDRPAPB_BASE + 0x15c), 7 , 7
#define cat_low_freq_sel                                  (DDRPAPB_BASE + 0x15c), 6 , 6
#define user_load_mode_busy                               (DDRPAPB_BASE + 0x15c), 5 , 5
#define pwrokcore                                         (DDRPAPB_BASE + 0x15c), 3 , 3
#define dll_lock_to_reg                                   (DDRPAPB_BASE + 0x15c), 2 , 2
#define lock_mpll                                         (DDRPAPB_BASE + 0x15c), 1 , 1
#define lock_pll_dqcmd                                    (DDRPAPB_BASE + 0x15c), 0 , 0
#define drvlegpd_zqcali_2reg                              (DDRPAPB_BASE + 0x160), 28, 24
#define drvlegpu_zqcali_2reg                              (DDRPAPB_BASE + 0x160), 20, 16
#define odtlegpd_zqcali_2reg                              (DDRPAPB_BASE + 0x160), 12, 8
#define odtlegpu_zqcali_2reg                              (DDRPAPB_BASE + 0x160), 4 , 0
#define reg_zqcali_done                                   (DDRPAPB_BASE + 0x164), 28, 28
#define rtrain_cnt_to_reg                                 (DDRPAPB_BASE + 0x164), 27, 24
#define pvt_compensation_cnt_to_reg                       (DDRPAPB_BASE + 0x164), 23, 20
#define osc_update_value                                  (DDRPAPB_BASE + 0x164), 19, 4
#define reg_drvpd_overflow                                (DDRPAPB_BASE + 0x164), 3 , 3
#define reg_drvpu_overflow                                (DDRPAPB_BASE + 0x164), 2 , 2
#define reg_odtpd_overflow                                (DDRPAPB_BASE + 0x164), 1 , 1
#define reg_odtpu_overflow                                (DDRPAPB_BASE + 0x164), 0 , 0
#define bist_error_dm                                     (DDRPAPB_BASE + 0x168), 16, 8
#define cmd_error_flag                                    (DDRPAPB_BASE + 0x168), 3 , 3
#define dm_error_flag                                     (DDRPAPB_BASE + 0x168), 2 , 2
#define dq_error_flag                                     (DDRPAPB_BASE + 0x168), 1 , 1
#define bist_complete                                     (DDRPAPB_BASE + 0x168), 0 , 0
#define bist_error_dq                                     (DDRPAPB_BASE + 0x16c), 31, 0
#define bist_error_cmd                                    (DDRPAPB_BASE + 0x170), 29, 0
#define wl_done_byte                                      (DDRPAPB_BASE + 0x174), 24, 16
#define reg_wl_end                                        (DDRPAPB_BASE + 0x174), 11, 11
#define calib_end                                         (DDRPAPB_BASE + 0x174), 10, 10
#define calib_error                                       (DDRPAPB_BASE + 0x174), 9 , 9
#define calib_done_byte                                   (DDRPAPB_BASE + 0x174), 8 , 0
#define cha_rank_cat_bp_cmd_send_rdy                      (DDRPAPB_BASE + 0x178), 19, 18
#define chb_rank_cat_bp_cmd_send_rdy                      (DDRPAPB_BASE + 0x178), 17, 16
#define cha_rank_cat_bp_done                              (DDRPAPB_BASE + 0x178), 15, 14
#define chb_rank_cat_bp_done                              (DDRPAPB_BASE + 0x178), 13, 12
#define ca_check_value                                    (DDRPAPB_BASE + 0x178), 11, 0
#define cha_cat_auto_cs_train_err                         (DDRPAPB_BASE + 0x17c), 27, 18
#define chb_cat_auto_cs_train_err                         (DDRPAPB_BASE + 0x17c), 17, 8
#define cha_cat_done                                      (DDRPAPB_BASE + 0x17c), 3 , 3
#define chb_cat_done                                      (DDRPAPB_BASE + 0x17c), 2 , 2
#define cha_cat_bp_cmd_send_done                          (DDRPAPB_BASE + 0x17c), 1 , 1
#define chb_cat_bp_cmd_send_done                          (DDRPAPB_BASE + 0x17c), 0 , 0
#define reg_cmd_invdelaysel                               (DDRPAPB_BASE + 0x180), 23, 16
#define reg_wrtrain_vref_max_value                        (DDRPAPB_BASE + 0x180), 14, 8
#define reg_wrtrain_vref_min_value                        (DDRPAPB_BASE + 0x180), 6 , 0
#define cha_rank0_ca0_min_perbit_skew_pass                (DDRPAPB_BASE + 0x184), 31, 24
#define cha_rank0_ca1_min_perbit_skew_pass                (DDRPAPB_BASE + 0x184), 23, 16
#define cha_rank0_ca2_min_perbit_skew_pass                (DDRPAPB_BASE + 0x184), 15, 8
#define cha_rank0_ca3_min_perbit_skew_pass                (DDRPAPB_BASE + 0x184), 7 , 0
#define cha_rank0_ca4_min_perbit_skew_pass                (DDRPAPB_BASE + 0x188), 31, 24
#define cha_rank0_ca5_min_perbit_skew_pass                (DDRPAPB_BASE + 0x188), 23, 16
#define cha_rank1_ca0_min_perbit_skew_pass                (DDRPAPB_BASE + 0x188), 15, 8
#define cha_rank1_ca1_min_perbit_skew_pass                (DDRPAPB_BASE + 0x188), 7 , 0
#define cha_rank1_ca2_min_perbit_skew_pass                (DDRPAPB_BASE + 0x18c), 31, 24
#define cha_rank1_ca3_min_perbit_skew_pass                (DDRPAPB_BASE + 0x18c), 23, 16
#define cha_rank1_ca4_min_perbit_skew_pass                (DDRPAPB_BASE + 0x18c), 15, 8
#define cha_rank1_ca5_min_perbit_skew_pass                (DDRPAPB_BASE + 0x18c), 7 , 0
#define cha_rank0_min_cs_perbit_skew_pass                 (DDRPAPB_BASE + 0x190), 31, 24
#define cha_rank1_min_cs_perbit_skew_pass                 (DDRPAPB_BASE + 0x190), 23, 16
#define cha_rank0_max_cs_perbit_skew_pass                 (DDRPAPB_BASE + 0x190), 15, 8
#define cha_rank1_max_cs_perbit_skew_pass                 (DDRPAPB_BASE + 0x190), 7 , 0
#define cha_rank0_ca0_max_perbit_skew_pass                (DDRPAPB_BASE + 0x194), 31, 24
#define cha_rank0_ca1_max_perbit_skew_pass                (DDRPAPB_BASE + 0x194), 23, 16
#define cha_rank0_ca2_max_perbit_skew_pass                (DDRPAPB_BASE + 0x194), 15, 8
#define cha_rank0_ca3_max_perbit_skew_pass                (DDRPAPB_BASE + 0x194), 7 , 0
#define cha_rank0_ca4_max_perbit_skew_pass                (DDRPAPB_BASE + 0x198), 31, 24
#define cha_rank0_ca5_max_perbit_skew_pass                (DDRPAPB_BASE + 0x198), 23, 16
#define cha_rank1_ca0_max_perbit_skew_pass                (DDRPAPB_BASE + 0x198), 15, 8
#define cha_rank1_ca1_max_perbit_skew_pass                (DDRPAPB_BASE + 0x198), 7 , 0
#define cha_rank1_ca2_max_perbit_skew_pass                (DDRPAPB_BASE + 0x19c), 31, 24
#define cha_rank1_ca3_max_perbit_skew_pass                (DDRPAPB_BASE + 0x19c), 23, 16
#define cha_rank1_ca4_max_perbit_skew_pass                (DDRPAPB_BASE + 0x19c), 15, 8
#define cha_rank1_ca5_max_perbit_skew_pass                (DDRPAPB_BASE + 0x19c), 7 , 0
#define chb_rank0_ca0_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a0), 31, 24
#define chb_rank0_ca1_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a0), 23, 16
#define chb_rank0_ca2_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a0), 15, 8
#define chb_rank0_ca3_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a0), 7 , 0
#define chb_rank0_ca4_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a4), 31, 24
#define chb_rank0_ca5_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a4), 23, 16
#define chb_rank1_ca0_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a4), 15, 8
#define chb_rank1_ca1_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a4), 7 , 0
#define chb_rank1_ca2_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a8), 31, 24
#define chb_rank1_ca3_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a8), 23, 16
#define chb_rank1_ca4_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a8), 15, 8
#define chb_rank1_ca5_min_perbit_skew_pass                (DDRPAPB_BASE + 0x1a8), 7 , 0
#define chb_rank0_min_cs_perbit_skew_pass                 (DDRPAPB_BASE + 0x1ac), 31, 24
#define chb_rank1_min_cs_perbit_skew_pass                 (DDRPAPB_BASE + 0x1ac), 23, 16
#define chb_rank0_max_cs_perbit_skew_pass                 (DDRPAPB_BASE + 0x1ac), 15, 8
#define chb_rank1_max_cs_perbit_skew_pass                 (DDRPAPB_BASE + 0x1ac), 7 , 0
#define chb_rank0_ca0_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b0), 31, 24
#define chb_rank0_ca1_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b0), 23, 16
#define chb_rank0_ca2_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b0), 15, 8
#define chb_rank0_ca3_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b0), 7 , 0
#define chb_rank0_ca4_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b4), 31, 24
#define chb_rank0_ca5_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b4), 23, 16
#define chb_rank1_ca0_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b4), 15, 8
#define chb_rank1_ca1_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b4), 7 , 0
#define chb_rank1_ca2_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b8), 31, 24
#define chb_rank1_ca3_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b8), 23, 16
#define chb_rank1_ca4_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b8), 15, 8
#define chb_rank1_ca5_max_perbit_skew_pass                (DDRPAPB_BASE + 0x1b8), 7 , 0
#define cha_rank0_min_cs_vref_pass                        (DDRPAPB_BASE + 0x1bc), 29, 24
#define cha_rank0_max_cs_vref_pass                        (DDRPAPB_BASE + 0x1bc), 21, 16
#define cha_rank1_min_cs_vref_pass                        (DDRPAPB_BASE + 0x1bc), 13, 8
#define cha_rank1_max_cs_vref_pass                        (DDRPAPB_BASE + 0x1bc), 5 , 0
#define chb_rank0_min_cs_vref_pass                        (DDRPAPB_BASE + 0x1c0), 29, 24
#define chb_rank0_max_cs_vref_pass                        (DDRPAPB_BASE + 0x1c0), 21, 16
#define chb_rank1_min_cs_vref_pass                        (DDRPAPB_BASE + 0x1c0), 13, 8
#define chb_rank1_max_cs_vref_pass                        (DDRPAPB_BASE + 0x1c0), 5 , 0
#define reg_train_error_for_rd_byte                       (DDRPAPB_BASE + 0x1c4), 26, 18
#define reg_wr_train_done_byte                            (DDRPAPB_BASE + 0x1c4), 17, 9
#define reg_wr_train_error_byte                           (DDRPAPB_BASE + 0x1c4), 8 , 0
#define reg_train_done_for_rd_to_reg_byte                 (DDRPAPB_BASE + 0x1c8), 31, 23
#define mdll_timeout_to_reg                               (DDRPAPB_BASE + 0x1c8), 16, 16
#define mdll_update_cnt                                   (DDRPAPB_BASE + 0x1c8), 14, 8
#define halfui_lock_code_to_reg                           (DDRPAPB_BASE + 0x1c8), 7 , 0
#define halfui_lock_code                                  (DDRPAPB_BASE + 0x1e8), 24, 17
#define bist_error_dq_byte8                               (DDRPAPB_BASE + 0x1e8), 16, 9
#define reg_rd_train_readback_data_valid_byte             (DDRPAPB_BASE + 0x1e8), 8 , 0
#define bist_error_dq_byte4_7                             (DDRPAPB_BASE + 0x1ec), 31, 0
#define reg_dfx_pvt_cmp_read_back_data                    (DDRPAPB_BASE + 0x1f0), 31, 0


// RW REGISTER LIST

#define reg_a_l_vref1_margsel_reg              (DDRPAPB_BASE + 0x200), 31, 23
#define reg_a_l_dq_odt_zqcali_en               (DDRPAPB_BASE + 0x200), 22, 22
#define reg_a_l_dq_drv_zqcali_en               (DDRPAPB_BASE + 0x200), 21, 21
#define reg_a_l_abutweakpddq_reg               (DDRPAPB_BASE + 0x200), 20, 20
#define reg_a_l_abutweakpubdq_reg              (DDRPAPB_BASE + 0x200), 19, 19
#define reg_a_l_abutdiffampseen_reg            (DDRPAPB_BASE + 0x200), 18, 18
#define reg_a_l_vref1_pd_reg                   (DDRPAPB_BASE + 0x200), 17, 17
#define reg_a_l_weakpd_reg                     (DDRPAPB_BASE + 0x200), 16, 15
#define reg_a_l_weakpub_reg                    (DDRPAPB_BASE + 0x200), 14, 13
#define reg_a_l_abutslewpu_reg                 (DDRPAPB_BASE + 0x200), 12, 8
#define reg_a_l_enb_lp4mode_reg                (DDRPAPB_BASE + 0x200), 7 , 7
#define reg_a_l_dqsweakpd_reg                  (DDRPAPB_BASE + 0x200), 6 , 6
#define reg_a_l_dqsbweakpub_reg                (DDRPAPB_BASE + 0x200), 5 , 5
#define reg_a_l_abutslewpd_reg                 (DDRPAPB_BASE + 0x200), 4 , 0
#define reg_a_l_dqfben_reg                     (DDRPAPB_BASE + 0x204), 30, 30
#define reg_a_l_dqfbsel_reg                    (DDRPAPB_BASE + 0x204), 29, 29
#define reg_a_l_abutnrcompdq_reg               (DDRPAPB_BASE + 0x204), 28, 24    // The driver pull-down resistance of byte0.Refer to CMD IO Drive Strengt hin the Databook.
#define reg_a_l_abutprcompdq_reg               (DDRPAPB_BASE + 0x204), 20, 16    // The driver pull-up resistance of byte0.Refer to CMD IO Drive Strengt hin the Databook.
#define reg_a_l_abutodtpddq_reg                (DDRPAPB_BASE + 0x204), 12, 8     // The ODT pull-down resistance of byte0.Refer to CMD IO Drive Strengt hin the Databook.
#define reg_a_l_abutodtpudq_reg                (DDRPAPB_BASE + 0x204), 4 , 0     // The ODT pull-up resistance of byte0.Refer to CMD IO Drive Strengt hin the Databook.

#define reg_a_l_abutnrcompdq4_reg              (DDRPAPB_BASE + 0x204), 28, 28
#define reg_a_l_abutnrcompdq3_reg              (DDRPAPB_BASE + 0x204), 27, 27
#define reg_a_l_abutnrcompdq2_reg              (DDRPAPB_BASE + 0x204), 26, 26
#define reg_a_l_abutnrcompdq1_reg              (DDRPAPB_BASE + 0x204), 25, 25
#define reg_a_l_abutnrcompdq0_reg              (DDRPAPB_BASE + 0x204), 24, 24
#define reg_a_l_abutprcompdq4_reg              (DDRPAPB_BASE + 0x204), 20, 20
#define reg_a_l_abutprcompdq3_reg              (DDRPAPB_BASE + 0x204), 19, 19
#define reg_a_l_abutprcompdq2_reg              (DDRPAPB_BASE + 0x204), 18, 18
#define reg_a_l_abutprcompdq1_reg              (DDRPAPB_BASE + 0x204), 17, 17
#define reg_a_l_abutprcompdq0_reg              (DDRPAPB_BASE + 0x204), 16, 16
#define reg_a_l_abutodtpddq4_reg               (DDRPAPB_BASE + 0x204), 12, 12
#define reg_a_l_abutodtpddq3_reg               (DDRPAPB_BASE + 0x204), 11, 11
#define reg_a_l_abutodtpddq2_reg               (DDRPAPB_BASE + 0x204), 10, 10
#define reg_a_l_abutodtpddq1_reg               (DDRPAPB_BASE + 0x204), 9 , 9
#define reg_a_l_abutodtpddq0_reg               (DDRPAPB_BASE + 0x204), 8 , 8
#define reg_a_l_abutodtpudq4_reg               (DDRPAPB_BASE + 0x204), 4 , 4
#define reg_a_l_abutodtpudq3_reg               (DDRPAPB_BASE + 0x204), 3 , 3
#define reg_a_l_abutodtpudq2_reg               (DDRPAPB_BASE + 0x204), 2 , 2
#define reg_a_l_abutodtpudq1_reg               (DDRPAPB_BASE + 0x204), 1 , 1
#define reg_a_l_abutodtpudq0_reg               (DDRPAPB_BASE + 0x204), 0 , 0

#define reg_a_l_rxmen0_delay_bp                (DDRPAPB_BASE + 0x208), 26, 24
#define reg_a_l_rxmen0_ophsel_bp               (DDRPAPB_BASE + 0x208), 23, 21
#define reg_a_l_rxmen0_sdlltap_bp              (DDRPAPB_BASE + 0x208), 20, 16
#define reg_a_h_rxmen0_delay_bp                (DDRPAPB_BASE + 0x308), 26, 24
#define reg_a_h_rxmen0_ophsel_bp               (DDRPAPB_BASE + 0x308), 23, 21
#define reg_a_h_rxmen0_sdlltap_bp              (DDRPAPB_BASE + 0x308), 20, 16
#define reg_a_l_rxmen1_delay_bp                (DDRPAPB_BASE + 0x208), 10, 8
#define reg_a_l_rxmen1_ophsel_bp               (DDRPAPB_BASE + 0x208), 7 , 5
#define reg_a_l_rxmen1_sdlltap_bp              (DDRPAPB_BASE + 0x208), 4 , 0
#define reg_a_l_rdodt0_delay                   (DDRPAPB_BASE + 0x20c), 26, 24
#define reg_a_l_rdodt0_ophsel                  (DDRPAPB_BASE + 0x20c), 23, 21
#define reg_a_l_rdodt0_dllsel                  (DDRPAPB_BASE + 0x20c), 20, 16
#define reg_a_l_rdodt1_delay                   (DDRPAPB_BASE + 0x20c), 10, 8
#define reg_a_l_rdodt1_ophsel                  (DDRPAPB_BASE + 0x20c), 7 , 5
#define reg_a_l_rdodt1_dllsel                  (DDRPAPB_BASE + 0x20c), 4 , 0
#define reg_a_l_dq_invdelay_lp_en              (DDRPAPB_BASE + 0x210), 29, 29
#define reg_a_l_dm_obsdataen                   (DDRPAPB_BASE + 0x210), 28, 28
#define reg_a_l_dqobsmuxsel                    (DDRPAPB_BASE + 0x210), 27, 24
#define reg_a_l_rxen_lp4                       (DDRPAPB_BASE + 0x210), 23, 23
#define reg_a_l_lp4x_en                        (DDRPAPB_BASE + 0x210), 22, 22
#define reg_a_l_pvt_comp_en                    (DDRPAPB_BASE + 0x210), 21, 21
#define reg_a_l_dqout_mux                      (DDRPAPB_BASE + 0x210), 20, 20
#define reg_a_l_dmout_mux                      (DDRPAPB_BASE + 0x210), 19, 19
#define reg_a_l_dq_ph90en_bp                   (DDRPAPB_BASE + 0x210), 18, 18
#define reg_a_l_dqs_ph90en_bp                  (DDRPAPB_BASE + 0x210), 17, 17
#define reg_a_l_rcvdqsmodsel                   (DDRPAPB_BASE + 0x210), 16, 16
#define reg_a_l_selfclren                      (DDRPAPB_BASE + 0x210), 15, 15
#define reg_a_l_wrptrclrb                      (DDRPAPB_BASE + 0x210), 14, 14
#define reg_a_l_rxm_odiffampen                 (DDRPAPB_BASE + 0x210), 13, 13
#define reg_a_l_abutobsmodeen                  (DDRPAPB_BASE + 0x210), 12, 12
#define reg_a_l_rxm4p5en                       (DDRPAPB_BASE + 0x210), 10, 10
#define reg_a_l_tsm_iobufact_bp                (DDRPAPB_BASE + 0x210), 9 , 9
#define reg_a_l_rxpst_bp                       (DDRPAPB_BASE + 0x210), 8 , 8
#define reg_a_l_rxm4p5en_r2                    (DDRPAPB_BASE + 0x210), 6 , 6
#define reg_a_l_rrankdly_4x_cs0                (DDRPAPB_BASE + 0x210), 5 , 3
#define reg_a_l_rrankdly_4x_cs1                (DDRPAPB_BASE + 0x210), 2 , 0
#define reg_a_l_rrankdly_1x_cs1                (DDRPAPB_BASE + 0x214), 18, 16
#define reg_a_l_rrankdly_1x_cs0                (DDRPAPB_BASE + 0x214), 14, 12
#define reg_a_l_wrankphsel                     (DDRPAPB_BASE + 0x214), 10, 8
#define reg_a_l_wrankdlysel                    (DDRPAPB_BASE + 0x214), 7 , 0
#define reg_a_l_cs0_dm_invdelaysel             (DDRPAPB_BASE + 0x218), 26, 18
#define reg_a_l_cs0_dq0_invdelaysel            (DDRPAPB_BASE + 0x218), 17, 9
#define reg_a_l_cs0_dq1_invdelaysel            (DDRPAPB_BASE + 0x218), 8 , 0
#define reg_a_l_cs0_dq2_invdelaysel            (DDRPAPB_BASE + 0x21c), 26, 18
#define reg_a_l_cs0_dq3_invdelaysel            (DDRPAPB_BASE + 0x21c), 17, 9
#define reg_a_l_cs0_dq4_invdelaysel            (DDRPAPB_BASE + 0x21c), 8 , 0
#define reg_a_l_cs0_dq5_invdelaysel            (DDRPAPB_BASE + 0x220), 26, 18
#define reg_a_l_cs0_dq6_invdelaysel            (DDRPAPB_BASE + 0x220), 17, 9
#define reg_a_l_cs0_dq7_invdelaysel            (DDRPAPB_BASE + 0x220), 8 , 0
#define reg_a_l_cs0_dqs_invdelaysel            (DDRPAPB_BASE + 0x224), 31, 24
#define reg_a_l_cs0_dqsb_invdelaysel           (DDRPAPB_BASE + 0x224), 15, 8
#define reg_a_l_cs0_loop_invdelaysel           (DDRPAPB_BASE + 0x228), 28, 24
#if 0
#define reg_a_l_cs0_dm_invdelayselrx           (DDRPAPB_BASE + 0x228), 14, 8
#define reg_a_l_cs0_dq0_invdelayselrx          (DDRPAPB_BASE + 0x22c), 30, 24
#define reg_a_l_cs0_dq1_invdelayselrx          (DDRPAPB_BASE + 0x22c), 22, 16
#define reg_a_l_cs0_dq2_invdelayselrx          (DDRPAPB_BASE + 0x22c), 14, 8
#define reg_a_l_cs0_dq3_invdelayselrx          (DDRPAPB_BASE + 0x22c), 6 , 0
#define reg_a_l_cs0_dq4_invdelayselrx          (DDRPAPB_BASE + 0x230), 30, 24
#define reg_a_l_cs0_dq5_invdelayselrx          (DDRPAPB_BASE + 0x230), 22, 16
#define reg_a_l_cs0_dq6_invdelayselrx          (DDRPAPB_BASE + 0x230), 14, 8
#define reg_a_l_cs0_dq7_invdelayselrx          (DDRPAPB_BASE + 0x230), 6 , 0
#define reg_a_l_cs0_dqs_invdelayselrx          (DDRPAPB_BASE + 0x234), 30, 24
#define reg_a_l_cs0_dqsb_invdelayselrx         (DDRPAPB_BASE + 0x234), 14, 8

#define reg_a_h_cs0_dm_invdelayselrx           (DDRPAPB_BASE + 0x328), 14, 8
#define reg_a_h_cs0_dq0_invdelayselrx          (DDRPAPB_BASE + 0x32c), 30, 24
#define reg_a_h_cs0_dq1_invdelayselrx          (DDRPAPB_BASE + 0x32c), 22, 16
#define reg_a_h_cs0_dq2_invdelayselrx          (DDRPAPB_BASE + 0x32c), 14, 8
#define reg_a_h_cs0_dq3_invdelayselrx          (DDRPAPB_BASE + 0x32c), 6 , 0
#define reg_a_h_cs0_dq4_invdelayselrx          (DDRPAPB_BASE + 0x330), 30, 24
#define reg_a_h_cs0_dq5_invdelayselrx          (DDRPAPB_BASE + 0x330), 22, 16
#define reg_a_h_cs0_dq6_invdelayselrx          (DDRPAPB_BASE + 0x330), 14, 8
#define reg_a_h_cs0_dq7_invdelayselrx          (DDRPAPB_BASE + 0x330), 6 , 0
#define reg_a_h_cs0_dqs_invdelayselrx          (DDRPAPB_BASE + 0x334), 30, 24
#define reg_a_h_cs0_dqsb_invdelayselrx         (DDRPAPB_BASE + 0x334), 14, 8
#else
#define reg_a_l_cs0_dm_invdelayselrx           (DDRPAPB_BASE + 0x228), 15, 8
#define reg_a_l_cs0_dq0_invdelayselrx          (DDRPAPB_BASE + 0x22c), 31, 24
#define reg_a_l_cs0_dq1_invdelayselrx          (DDRPAPB_BASE + 0x22c), 23, 16
#define reg_a_l_cs0_dq2_invdelayselrx          (DDRPAPB_BASE + 0x22c), 15, 8
#define reg_a_l_cs0_dq3_invdelayselrx          (DDRPAPB_BASE + 0x22c), 7 , 0
#define reg_a_l_cs0_dq4_invdelayselrx          (DDRPAPB_BASE + 0x230), 31, 24
#define reg_a_l_cs0_dq5_invdelayselrx          (DDRPAPB_BASE + 0x230), 23, 16
#define reg_a_l_cs0_dq6_invdelayselrx          (DDRPAPB_BASE + 0x230), 15, 8
#define reg_a_l_cs0_dq7_invdelayselrx          (DDRPAPB_BASE + 0x230), 7 , 0
#define reg_a_l_cs0_dqs_invdelayselrx          (DDRPAPB_BASE + 0x234), 31, 24
#define reg_a_l_cs0_dqsb_invdelayselrx         (DDRPAPB_BASE + 0x234), 15, 8

#define reg_a_h_cs0_dm_invdelayselrx           (DDRPAPB_BASE + 0x328), 15, 8
#define reg_a_h_cs0_dq0_invdelayselrx          (DDRPAPB_BASE + 0x32c), 31, 24
#define reg_a_h_cs0_dq1_invdelayselrx          (DDRPAPB_BASE + 0x32c), 23, 16
#define reg_a_h_cs0_dq2_invdelayselrx          (DDRPAPB_BASE + 0x32c), 15, 8
#define reg_a_h_cs0_dq3_invdelayselrx          (DDRPAPB_BASE + 0x32c), 7 , 0
#define reg_a_h_cs0_dq4_invdelayselrx          (DDRPAPB_BASE + 0x330), 31, 24
#define reg_a_h_cs0_dq5_invdelayselrx          (DDRPAPB_BASE + 0x330), 23, 16
#define reg_a_h_cs0_dq6_invdelayselrx          (DDRPAPB_BASE + 0x330), 15, 8
#define reg_a_h_cs0_dq7_invdelayselrx          (DDRPAPB_BASE + 0x330), 7 , 0
#define reg_a_h_cs0_dqs_invdelayselrx          (DDRPAPB_BASE + 0x334), 31, 24
#define reg_a_h_cs0_dqsb_invdelayselrx         (DDRPAPB_BASE + 0x334), 15, 8
#endif
#define reg_a_l_cs1_dm_invdelaysel             (DDRPAPB_BASE + 0x238), 26, 18
#define reg_a_l_cs1_dq0_invdelaysel            (DDRPAPB_BASE + 0x238), 17, 9
#define reg_a_l_cs1_dq1_invdelaysel            (DDRPAPB_BASE + 0x238), 8 , 0
#define reg_a_l_cs1_dq2_invdelaysel            (DDRPAPB_BASE + 0x23c), 26, 18
#define reg_a_l_cs1_dq3_invdelaysel            (DDRPAPB_BASE + 0x23c), 17, 9
#define reg_a_l_cs1_dq4_invdelaysel            (DDRPAPB_BASE + 0x23c), 8 , 0
#define reg_a_l_cs1_dq5_invdelaysel            (DDRPAPB_BASE + 0x240), 26, 18
#define reg_a_l_cs1_dq6_invdelaysel            (DDRPAPB_BASE + 0x240), 17, 9
#define reg_a_l_cs1_dq7_invdelaysel            (DDRPAPB_BASE + 0x240), 8 , 0
#define reg_a_l_cs1_dqs_invdelaysel            (DDRPAPB_BASE + 0x244), 31, 24
#define reg_a_l_cs1_dqsb_invdelaysel           (DDRPAPB_BASE + 0x244), 15, 8
#define reg_a_l_cs1_loop_invdelaysel           (DDRPAPB_BASE + 0x248), 28, 24
#define reg_a_l_cs1_dm_invdelayselrx           (DDRPAPB_BASE + 0x248), 14, 8
#define reg_a_l_cs1_dq0_invdelayselrx          (DDRPAPB_BASE + 0x24c), 30, 24
#define reg_a_l_cs1_dq1_invdelayselrx          (DDRPAPB_BASE + 0x24c), 22, 16
#define reg_a_l_cs1_dq2_invdelayselrx          (DDRPAPB_BASE + 0x24c), 14, 8
#define reg_a_l_cs1_dq3_invdelayselrx          (DDRPAPB_BASE + 0x24c), 6 , 0
#define reg_a_l_cs1_dq4_invdelayselrx          (DDRPAPB_BASE + 0x250), 30, 24
#define reg_a_l_cs1_dq5_invdelayselrx          (DDRPAPB_BASE + 0x250), 22, 16
#define reg_a_l_cs1_dq6_invdelayselrx          (DDRPAPB_BASE + 0x250), 14, 8
#define reg_a_l_cs1_dq7_invdelayselrx          (DDRPAPB_BASE + 0x250), 6 , 0
#define reg_a_l_cs1_dqs_invdelayselrx          (DDRPAPB_BASE + 0x254), 30, 24
#define reg_a_l_cs1_dqsb_invdelayselrx         (DDRPAPB_BASE + 0x254), 14, 8
#define reg_a_l_rd_train_dqs_default           (DDRPAPB_BASE + 0x258), 30, 24
#define reg_a_l_train_dqs_default              (DDRPAPB_BASE + 0x258), 23, 16
#define reg_a_l_rd_train_dqs_range_max         (DDRPAPB_BASE + 0x258), 14, 8
#define reg_a_l_rd_train_dqs_range_min         (DDRPAPB_BASE + 0x258), 6 , 0
#define reg_a_l_rdtrain_check_wrap0            (DDRPAPB_BASE + 0x25c), 31, 24
#define reg_a_l_rdtrain_check_wrap1            (DDRPAPB_BASE + 0x25c), 23, 16
#define reg_a_l_cat_wrap_sel                   (DDRPAPB_BASE + 0x25c), 15, 14
#define reg_a_l_dm_bit_wrap_sel                (DDRPAPB_BASE + 0x25c), 13, 10
#define reg_a_l_dq7_bit_wrap_sel               (DDRPAPB_BASE + 0x260), 31, 28
#define reg_a_l_dq6_bit_wrap_sel               (DDRPAPB_BASE + 0x260), 27, 24
#define reg_a_l_dq5_bit_wrap_sel               (DDRPAPB_BASE + 0x260), 23, 20
#define reg_a_l_dq4_bit_wrap_sel               (DDRPAPB_BASE + 0x260), 19, 16
#define reg_a_l_dq3_bit_wrap_sel               (DDRPAPB_BASE + 0x260), 15, 12
#define reg_a_l_dq2_bit_wrap_sel               (DDRPAPB_BASE + 0x260), 11, 8
#define reg_a_l_dq1_bit_wrap_sel               (DDRPAPB_BASE + 0x260), 7 , 4
#define reg_a_l_dq0_bit_wrap_sel               (DDRPAPB_BASE + 0x260), 3 , 0


// RO REGISTER LIST
#define reg_a_l_dqs_idqshigh                   (DDRPAPB_BASE + 0x280), 30, 30
#define reg_a_l_tdqs_invdelaysel0              (DDRPAPB_BASE + 0x280), 23, 16
#define reg_a_l_tdqs_invdelaysel1              (DDRPAPB_BASE + 0x280), 7 , 0
#define reg_a_l_cycsel                         (DDRPAPB_BASE + 0x284), 10, 8
#define reg_a_l_ophsel                         (DDRPAPB_BASE + 0x284), 7 , 5
#define reg_a_l_dllsel                         (DDRPAPB_BASE + 0x284), 4 , 0
#define reg_a_l_calib_result_cs0               (DDRPAPB_BASE + 0x288), 26, 16
#define reg_a_l_calib_result_cs1               (DDRPAPB_BASE + 0x288), 10, 0
#define reg_a_l_cs0_value_dqx_invdelaysel      (DDRPAPB_BASE + 0x28c), 8 , 0
#define reg_a_l_cs1_value_dqx_invdelaysel      (DDRPAPB_BASE + 0x290), 8 , 0
#define reg_a_l_train_min_for_rd_dq0           (DDRPAPB_BASE + 0x294), 30, 24
#define reg_a_l_train_min_for_rd_dq1           (DDRPAPB_BASE + 0x294), 22, 16
#define reg_a_l_train_min_for_rd_dq2           (DDRPAPB_BASE + 0x294), 14, 8
#define reg_a_l_train_min_for_rd_dq3           (DDRPAPB_BASE + 0x294), 6 , 0
#define reg_a_l_train_min_for_rd_dq4           (DDRPAPB_BASE + 0x298), 30, 24
#define reg_a_l_train_min_for_rd_dq5           (DDRPAPB_BASE + 0x298), 22, 16
#define reg_a_l_train_min_for_rd_dq6           (DDRPAPB_BASE + 0x298), 14, 8
#define reg_a_l_train_min_for_rd_dq7           (DDRPAPB_BASE + 0x298), 6 , 0
#define reg_a_l_train_min_for_rd_dqs           (DDRPAPB_BASE + 0x29c), 14, 8
#define reg_a_l_train_max_for_rd_dq0           (DDRPAPB_BASE + 0x2a0), 30, 24
#define reg_a_l_train_max_for_rd_dq1           (DDRPAPB_BASE + 0x2a0), 22, 16
#define reg_a_l_train_max_for_rd_dq2           (DDRPAPB_BASE + 0x2a0), 14, 8
#define reg_a_l_train_max_for_rd_dq3           (DDRPAPB_BASE + 0x2a0), 6 , 0
#define reg_a_l_train_max_for_rd_dq4           (DDRPAPB_BASE + 0x2a4), 30, 24
#define reg_a_l_train_max_for_rd_dq5           (DDRPAPB_BASE + 0x2a4), 22, 16
#define reg_a_l_train_max_for_rd_dq6           (DDRPAPB_BASE + 0x2a4), 14, 8
#define reg_a_l_train_max_for_rd_dq7           (DDRPAPB_BASE + 0x2a4), 6 , 0
#define reg_a_l_train_max_for_rd_dqs           (DDRPAPB_BASE + 0x2a8), 14, 8
#define reg_a_l_train_result_for_rd_base_dqs   (DDRPAPB_BASE + 0x2ac), 30, 24
#define reg_a_l_change_rd_dqs_default          (DDRPAPB_BASE + 0x2ac), 5 , 5
#define reg_a_l_left_boundary_overflow_for_rd  (DDRPAPB_BASE + 0x2ac), 3 , 3
#define reg_a_l_right_boundary_overflow_for_rd (DDRPAPB_BASE + 0x2ac), 2 , 2
#define reg_a_l_rd_train_readback_data_dq0     (DDRPAPB_BASE + 0x2b0), 31, 16
#define reg_a_l_rd_train_readback_data_dq1     (DDRPAPB_BASE + 0x2b0), 15, 0
#define reg_a_l_rd_train_readback_data_dq2     (DDRPAPB_BASE + 0x2b4), 31, 16
#define reg_a_l_rd_train_readback_data_dq3     (DDRPAPB_BASE + 0x2b4), 15, 0
#define reg_a_l_rd_train_readback_data_dq4     (DDRPAPB_BASE + 0x2b8), 31, 16
#define reg_a_l_rd_train_readback_data_dq5     (DDRPAPB_BASE + 0x2b8), 15, 0
#define reg_a_l_rd_train_readback_data_dq6     (DDRPAPB_BASE + 0x2bc), 31, 16
#define reg_a_l_rd_train_readback_data_dq7     (DDRPAPB_BASE + 0x2bc), 15, 0
#define reg_a_l_change_dqs_default             (DDRPAPB_BASE + 0x2c0), 26, 26
#define reg_a_l_train_min_for_dqs              (DDRPAPB_BASE + 0x2c0), 25, 18
#define reg_a_l_train_min_for_dq0              (DDRPAPB_BASE + 0x2c0), 17, 9
#define reg_a_l_train_min_for_dq1              (DDRPAPB_BASE + 0x2c0), 8 , 0
#define reg_a_l_train_min_for_dq2              (DDRPAPB_BASE + 0x2c4), 26, 18
#define reg_a_l_train_min_for_dq3              (DDRPAPB_BASE + 0x2c4), 17, 9
#define reg_a_l_train_min_for_dq4              (DDRPAPB_BASE + 0x2c4), 8 , 0
#define reg_a_l_train_min_for_dq5              (DDRPAPB_BASE + 0x2c8), 26, 18
#define reg_a_l_train_min_for_dq6              (DDRPAPB_BASE + 0x2c8), 17, 9
#define reg_a_l_train_min_for_dq7              (DDRPAPB_BASE + 0x2c8), 8 , 0
#define reg_a_l_train_max_for_dqs              (DDRPAPB_BASE + 0x2cc), 25, 18
#define reg_a_l_train_max_for_dq0              (DDRPAPB_BASE + 0x2cc), 17, 9
#define reg_a_l_train_max_for_dq1              (DDRPAPB_BASE + 0x2cc), 8 , 0
#define reg_a_l_train_max_for_dq2              (DDRPAPB_BASE + 0x2e0), 26, 18
#define reg_a_l_train_max_for_dq3              (DDRPAPB_BASE + 0x2e0), 17, 9
#define reg_a_l_train_max_for_dq4              (DDRPAPB_BASE + 0x2e0), 8 , 0
#define reg_a_l_train_max_for_dq5              (DDRPAPB_BASE + 0x2e4), 26, 18
#define reg_a_l_train_max_for_dq6              (DDRPAPB_BASE + 0x2e4), 17, 9
#define reg_a_l_train_max_for_dq7              (DDRPAPB_BASE + 0x2e4), 8 , 0
#define reg_a_l_rdtrain_vref_max               (DDRPAPB_BASE + 0x2e8), 24, 16
#define reg_a_l_rdtrain_vref_min               (DDRPAPB_BASE + 0x2e8), 8 , 0
#define reg_a_l_train_min_for_dm               (DDRPAPB_BASE + 0x2ec), 31, 23
#define reg_a_l_train_max_for_dm               (DDRPAPB_BASE + 0x2ec), 22, 14
#define reg_a_l_train_min_for_rd_dm            (DDRPAPB_BASE + 0x2ec), 13, 7
#define reg_a_l_train_max_for_rd_dm            (DDRPAPB_BASE + 0x2ec), 6 , 0


// RW REGISTER LIST

#define reg_a_h_vref1_margsel_reg              (DDRPAPB_BASE + 0x300), 31, 23
#define reg_a_h_dq_odt_zqcali_en               (DDRPAPB_BASE + 0x300), 22, 22
#define reg_a_h_dq_drv_zqcali_en               (DDRPAPB_BASE + 0x300), 21, 21
#define reg_a_h_abutweakpddq_reg               (DDRPAPB_BASE + 0x300), 20, 20
#define reg_a_h_abutweakpubdq_reg              (DDRPAPB_BASE + 0x300), 19, 19
#define reg_a_h_abutdiffampseen_reg            (DDRPAPB_BASE + 0x300), 18, 18
#define reg_a_h_vref1_pd_reg                   (DDRPAPB_BASE + 0x300), 17, 17
#define reg_a_h_weakpd_reg                     (DDRPAPB_BASE + 0x300), 16, 15
#define reg_a_h_weakpub_reg                    (DDRPAPB_BASE + 0x300), 14, 13
#define reg_a_h_abutslewpu_reg                 (DDRPAPB_BASE + 0x300), 12, 8
#define reg_a_h_enb_lp4mode_reg                (DDRPAPB_BASE + 0x300), 7 , 7
#define reg_a_h_dqsweakpd_reg                  (DDRPAPB_BASE + 0x300), 6 , 6
#define reg_a_h_dqsbweakpub_reg                (DDRPAPB_BASE + 0x300), 5 , 5
#define reg_a_h_abutslewpd_reg                 (DDRPAPB_BASE + 0x300), 4 , 0
#define reg_a_h_dqfben_reg                     (DDRPAPB_BASE + 0x304), 30, 30
#define reg_a_h_dqfbsel_reg                    (DDRPAPB_BASE + 0x304), 29, 29
#define reg_a_h_abutnrcompdq_reg               (DDRPAPB_BASE + 0x304), 28, 24    // The driver pull-down resistance of byte0.Refer to CMD IO Drive Strengt hin the Databook.
#define reg_a_h_abutprcompdq_reg               (DDRPAPB_BASE + 0x304), 20, 16    // The driver pull-up resistance of byte0.Refer to CMD IO Drive Strengt hin the Databook.
#define reg_a_h_abutodtpddq_reg                (DDRPAPB_BASE + 0x304), 12, 8     // The ODT pull-down resistance of byte0.Refer to CMD IO Drive Strengt hin the Databook.
#define reg_a_h_abutodtpudq_reg                (DDRPAPB_BASE + 0x304), 4 , 0     // The ODT pull-up resistance of byte0.Refer to CMD IO Drive Strengt hin the Databook.
#define reg_a_h_abutnrcompdq4_reg              (DDRPAPB_BASE + 0x304), 28, 28
#define reg_a_h_abutnrcompdq3_reg              (DDRPAPB_BASE + 0x304), 27, 27
#define reg_a_h_abutnrcompdq2_reg              (DDRPAPB_BASE + 0x304), 26, 26
#define reg_a_h_abutnrcompdq1_reg              (DDRPAPB_BASE + 0x304), 25, 25
#define reg_a_h_abutnrcompdq0_reg              (DDRPAPB_BASE + 0x304), 24, 24
#define reg_a_h_abutprcompdq4_reg              (DDRPAPB_BASE + 0x304), 20, 20
#define reg_a_h_abutprcompdq3_reg              (DDRPAPB_BASE + 0x304), 19, 19
#define reg_a_h_abutprcompdq2_reg              (DDRPAPB_BASE + 0x304), 18, 18
#define reg_a_h_abutprcompdq1_reg              (DDRPAPB_BASE + 0x304), 17, 17
#define reg_a_h_abutprcompdq0_reg              (DDRPAPB_BASE + 0x304), 16, 16
#define reg_a_h_abutodtpddq4_reg               (DDRPAPB_BASE + 0x304), 12, 12
#define reg_a_h_abutodtpddq3_reg               (DDRPAPB_BASE + 0x304), 11, 11
#define reg_a_h_abutodtpddq2_reg               (DDRPAPB_BASE + 0x304), 10, 10
#define reg_a_h_abutodtpddq1_reg               (DDRPAPB_BASE + 0x304), 9 , 9
#define reg_a_h_abutodtpddq0_reg               (DDRPAPB_BASE + 0x304), 8 , 8
#define reg_a_h_abutodtpudq4_reg               (DDRPAPB_BASE + 0x304), 4 , 4
#define reg_a_h_abutodtpudq3_reg               (DDRPAPB_BASE + 0x304), 3 , 3
#define reg_a_h_abutodtpudq2_reg               (DDRPAPB_BASE + 0x304), 2 , 2
#define reg_a_h_abutodtpudq1_reg               (DDRPAPB_BASE + 0x304), 1 , 1
#define reg_a_h_abutodtpudq0_reg               (DDRPAPB_BASE + 0x304), 0 , 0
#define reg_a_h_rxmen0_delay_bp                (DDRPAPB_BASE + 0x308), 26, 24
#define reg_a_h_rxmen0_ophsel_bp               (DDRPAPB_BASE + 0x308), 23, 21
#define reg_a_h_rxmen0_sdlltap_bp              (DDRPAPB_BASE + 0x308), 20, 16
#define reg_a_h_rxmen1_delay_bp                (DDRPAPB_BASE + 0x308), 10, 8
#define reg_a_h_rxmen1_ophsel_bp               (DDRPAPB_BASE + 0x308), 7 , 5
#define reg_a_h_rxmen1_sdlltap_bp              (DDRPAPB_BASE + 0x308), 4 , 0
#define reg_a_h_rdodt0_delay                   (DDRPAPB_BASE + 0x30c), 26, 24
#define reg_a_h_rdodt0_ophsel                  (DDRPAPB_BASE + 0x30c), 23, 21
#define reg_a_h_rdodt0_dllsel                  (DDRPAPB_BASE + 0x30c), 20, 16
#define reg_a_h_rdodt1_delay                   (DDRPAPB_BASE + 0x30c), 10, 8
#define reg_a_h_rdodt1_ophsel                  (DDRPAPB_BASE + 0x30c), 7 , 5
#define reg_a_h_rdodt1_dllsel                  (DDRPAPB_BASE + 0x30c), 4 , 0
#define reg_a_h_dq_invdelay_lp_en              (DDRPAPB_BASE + 0x310), 29, 29
#define reg_a_h_dm_obsdataen                   (DDRPAPB_BASE + 0x310), 28, 28
#define reg_a_h_dqobsmuxsel                    (DDRPAPB_BASE + 0x310), 27, 24
#define reg_a_h_rxen_lp4                       (DDRPAPB_BASE + 0x310), 23, 23
#define reg_a_h_lp4x_en                        (DDRPAPB_BASE + 0x310), 22, 22
#define reg_a_h_pvt_comp_en                    (DDRPAPB_BASE + 0x310), 21, 21
#define reg_a_h_dqout_mux                      (DDRPAPB_BASE + 0x310), 20, 20
#define reg_a_h_dmout_mux                      (DDRPAPB_BASE + 0x310), 19, 19
#define reg_a_h_dq_ph90en_bp                   (DDRPAPB_BASE + 0x310), 18, 18
#define reg_a_h_dqs_ph90en_bp                  (DDRPAPB_BASE + 0x310), 17, 17
#define reg_a_h_rcvdqsmodsel                   (DDRPAPB_BASE + 0x310), 16, 16
#define reg_a_h_selfclren                      (DDRPAPB_BASE + 0x310), 15, 15
#define reg_a_h_wrptrclrb                      (DDRPAPB_BASE + 0x310), 14, 14
#define reg_a_h_rxm_odiffampen                 (DDRPAPB_BASE + 0x310), 13, 13
#define reg_a_h_abutobsmodeen                  (DDRPAPB_BASE + 0x310), 12, 12
#define reg_a_h_rxm4p5en                       (DDRPAPB_BASE + 0x310), 10, 10
#define reg_a_h_tsm_iobufact_bp                (DDRPAPB_BASE + 0x310), 9 , 9
#define reg_a_h_rxpst_bp                       (DDRPAPB_BASE + 0x310), 8 , 8
#define reg_a_h_rxm4p5en_r2                    (DDRPAPB_BASE + 0x310), 6 , 6
#define reg_a_h_rrankdly_4x_cs0                (DDRPAPB_BASE + 0x310), 5 , 3
#define reg_a_h_rrankdly_4x_cs1                (DDRPAPB_BASE + 0x310), 2 , 0
#define reg_a_h_rrankdly_1x_cs1                (DDRPAPB_BASE + 0x314), 18, 16
#define reg_a_h_rrankdly_1x_cs0                (DDRPAPB_BASE + 0x314), 14, 12
#define reg_a_h_wrankphsel                     (DDRPAPB_BASE + 0x314), 10, 8
#define reg_a_h_wrankdlysel                    (DDRPAPB_BASE + 0x314), 7 , 0
#define reg_a_h_cs0_dm_invdelaysel             (DDRPAPB_BASE + 0x318), 26, 18
#define reg_a_h_cs0_dq0_invdelaysel            (DDRPAPB_BASE + 0x318), 17, 9
#define reg_a_h_cs0_dq1_invdelaysel            (DDRPAPB_BASE + 0x318), 8 , 0
#define reg_a_h_cs0_dq2_invdelaysel            (DDRPAPB_BASE + 0x31c), 26, 18
#define reg_a_h_cs0_dq3_invdelaysel            (DDRPAPB_BASE + 0x31c), 17, 9
#define reg_a_h_cs0_dq4_invdelaysel            (DDRPAPB_BASE + 0x31c), 8 , 0
#define reg_a_h_cs0_dq5_invdelaysel            (DDRPAPB_BASE + 0x320), 26, 18
#define reg_a_h_cs0_dq6_invdelaysel            (DDRPAPB_BASE + 0x320), 17, 9
#define reg_a_h_cs0_dq7_invdelaysel            (DDRPAPB_BASE + 0x320), 8 , 0
#define reg_a_h_cs0_dqs_invdelaysel            (DDRPAPB_BASE + 0x324), 31, 24
#define reg_a_h_cs0_dqsb_invdelaysel           (DDRPAPB_BASE + 0x324), 15, 8
#define reg_a_h_cs0_loop_invdelaysel           (DDRPAPB_BASE + 0x328), 28, 24
#if 0
#define reg_a_h_cs0_dm_invdelayselrx           (DDRPAPB_BASE + 0x328), 14, 8
#define reg_a_h_cs0_dq0_invdelayselrx          (DDRPAPB_BASE + 0x32c), 30, 24
#define reg_a_h_cs0_dq1_invdelayselrx          (DDRPAPB_BASE + 0x32c), 22, 16
#define reg_a_h_cs0_dq2_invdelayselrx          (DDRPAPB_BASE + 0x32c), 14, 8
#define reg_a_h_cs0_dq3_invdelayselrx          (DDRPAPB_BASE + 0x32c), 6 , 0
#define reg_a_h_cs0_dq4_invdelayselrx          (DDRPAPB_BASE + 0x330), 30, 24
#define reg_a_h_cs0_dq5_invdelayselrx          (DDRPAPB_BASE + 0x330), 22, 16
#define reg_a_h_cs0_dq6_invdelayselrx          (DDRPAPB_BASE + 0x330), 14, 8
#define reg_a_h_cs0_dq7_invdelayselrx          (DDRPAPB_BASE + 0x330), 6 , 0
#define reg_a_h_cs0_dqs_invdelayselrx          (DDRPAPB_BASE + 0x334), 30, 24
#define reg_a_h_cs0_dqsb_invdelayselrx         (DDRPAPB_BASE + 0x334), 14, 8
#endif
#define reg_a_h_cs1_dm_invdelaysel             (DDRPAPB_BASE + 0x338), 26, 18
#define reg_a_h_cs1_dq0_invdelaysel            (DDRPAPB_BASE + 0x338), 17, 9
#define reg_a_h_cs1_dq1_invdelaysel            (DDRPAPB_BASE + 0x338), 8 , 0
#define reg_a_h_cs1_dq2_invdelaysel            (DDRPAPB_BASE + 0x33c), 26, 18
#define reg_a_h_cs1_dq3_invdelaysel            (DDRPAPB_BASE + 0x33c), 17, 9
#define reg_a_h_cs1_dq4_invdelaysel            (DDRPAPB_BASE + 0x33c), 8 , 0
#define reg_a_h_cs1_dq5_invdelaysel            (DDRPAPB_BASE + 0x340), 26, 18
#define reg_a_h_cs1_dq6_invdelaysel            (DDRPAPB_BASE + 0x340), 17, 9
#define reg_a_h_cs1_dq7_invdelaysel            (DDRPAPB_BASE + 0x340), 8 , 0
#define reg_a_h_cs1_dqs_invdelaysel            (DDRPAPB_BASE + 0x344), 31, 24
#define reg_a_h_cs1_dqsb_invdelaysel           (DDRPAPB_BASE + 0x344), 15, 8
#define reg_a_h_cs1_loop_invdelaysel           (DDRPAPB_BASE + 0x348), 28, 24
#define reg_a_h_cs1_dm_invdelayselrx           (DDRPAPB_BASE + 0x348), 14, 8
#define reg_a_h_cs1_dq0_invdelayselrx          (DDRPAPB_BASE + 0x34c), 30, 24
#define reg_a_h_cs1_dq1_invdelayselrx          (DDRPAPB_BASE + 0x34c), 22, 16
#define reg_a_h_cs1_dq2_invdelayselrx          (DDRPAPB_BASE + 0x34c), 14, 8
#define reg_a_h_cs1_dq3_invdelayselrx          (DDRPAPB_BASE + 0x34c), 6 , 0
#define reg_a_h_cs1_dq4_invdelayselrx          (DDRPAPB_BASE + 0x350), 30, 24
#define reg_a_h_cs1_dq5_invdelayselrx          (DDRPAPB_BASE + 0x350), 22, 16
#define reg_a_h_cs1_dq6_invdelayselrx          (DDRPAPB_BASE + 0x350), 14, 8
#define reg_a_h_cs1_dq7_invdelayselrx          (DDRPAPB_BASE + 0x350), 6 , 0
#define reg_a_h_cs1_dqs_invdelayselrx          (DDRPAPB_BASE + 0x354), 30, 24
#define reg_a_h_cs1_dqsb_invdelayselrx         (DDRPAPB_BASE + 0x354), 14, 8
#define reg_a_h_rd_train_dqs_default           (DDRPAPB_BASE + 0x358), 30, 24
#define reg_a_h_train_dqs_default              (DDRPAPB_BASE + 0x358), 23, 16
#define reg_a_h_rd_train_dqs_range_max         (DDRPAPB_BASE + 0x358), 14, 8
#define reg_a_h_rd_train_dqs_range_min         (DDRPAPB_BASE + 0x358), 6 , 0
#define reg_a_h_rdtrain_check_wrap0            (DDRPAPB_BASE + 0x35c), 31, 24
#define reg_a_h_rdtrain_check_wrap1            (DDRPAPB_BASE + 0x35c), 23, 16
#define reg_a_h_cat_wrap_sel                   (DDRPAPB_BASE + 0x35c), 15, 14
#define reg_a_h_dm_bit_wrap_sel                (DDRPAPB_BASE + 0x35c), 13, 10
#define reg_a_h_dq7_bit_wrap_sel               (DDRPAPB_BASE + 0x360), 31, 28
#define reg_a_h_dq6_bit_wrap_sel               (DDRPAPB_BASE + 0x360), 27, 24
#define reg_a_h_dq5_bit_wrap_sel               (DDRPAPB_BASE + 0x360), 23, 20
#define reg_a_h_dq4_bit_wrap_sel               (DDRPAPB_BASE + 0x360), 19, 16
#define reg_a_h_dq3_bit_wrap_sel               (DDRPAPB_BASE + 0x360), 15, 12
#define reg_a_h_dq2_bit_wrap_sel               (DDRPAPB_BASE + 0x360), 11, 8
#define reg_a_h_dq1_bit_wrap_sel               (DDRPAPB_BASE + 0x360), 7 , 4
#define reg_a_h_dq0_bit_wrap_sel               (DDRPAPB_BASE + 0x360), 3 , 0


// RO REGISTER LIST
#define reg_a_h_dqs_idqshigh                   (DDRPAPB_BASE + 0x380), 30, 30
#define reg_a_h_tdqs_invdelaysel0              (DDRPAPB_BASE + 0x380), 23, 16
#define reg_a_h_tdqs_invdelaysel1              (DDRPAPB_BASE + 0x380), 7 , 0
#define reg_a_h_cycsel                         (DDRPAPB_BASE + 0x384), 10, 8
#define reg_a_h_ophsel                         (DDRPAPB_BASE + 0x384), 7 , 5
#define reg_a_h_dllsel                         (DDRPAPB_BASE + 0x384), 4 , 0
#define reg_a_h_calib_result_cs0               (DDRPAPB_BASE + 0x388), 26, 16
#define reg_a_h_calib_result_cs1               (DDRPAPB_BASE + 0x388), 10, 0
#define reg_a_h_cs0_value_dqx_invdelaysel      (DDRPAPB_BASE + 0x38c), 8 , 0
#define reg_a_h_cs1_value_dqx_invdelaysel      (DDRPAPB_BASE + 0x390), 8 , 0
#define reg_a_h_train_min_for_rd_dq0           (DDRPAPB_BASE + 0x394), 30, 24
#define reg_a_h_train_min_for_rd_dq1           (DDRPAPB_BASE + 0x394), 22, 16
#define reg_a_h_train_min_for_rd_dq2           (DDRPAPB_BASE + 0x394), 14, 8
#define reg_a_h_train_min_for_rd_dq3           (DDRPAPB_BASE + 0x394), 6 , 0
#define reg_a_h_train_min_for_rd_dq4           (DDRPAPB_BASE + 0x398), 30, 24
#define reg_a_h_train_min_for_rd_dq5           (DDRPAPB_BASE + 0x398), 22, 16
#define reg_a_h_train_min_for_rd_dq6           (DDRPAPB_BASE + 0x398), 14, 8
#define reg_a_h_train_min_for_rd_dq7           (DDRPAPB_BASE + 0x398), 6 , 0
#define reg_a_h_train_min_for_rd_dqs           (DDRPAPB_BASE + 0x39c), 14, 8
#define reg_a_h_train_max_for_rd_dq0           (DDRPAPB_BASE + 0x3a0), 30, 24
#define reg_a_h_train_max_for_rd_dq1           (DDRPAPB_BASE + 0x3a0), 22, 16
#define reg_a_h_train_max_for_rd_dq2           (DDRPAPB_BASE + 0x3a0), 14, 8
#define reg_a_h_train_max_for_rd_dq3           (DDRPAPB_BASE + 0x3a0), 6 , 0
#define reg_a_h_train_max_for_rd_dq4           (DDRPAPB_BASE + 0x3a4), 30, 24
#define reg_a_h_train_max_for_rd_dq5           (DDRPAPB_BASE + 0x3a4), 22, 16
#define reg_a_h_train_max_for_rd_dq6           (DDRPAPB_BASE + 0x3a4), 14, 8
#define reg_a_h_train_max_for_rd_dq7           (DDRPAPB_BASE + 0x3a4), 6 , 0
#define reg_a_h_train_max_for_rd_dqs           (DDRPAPB_BASE + 0x3a8), 14, 8
#define reg_a_h_train_result_for_rd_base_dqs   (DDRPAPB_BASE + 0x3ac), 30, 24
#define reg_a_h_change_rd_dqs_default          (DDRPAPB_BASE + 0x3ac), 5 , 5
#define reg_a_h_left_boundary_overflow_for_rd  (DDRPAPB_BASE + 0x3ac), 3 , 3
#define reg_a_h_right_boundary_overflow_for_rd (DDRPAPB_BASE + 0x3ac), 2 , 2
#define reg_a_h_rd_train_readback_data_dq0     (DDRPAPB_BASE + 0x3b0), 31, 16
#define reg_a_h_rd_train_readback_data_dq1     (DDRPAPB_BASE + 0x3b0), 15, 0
#define reg_a_h_rd_train_readback_data_dq2     (DDRPAPB_BASE + 0x3b4), 31, 16
#define reg_a_h_rd_train_readback_data_dq3     (DDRPAPB_BASE + 0x3b4), 15, 0
#define reg_a_h_rd_train_readback_data_dq4     (DDRPAPB_BASE + 0x3b8), 31, 16
#define reg_a_h_rd_train_readback_data_dq5     (DDRPAPB_BASE + 0x3b8), 15, 0
#define reg_a_h_rd_train_readback_data_dq6     (DDRPAPB_BASE + 0x3bc), 31, 16
#define reg_a_h_rd_train_readback_data_dq7     (DDRPAPB_BASE + 0x3bc), 15, 0
#define reg_a_h_change_dqs_default             (DDRPAPB_BASE + 0x3c0), 26, 26
#define reg_a_h_train_min_for_dqs              (DDRPAPB_BASE + 0x3c0), 25, 18
#define reg_a_h_train_min_for_dq0              (DDRPAPB_BASE + 0x3c0), 17, 9
#define reg_a_h_train_min_for_dq1              (DDRPAPB_BASE + 0x3c0), 8 , 0
#define reg_a_h_train_min_for_dq2              (DDRPAPB_BASE + 0x3c4), 26, 18
#define reg_a_h_train_min_for_dq3              (DDRPAPB_BASE + 0x3c4), 17, 9
#define reg_a_h_train_min_for_dq4              (DDRPAPB_BASE + 0x3c4), 8 , 0
#define reg_a_h_train_min_for_dq5              (DDRPAPB_BASE + 0x3c8), 26, 18
#define reg_a_h_train_min_for_dq6              (DDRPAPB_BASE + 0x3c8), 17, 9
#define reg_a_h_train_min_for_dq7              (DDRPAPB_BASE + 0x3c8), 8 , 0
#define reg_a_h_train_max_for_dqs              (DDRPAPB_BASE + 0x3cc), 25, 18
#define reg_a_h_train_max_for_dq0              (DDRPAPB_BASE + 0x3cc), 17, 9
#define reg_a_h_train_max_for_dq1              (DDRPAPB_BASE + 0x3cc), 8 , 0
#define reg_a_h_train_max_for_dq2              (DDRPAPB_BASE + 0x3e0), 26, 18
#define reg_a_h_train_max_for_dq3              (DDRPAPB_BASE + 0x3e0), 17, 9
#define reg_a_h_train_max_for_dq4              (DDRPAPB_BASE + 0x3e0), 8 , 0
#define reg_a_h_train_max_for_dq5              (DDRPAPB_BASE + 0x3e4), 26, 18
#define reg_a_h_train_max_for_dq6              (DDRPAPB_BASE + 0x3e4), 17, 9
#define reg_a_h_train_max_for_dq7              (DDRPAPB_BASE + 0x3e4), 8 , 0
#define reg_a_h_rdtrain_vref_max               (DDRPAPB_BASE + 0x3e8), 24, 16
#define reg_a_h_rdtrain_vref_min               (DDRPAPB_BASE + 0x3e8), 8 , 0
#define reg_a_h_train_min_for_dm               (DDRPAPB_BASE + 0x3ec), 31, 23
#define reg_a_h_train_max_for_dm               (DDRPAPB_BASE + 0x3ec), 22, 14
#define reg_a_h_train_min_for_rd_dm            (DDRPAPB_BASE + 0x3ec), 13, 7
#define reg_a_h_train_max_for_rd_dm            (DDRPAPB_BASE + 0x3ec), 6 , 0

#define ddrc_writel(addr,value)	writel((value), addr)
#define ddrc_readl(addr) readl(addr)

static inline unsigned int READ_INNOPHY_REG2(unsigned int ADDR, unsigned int A, unsigned int B)
{
	unsigned int value = 0;
	value = (*(volatile unsigned int *)(ADDR)) >> B;
	value &= (1 << (A - B + 1)) - 1;
	return value;
}

#define READ_INNOPHY_REG(A)           READ_INNOPHY_REG2(A)

#define SET_INNOPHY_REG(A, B)           SET_INNOPHY_REG2(A,B)
#define GET_INNOPHY_REG2(ADDR,A,B)					\
	({												\
		int data = 0;								\
		data = *(volatile unsigned int *)(ADDR);	\
		dwc_debug("phy %x is %x\n",ADDR, data);				\
	 })

#define GET_INNOPHY_REG(A)           GET_INNOPHY_REG2(A)
#define BNE_INNOPHY_REG(A, B)       BNE_INNOPHY_REG2(A, B)
#define STR(x) #x

static inline void SET_INNOPHY_REG2(unsigned int ADDR, unsigned int A, unsigned int B, unsigned int DATA)
{
    unsigned int value = 0, off = 0;
    if (DATA < (2 << (A - B)))
    {
        value = *(volatile unsigned int *)(ADDR);
        off = (1 << (A - B + 1)) - 1;
        value &= ~(off << B);
        value |= DATA << B;
        *(volatile unsigned int *)(ADDR) = value;
    }
    else
    {
        printf("data is err!, addr is %x, data is %x\n", ADDR, DATA);
    }
}

static inline int BNE_INNOPHY_REG2(uint32_t addr, unsigned int A, unsigned int B, uint32_t data)
{
	volatile uint32_t *reg = (volatile uint32_t *)addr;
	uint32_t mask = 0xFFFFFFFF >> (31 - A);
	uint32_t value;

	do {
		value = *reg;
		value &= mask;
		value >>= B;
	} while (value != data);

	return value;
}

#endif // T33_DDR__H
