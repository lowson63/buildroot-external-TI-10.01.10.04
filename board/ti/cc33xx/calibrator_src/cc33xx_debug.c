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

#include "calibrator.h"
#include "plt.h"
#include "cc33xx_debug.h"

SECTION(CC33xx_debug);

/*******************************************************************************/
/* RATE_OVERRIDE_CFG                                                           */
/*******************************************************************************/
static int CC33xx_debug_rate_override(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_rate_override prms;



    prms.overEnable = atoi(argv[0]);
    prms.bw = atoi(argv[1]);
    prms.preamble = atoi(argv[2]);
    prms.rate = atoi(argv[3]);
    prms.dcm = atoi(argv[4]);
    prms.txPower = atoi(argv[5]);
    prms.giLTF = atoi(argv[6]);

    //if( prms.enable_val > 1 || prms.enable_val < 0)
    //{
    //    fprintf(stderr, "enable value can be 0/1\n");
    //    return 1;
    //}

    printf("\nRate override enable/disable: %d, rate: %d preamble: %d\n", prms.overEnable, prms.rate, prms.preamble);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, RATE_OVERRRIDE_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, rate_override , "<overEnable> <bw> <preamble> <rate> <dcm> <txPower> <giLTF> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_rate_override,
    " Rate Override \n");

/*******************************************************************************/
/* UPLINK_MULTI_USER_CFG                                                       */
/*******************************************************************************/
static int CC33xx_debug_uplink_mu(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_uplink_mu prms;

    if (argc != 1)
        return 1;

    prms.enable_val = atoi(argv[0]);

    if( prms.enable_val > 1 || prms.enable_val < 0)
    {
        fprintf(stderr, "enable value can be 0/1\n");
        return 1;
    }

    printf("\nUplink Multiuser enable/disable: %d\n", prms.enable_val);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, UPLINK_MULTI_USER_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, uplink_mu , "<enable> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_uplink_mu,
    " Uplink Multiuser enable/disable.\n");

/*******************************************************************************/
/* UPLINK_MULTI_USER_DATA_CFG                                                  */
/*******************************************************************************/
static int CC33xx_debug_uplink_mu_data(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_uplink_mu_data prms;

    if (argc != 1)
        return 1;

    prms.enable_val = atoi(argv[0]);

    if( prms.enable_val > 1 || prms.enable_val < 0)
    {
        fprintf(stderr, "enable value can be 0/1\n");
        return 1;
    }

    printf("\nUplink Multiuser data enable/disable: %d\n", prms.enable_val);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, UPLINK_MULTI_USER_DATA_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, uplink_mu_data , "<enable> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_uplink_mu_data,
    " Uplink Multiuser data enable/disable.\n");

/*******************************************************************************/
/* OPERATION_MODE_CTRL_CFG                                                     */
/*******************************************************************************/
static int CC33xx_debug_oper_mode_ctrl(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_oper_mode_ctrl prms;

    if (argc != 1)
        return 1;

    prms.enable_val = atoi(argv[0]);

    if( prms.enable_val > 1 || prms.enable_val < 0)
    {
        fprintf(stderr, "enable value can be 0/1\n");
        return 1;
    }

    printf("\nOperation mode control enable/disable: %d\n", prms.enable_val);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, OPERATION_MODE_CTRL_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, oper_mode , "<enable> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_oper_mode_ctrl,
    " Operation mode control enable/disable.\n");

/*******************************************************************************/
/* UPLINK_POWER_HEADER_CFG                                                     */
/*******************************************************************************/
static int CC33xx_debug_uplink_pwr_hdr(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_uplink_pwr_hdr prms;

    if (argc != 1)
        return 1;

    prms.enable_val = atoi(argv[0]);

    if( prms.enable_val > 1 || prms.enable_val < 0)
    {
        fprintf(stderr, "enable value can be 0/1\n");
        return 1;
    }

    printf("\nUplink power header enable/disable: %d\n", prms.enable_val);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, UPLINK_POWER_HEADER_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, power_head , "<enable> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_uplink_pwr_hdr,
    " Uplink power head enable/disable.\n");

