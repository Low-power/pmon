/*	$OpenBSD: atareg.h,v 1.2 1999/09/05 21:45:22 niklas Exp $	*/
/*	$NetBSD: atareg.h,v 1.5 1999/01/18 20:06:24 bouyer Exp $	*/

/*
 * Drive parameter structure for ATA/ATAPI.
 * Bit fields: WDC_* : common to ATA/ATAPI
 *             ATA_* : ATA only
 *             ATAPI_* : ATAPI only.
 */
struct ataparams {
    /* drive info */
    uint16_t	atap_config;		/* 0: general configuration */
#define WDC_CFG_ATAPI_MASK    		0xc000
#define WDC_CFG_ATAPI    		0x8000
#define	ATA_CFG_REMOVABLE		0x0080
#define	ATA_CFG_FIXED			0x0040
#define ATAPI_CFG_TYPE_MASK		0x1f00
#define ATAPI_CFG_TYPE(x)		(((x) & ATAPI_CFG_TYPE_MASK) >> 8)
#define ATAPI_CFG_TYPE_DIRECT		0x00
#define ATAPI_CFG_TYPE_SEQUENTIAL	0x01
#define ATAPI_CFG_TYPE_CDROM		0x05
#define ATAPI_CFG_TYPE_OPTICAL		0x07
#define ATAPI_CFG_TYPE_NODEVICE		0x1F
#define	ATAPI_CFG_REMOV			0x0080
#define ATAPI_CFG_DRQ_MASK		0x0060
#define ATAPI_CFG_STD_DRQ		0x0000
#define ATAPI_CFG_IRQ_DRQ		0x0020
#define ATAPI_CFG_ACCEL_DRQ		0x0040
#define ATAPI_CFG_CMD_MASK		0x0003
#define ATAPI_CFG_CMD_12		0x0000
#define ATAPI_CFG_CMD_16		0x0001
/* words 1-9 are ATA only */
    uint16_t	atap_cylinders;		/* 1: # of non-removable cylinders */
    uint16_t	__reserved1;
    uint16_t	atap_heads;		/* 3: # of heads */
    uint16_t	__retired1[2];		/* 4-5: # of unform. bytes/track */
    uint16_t	atap_sectors;		/* 6: # of sectors */
    uint16_t	__retired2[3];

