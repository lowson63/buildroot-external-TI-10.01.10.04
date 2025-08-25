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
#include "calibrator.h"
#include "plt.h"
#include "cc33xx_plt.h"
#include <ctype.h>


SECTION(cc33xx_plt);
SECTION(plt);
SECTION(get);
SECTION(set);

#define RX_PARAMS_INVALID_AID           (0xFFFF)
#define RX_PARAMS_INVALID_PREAMBLE_TYPE (0xFF)
#define RX_PARAMS_INVALID_RATE          (0xFF)

/* PHY power modes */
enum
{
    PHY_MODE_NORMAL_E,
	PHY_MODE_FIRST_E 	= PHY_MODE_NORMAL_E,
    PHY_MODE_RX_ONLY_E,
    PHY_MODE_LISTEN_E,
	NUM_OF_PHY_ACTIVE_MODES_E,
	PHY_MODE_RF_OFF_E 	= NUM_OF_PHY_ACTIVE_MODES_E,
    PHY_MODE_LPDS_E,
	PHY_MODE_PREPARED_TO_SLEEP_E,
    NUM_OF_PHY_MODES_E
};


/* util function for checking if an arguments contains only digits */
/* return 0 if true, 1 otherwise */
static bool check_str_all_digits(char *str)
{
	bool all_digits = true;
	while (*str != '\0') {
		if (!(isdigit(*str))) {
			all_digits = false;
			break;
		}
		str++;
	}
	return all_digits;
} 

/* tune_channel */
static int plt_cc33xx_tune_channel(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	
	struct nlattr *key;
	struct cc33xx_cmd_channel_tune prms;

	if (argc != 3)
		return 1;

	prms.test.id	= CC33XX_TEST_CMD_CHANNEL_TUNE;
	prms.channel	= (__u8)atoi(argv[0]);
	prms.band       = (__u8)atoi(argv[1]);
	prms.bandwidth	= (__u8)atoi(argv[2]);
    
	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, tune_channel, "<channel> <band> <bandwidth>",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_tune_channel,
	"\n\r\tSet channel, band and bandwith for PLT.\n");


static int plt_cc33xx_set_tx_params(struct nl80211_state *state, struct nl_cb *cb,
			       struct nl_msg *msg, int argc, char **argv)
{
	//set tx params
	//CC33XX_TEST_CMD_SET_PARAMS

	struct nlattr *key;
	struct cc33xx_cmd_start_tx prms;
	prms.test.id = CC33XX_TEST_CMD_SET_PARAMS;
	int num_params = argc;
	num_params/=2;
	fprintf(stderr, "set_tx_params has been called with: %d Parameters \n",num_params);
	int counter = 0;
	prms.bitmask=0;
	// if argc isnt divisible by 2 then no no
	if((argc%2!=0) || (argc==0))
	{
		fprintf(stderr, "Error: Invalid Syntax/Amount of Parameters \n");//missing stuff
		fprintf(stderr, "Hint: if you wrote -default then re do it with -default 0 \n");//missing stuff
		return 1;
	}

	while(counter<argc){


		if(!strcmp(argv[counter], "-default"))
		{

			/* default */
			printf("values were all reset to default - any previous value changs  were reset\n");
			prms.bitmask = 0;
			counter=argc;
			break;
		
		}

		if(!strcmp(argv[counter], "-preamble_type"))
		{
	
			/* preambleType */

			//11n GF and 11ax MU are currently not supported (preamble types 4,6)
			int preamble_type = atoi(argv[counter+1]);
			
			if((preamble_type > 10) || (preamble_type < 0))
			{
					fprintf(stderr, "\npreambleType is out of range (valid range: 0-10)\n");
					return 0;
			}

			if((preamble_type == 6) || (preamble_type == 4))
			{
					fprintf(stderr, "\n11n GF and 11ax MU are currently not supported (preamble types 4,6) \n");
					return 0;	
			}

			prms.preambleType = (__u8)preamble_type;
			prms.bitmask = prms.bitmask | (1 << PREAMBLE_TYPE);
		}

		
		else if(!strcmp(argv[counter], "-phy_rate"))
		{
			/* PhyRate */
			if(atoi(argv[counter+1]) > 20 || (atoi(argv[counter+1]) < 1))
			{
				fprintf(stderr, "\nPhyRate is out of range (valid range: 0-20)\n");
				return 0;
			}
			prms.PhyRate = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << PHY_RATE );
		}


		else if(!strcmp(argv[counter], "-tx_power"))
		{
			/* TxPower */
			if(atoi(argv[counter+1]) > 31 || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nTxPower is out of range (valid range: 0-31)\n");
				return 0;
			}
			prms.TxPower = atoi(argv[counter+1]); 
			prms.bitmask = prms.bitmask | (1 << TX_POWER);
		}


