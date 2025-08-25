#ifndef __PLT_H
#define __PLT_H

#include <linux/ethtool.h>

enum wl1271_tm_commands {
	WL1271_TM_CMD_UNSPEC,
	WL1271_TM_CMD_TEST,
	WL1271_TM_CMD_INTERROGATE,
	WL1271_TM_CMD_CONFIGURE,
	WL1271_TM_CMD_NVS_PUSH,
	WL1271_TM_CMD_SET_PLT_MODE,
	WL1271_TM_CMD_RECOVER,
	WL1271_TM_CMD_GET_MAC,

	__WL1271_TM_CMD_AFTER_LAST
};

enum wl1271_tm_attrs {
	WL1271_TM_ATTR_UNSPEC,
	WL1271_TM_ATTR_CMD_ID,
	WL1271_TM_ATTR_ANSWER,
	WL1271_TM_ATTR_DATA,
	WL1271_TM_ATTR_IE_ID,
	WL1271_TM_ATTR_PLT_MODE,
	__WL1271_TM_ATTR_AFTER_LAST
};

#define WL1271_TM_ATTR_MAX (__WL1271_TM_ATTR_AFTER_LAST - 1)

enum plt_mode {
	PLT_OFF = 0,
	PLT_ON = 1,
	PLT_FEM_DETECT = 2,
	PLT_CHIP_AWAKE = 3,
};
struct NAB_header{
	__le32 sync_pattern;
	__le16 opcode;
	__le16 len;
};

struct wl1271_cmd_header {
	
	struct NAB_header NAB_header;
	__u16 id;
	__u16 status;
	/* payload */
	unsigned char data[0];
} __attribute__((packed));

struct debug_header {
    struct wl1271_cmd_header cmd;

    /* acx (or information element) header */
    __u16 id;

    /* payload length (not including headers */
    __u16 len;
} __attribute__((packed));
struct cfg_header {
    struct wl1271_cmd_header cmd;

    /* acx (or information element) header */
    __u16 id;

    /* payload length (not including headers */
    __u16 len;
} __attribute__((packed));

struct wl1271_cmd_test_header {
	unsigned char id;
	unsigned char padding[3];
} __attribute__((packed));


#define MAC_ADDR_LEN  6


int get_mac_addr(int ifc_num, unsigned char *mac_addr);

int file_exist(const char *filename);
int do_get_drv_info(char *dev_name, int *hw_ver,
		    struct ethtool_drvinfo *out_drvinfo);


#endif /* __PLT_H */
