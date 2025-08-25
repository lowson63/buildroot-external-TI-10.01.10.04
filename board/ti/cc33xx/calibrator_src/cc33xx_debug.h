/*
 * This file is part of calibrator
 *
 * Copyright (C) 2011 Texas Instruments
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
 * 02110-1301 USA
 *
 */

#ifndef __CC33xx_DEBUG_H__
#define __CC33xx_DEBUG_H__

enum traffic_parameters
{
    ACTIVE_MODE_RX_TH  = 0,
    ACTIVE_MODE_TX_TH  = 1,
    ACTIVE_MODE_RX_TO  = 2,
    ACTIVE_MODE_TX_TO  = 3,
    FORCE_POWER_MODE   = 4,

} ;

enum force_ps
{
    NO_FORCE          = 0,
    FORCE_ACTIVE      = 1,
    FORCE_POWER_SAVE  = 2,
} ;

enum CC33xx_debug_cmds {

//////// 11ax configuration /////////
    UPLINK_MULTI_USER_CFG        ,
    UPLINK_MULTI_USER_DATA_CFG   ,
    OPERATION_MODE_CTRL_CFG      ,
    UPLINK_POWER_HEADER_CFG      ,
    MCS_FIXED_RATE_CFG           ,
    GI_LTF_CFG                   ,
    TRANSMIT_OMI_CFG             ,
    TB_ONLY_CFG                  ,
    BA_SESSION_CFG               ,
///////////////////////////////////// 
    FORCE_PS_CFG                 ,
    RATE_OVERRRIDE_CFG           ,
    BLS_CFG                      ,
    BLE_ENABLE                   ,
	SET_TSF                      ,
	RTS_TH_CFG                   ,
	LINK_ADAPT_CFG               ,
    CALIB_BITMAP_CFG             ,
    PWR_PARTIAL_MODES_CFG        ,
    TRIGGER_FW_ASSERT            ,
    BURST_MODE_CFG               ,
    LAST_DEBUG_VALUE             ,
    MAX_DEBUG = 0xFFF            /*force enumeration to 16bits*/
};

enum CC33xx_debug_read
{
    BEACON_RSSI_INTR             ,

    LAST_DEBUG_INTR_VALUE        ,

    MAX_DEBUG_INTR = 0xFFFF      /*force enumeration to 16bits*/
};

enum link_adapt_debug
{
    FORCE_DISABLE_DCM          = 0,
    FORCE_DISABLE_ER           = 1,
    FORCE_DISABLE_ER_UPPER     = 2,
    FORCE_LONG_TERM_POLICY     = 3,
    FORCE_NOMINAL_PADDING      = 4,
    ENABLE_DEBUG_TRACE         = 5
} ;

struct CC33xx_cmd_rate_override { /* RATE_OVERRRIDE_CFG */
    struct debug_header header;

    __u8 overEnable;
    __u8 bw;
    __u8 preamble;
    __u8 rate;
    __u8 dcm;
    __u8 txPower;
    __u8 giLTF;
    __u8 padding;
} __attribute__((packed));

struct CC33xx_cmd_uplink_mu { /* UPLINK_MULTI_USER_CFG */
    struct debug_header header;

    __u8 enable_val;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_uplink_mu_data { /* UPLINK_MULTI_USER_DATA_CFG */
    struct debug_header header;

    __u8 enable_val;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_oper_mode_ctrl { /* OPERATION_MODE_CTRL_CFG */
    struct debug_header header;

    __u8 enable_val;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_uplink_pwr_hdr { /* UPLINK_POWER_HEADER_CFG */
    struct debug_header header;

    __u8 enable_val;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_mcs_rate{ /* MCS_FIXED_RATE_CFG */
    struct debug_header header;

    __u8 rate_val;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_gi_ltf { /* GI_LTF_CFG */
    struct debug_header header;

    __u8 value;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_transmit_omi { /* TRANSMIT_OMI_CFG */
    struct debug_header header;

    __u8 enable_val;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_tb_only { /* TB_ONLY_CFG */
    struct debug_header header;

    __u8 enable_val;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_ba_session { /* BA_SESSION_CFG */
    struct debug_header header;

    __u8 block_ack_rx;
    __u8 block_ack_tx;
    __u8 padding[2];
} __attribute__((packed));

struct CC33xx_cmd_beacon_rssi { /* BEACON_RSSI_INTR */
    struct debug_header header;

    __u8 beacon_rssi;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_force_ps { /* FORCE_PS_CFG */
    struct debug_header header;

    __u8 param_index;
    __u32 param_value;

    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_bls_cfg { /* BLS_CFG */
    struct debug_header header;

    __u8  bls_allowed;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_cmd_rts_th_cfg { /* RTS_TH_CFG */
    struct debug_header header;

    __u16 rts_th;
    __u8  roleid;
    __u8 padding;
} __attribute__((packed));

struct CC33xx_cmd_link_adapt_cfg { /* LINK_ADAPT_CFG */
    struct debug_header header;

    __u32 param1;
    __u32 param2;
    __u8  type;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_pwr_partial_modes_cfg { /* PWR_PARTIAL_MODES_CFG */
    struct debug_header header;

    __u8 partial_modes_allowed;
    __u8 padding[3];
} __attribute__((packed));

struct CC33xx_calib_bitmap_cfg { /* CALIB_BITMAP_CFG */
    struct debug_header header;

    __u16 calib_bitmap;
    __u8 padding[2];
} __attribute__((packed));


#endif /* __CC33xx_DEBUG_H__ */
