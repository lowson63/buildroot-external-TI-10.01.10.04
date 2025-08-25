/*
 * This file is part of calibrator
 *
 * Copyright (C) 2021 Texas Instruments
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
#ifndef __CC33XX_PLT_H__
#define __CC33XX_PLT_H__


#define CALIB_RX_IQMM_BITWISE_MASK              (1 << CALIBRATION_RX_IQMM_ID)
#define CALIB_RX_DC_BITWISE_MASK                (1 << CALIBRATION_RX_DC_CORRECTION_ID)
#define CALIB_RX_SPUR_CANCELER_BITWISE_MASK		(1 << CALIB_RX_SPUR_CANCELER_ID)
#define CALIB_TX_AUX_RX_DC_BITWISE_MASK         (1 << CALIBRATION_TX_AUX_RX_DC_ID)
#define CALIB_TX_IQMM_BITWISE_MASK              (1 << CALIBRATION_TX_IQMM_ID)
#define CALIB_TX_LOL_BITWISE_MASK               (1 << CALIBRATION_TX_LOL_ID)
#define CALIB_TX_RFNL_AND_DPD_BITWISE_MASK      (1 << CALIBRATION_TX_RFNL_AND_DPD_ID)

enum cc33xx_manual_calibIDs_e
{
	/*  0x0    */	CALIBRATION_RX_IQMM_ID   				= 0x0,
	/*  0x1    */	CALIBRATION_RX_DC_CORRECTION_ID   		= 0x1,
	/*  0x2    */	CALIBRATION_TX_AUX_RX_DC_ID   			= 0x2,
	/*  0x3    */	CALIBRATION_TX_LOL_ID   				= 0x3,
	/*  0x4    */	CALIBRATION_TX_IQMM_ID   				= 0x4,
	/*  0x5    */	CALIBRATION_TX_RFNL_AND_DPD_ID   		= 0x5,
	/*  0x6    */	CALIB_RX_SPUR_CANCELER_ID 				= 0x6,
};


enum cc33xx_test_cmds {
    /*  0x1    */	CC33XX_TEST_CMD_CHANNEL_TUNE = 0x1,
	/*  0x2    */	CC33XX_TEST_CMD_TX_START = 0x2,
	/*  0x3    */	CC33XX_TEST_CMD_TX_TONE_START = 0x3,
	/*  0x4    */   CC33XX_TEST_CMD_TX_STOP  =0x4,
	/*  0x5    */	CC33XX_TEST_CMD_START_RX_SIMULATION = 0x5,
	/*  0x6    */ 	CC33XX_TEST_CMD_RESET_RX_STATS = 0x06,
    /*  0x7    */   CC33XX_TEST_CMD_STOP_RX_STATS = 0x07,
    /*  0x8    */   CC33XX_TEST_CMD_GET_RX_STATS = 0x8,
    /*  0x9    */   CC33XX_TEST_CMD_SWITCH_ANT = 0x9,
    /*  0xA    */   CC33XX_TEST_CMD_GET_VERSION = 0xA,
    /*  0xB    */   CC33XX_TEST_CMD_GET_MAC_ADDRESS = 0xB,                
    /*  0xC    */   CC33XX_TEST_CMD_SET_PARAMS = 0xC,
    /*  0xD    */   CC33XX_TEST_CMD_GET_PARAMS = 0xD,
    /*  0xE    */   CC33XX_TEST_CMD_STATIC_CALIBRATION = 0xE,
    /*  0xF    */   CC33XX_TEST_CMD_GET_PHY_CPE_VERSION = 0xF,
    /*  0x10   */   CC3XX_TEST_CMD_MANUAL_CALIBRATION = 0x10,
    /*  0x11   */   CC3XX_TEST_CMD_GET_CALIBRATION_STATUS = 0x11,
    /*  0x12   */   CC3XX_TEST_CMD_GET_SYSTEM_INFO = 0x12,
	/* 	0x13   */   CC33XX_TEST_CMD_CHANGE_PHY_MODE	= 0x13,
	/* 	0x14   */   CC33XX_TEST_CMD_GET_LATEST_GPADC_MEASUREMENT = 0x14,
    /*  0x15   */   CC33XX_TEST_CMD_GET_SELECTED_ANT = 0x15,
    /*  0x16   */   CC33XX_TEST_CMD_BLE_ENABLE = 0x16,
    /*  0x17   */   CC33XX_TEST_CMD_SET_IO_CFG = 0x17,
    /*  0x18   */   CC33XX_TEST_CMD_GET_IO_CFG = 0x18
};


 enum cc33xx_start_tx_e {
    /*  0x0    */	PREAMBLE_TYPE = 0x0,
    /*  0x1    */	PHY_RATE = 0x1,
	/*  0x2    */	TX_POWER = 0x2,
	/*  0x3    */	GI_LTF_TYPE = 0x3,
	/*  0x4    */   DCM  =0x4,
	/*  0x5    */	LENGTH = 0x5,
	/*  0x6    */ 	DELAY = 0x06,
    /*  0x7    */   PACKET_MODE = 0x07,
    /*  0x8    */   NUMBER_OF_PACKETS = 0x8,
    /*  0x9    */   DATA_MODE = 0x9,
    /*  0xA    */   DATA_CONST_VALUE = 0xA,
    /*  0xB    */   ENABLE_CCA = 0xB,
    /*  0xC    */   BSS_COLOR = 0xC,
    /*  0xD    */   SU_ER_BANDWIDTH = 0xD,
    /*  0xE    */   PARTIAL_AID = 0xE,
    /*  0xF    */   SRC_ADDR = 0xF,
    /*  0x10   */   DST_ADDR = 0x10,
    /*  0x11   */   NOMINAL_PACKET_EXTENSION = 0x11,
    /*  0x12   */   FEEDBACK_STATUS = 0x12,
    /*  0x13   */   AID = 0x13,
	/*  0x14   */   GROUP_ID = 0x14,
    /*  0x15   */   LTF_MODE = 0x15,
    /*  0x16   */   HE_LTF_NUM = 0x16,
    /*  0x17   */   PRE_FEC_PADDING_FACTOR = 0x17,
    /*  0x18   */   COMMON_INFO_LEN = 0x18,
    /*  0x19   */   RU_ALLOC = 0x19,
    /*  0x1A   */   UL_BW = 0x1A,
    /*  0x1B   */   STARTS_STS_NUM = 0x1B,
	/*  0x1C   */   TB_AUTO_MODE = 0x1C,
	/*  0x1D   */   TB_DISAMB_MODE = 0x1D,

};

