/* $Id: fat.h,v 1.1 2003/07/08 20:57:50 pefo Exp $ */

/*
 * Copyright (c) 2003 Opsycon AB (www.opsycon.se)
 * Copyright (c) 2003 Patrik Lindergren (www.lindergren.com)
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by Opsycon AB, Sweden.
 *	This product includes software developed by Patrik Lindergren.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 */
#ifndef __FATFS_H__
#define __FATFS_H__

/*
 * Contants
 */
#define SECTORSIZE 512

/*
 * Partition types
 */
#define TYPE_FAT12 1
#define TYPE_FAT16 2
#define TYPE_FAT32 3

struct ext_fat16 {
	uint8_t	bsDrvNum;	/* */
	uint8_t	bsReserved1;	/* */
	uint8_t	bsBootSig;	/* */
	uint8_t	bsVolLab[11];	/* Volume label */
	uint8_t	bsFilSysType[8]; /* */
} __attribute__((__packed__));

#define BPB_SIG_VALUE1	0x28
#define BPB_SIG_VALUE2	0x29

struct ext_fat32 {
	uint32_t	bpbFATSz32;	/* like bpbFATsecs for FAT32 */
	uint16_t	bpbExtFlags;	/* extended flags: */
	uint16_t	bpbFSVer;	/* */
	uint32_t	bpbRootClus;	/* */
	uint16_t	bpbFSInfo;	/* */
	uint16_t	bpbBkBootSec;	/* */
	uint8_t	bpbReserved;
	uint8_t	bpbDrvNum;
	uint8_t	bpbReserved1;
	uint8_t	bsBootSig;
	uint32_t	bsVolID;
	uint8_t	bsVolLab[11];
	uint8_t	bsFilSysType[8];
} __attribute__((__packed__));

struct bpb_t {
	uint8_t	bsJump[3];		/* jump inst E9xxxx or EBxx90 */
	int8_t		bsOemName[8];		/* OEM name and version */
	uint16_t	bpbBytesPerSec;	/* bytes per sector */
	uint8_t	bpbSecPerClust;	/* sectors per cluster */
	uint16_t	bpbResSectors;	/* number of reserved sectors */
	uint8_t	bpbFATs;	/* number of FATs */
	uint16_t	bpbRootDirEnts;	/* number of root directory entries */
	uint16_t	bpbSectors;	/* total number of sectors */
	uint8_t	bpbMedia;	/* media descriptor */
	uint16_t	bpbFATsecs;	/* number of sectors per FAT */
	uint16_t	bpbSecPerTrack;	/* sectors per track */
	uint16_t	bpbHeads;	/* number of heads */
	uint32_t	bpbHiddenSecs;	/* # of hidden sectors */
	uint32_t	bpbHugeSectors;	/* # of sectors if bpbSectors == 0 */
#if 0
	union {
		struct ext_fat16 efat16;
		struct ext_fat32 efat32;
	};
#else
	struct ext_fat32 efat32;
#endif
} __attribute__((__packed__));

struct partition_t {
	uint8_t bootid;   /* bootable?  0=no, 128=yes  */
	uint8_t beghead;  /* beginning head number */
	uint8_t begsect;  /* beginning sector number */
	uint8_t begcyl;   /* 10 bit nmbr, with high 2 bits put in begsect */	
	uint8_t systid;   /* Operating System type indicator code */
	uint8_t endhead;  /* ending head number */
	uint8_t endsect;  /* ending sector number */
	uint8_t endcyl;   /* also a 10 bit nmbr, with same high 2 bit trick */
	uint32_t relsect;            /* first sector relative to start of disk */
	uint32_t numsect;            /* number of sectors in partition */
} __attribute__((__packed__));

#define PART_SIZE 4
#define PART_BOOTID_ACTIVE 0x80
#define PART_TYPE_FAT12 0x01
#define PART_TYPE_FAT16 0x04
#define PART_TYPE_FAT16BIG 0x06
#define PART_TYPE_FAT32 0x0b


struct mbr_t {
	uint8_t	bootinst[446];
	struct partition_t partition[4];
	uint16_t signature;
} __attribute__((__packed__));

struct fatchain {
	int start;
	uint32_t *entries;
	int count;
};

struct fat_fileentry {
	int8_t		longName[256];
	int8_t		shortName[14];
	uint16_t	HighClust;	/* high byte of cluster number */
	uint16_t	StartCluster; /* starting cluster of file */
	uint32_t	FileSize;	/* size of file in bytes */
	struct fatchain Chain;
};

/*
 * FAT structure
 */
