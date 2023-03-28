/*	$OpenBSD: cd.h,v 1.1 1999/07/20 06:21:59 csapuntz Exp $	*/
/*	$NetBSD: scsi_cd.h,v 1.6 1996/03/19 03:06:39 mycroft Exp $	*/

/*
 * Written by Julian Elischer (julian@tfs.com)
 * for TRW Financial Systems.
 *
 * TRW Financial Systems, in accordance with their agreement with Carnegie
 * Mellon University, makes this software available to CMU to distribute
 * or use in any manner that they see fit as long as this message is kept with 
 * the software. For this reason TFS also grants any other persons or
 * organisations permission to use or modify this software.
 *
 * TFS supplies this software to be publicly redistributed
 * on the understanding that TFS is not responsible for the correct
 * functioning of this software in any circumstances.
 *
 * Ported to run under 386BSD by Julian Elischer (julian@tfs.com) Sept 1992
 */
#ifndef	_SCSI_CD_H
#define _SCSI_CD_H 1

/*
 *	Define two bits always in the same place in byte 2 (flag byte)
 */
#define	CD_RELADDR	0x01
#define	CD_MSF		0x02

/*
 * SCSI command format
 */

struct scsi_read_capacity_cd {
	uint8_t opcode;
	uint8_t byte2;
	uint8_t addr[4];
	uint8_t unused[3];
	uint8_t control;
};

struct scsi_pause {
	uint8_t opcode;
	uint8_t byte2;
	uint8_t unused[6];
	uint8_t resume;
	uint8_t control;
};
#define	PA_PAUSE	1
#define PA_RESUME	0

struct scsi_play_msf {
	uint8_t opcode;
	uint8_t byte2;
	uint8_t unused;
	uint8_t start_m;
	uint8_t start_s;
	uint8_t start_f;
	uint8_t end_m;
	uint8_t end_s;
	uint8_t end_f;
	uint8_t control;
};

struct scsi_play_track {
	uint8_t opcode;
	uint8_t byte2;
	uint8_t unused[2];
	uint8_t start_track;
	uint8_t start_index;
	uint8_t unused1;
	uint8_t end_track;
	uint8_t end_index;
	uint8_t control;
};

struct scsi_play {
	uint8_t opcode;
	uint8_t byte2;
	uint8_t blk_addr[4];
	uint8_t unused;
	uint8_t xfer_len[2];
	uint8_t control;
};

struct scsi_play_big {
	uint8_t opcode;
	uint8_t byte2;	/* same as above */
	uint8_t blk_addr[4];
	uint8_t xfer_len[4];
	uint8_t unused;
	uint8_t control;
};

struct scsi_play_rel_big {
	uint8_t opcode;
	uint8_t byte2;	/* same as above */
	uint8_t blk_addr[4];
	uint8_t xfer_len[4];
	uint8_t track;
	uint8_t control;
};

struct scsi_read_header {
	uint8_t opcode;
	uint8_t byte2;
	uint8_t blk_addr[4];
	uint8_t unused;
	uint8_t data_len[2];
	uint8_t control;
};

struct scsi_read_subchannel {
	uint8_t opcode;
	uint8_t byte2;
	uint8_t byte3;
#define	SRS_SUBQ	0x40
	uint8_t subchan_format;
	uint8_t unused[2];
	uint8_t track;
	uint8_t data_len[2];
	uint8_t control;
};

struct scsi_read_toc {
	uint8_t opcode;
	uint8_t byte2;
	uint8_t unused[4];
	uint8_t from_track;
	uint8_t data_len[2];
	uint8_t control;
};
;

struct scsi_read_cd_capacity {
	uint8_t opcode;
	uint8_t byte2;
	uint8_t addr[4];
	uint8_t unused[3];
	uint8_t control;
};

/*
 * Opcodes
 */

#define READ_CD_CAPACITY	0x25	/* slightly different from disk */
#define READ_SUBCHANNEL		0x42	/* cdrom read Subchannel */
#define READ_TOC		0x43	/* cdrom read TOC */
#define READ_HEADER		0x44	/* cdrom read header */
#define PLAY			0x45	/* cdrom play  'play audio' mode */
#define PLAY_MSF		0x47	/* cdrom play Min,Sec,Frames mode */
#define PLAY_TRACK		0x48	/* cdrom play track/index mode */
#define PLAY_TRACK_REL		0x49	/* cdrom play track/index mode */
#define PAUSE			0x4b	/* cdrom pause in 'play audio' mode */
#define PLAY_BIG		0xa5	/* cdrom pause in 'play audio' mode */
#define PLAY_TRACK_REL_BIG	0xa9	/* cdrom play track/index mode */


struct scsi_read_cd_cap_data {
	uint8_t addr[4];
	uint8_t length[4];
};

struct cd_audio_page {
	uint8_t page_code;
#define	CD_PAGE_CODE	0x3F
#define	AUDIO_PAGE	0x0e
#define	CD_PAGE_PS	0x80
	uint8_t param_len;
	uint8_t flags;
#define		CD_PA_SOTC	0x02
#define		CD_PA_IMMED	0x04
	uint8_t unused[2];
	uint8_t format_lba;
#define		CD_PA_FORMAT_LBA	0x0F
#define		CD_PA_APR_VALID	0x80
	uint8_t lb_per_sec[2];
	struct	port_control {
		uint8_t channels;
#define	CHANNEL 0x0F
#define	CHANNEL_0 1
#define	CHANNEL_1 2
#define	CHANNEL_2 4
#define	CHANNEL_3 8
#define	LEFT_CHANNEL	CHANNEL_0
#define	RIGHT_CHANNEL	CHANNEL_1
#define MUTE_CHANNEL    0x0
#define BOTH_CHANNEL    LEFT_CHANNEL | RIGHT_CHANNEL
		uint8_t volume;
	} port[4];
#define	LEFT_PORT	0
#define	RIGHT_PORT	1
};

#ifdef CDDA
/*
 * There are 2352 bytes in a CD digital audio frame.  One frame is 1/75 of a
 * second, at 44.1kHz sample rate, 16 bits/sample, 2 channels.
 *
 * The frame data have the two channels interleaved, with the left
 * channel first.  Samples are little endian 16-bit signed values.
 */
#define CD_DA_BLKSIZ		2352	/* # bytes in CD-DA frame */
#ifndef CD_NORMAL_DENSITY_CODE
#define CD_NORMAL_DENSITY_CODE	0x00	/* from Toshiba CD-ROM specs */
#endif
#ifndef CD_DA_DENSITY_CODE
#define CD_DA_DENSITY_CODE	0x82	/* from Toshiba CD-ROM specs */
#endif
#endif /* CDDA */

#endif /*_SCSI_CD_H*/