		else if(!strcmp(argv[counter], "-gi_ltf_type"))
		{
			/* GI_LTF_Type */
			if(atoi(argv[counter+1]) > 5 || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nGI_LTF_Type is out of range (valid range: 0-5)\n");
				return 0;
			}
			prms.GI_LTF_Type = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << GI_LTF_TYPE );
		}


		else if(!strcmp(argv[counter], "-dcm"))
		{
			/* DCM */
			if(atoi(argv[counter+1]) > 5 || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nDCM is out of range (valid range: 0-1)\n");
				return 0;
			}
			prms.DCM  = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << DCM );
		}
		

		else if(!strcmp(argv[counter], "-length"))
		{
			/* length <range> <start length> <end_length> 
								OR
					 length <const> <length> */
			if(!strcmp(argv[counter+1], "const"))
			{
				if(strcmp(argv[counter+2], "packet"))
				{
						fprintf(stderr, "\ncorrect length syntax is: -length const packet <length size>\n");
						return 0;
				}
					if(atoi(argv[counter+3]) > 16000 || (atoi(argv[counter+3]) < 0))
					{
						fprintf(stderr, "\nconst length is out of range:\n"
						"(valid range: Non MCS rate: 0-3500)\n(valid range: for MCS rate :0-16000)\n");
						return 0;
					}
					if( atoi(argv[counter+3]) > 3500)
					{
						fprintf(stderr, "\n Warning: You set length over 3500, make sure you are using aggregation (MCS rates)\n\n");
					}
					if(atoi(argv[counter+3]) < 4)
					{
						fprintf(stderr, "\nconst length is less than 4, malformed packet will be transmitted\n\n");
					}
					
					prms.start_length  = (__le16)atoi(argv[counter+3]);
					prms.end_length  = (__le16)atoi(argv[counter+3]);
			}
			else if (!strcmp(argv[counter+1], "range"))
			{
					if(atoi(argv[counter+2]) > 3500 || (atoi(argv[counter+2]) < 100))
					{
						fprintf(stderr, "\nstart length is out of range (valid range: 100-3500)\n");
						return 0;
					}
					if(atoi(argv[counter+3]) < atoi(argv[counter+2]))
					{
						fprintf(stderr, "\nend length cant be less than start length\n");
						return 0;
					}
					if(atoi(argv[counter+3])> 3500)
					{
						fprintf(stderr, "\nend length cant be more  than 3500\n");
						return 0;
					}

					prms.start_length  = (__le16)atoi(argv[counter+2]);
					prms.end_length  = (__le16)atoi(argv[counter+3]);
			}
			else
			{
				return 1; //ERROR HERE BASHAR
			}

			
			
			//counter below wil be incremented by 2 but since this command takes 4 params we need to increament by 4
			// incrementing 2 now and 4 will be later.
					counter+=2;
			prms.bitmask = prms.bitmask | (1 << LENGTH);
		
		}

		else if(!strcmp(argv[counter], "-delay"))
		{

			/* delay */
			if((atoi(argv[counter+1]) > 1000000) || ((atoi(argv[counter+1]) < 50))  )
			{
				fprintf(stderr, "\ndelay is out of range (valid range: 50-1000000\n");
				return 0;
			}
			prms.delay  = (__le32)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << DELAY  );
		
		}

		
		else if(!strcmp(argv[counter], "-pkt_mode"))
		{

			/* packetMode */
			if(atoi(argv[counter+1]) > 2 || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\npacketMode is out of range (valid range: 0-2)\n");
				return 0;
			}
			prms.packetMode  = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << PACKET_MODE   );
		
		}

		
		else if(!strcmp(argv[counter], "-num_pkts"))
		{

			/* numberOfPackets */
			if((atoi(argv[counter+1]) > 10000) || (atoi(argv[counter+1]) < 1))
			{
				fprintf(stderr, "\nnumberOfPackets is out of range (valid range: 1-10000)\n");
				return 0;
			}
			prms.numberOfPackets = (__le16)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << NUMBER_OF_PACKETS );
		}



		else if(!strcmp(argv[counter], "-data_mode"))
		{

			/* dataMode */
			if((atoi(argv[counter+1]) > 2) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\ndataMode is out of range (valid range: 0-2)\n");
				return 0;
			}
			prms.dataMode = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << DATA_MODE  );
		
		}

		

		else if(!strcmp(argv[counter], "-data_const_val"))
		{

			/* dataConstValue */
			if((atoi(argv[counter+1]) > 255) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\ndataConstValue is out of range (valid range 0-255 (ASCI VALUE))\n");
				return 0;
			}
			prms.dataConstValue = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << DATA_CONST_VALUE  );
		
		}

		
		else if(!strcmp(argv[counter], "-cca"))
		{

			/* enableCCA */
			if((atoi(argv[counter+1]) > 1) || (atoi(argv[counter+1]) < 0))	
			{
				fprintf(stderr, "\nenableCCA is out of range (valid range 0-1)\n");
				return 0;
			}
			prms.enableCCA = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << ENABLE_CCA);
		
		}


		else if(!strcmp(argv[counter], "-bss_color"))
		{

			/* BSSColor */
			if((atoi(argv[counter+1]) > 63) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nBSSColor is out of range (valid range 0-63)\n");
				return 0;
			}
			prms.BSSColor  = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << BSS_COLOR );
		
		}

		
		else if(!strcmp(argv[counter], "-su_er_bw"))
		{

			/* SU_ER_Bandwidth */
			if((atoi(argv[counter+1]) > 1) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nSU_ER_Bandwidth is out of range (valid range: 0-1)\n");
				return 0;
			}
			prms.SU_ER_Bandwidth = (__le16)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << SU_ER_BANDWIDTH  );
		
		}


		else if(!strcmp(argv[counter], "-partial_aid"))
		{

			/* partialAID */
			if((atoi(argv[counter+1]) > 1023) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\npartialAID is out of range (valid range: 0-1023 (9 bits))\n");
				return 0;
			}
			prms.partialAID = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << PARTIAL_AID  );
		
		}



		else if(!strcmp(argv[counter], "-src_addr"))
		{

				/* src_addr */
			str2mac(prms.src_addr, argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << SRC_ADDR   );
		
		}


		else if(!strcmp(argv[counter], "-dst_addr"))
		{

			/* dst_addr */
			str2mac(prms.dst_addr, argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << DST_ADDR);
		
		}

		else if(!strcmp(argv[counter], "-nominal_pkt_ext"))
		{

			/* nominalPacketExtension */
			if((atoi(argv[counter+1]) > 2) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nnominalPacketExtension is out of range (valid range: 0-2)\n");
				return 0;
			}
			prms.nominalPacketExtension = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << NOMINAL_PACKET_EXTENSION );
		
		}


		else if(!strcmp(argv[counter], "-feed_status"))
		{

		
			/* feedbackStatus */
			if((atoi(argv[counter+1]) > 1 ) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nfeedbackStatus is out of range (valid range 0-1)\n");
				return 0;
			}
			prms.feedbackStatus = (__u8)atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << FEEDBACK_STATUS  );
		
		}
		

		
		else if(!strcmp(argv[counter], "-aid"))
		{

			/* aid */
			if((atoi(argv[counter+1]) > 16383 ) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\naid is out of range (valid range: 0-16384)\n");
				return 0;
			}
			prms.aid =(__u16) atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << AID   );
		
		}


		else if(!strcmp(argv[counter], "-group_id"))
		{

			/* group_id */
			/*if((atoi(argv[counter+1]) > 16383 ) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "group_id is out of range (valid range: 0-16384)\n");
				return 1;
			}*/
			prms.group_id =(__u16) atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << GROUP_ID );
		
		}


		else
		{
				fprintf(stderr, "\nError, Wrong Syntax, %s is not a valid parameter \n",argv[counter]);
				return 1;
		}

		counter+=2;
	}
	

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}
	printf("Calibrator:: Setting TX Parameters\n");

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);

	return 0;
nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;

}