    uint8_t	atap_serial[20];	/* 10-19: serial number */
    uint16_t	__retired3[2];
    uint16_t	__obsolete1;
    uint8_t	atap_revision[8];	/* 23-26: firmware revision */
    uint8_t	atap_model[40];		/* 27-46: model number */
    uint16_t	atap_multi;		/* 47: maximum sectors per irq (ATA) */
    uint16_t	__reserved2;
    uint16_t	atap_capabilities1;	/* 49: capability flags */
#define WDC_CAP_IORDY	0x0800
#define WDC_CAP_IORDY_DSBL 0x0400
#define	WDC_CAP_LBA	0x0200
#define	WDC_CAP_DMA	0x0100
#define ATA_CAP_STBY	0x2000
#define ATAPI_CAP_INTERL_DMA	0x8000
#define ATAPI_CAP_CMD_QUEUE	0x4000
#define	ATAPI_CAP_OVERLP	0x2000
#define ATAPI_CAP_ATA_RST	0x1000
    uint16_t	atap_capabilities2;	/* 50: capability flags (ATA) */
#if BYTE_ORDER == LITTLE_ENDIAN
    uint8_t	__junk2;
    uint8_t	atap_oldpiotiming;	/* 51: old PIO timing mode */
    uint8_t	__junk3;
    uint8_t	atap_olddmatiming;	/* 52: old DMA timing mode (ATA) */
#else
    uint8_t	atap_oldpiotiming;	/* 51: old PIO timing mode */
    uint8_t	__junk2;
    uint8_t	atap_olddmatiming;	/* 52: old DMA timing mode (ATA) */
    uint8_t	__junk3;
#endif
    uint16_t	atap_extensions;	/* 53: extentions supported */
#define WDC_EXT_UDMA_MODES	0x0004
#define WDC_EXT_MODES		0x0002
#define WDC_EXT_GEOM		0x0001
/* words 54-62 are ATA only */
    uint16_t	atap_curcylinders;	/* 54: current logical cyliners */
    uint16_t	atap_curheads;		/* 55: current logical heads */
    uint16_t	atap_cursectors;	/* 56: current logical sectors/tracks */
    uint16_t	atap_curcapacity[2];	/* 57-58: current capacity */
    uint16_t	atap_curmulti;		/* 59: current multi-sector setting */
#define WDC_MULTI_VALID 0x0100
#define WDC_MULTI_MASK  0x00ff
    uint16_t	atap_capacity[2];  	/* 60-61: total capacity (LBA only) */
    uint16_t	__retired4;
#if BYTE_ORDER == LITTLE_ENDIAN
    uint8_t	atap_dmamode_supp; 	/* 63: multiword DMA mode supported */
    uint8_t	atap_dmamode_act; 	/*     multiword DMA mode active */
    uint8_t	atap_piomode_supp;       /* 64: PIO mode supported */
    uint8_t	__junk4;
#else
    uint8_t	atap_dmamode_act; 	/*     multiword DMA mode active */
    uint8_t	atap_dmamode_supp; 	/* 63: multiword DMA mode supported */
    uint8_t	__junk4;
    uint8_t	atap_piomode_supp;       /* 64: PIO mode supported */
#endif
    uint16_t	atap_dmatiming_mimi;	/* 65: minimum DMA cycle time */
    uint16_t	atap_dmatiming_recom;	/* 66: recomended DMA cycle time */
    uint16_t	atap_piotiming;    	/* 67: mini PIO cycle time without FC */
    uint16_t	atap_piotiming_iordy;	/* 68: mini PIO cycle time with IORDY FC */
    uint16_t	__reserved3[2];
/* words 71-72 are ATAPI only */
    uint16_t	atap_pkt_br;		/* 71: time (ns) to bus release */
    uint16_t	atap_pkt_bsyclr;	/* 72: tme to clear BSY after service */
    uint16_t	__reserved4[2];	
    uint16_t	atap_queuedepth;   	/* 75: */
#define WDC_QUEUE_DEPTH_MASK 0x0F
    uint16_t	__reserved5[4];   	
    uint16_t	atap_ata_major;  	/* 80: Major version number */
#define	WDC_VER_ATA1	0x0002
#define	WDC_VER_ATA2	0x0004
#define	WDC_VER_ATA3	0x0008
#define	WDC_VER_ATA4	0x0010
#define	WDC_VER_ATA5	0x0020
    uint16_t   atap_ata_minor;  	/* 81: Minor version number */
    uint16_t	atap_cmd_set1;    	/* 82: command set suported */
#define WDC_CMD1_NOP	0x4000
#define WDC_CMD1_RB	0x2000
#define WDC_CMD1_WB	0x1000
#define WDC_CMD1_HPA	0x0400
#define WDC_CMD1_DVRST	0x0200
#define WDC_CMD1_SRV	0x0100
#define WDC_CMD1_RLSE	0x0080
#define WDC_CMD1_AHEAD	0x0040
#define WDC_CMD1_CACHE	0x0020
#define WDC_CMD1_PKT	0x0010
#define WDC_CMD1_PM	0x0008
#define WDC_CMD1_REMOV	0x0004
#define WDC_CMD1_SEC	0x0002
#define WDC_CMD1_SMART	0x0001
    uint16_t	atap_cmd_set2;    	/* 83: command set suported */
#define WDC_CMD2_RMSN	0x0010
#define WDC_CMD2_DM	0x0001
#define ATA_CMD2_APM	0x0008
#define ATA_CMD2_CFA	0x0004
#define ATA_CMD2_RWQ	0x0002
    uint16_t	atap_cmd_ext;		/* 84: command/features supp. ext. */
    uint16_t	atap_cmd1_en;		/* 85: cmd/features enabled */
/* bits are the same as atap_cmd_set1 */
    uint16_t	atap_cmd2_en;		/* 86: cmd/features enabled */
/* bits are the same as atap_cmd_set2 */
    uint16_t	atap_cmd_def;		/* 87: cmd/features default */
#if BYTE_ORDER == LITTLE_ENDIAN
    uint8_t	atap_udmamode_supp; 	/* 88: Ultra-DMA mode supported */
    uint8_t	atap_udmamode_act; 	/*     Ultra-DMA mode active */
#else
    uint8_t	atap_udmamode_act; 	/*     Ultra-DMA mode active */
    uint8_t	atap_udmamode_supp; 	/* 88: Ultra-DMA mode supported */
#endif
/* 89-92 are ATA-only */
    uint16_t	atap_seu_time;		/* 89: Sec. Erase Unit compl. time */
    uint16_t	atap_eseu_time;		/* 90: Enhanced SEU compl. time */
    uint16_t	atap_apm_val;		/* 91: current APM value */
    uint16_t	__reserved6[35];	/* 92-126: reserved */
    uint16_t	atap_rmsn_supp;		/* 127: remov. media status notif. */
#define WDC_RMSN_SUPP_MASK 0x0003
#define WDC_RMSN_SUPP 0x0001
    uint16_t	atap_sec_st;		/* 128: security status */
#define WDC_SEC_LEV_MAX	0x0100
#define WDC_SEC_ESE_SUPP 0x0020
#define WDC_SEC_EXP	0x0010
#define WDC_SEC_FROZEN	0x0008
#define WDC_SEC_LOCKED	0x0004
#define WDC_SEC_EN	0x0002
#define WDC_SEC_SUPP	0x0001
};