/*******************************************************************************/
/* MCS_FIXED_RATE_CFG                                                          */
/*******************************************************************************/
static int CC33xx_debug_mcs_rate(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_mcs_rate prms;

    if (argc != 1)
        return 1;

    prms.rate_val = atoi(argv[0]);

    if( prms.rate_val <= 0 )
    {
        fprintf(stderr, "MSC rate must be grater than 0\n");
        return 1;
    }

    printf("\nMCS Fixed rate value: %d\n", prms.rate_val);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, MCS_FIXED_RATE_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, mcs_rate , "<rate> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_mcs_rate,
    " MCS rate value .\n");

/*******************************************************************************/
/* GI_LTF_CFG                                                                  */
/*******************************************************************************/
static int CC33xx_debug_gi_ltf(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_gi_ltf prms;

    if (argc != 1)
        return 1;

    prms.value = atoi(argv[0]);

    if( prms.value <= 0 )
    {
        fprintf(stderr, "GI_LTF must be grater than 0\n");
        return 1;
    }

    printf("\nGI_LTF value: %d\n", prms.value);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, GI_LTF_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, gi_ltf , "<value> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_gi_ltf,
    " GI_LTF value .\n");

/*******************************************************************************/
/* TRANSMIT_OMI_CFG                                                            */
/*******************************************************************************/
static int CC33xx_debug_transmit_omi(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_transmit_omi prms;

    if (argc != 1)
        return 1;

    prms.enable_val = atoi(argv[0]);

    if( prms.enable_val > 1 || prms.enable_val < 0)
    {
        fprintf(stderr, "TRANSMIT_OMI value can be 0/1\n");
        return 1;
    }

    printf("\nTRANSMIT_OMI value: %d\n", prms.enable_val);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, TRANSMIT_OMI_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, trans_omi , "<enable> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_transmit_omi,
    " TRANSMIT_OMI enable/disable.\n");

/*******************************************************************************/
/* TB_ONLY_CFG                                                                 */
/*******************************************************************************/
static int CC33xx_debug_tb_only(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_tb_only prms;

    if (argc != 1)
        return 1;

    prms.enable_val = atoi(argv[0]);

    if( prms.enable_val > 1 || prms.enable_val < 0)
    {
        fprintf(stderr, "TB ONLY value can be 0/1\n");
        return 1;
    }

    printf("\nTB_ONLY value: %d\n", prms.enable_val);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, TB_ONLY_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, tb_only , "<enable> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_tb_only,
    " TB ONLY enable/disable.\n");


/*******************************************************************************/
/* BA_SESSION_CFG                                                              */
/*******************************************************************************/
static int CC33xx_debug_ba_session(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_ba_session prms;

	if (argc < 1 || argc > 2)
		return 1;

    prms.block_ack_rx = atoi(argv[0]);
    prms.block_ack_tx = atoi(argv[1]);

    if( (prms.block_ack_rx > 1 || prms.block_ack_rx < 0 ) ||
            (prms.block_ack_tx > 1 || prms.block_ack_tx < 0 ))
    {
        fprintf(stderr, "block ack rx/tx value can be 0/1\n");
        return 1;
    }

    printf(" block_ack_rx value: %d block_ack_tx value: %d \n",
           prms.block_ack_rx,
           prms.block_ack_tx);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, BA_SESSION_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, ba_session , "<block_ack_rx> <block_ack_tx>",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_ba_session,
    " block ack rx/tx enable/disable.\n");


/*******************************************************************************/
/* BLS_CFG                                                              */
/*******************************************************************************/
static int CC33xx_debug_bls_cfg(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_bls_cfg prms;

    if (argc < 1 || argc > 1)
        return 1;

    prms.bls_allowed = atoi(argv[0]);

    if( (prms.bls_allowed > 1 || prms.bls_allowed < 0 ))
    {
        fprintf(stderr, "bls allowed value can be 0/1\n");
        return 1;
    }

    printf(" bls allowed value: %d  \n",
           prms.bls_allowed);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, BLS_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, bls_cfg , "<bls_allowed> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_bls_cfg,
    " bls allowed enable/disable.\n");