COMMAND(cc33xx_plt, set_tx, " -default 0 -preamble_type -phy_rate -tx_power -gi_ltf_type -dcm -length\n\r\t\t "
	"-delay -pkt_mode -num_pkts -data_mode -data_const_val -cca -bss_color \n\r\t\t "
	"-su_er_bw -partial_aid -src_addr -dst_addr -nominal_pkt_ext -feed_status -aid -group_id\n\r\t\t",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_set_tx_params,
	"Set TX transmissions for PLT - cc33xx.\n\r"
	"\tfor Default values write: -default 0\n\r"
	"\tset_tx -preamble_type\tvalid range: 0-10 \n\r"
	"\t<Preamble Types>\n\r"
	"\t\t0  - 11b short preamble\n\r"
	"\t\t1  - 11b long preamble\n\r"
	"\t\t2  - 11a/g (legacy OFDM)\n\r"
	"\t\t3  - 11n mixed mode\n\r"
	"\t\t4  - 11n GF (Not Supported)\n\r"
	"\t\t5  - 11ax SU\n\r"
	"\t\t6  - 11ax MU (Not Supported)\n\r"
	"\t\t7  - 11ax SU ER\n\r"
	"\t\t8  - 11ax TB (non NDP FB)\n\r"
	"\t\t9  - 11ax TB NDP FB\n\r"
	"\t\t10 - 11ac VHT\n\r"

	"\t\nset_tx -phy_rate\n\r"
	"\t<Phy Rate>\tvalid range: 1-20 \n\r"
	"\t\t1  -  RATE 1MBPS  \n\r"  
    "\t\t2  -  RATE 2MBPS  \n\r"  
    "\t\t3  -  RATE 5_5MBPS\n\r"  
    "\t\t4  -  RATE 11MBPS \n\r"  
    "\t\t5  -  RATE 6MBPS  \n\r"  
    "\t\t6  -  RATE 9MBPS  \n\r"  
    "\t\t7  -  RATE 12MBPS \n\r"  
    "\t\t8  -  RATE 18MBPS \n\r"  
    "\t\t9  -  RATE 24MBPS \n\r"  
    "\t\t10 -  RATE 36MBPS        \n\r"
    "\t\t11 -  RATE 48MBPS        \n\r"
    "\t\t12 -  RATE 54MBPS        \n\r"
    "\t\t13 -  RATE 6.5MBPS (MCS0)\n\r"   
    "\t\t14 -  RATE 13 MBPS (MCS1)\n\r"  
    "\t\t15 -  RATE 19.5 MBPS (MCS2)\n\r"
    "\t\t16 -  RATE 26MBPS (MCS3)\n\r"  
    "\t\t17 -  RATE 39MBPS (MCS4)\n\r"  
    "\t\t18 -  RATE 52MBPS (MCS5)\n\r"  
    "\t\t19 -  RATE 58.5MBPS (MCS6)\n\r"
    "\t\t20 -  RATE 65MBPS (MCS7)\n\r"   

	"\t\nset_tx -tx_power\n\r"
	"\t<Tx Power>\tvalid range: 0-31 \n\r \t\tPower level: -10dBm to 21dBm with 1dB resolution.\n\r"
	"\t\t0: -10dBm\n\r"
	"\t\t1: -9dBm\n\r"
	"\t\t2: -8dBm\n\r"
	"\t\t...\n\r"
	"\t\t31: +21dBm\n\r"
 
	"\t\nset_tx -gi_ltf_type\n\r"
	"\t<GI LTF Type>\tvalid range: 0-5 \n\r"
	"\t\t0 - 1xLTF+1.6 us GI\n\r"
	"\t\t1 - 2xLTF+1.6 us GI\n\r"
	"\t\t2 - 4xLTF+3.2 us GI\n\r"
	"\t\t3 - 2xLTF+0.8 us GI\n\r"
	"\t\t4 - 1xLTF+0.8 us GI\n\r"
	"\t\t5 - 4xLTF+0.8 us GI\n\r"

	"\t\nset_tx -dcm\n\r"
	"\t<DCM>\tvalid range: 0-1\n\r"
	"\t\tBoolean indicating if Dual carrier modulation is used\n\r"
	"\t\tNote: DCM is only applied to MCS0, MCS1, MCS3 and MCS4\n\r"
	"\t\t0 - Dual carrier modulation isn't used\n\r"
	"\t\t1 - Dual carrier modulation is used\n\r"

	"\t\nset_tx -length range <start length> <end_length> \n\r"
	"\t<Packet length>\tvalid range: 100-3500 \n\r"

	"\t\nSet_tx -length const packet <length> \n\r"
	"\t\tNon MCS rate: Number of data bytes (except mac 80211 header) range: 0-3500\n\r"
	"\t\tMCS rate: Number of data bytes (except mac 80211 header) range: 0-16000\n\r"

	"\t\nset_tx -delay\n\r"
	"\t<Delay>\tvalid range: 50-1000000 \n\r\t\tDelay between packets [μs] – range: 50 – 1000000\n\r"

	"\t\nset_tx -pkt_mode \n\r"
	"\t<Packet mode>\tvalid range: 0-2 \n\r"
	"\t\t0 - Continuous mode (Will send packets till \"tx_stop\" will be performed\n\r"
	"\t\t1 - Single packet (Will send only single packet)\n\r"
	"\t\t2 - Multi packets (Need to update next variable: \"<Number of packets>\" with number of packets. Range: 1-10000)\n\r"

	"\t\nset_tx -num_pkts \n\r"
	"\t<Number of packets>\tvalid range: 1-10000 (In case where “Multi” mode is chosen – range: 1-10000)\n\r"

	"\t\nset_tx -data_mode\n\r"
	"\t<Data mode>\tvalid range: 0-2 \n\r"
	"\t\t0 - constant value\n\r"
	"\t\t1 - increment\n\r"
	"\t\t2 - random value\n\r"

	"\t\nset_tx -data_const_val\n\r"
	"\t<Data const value>\tvalid range: 0-255 (In case that “Data mode” == 0, each data byte will carry this value (from 0-255))\n\r"

 
	"\t\nset_tx -cca\n\r"
	"\t<Enable CCA>\tvalid range: 0-1\n\r"
	"\t\t0 - Disable CCA\n\r"
	"\t\t1 - Enable CCA\n\r"

	"\t\nset_tx -bss_color\n\r"
	"\t<BSS Color>\tvalid range: 0-63 (This field is relevant for HE_SU and HE_SU_ER, Values: 0-63)\n\r"

	"\t\nset_tx -su_er_bw\n\r"
	"\t<SU_ER_Bandwidth>\tvalid range: 0-1 (0 for 242-tone RU, 1 for upper frequency 106-tone RU within the primary 20MHz)\n\r"

	"\t\nset_tx -partial_aid\n\r"
	"\t<Partial AID>\t(This field is relevant for VHT transmission)\n\r"

	"\t\nset_tx -src_addr\n\r"
	"\t<source MAC>\tsource MAC address (XX:XX:XX:XX:XX:XX)\n\r"

	"\t\nset_tx -dst_addr\n\r"
	"\t<dest MAC>\tdestination MAC address (XX:XX:XX:XX:XX:XX)\n\r"

	"\t\nset_tx -nominal_pkt_ext\n\r"
	"\t<Nominal packet extension>\tvalid range: 0-2(This field is relevant for 11ax transmissions only)\n\r"
	"\t\t0 - 0 usec (1)\n\r"
	"\t\t1 - 8 usec\n\r"
	"\t\t2 - 16 usec\n\r"

	"\t\nset_tx -feed_status\n\r"
	"\t<Feedback status>\tvalid range: 0-1 "
	"(Indicates the value of the one bit used to modulate the tones in each tone set.This field is relevant for HE TB NDP FB transmissions only)\n\r"

	"\t\nset_tx -aid\n\r"
	"\t<AID>\tvalid range: 0-16383. AID of station can be up to 16383 (14 bits)\n\r");