struct fat_sc {
	int fd;
	uint8_t	FatType;	/* FAT type (12, 16, 32) */
	uint16_t	BytesPerSec;	/* bytes per sector */
	uint8_t	SecPerClust;	/* sectors per cluster */
	uint16_t	ResSectors;	/* number of reserved sectors */
	uint8_t	NumFATs;	/* number of FATs */
	uint16_t	RootDirEnts;	/* number of root directory entries */
	uint32_t	TotalSectors;	/* total number of sectors */
	uint32_t	FATsecs;	/* number of sectors per FAT */
	uint16_t	SecPerTrack;	/* sectors per track */
	uint32_t	HiddenSecs;	/* # of hidden sectors */
	uint32_t	RootDirSectors;
	uint32_t	CountOfClusters;
	uint32_t	ClusterSize;
	uint32_t	FirstRootDirSecNum;
	uint32_t	DataSectors;
	uint32_t	DataSectorBase;
	uint32_t	PartitionStart;
	uint8_t	FatBuffer[SECTORSIZE];
	int		FatCacheNum;
	uint8_t	DirBuffer[SECTORSIZE];
	int		DirCacheNum;
	struct fat_fileentry file;
	int		LastSector;	/* Sector in last sector buffer */
	uint8_t	LastSectorBuffer[SECTORSIZE];
};

/*
 * Some useful cluster numbers.
 */
#define	MSDOSFSROOT	0		/* cluster 0 means the root dir */
#define	CLUST_FREE	0		/* cluster 0 also means a free cluster */
#define	MSDOSFSFREE	CLUST_FREE
#define	CLUST_FIRST	2		/* first legal cluster number */
#define	CLUST_RSRVD	0xfffffff6	/* reserved cluster range */
#define	CLUST_BAD	0xfffffff7	/* a cluster with a defect */
#define	CLUST_EOFS	0xfffffff8	/* start of eof cluster range */
#define	CLUST_EOFE	0xffffffff	/* end of eof cluster range */

#define	FAT12_MASK	0x00000ff8	/* mask for 12 bit cluster numbers */
#define	FAT16_MASK	0x0000fff8	/* mask for 16 bit cluster numbers */
#define	FAT32_MASK	0x0ffffff8	/* mask for FAT32 cluster numbers */
 
/*
 * Structure of a dos directory entry.
 */
struct direntry {
	uint8_t	dirName[8];	/* filename, blank filled */
#define	SLOT_EMPTY	0x00		/* slot has never been used */
#define	SLOT_E5		0x05		/* the real value is 0xe5 */
#define	SLOT_DELETED	0xe5		/* file in this slot deleted */
	uint8_t	dirExtension[3];	/* extension, blank filled */
	uint8_t	dirAttributes;	/* file attributes */
#define	ATTR_NORMAL	0x00		/* normal file */
#define	ATTR_READONLY	0x01		/* file is readonly */
#define	ATTR_HIDDEN	0x02		/* file is hidden */
#define	ATTR_SYSTEM	0x04		/* file is a system file */
#define	ATTR_VOLUME	0x08		/* entry is a volume label */
#define	ATTR_DIRECTORY	0x10		/* entry is a directory name */
#define	ATTR_ARCHIVE	0x20		/* file is new or modified */
	uint8_t	dirLowerCase;	/* case for base and extension */
#define	CASE_LOWER_BASE	0x08		/* base is lower case */
#define	CASE_LOWER_EXT	0x10		/* extension is lower case */
	uint8_t	dirCTimeHundredth; /* create time, 1/100th of a sec */
	uint16_t	dirCTime;	/* create time */
	uint16_t	dirCDate;	/* create date */
	uint16_t	dirADate;	/* access date */
	uint16_t	dirHighClust;	/* high byte of cluster number */
	uint16_t	dirMTime;	/* last update time */
	uint16_t	dirMDate;	/* last update date */
	uint16_t	dirStartCluster; /* starting cluster of file */
	uint32_t	dirFileSize;	/* size of file in bytes */
}  __attribute__((__packed__));

/*
 * Structure of a Win95 long name directory entry
 */
struct winentry {
	uint8_t	weCnt;
#define	WIN_LAST	0x40
#define	WIN_CNT		0x3f
	uint8_t	wePart1[10];
	uint8_t	weAttributes;
#define	ATTR_WIN95	0x0f
	uint8_t	weReserved1;
	uint8_t	weChksum;
	uint8_t	wePart2[12];
	uint16_t	weReserved2;
	uint8_t	wePart3[4];
} __attribute__((__packed__));
#define	WIN_CHARS	13	/* Number of chars per winentry */

/*
 * This is the format of the contents of the deTime field in the direntry
 * structure.
 * We don't use bitfields because we don't know how compilers for
 * arbitrary machines will lay them out.
 */
#define DT_2SECONDS_MASK	0x1F	/* seconds divided by 2 */
#define DT_2SECONDS_SHIFT	0
#define DT_MINUTES_MASK		0x7E0	/* minutes */
#define DT_MINUTES_SHIFT	5
#define DT_HOURS_MASK		0xF800	/* hours */
#define DT_HOURS_SHIFT		11

/*
 * This is the format of the contents of the deDate field in the direntry
 * structure.
 */
#define DD_DAY_MASK		0x1F	/* day of month */
#define DD_DAY_SHIFT		0
#define DD_MONTH_MASK		0x1E0	/* month */
#define DD_MONTH_SHIFT		5
#define DD_YEAR_MASK		0xFE00	/* year - 1980 */
#define DD_YEAR_SHIFT		9

#endif /* __FATFS_H__ */