struct cc33xx_cmd_channel_tune {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;

	__u8	channel;
	__u8	band;
	__u8	bandwidth;
	__u8	padding[1];
} __attribute__((packed));


struct cc33xx_cmd_start_tx {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;

	__le32 	bitmask;
	__u8 	preambleType;
	__u8 	PhyRate;
	__s8 	TxPower;
	__u8 	GI_LTF_Type;
	__u8 	DCM;
	__le16 	start_length;//instead of uint16 length param we have now start_length and end_length
	__le16 	end_length;// end_lenght
	__le32 	delay;
	__u8 	packetMode;
	__le16 	numberOfPackets;
	__u8 	dataMode;
	__u8 	dataConstValue;
	__u8 	enableCCA;
	__u8 	BSSColor;
	__le16 	SU_ER_Bandwidth;
	__u8 	partialAID;
	__u8  	src_addr[MAC_ADDR_LEN];
	__u8  	dst_addr[MAC_ADDR_LEN];
	__u8 	nominalPacketExtension;
	__u8 	feedbackStatus;
	__u16 	aid;
	__u8     group_id;
	__u8 	ltf_mode;
	__u8 	he_ltf_num;
	__u8 	pre_fec_padding_factor;
	__u16 	common_info_len;
	__u16 	ru_alloc;
	__u8 	ul_bw;
	__u8 	starts_sts_num;
	__u8    tb_auto_mode;
	__u8	disamb;
	__u8    padding[1];//instead of padding of 3 now we have a padding of 1

}__attribute__((packed));

struct cc33xx_cmd_stop_tx {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;
	__u32 dummy;

}__attribute__((packed));


// struct cc33xx_cmd_start_rx {
// 	struct wl1271_cmd_header header;
// 	struct wl1271_cmd_test_header test;

// 	__u8   ACKenable;
// 	__u8   src_addr[MAC_ADDR_LEN];
// 	__u8   dst_addr[MAC_ADDR_LEN];
// } __attribute__((packed));


struct cc33xx_cmd_start_rx {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;

	__u8   ACKenable;
	__u8   mac_addr[MAC_ADDR_LEN];
	__u16  aid;
	__u8   preamble_type;
	__u8   rate;
    __u8   padding; 
} __attribute__((packed));

struct cc33xx_cmd_stop_rx {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;
	__u32 dummy;

} __attribute__((packed));


struct cc33xx_cmd_get_rx_stats {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;
	
	__le32  ReceivedTotalPacketsNumber;                           /* sum of the packets that been received OK (include filtered) */
    __le32  ReceivedFcsErrorPacketsNumber;                        /* sum of the packets that been dropped due to FCS error */
    __le32  ReceivedAddressMismatchPacketsNumber;                 /* sum of the packets that been received but filtered out by one of the HW filters */
    __le32  ReceivedGoodPackets;                                  /* sum of all the good packets received */
    __s16   AverageDataCtrlRssi;                                  /* average RSSI for all valid data packets received */
    __s16   AverageDataCtrlSNR; 

} __attribute__((packed));

struct cc33xx_cmd_switch_ant {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;
	__u8 ant;
	__u8 padding[3];
} __attribute__((packed));

struct cc33xx_cmd_set_io_cfg {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;
	__u16 io_id;
	__u8  direction;
	__u8  state;
} __attribute__((packed));

struct cc33xx_cmd_get_io_cfg {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;
	__u16 io_id;
	__u8  direction;
	__u8  value;
} __attribute__((packed));

struct cc33xx_cmd_start_tone_tx {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;
	__u8 mode;
	__s8 offset;
	__s8 output_power;
	__u8 padding[1];
} __attribute__((packed));

struct cc33xx_cmd_set_manual_calib{
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;
	__u16 calibration_bitmap;
	__u8 padding[2];
} __attribute__((packed));

struct cc33xx_cmd_change_mode {
	struct wl1271_cmd_header header;
	struct wl1271_cmd_test_header test;

	__u8	mode;
	__u8	mode_after_sleep;
	__u8	padding[2];
	__u32	duartion;
} __attribute__((packed));


#endif // __CC33XX_PLT_H__