static int plt_cc33xx_set_tx_tb_params(struct nl80211_state *state, struct nl_cb *cb,
			       struct nl_msg *msg, int argc, char **argv)
{
	// set tx_tb params
	//CC33XX_TEST_CMD_SET_PARAMS
	struct nlattr *key;
	struct cc33xx_cmd_start_tx prms;
	prms.test.id = CC33XX_TEST_CMD_SET_PARAMS;
	int counter =0;
	prms.bitmask=0;
	while(counter<argc)
	{
		
		if(!strcmp(argv[counter], "-he_ltf_num"))
		{

			/* he_ltf_num */
			if((atoi(argv[counter+1]) > 4 ) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nhe_ltf_num is out of range (valid range: 0-4)\n");
				return 1;
			}
			prms.he_ltf_num =(__u8 ) atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << HE_LTF_NUM  );
		
		}

		else if(!strcmp(argv[counter], "-ru_alloc"))
		{

			/* ru_alloc */
			if((atoi(argv[counter+1]) > 61 ) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nru_alloc is out of range (valid range: 0-61)\n");
				return 1;
			}
			prms.ru_alloc =(__u16 ) atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << RU_ALLOC );
		
		}


		else if(!strcmp(argv[counter], "-ul_bw"))
		{

			/* ul_bw */
			if((atoi(argv[counter+1]) > 2 ) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "\nul_bw is out of range (valid range: 0-2)\n");
				return 1;
			}
			prms.ul_bw =(__u8 ) atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << UL_BW  );
		
		}

		else if(!strcmp(argv[counter], "-tb_auto"))
		{

			/* tb_auto_mode  
			if((atoi(argv[counter+1]) > 3 ) || (atoi(argv[counter+1]) < 0))//check range 
			{
				fprintf(stderr, "tb_auto_mode  is out of range (valid range: 0-3)\n");
				return 1;
			}*/
			prms.tb_auto_mode =(__u8 ) atoi(argv[counter+1]);
			prms.bitmask = prms.bitmask | (1 << TB_AUTO_MODE  );
		
		}


		else
		{
				fprintf(stderr, "\nError, Wrong Syntax, %s is not a valid parameter \n",argv[counter]);
				return 1;
		}		

		counter+=2;
	}
	fprintf(stderr, "set_tx_tb_params has been called with: %d Parameters \n",argc); 

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}
	printf("Calibrator:: Setting TB_TX Parameters\n");

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);

	return 0;
nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, set_tb_tx,  "-he_ltf_num -ru_alloc -ul_bw\n\r\t\t "
		"-tb_auto \n\r\t\t ",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_set_tx_tb_params,
	"Set TB_TX parameters.\n"); //maybe add explanation to modes here??

static int plt_cc33xx_start_tx(struct nl80211_state *state, struct nl_cb *cb,
			       struct nl_msg *msg, int argc, char **argv) //here we shouldent have any params ..
{
	struct nlattr *key;
	struct cc33xx_cmd_start_tx prms;
	prms.test.id = CC33XX_TEST_CMD_TX_START;

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	printf("Calibrator:: Starting TX Simulation\n");

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}
	

COMMAND(cc33xx_plt, start_tx, "Start TX" ,
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_start_tx,
	"\n"
);

static int plt_cc33xx_display_tx_params(struct nl_msg *msg, void *arg)
{
	struct nlattr *tb[NL80211_ATTR_MAX + 1];
	struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
	struct nlattr *td[WL1271_TM_ATTR_MAX + 1];
	struct cc33xx_cmd_start_tx *prms;

	nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
		genlmsg_attrlen(gnlh, 0), NULL);

	if (!tb[NL80211_ATTR_TESTDATA]) {
		fprintf(stderr, "no data!\n");
		return NL_SKIP;
	}

	nla_parse(td, WL1271_TM_ATTR_MAX, nla_data(tb[NL80211_ATTR_TESTDATA]),
		  nla_len(tb[NL80211_ATTR_TESTDATA]), NULL);

	prms = (struct cc33xx_cmd_start_tx *) nla_data(td[WL1271_TM_ATTR_DATA]);

	printf("\nParameter\t\t\tValue\n\n");
	printf("Preamble Type:\t\t\t%d\n", prms->preambleType);
	printf("PhyRate:\t\t\t%d\n", prms->PhyRate);
	printf("Tx Power:\t\t\t%d\n", prms->TxPower);
	printf("GI LTF TYPE:\t\t\t%d\n", prms->GI_LTF_Type);
	printf("DCM:\t\t\t\t%d\n", prms->DCM);
	printf("start Length:\t\t\t%hi\n", prms->start_length);
	printf("end Length:\t\t\t%hi\n", prms->start_length);
	printf("Delay:\t\t\t\t%hi\n", prms->delay);
	printf("Packet Mode:\t\t\t%hi\n", prms->packetMode);
	printf("Number of Packets:\t\t%hi\n", prms->numberOfPackets);
	printf("dataMode:\t\t\t%hi\n", prms->dataMode);
	printf("data Const Value:\t\t%hi\n", prms->dataConstValue);
	printf("Enable CCA:\t\t\t%hi\n", prms->enableCCA);
	printf("BSS Color:\t\t\t%hi\n", prms->BSSColor);
	printf("SU ER Bandwidth:\t\t%hi\n", prms->SU_ER_Bandwidth);
	printf("SRC Addr:  \t\t\t%02x:%02x:%02x:%02x:%02x:%02x\n", prms->src_addr[0],prms->src_addr[1],prms->src_addr[2],prms->src_addr[3],prms->src_addr[4],prms->src_addr[5]);
	printf("DST Addr:  \t\t\t%02x:%02x:%02x:%02x:%02x:%02x\n", prms->dst_addr[0],prms->dst_addr[1],prms->dst_addr[2],prms->dst_addr[3],prms->dst_addr[4],prms->dst_addr[5]);
	printf("nominal Packet Extension:\t%hi\n", prms->nominalPacketExtension);
	printf("Feedback Status:\t\t%hi\n", prms->feedbackStatus);
	printf("Aid:\t\t\t\t%hi\n", prms->aid);
	printf("Group ID:\t\t\t%hi\n", prms->group_id);
	printf("TB HE LTF Number:\t\t%hi\n", prms->he_ltf_num);
	printf("TB RU ALLOCATION:\t\t%hi\n", prms->ru_alloc);
	printf("TB UL BW:\t\t\t%hi\n", prms->ul_bw);
	printf("TB auto mode:     \t\t%hi\n", prms->tb_auto_mode);
	printf("status:\t\t\t\t%d\n",prms->header.status);
	
	return NL_SKIP;
}