/*******************************************************************************/
/* BEACON_RSSI_INTR                                                            */
/*******************************************************************************/
static int CC33xx_debug_display_beacon_rssi(struct nl_msg *msg, void *arg)
{
    struct nlattr *tb[NL80211_ATTR_MAX + 1];
    struct genlmsghdr *gnlh = nlmsg_data(nlmsg_hdr(msg));
    struct nlattr *td[WL1271_TM_ATTR_MAX + 1];
    struct CC33xx_cmd_beacon_rssi *prms;

    nla_parse(tb, NL80211_ATTR_MAX, genlmsg_attrdata(gnlh, 0),
        genlmsg_attrlen(gnlh, 0), NULL);

    if (!tb[NL80211_ATTR_TESTDATA]) {
        fprintf(stderr, "no data!\n");
        return NL_SKIP;
    }

    nla_parse(td, WL1271_TM_ATTR_MAX, nla_data(tb[NL80211_ATTR_TESTDATA]),
          nla_len(tb[NL80211_ATTR_TESTDATA]), NULL);

    prms = (struct CC33xx_cmd_beacon_rssi *) nla_data(td[WL1271_TM_ATTR_DATA]);
    if(prms->beacon_rssi)
    {
        printf("\nBeacon RSSI: -%d\n", 1 + (unsigned char)(~ prms->beacon_rssi));
    }
    else
    {
        printf("\nBeacon RSSI: Not connected!\n");
    }

    return NL_SKIP;
}


static int CC33xx_debug_beacon_rssi(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_beacon_rssi prms;

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_INTERROGATE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, BEACON_RSSI_INTR);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);


    nl_cb_set(cb, NL_CB_VALID, NL_CB_CUSTOM,
                CC33xx_debug_display_beacon_rssi, NULL);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, beacon_rssi , "",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_beacon_rssi,
    " beacon Rssi.\n");

/*******************************************************************************/
/* POWER_SAVE_CFG                                                              */
/*******************************************************************************/
static int CC33xx_debug_psm(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_force_ps prms;

    if (argc != 2)
        return 1;

    prms.param_index = atoi(argv[0]);
    prms.param_value = atoi(argv[1]);

    switch ((enum traffic_parameters)prms.param_index)
    {
        case ACTIVE_MODE_RX_TH:
        case ACTIVE_MODE_TX_TH:
        {
            if(prms.param_value > 255)
            {
                goto nla_put_failure;
            }

        }break;
        case ACTIVE_MODE_RX_TO:
        case ACTIVE_MODE_TX_TO:
        {

        }break;
        case FORCE_POWER_MODE:
        {
            if(prms.param_value > 2)
            {
                goto nla_put_failure;
            }
        }break;
        default:
            fprintf(stderr, "unvalid param index\n");
            return 1;
    }

    printf("\nparam_index: %d param_value%d\n", prms.param_index , prms.param_value);

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, FORCE_PS_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, psm , "<index> <value>",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_psm,
    " Active Mode index and value.\n");


/*******************************************************************************/
/* RTS_TH_CFG                                                              */
/*******************************************************************************/
static int CC33xx_debug_rts_th_cfg(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_rts_th_cfg prms;

    if (argc != 2)
        return 1;

    prms.rts_th = atoi(argv[0]);
    prms.roleid = atoi(argv[1]);

    if( (prms.roleid > 4 || prms.roleid < 0 ))
    {
        fprintf(stderr, "role id unsupported - %d\n", prms.roleid);
        return 1;
    }

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }

    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, RTS_TH_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, rts_th_cfg , "<rts threshold> <roleid> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_rts_th_cfg,
    " rts threshold value and role id\n");