static int plt_cc33xx_get_tx_params(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_start_tx prms;

	if (argc != 0)
		return 1;

	prms.test.id = CC33XX_TEST_CMD_GET_PARAMS;

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);
	NLA_PUT_U8(msg, WL1271_TM_ATTR_ANSWER, 1);

	nla_nest_end(msg, key);

	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM,
		  plt_cc33xx_display_tx_params, NULL);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, get_tx_params, "",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_get_tx_params,
	"Retrieve TX params for PLT.\n");


static int plt_cc33xx_stop_tx(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_stop_tx prms;

	if (argc != 0)
		return 1;

	prms.test.id	= CC33XX_TEST_CMD_TX_STOP;
	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, stop_tx, "",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_stop_tx,
	"Stop any TX work. including Tone TX and Packets TX for PLT.\n");



static int plt_cc33xx_start_rx(struct nl80211_state *state, struct nl_cb *cb,
			       struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_start_rx prms;
	size_t arg_counter = 0;

	if (0 != (argc % 2)) {
		fprintf(stderr, "Invalid parameters combination\n");
		return 1;
	}

	prms.test.id = CC33XX_TEST_CMD_START_RX_SIMULATION;

	/* Default paramters setting - don't filter aid, rate and preamble type */
	str2mac(prms.mac_addr, "ff:ff:ff:ff:ff:ff");
	prms.ACKenable = false;
	prms.aid = (__u16 )RX_PARAMS_INVALID_AID;
	prms.rate = (__u8 )RX_PARAMS_INVALID_RATE;
	prms.preamble_type = (__u8 )RX_PARAMS_INVALID_PREAMBLE_TYPE;

	/* Assign user`s parameters */
	while (arg_counter < argc) {
		if (0 == strcmp(argv[arg_counter], "-source_mac")) {
			str2mac(prms.mac_addr, argv[arg_counter + 1]);
		} else if (0 == strcmp(argv[arg_counter], "-ack_enable")) {
			prms.ACKenable = (__u8)atoi(argv[arg_counter + 1]);
		} else if (0 == strcmp(argv[arg_counter], "-aid")) {
			prms.aid = (__u16)atoi(argv[arg_counter + 1]);
		} else if (0 == strcmp(argv[arg_counter], "-rate")) {
			prms.rate = (__u8)atoi(argv[arg_counter + 1]);
		} else if (0 == strcmp(argv[arg_counter], "-preamble_type")) {
			prms.preamble_type = (__u8)atoi(argv[arg_counter + 1]);
		} else {
			fprintf(stderr, "Error, Wrong Syntax, %s is not a valid parameter \n", argv[arg_counter]);
			return 1;
		}
		arg_counter += 2;
	}

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);
	printf("Calibrator:: Starting RX Simulation (Note that statistics counters are being reset)...\n");

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, start_rx, "[SWITCHES...]",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_start_rx,
	"DESCRIPTION\n\r"
	"\tThe command triggers the device to gather RX statistics for PLT. Use SWITCHES to add optional\n\r"
	"\tfilters for RXed packets. Use <source_mac> to designate source mac address to listen to.\n\r"
	"\tIf you don't specify rate/preamble_type/aid, it won't be filtered at all. ACK_enable is defaulted FALSE\n\r\n\r"
	"SWITCHES\n\r"
	"\t-source_mac\n\r"
	"\t\tDesignate source mac address to filter RXed packets. Default is FF:FF:FF:FF:FF:FF\n\r\n\r"
	"\t-ack_enable\n\r"
	"\t\tChoose whether you'd like to work with ACKs or not. Default is false - disabling ACK response\n\r"
	"\t\t0 - disabled\n\r"
	"\t\t1 - enabled\n\r\n\r"	
	"\t-aid\n\r"
	"\t\tSpecify requseted AID for PHY configuration\n\r\n\r"
	"\t-rate\n\r"
	"\t\tChoose desired rate to filter from received packets\n\r"
	"\t\t\t1 - 1Mb/s\n\r"
	"\t\t\t2 - 2Mb/s\n\r"
	"\t\t\t3 - 5.5Mb/s\n\r"
	"\t\t\t4 - 11Mb/s\n\r"
	"\t\t\t5 - 6Mb/s\n\r"
	"\t\t\t6 - 9Mb/s\n\r"
	"\t\t\t7 - 12Mb/s\n\r"
	"\t\t\t8 - 18Mb/s\n\r"
	"\t\t\t9 - 24Mb/s\n\r"
	"\t\t\t10 - 36Mb/s\n\r"
	"\t\t\t11 - 48Mb/s\n\r"
	"\t\t\t12 - 54Mb/s\n\r"
	"\t\t\t13 - MCS0 (6.5Mb/s)\n\r"
	"\t\t\t14 - MCS1 (13Mb/s)\n\r"
	"\t\t\t15 - MCS2 (19.5.5Mb/s)\n\r"
	"\t\t\t16 - MCS3 (26Mb/s)\n\r"
	"\t\t\t17 - MCS4 (39Mb/s)\n\r"
	"\t\t\t18 - MCS5 (52Mb/s)\n\r"
	"\t\t\t19 - MCS6 (58.5Mb/s)\n\r"
	"\t\t\t20 - MCS7 (65Mb/s)\n\r\n\r"
	"\t-preamble_type\n\r"
	"\t\tChoose preamble type - standard to filter from received packets\n\r"
	"\t\t\t0 - PREAMBLE_TYPE_SHORT\n\r"
	"\t\t\t1 - PREAMBLE_TYPE_LONG\n\r"
	"\t\t\t2 - PREAMBLE_TYPE_OFDM\n\r"
	"\t\t\t3 - PREAMBLE_TYPE_N_MIXED_MODE\n\r"
	"\t\t\t4 - PREAMBLE_TYPE_GREENFIELD\n\r"
	"\t\t\t5 - PREAMBLE_TYPE_AX_SU\n\r"
	"\t\t\t6 - PREAMBLE_TYPE_AX_MU\n\r"
	"\t\t\t7 - PREAMBLE_TYPE_AX_SU_ER\n\r"
	"\t\t\t8 - PREAMBLE_TYPE_AX_TB\n\r"
	"\t\t\t9 - PREAMBLE_TYPE_AX_TB_NDP_FB\n\r"
	"\t\t\t10 - PREAMBLE_TYPE_AC_VHT\n\r\n\r"
	"EXAMPLES\n\r"
	"\tstart_rx -source_mac f0:f8:f2:4c:0d:da -ack_enable 1 -aid 4 -rate 9 -preamble_type 10\n\r"
	"\tstart_rx -ack_enable 0 -rate 4");


static int plt_cc33xx_stop_rx(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_stop_rx prms;

	if (argc != 0)
		return 1;

	prms.test.id	= CC33XX_TEST_CMD_STOP_RX_STATS;
	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, stop_rx, "",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_stop_rx,
	"Stop any RX statistics work \n");


static int plt_cc33xx_display_rx_stats(struct nl_msg *msg, void *arg)
{
	struct nlattr *tb[NL80211_ATTR_MAX + 1];
	struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
	struct nlattr *td[WL1271_TM_ATTR_MAX + 1];
	struct cc33xx_cmd_get_rx_stats *prms;

	nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
		genlmsg_attrlen(gnlh, 0), NULL);

	if (!tb[NL80211_ATTR_TESTDATA]) {
		fprintf(stderr, "no data!\n");
		return NL_SKIP;
	}

	nla_parse(td, WL1271_TM_ATTR_MAX, nla_data(tb[NL80211_ATTR_TESTDATA]),
		  nla_len(tb[NL80211_ATTR_TESTDATA]), NULL);

	prms = (struct cc33xx_cmd_get_rx_stats *) nla_data(td[WL1271_TM_ATTR_DATA]);

	printf("Total Received Packets:\t%d\n", prms->ReceivedTotalPacketsNumber);
	printf("FCS Errors:\t\t%d\n", prms->ReceivedFcsErrorPacketsNumber);
	printf("MAC Mismatch:\t\t%d\n", prms->ReceivedAddressMismatchPacketsNumber);
	printf("Good Packets:\t\t%d\n", prms->ReceivedGoodPackets);
	printf("Average RSSI (SOC):\t%hi\n", prms->AverageDataCtrlRssi);
	printf("Average RSSI (ANT):\t%hi\n", prms->AverageDataCtrlSNR);
	printf("status: %d\n",prms->header.status);
	
	if(prms->ReceivedTotalPacketsNumber) {
		float per = ((float)prms->ReceivedTotalPacketsNumber - (float)prms->ReceivedGoodPackets)/(float)prms->ReceivedTotalPacketsNumber;
		printf("PER:\t\t\t%f     # PER = Total Bad / Total Received\n", per);
	} else {
		printf("PER:\t\t\tN/A     # PER = Total Bad / Total Received\n");
	}

	return NL_SKIP;
}

static int plt_cc33xx_get_rx_stats(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_get_rx_stats prms;

	if (argc != 0)
		return 1;

	prms.test.id = CC33XX_TEST_CMD_GET_RX_STATS;

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);
	NLA_PUT_U8(msg, WL1271_TM_ATTR_ANSWER, 1);

	nla_nest_end(msg, key);

	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM,
		  plt_cc33xx_display_rx_stats, NULL);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, get_rx_stats, "",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_get_rx_stats,
	"Retrieve RX statistics for PLT.\n");