/*******************************************************************************/
/* LINK_ADAPT_CFG                                                              */
/*******************************************************************************/
static int CC33xx_debug_link_adapt_cfg(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_cmd_link_adapt_cfg prms;

    if ((argc < 2) || (argc >3))
        return 1;

    if(!strcmp(argv[0], "-force_disable_dcm"))
    {
        prms.type = FORCE_DISABLE_DCM;
        prms.param1 = atoi(argv[1]);
    }

    if(!strcmp(argv[0], "-force_disable_er"))
    {
        prms.type = FORCE_DISABLE_ER;
        prms.param1 = atoi(argv[1]);
    }

    if(!strcmp(argv[0], "-force_disable_er_upper"))
    {
        prms.type = FORCE_DISABLE_ER_UPPER;
        prms.param1 = atoi(argv[1]);
    }

    if(!strcmp(argv[0], "-force_long_term_policy"))
    {
        prms.type = FORCE_LONG_TERM_POLICY;
        prms.param1 = atoi(argv[1]);
        prms.param2 = atoi(argv[2]);
    }
    
    if(!strcmp(argv[0], "-override_nominal_padding"))
    {
        prms.type = FORCE_NOMINAL_PADDING;
        prms.param1 = atoi(argv[1]);
        prms.param2 = atoi(argv[2]);
    }
    
    if(!strcmp(argv[0], "-enable_debug_trace"))
    {
        prms.type = ENABLE_DEBUG_TRACE;
        prms.param1 = atoi(argv[1]);
    }

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }
    
    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, LINK_ADAPT_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, link_adapt_cfg , "<-force_disable_dcm 0/1> <-force_disable_er 0/1> <-force_disable_er_upper 0/1> <-force_long_term_policy 32msb 32lsb> <-override_nominal_padding override value> <-enable_debug_trace 0/1 ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_link_adapt_cfg,
    " debug configuration for link adaptation\n");


/*******************************************************************************/
/* PWR_PARTIAL_MODES_CFG                                                              */
/*******************************************************************************/
static int CC33xx_debug_pwr_partial_modes_cfg(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_pwr_partial_modes_cfg prms;

    if (argc < 1 || argc > 1)
        return 1;

    prms.partial_modes_allowed = atoi(argv[0]);
    if(prms.partial_modes_allowed > 1 )
    {
        fprintf(stderr, "bad input, input range is (0-1) your input: %x\n", prms.partial_modes_allowed);
        return 1;     
    }
    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }
    
    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, PWR_PARTIAL_MODES_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, pwr_partial_mode_cfg , "<Disable/Enable partial modes 0/1> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_pwr_partial_modes_cfg,
    " debug configuration for partial modes\n");


/*******************************************************************************/
/* CALIB_BITMAP_CFG                                                              */
/*******************************************************************************/
static int CC33xx_debug_calib_bitmap_cfg(struct nl80211_state *state, struct nl_cb *cb,
                  struct nl_msg *msg, int argc, char **argv)
{
    struct nlattr *key;
    struct CC33xx_calib_bitmap_cfg prms;

    if (argc < 1 || argc > 1)
        return 1;

    prms.calib_bitmap = atoi(argv[0]);

    if(prms.calib_bitmap > 0x3F)
    {
        fprintf(stderr, "invalid calib bitmap range (0 - 63 )- your input: %x\n", prms.calib_bitmap);
        return 1;
    }

    key = nla_nest_start(msg, NL80211_ATTR_TESTDATA);
    if (!key) {
        fprintf(stderr, "fail to nla_nest_start()\n");
        return 1;
    }
    
    NLA_PUT_U32(msg, WL1271_TM_ATTR_CMD_ID, WL1271_TM_CMD_CONFIGURE);
    NLA_PUT_U32(msg, WL1271_TM_ATTR_IE_ID, CALIB_BITMAP_CFG);
    NLA_PUT(msg, WL1271_TM_ATTR_DATA, sizeof(prms), &prms);

    nla_nest_end(msg, key);

    return 0;

nla_put_failure:
    fprintf(stderr, "%s> building message failed\n", __func__);
    return 2;
}

COMMAND(CC33xx_debug, calib_bitmap_cfg , "<bitmap (Range: 0 - 0x3F)> ",
    NL80211_CMD_TESTMODE, 0, CIB_NETDEV, CC33xx_debug_calib_bitmap_cfg,
    " debug configuration for partial modes\n");