static int plt_cc33xx_switch_ant(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{ 
	struct nlattr *key;
	struct cc33xx_cmd_switch_ant prms;

	if (argc != 1)
		return 1;

	prms.test.id	= CC33XX_TEST_CMD_SWITCH_ANT;
	prms.ant	= (__u8)atoi(argv[0]);

    
	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, switch_ant, "<antenna>",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_switch_ant,
	"Set antenna mode for PLT:\n\r\tParameters:\n\r\tFor antenna number 0 - \"0\"\n\r\tFor antenna number 1 - \"1\"\n\r\tToggle the antenna(default) - \"2\"\n\r");

static int power_mode_str_to_int(char *power_mode_str)
{
	int mode;
	if (0 == strcmp(power_mode_str, "normal_mode")) {
		mode = (__u8)PHY_MODE_NORMAL_E;
	} else if (0 == strcmp(power_mode_str, "rx_only")) {
		mode = (__u8)PHY_MODE_RX_ONLY_E;
	} else if (0 == strcmp(power_mode_str, "rx_listen")) {
		mode = (__u8)PHY_MODE_LISTEN_E;
	} else if (0 == strcmp(power_mode_str, "rf_off")) {
		mode = (__u8)PHY_MODE_RF_OFF_E;
	} else if (0 == strcmp(power_mode_str, "lpds")) {
		mode = (__u8)PHY_MODE_LPDS_E;
	} else if (0 == strcmp(power_mode_str, "prepared_to_sleep")) {
		mode = (__u8)PHY_MODE_PREPARED_TO_SLEEP_E;
	} else {
		mode = -1;
	}
	return mode;
}

static int plt_cc33xx_change_phy_mode(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_change_mode prms;
	bool valid_second_param;
	int power_mode;
	
	prms.test.id = CC33XX_TEST_CMD_CHANGE_PHY_MODE;

	if (argc < 1)
		return 1;

	power_mode = power_mode_str_to_int(argv[0]);

	/* Check first parameter */
	if (power_mode < 0) {
		fprintf(stderr, "Error, Wrong Syntax, %s is not a valid <mode> parameter \n",argv[0]);
		return 1;
	} else if (PHY_MODE_LPDS_E == power_mode) {
		/* We should support a waiting period and new state in this case */
		if (argc != 3)
			return 1;

		prms.mode = (__u8)power_mode;

		/* Check second parameter validity */
		valid_second_param = check_str_all_digits(argv[1]);
		if (!valid_second_param) {
			fprintf(stderr, "Error, Wrong Syntax, %s is not a valid <duration> parameter \n",argv[1]);
			return 1;
		}
		prms.duartion = (__u32)atoi(argv[1]);

		/* Check third parameter */
		power_mode = power_mode_str_to_int(argv[2]);
		if (prms.mode_after_sleep < 0) {
			fprintf(stderr, "Error, Wrong Syntax, %s is not a valid <mode_after_sleep> parameter \n",argv[2]);
			return 1;
		}
		prms.mode_after_sleep = (__u8)power_mode;
	} else {
		if (argc != 1)
			return 1;

		prms.mode = (__u8)power_mode;

		/* Just for initialization purpose */
		prms.duartion = 0;
		prms.mode_after_sleep = 0;
	}

		key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, change_power_mode, "<mode> <duration> <mode_after_sleep>",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_change_phy_mode,
	"Set cc33xx power mode:\n\r\n\r" 
	"<mode> Power mode options:\n\r"
	"\tnormal_mode\n\r"
	"\trx_only\n\r"
	"\trx_listen\n\r"
	"\trf_off\n\r"
	"\tlpds\n\r"
	"\tprepared_to_sleep\n\r"
	"<duration> Sleep duration in usec, applicable only on lpds\n\r"
	"\tUsed only for sleep mode, in order to wake the device up after the specified duration. Default is zero.\n\r"
	"\tContains any unsigned value of 4 bytes max.\n\r"
	"<mode_after_sleep> Mode to switch to after sleep, applicable only on lpds\n\r"
	"\tAfter sleep duration wakeup will occur to this specified mode. Units are uSec."
	"\tMax value is as much as a 32 unsigned integer can hold");

static int plt_cc33xx_set_io_conf(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_set_io_cfg params;

	if (argc != 3)
		return 1;

	params.test.id = CC33XX_TEST_CMD_SET_IO_CFG;
	params.io_id = (__u16)atoi(argv[0]);
	params.direction = (__u8)atoi(argv[1]);
	params.state = (__u8)atoi(argv[2]);
	
	//if mode is not single tone then not supported
	if ( (params.io_id < 0) || (params.io_id > 17)) {
		fprintf(stderr, "io id %d isn't valid", params.io_id);
		return 1;
	}
	if ( (params.direction < 0) || (params.direction > 1)) {
		fprintf(stderr, "direction %d isn't valid", params.direction);
		return 1;
	}
	if ( (params.state < 0) || (params.state > 1)) {
		fprintf(stderr, "state %d isn't valid", params.state);
		return 1;
	}
	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(params), &params);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, set_io_conf, "<io_id> <direction> <state>",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_set_io_conf,
	"Set IO Configuration\n\n"
    "<io_id>\t\t IO ID - valid range 0-17:\n"
    "\t\t\t0 = SLOW_CLOCK_IN\n"
    "\t\t\t1 = SDIO_CLK\n"
    "\t\t\t2 = SDIO_CMD\n"
	"\t\t\t3 = SDIO_D0\n"
	"\t\t\t4 = SDIO_D1\n"
	"\t\t\t5 = SDIO_D2\n"
	"\t\t\t6 = SDIO_D3\n"
	"\t\t\t7 = HOST_IRQ_WL\n"
	"\t\t\t8 = UART1_TX\n"
	"\t\t\t9 = UART1_RX\n"
	"\t\t\t10 = UART1_CTS\n"
	"\t\t\t11 = UART1_RTS\n"
	"\t\t\t12 = COEX_PRIORITY\n"
	"\t\t\t13 = COEX_PEQ\n"
	"\t\t\t14 = COEX_GRANT\n"
	"\t\t\t15 = HOST_IRQ_BLE\n"
	"\t\t\t16 = FAST_CLK_REQ\n"
	"\t\t\t17 = ANT_SEL\n"
	"<direction>\t\t IO direction:\n"
	"\t\t\t0 = output\n"
	"\t\t\t1 = input\n"
	"<state>\t\t set the state:\n"
	"\t\t\t0 = low\n"
	"\t\t\t1 = high\n"	);


static int plt_cc33xx_display_io_stats(struct nl_msg *msg, void *arg)
{
	struct nlattr *tb[NL80211_ATTR_MAX + 1];
	struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
	struct nlattr *td[WL1271_TM_ATTR_MAX + 1];
	struct cc33xx_cmd_get_io_cfg *prms;

	nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
		genlmsg_attrlen(gnlh, 0), NULL);

	if (!tb[NL80211_ATTR_TESTDATA]) {
		fprintf(stderr, "no data!\n");
		return NL_SKIP;
	}

	nla_parse(td, WL1271_TM_ATTR_MAX, nla_data(tb[NL80211_ATTR_TESTDATA]),
		  nla_len(tb[NL80211_ATTR_TESTDATA]), NULL);

	prms = (struct cc33xx_cmd_get_io_cfg *) nla_data(td[WL1271_TM_ATTR_DATA]);

	printf(" io_id=%d direrction=%d state=%d\n", prms->io_id, prms->direction, prms->value);

	return NL_SKIP;
}
static int plt_cc33xx_get_io_conf(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_get_io_cfg prms;

	if (argc != 1)
		return 1;

	
	prms.io_id = (__u16)atoi(argv[0]);
	if ( (prms.io_id < 0) || (prms.io_id > 17)) {
		fprintf(stderr, "io id %d isn't valid", prms.io_id);
		return 1;
	}
	prms.test.id = CC33XX_TEST_CMD_GET_IO_CFG;

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);
	NLA_PUT_U8(msg, WL1271_TM_ATTR_ANSWER, 1);

	nla_nest_end(msg, key);

	nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM,
		  plt_cc33xx_display_io_stats, NULL);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;

}

COMMAND(cc33xx_plt, get_io_conf, "<io_id>",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_get_io_conf,
	"Set IO Configuration\n\n"
    "<io_id>\t\t IO ID - valid range 0-17:\n"
    "\t\t\t0 = SLOW_CLOCK_IN\n"
    "\t\t\t1 = SDIO_CLK\n"
    "\t\t\t2 = SDIO_CMD\n"
	"\t\t\t3 = SDIO_D0\n"
	"\t\t\t4 = SDIO_D1\n"
	"\t\t\t5 = SDIO_D2\n"
	"\t\t\t6 = SDIO_D3\n"
	"\t\t\t7 = HOST_IRQ_WL\n"
	"\t\t\t8 = UART1_TX\n"
	"\t\t\t9 = UART1_RX\n"
	"\t\t\t10 = UART1_CTS\n"
	"\t\t\t11 = UART1_RTS\n"
	"\t\t\t12 = COEX_PRIORITY\n"
	"\t\t\t13 = COEX_PEQ\n"
	"\t\t\t14 = COEX_GRANT\n"
	"\t\t\t15 = HOST_IRQ_BLE\n"
	"\t\t\t16 = FAST_CLK_REQ\n"
	"\t\t\t17 = ANT_SEL\n"
	"<direction>\t\t IO direction:\n"
	"\t\t\t0 = output\n"
	"\t\t\t1 = input\n"
	"<state>\t\t set the state:\n"
	"\t\t\t0 = low\n"
	"\t\t\t1 = high\n"	);
static int plt_cc33xx_ble(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_set_io_cfg params;

	params.test.id = CC33XX_TEST_CMD_BLE_ENABLE;
	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(params), &params);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, ble_plt, "",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_ble,
	"Enable hci ble test\n\n");

static int plt_cc33xx_tx_tone_start(struct nl80211_state *state, struct nl_cb *cb,
				   struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_start_tone_tx params;

	if (argc != 2)
		return 1;

	params.test.id = CC33XX_TEST_CMD_TX_TONE_START;
	params.mode = (__u8)atoi(argv[0]);
	params.offset = (__u8)atoi(argv[1]);

	/*right now output power isnt relevant, will change when working*/
	//params.output_power = (__u8)atoi(argv[2]);

	//default value
	params.output_power = 8;
	
	//if mode is not single tone then not supported
	if (params.mode != 2) {
		fprintf(stderr, "mode %d isn't supported", params.mode);
		return 1;
	}
	if ( (params.offset > 40) || (params.offset < -40)) {
		fprintf(stderr, "offset %d isn't supported", params.offset);
		return 1;
	}

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(params), &params);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, tx_start_tone, "<mode> <offset>",//later <output_power>
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_tx_tone_start,
	"Start TX Tone\n\n"
    "<mode>\t\t tone mode:\n"
    "\t\t\t0 = silence: Tx chain is on but no signal is transmitted (Not Supported)\n"
    "\t\t\t1 = carrier feedthrough: Tx chain is on and the LO leakage is transmitted (Not Supported)\n"
    "\t\t\t2 = single tone: Tx chain is on and a sinusoidal signal is transmitted\n"
	"<offset>\t\t Tx tone offset:\n"
	"<offset>\t\t-10MHz to +10MHz in steps of 0.25MHz\n"
	"\t\t\t-40 to 40 steps (0.25MHz each step)\n");

static int plt_cc33xx_tx_tone_stop(struct nl80211_state *state, struct nl_cb *cb,
                				   struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct cc33xx_cmd_stop_tx prms;

    if (argc != 0)
        return 1;

    prms.test.id = CC33XX_TEST_CMD_TX_STOP;

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    printf("Calibrator:: Stopping TX Tone\n");

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(cc33xx_plt, tx_tone_stop , "",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_tx_tone_stop,
    " Stop TX Tone\n");


static int plt_cc33xx_set_manual_calib(struct nl80211_state *state, struct nl_cb *cb,
			       struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	struct cc33xx_cmd_set_manual_calib prms;
	prms.test.id = CC3XX_TEST_CMD_MANUAL_CALIBRATION;
	int counter = 0;
	prms.calibration_bitmap = 0;

	if (0 == (argc))
	{
		fprintf(stderr, "Invalid parameters combination\n");
		return 1;
	}
	if (0 != (argc % 2)) {
		fprintf(stderr, "Invalid parameters combination\n");
		return 1;
	}

	while (counter < argc)
	{
		if (!strcmp(argv[counter], "-rx")) {
			/* rx */
			if((atoi(argv[counter+1]) > 1 ) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "rx is out of range (valid range: 0 or 1)\n");
				return 1;
			}
			if(atoi(argv[counter+1]) == 1)
				prms.calibration_bitmap = prms.calibration_bitmap | CALIB_RX_IQMM_BITWISE_MASK | CALIB_RX_DC_BITWISE_MASK | CALIB_RX_SPUR_CANCELER_BITWISE_MASK;

		} else if (!strcmp(argv[counter], "-tx")) {
			/* tx */
			if ((atoi(argv[counter+1]) > 1 ) || (atoi(argv[counter+1]) < 0))
			{
				fprintf(stderr, "tx is out of range (valid range: 0 or 1)\n");
				return 1;
			}

			if (atoi(argv[counter+1]) == 1)
					prms.calibration_bitmap = prms.calibration_bitmap | CALIB_TX_AUX_RX_DC_BITWISE_MASK | CALIB_TX_IQMM_BITWISE_MASK 
												| CALIB_TX_LOL_BITWISE_MASK | CALIB_TX_RFNL_AND_DPD_BITWISE_MASK;
		} else {
			fprintf(stderr, "Error, Wrong Syntax, %s is not a valid parameter \n",argv[counter]);
			return 1;
		}		

		counter += 2;
	}
	fprintf(stderr, "set_manual_calib has been called with: %d Parameters \n",argc); 

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "fail to nla_nest_start()\n");
		return 1;
	}

	printf("Calibrator:: Setting Calibration Parameters\n");

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_TEST);
	NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(cc33xx_plt, set_manual_calib , "-rx -tx ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_cc33xx_set_manual_calib,
    "Set Manual Calibration for Rx/Tx\n"
	"example: set_manual_calib -rx 1 -tx 1\n"
    "calibrate rx: range: 0-1 (False/True)\n"
    "calibrate tx:  range 0-1 (False/True)\n");

static int plt_power_mode(struct nl80211_state *state, struct nl_cb *cb,
			  struct nl_msg *msg, int argc, char **argv)
{
	struct nlattr *key;
	unsigned int pmode;

	if (argc != 1) {
		fprintf(stderr, "%s> Missing arguments\n", __func__);
		return 2;
	}

	if (strcmp(argv[0], "on") == 0)
		pmode = PLT_ON;
	else if (strcmp(argv[0], "off") == 0)
		pmode = PLT_OFF;
	else if (strcmp(argv[0], "fem_detect") == 0)
		pmode = PLT_FEM_DETECT;
	else if (strcmp(argv[0], "chip_awake") == 0)
		pmode = PLT_CHIP_AWAKE;
	else {
		fprintf(stderr, "%s> Invalid parameter\n", __func__);
		return 2;
	}

	key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
	if (!key) {
		fprintf(stderr, "%s> fail to nla_nest_start()\n", __func__);
		return 1;
	}

	NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_SET_PLT_MODE);
	NLA_PUT_U32(msg, WL1271_TM_ATTR_PLT_MODE, pmode);

	nla_nest_end(msg, key);

	return 0;

nla_put_failure:
	fprintf(stderr, "%s> building message failed\n", __func__);
	return 2;
}

COMMAND(plt, power_mode, "<on|off|fem_detect|chip_awake>",
	NL80211_CMD_TESTMODE, 0, CIB_NETDEV, plt_power_mode,
	"Set PLT power mode\n");
