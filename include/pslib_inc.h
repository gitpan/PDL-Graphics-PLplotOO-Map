/*--------------------------------------------------------------------
 *    The GMT-system:	@(#)pslib_inc.h	2.65  11/10/99
 *
 *	Copyright (c) 1991-1999 by P. Wessel and W. H. F. Smith
 *	See COPYING file for copying and redistribution conditions.
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; version 2 of the License.
 *
 *	This program is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	Contact info: www.soest.hawaii.edu/gmt
 *--------------------------------------------------------------------*/
/*
 * This include file is only included in pslib.c.  Calling programs must
 * include file pslib.h
 *
 * Author:	Paul Wessel
 * Date:	15-NOV-1999
 * Version:	2.6
 */

#ifndef _PSLIB_INC_H
#define _PSLIB_INC_H

/*  PSL is POSIX COMPLIANT  */

#define _POSIX_SOURCE 1

/* Declaration modifiers for DLL support (MSC et al) */

#if defined(DLL_PSL)		/* define when library is a DLL */
#if defined(DLL_EXPORT)		/* define when building the library */
#define MSC_EXTRA_PSL __declspec(dllexport)
#else
#define MSC_EXTRA_PSL __declspec(dllimport)
#endif
#else
#define MSC_EXTRA_PSL
#endif				/* defined(DLL_PSL) */

#ifndef EXTERN_MSC
#define EXTERN_MSC extern MSC_EXTRA_PSL
#endif

/* So unless DLL_PSL is defined, EXTERN_MSC is simply extern */

/*--------------------------------------------------------------------
 *			SYSTEM HEADER FILES
 *--------------------------------------------------------------------*/

#include <ctype.h>
#include <float.h>
#include <math.h>
#include <stddef.h>
#ifdef __MACHTEN__
/* Kludge to fix a Macthen POSIX bug */
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "gmt_notunix.h"
#include "gmt_math.h"

#ifndef WIN32
#include <unistd.h>
#endif

/*--------------------------------------------------------------------
 *			PSL CONSTANTS MACRO DEFINITIONS
 *--------------------------------------------------------------------*/

#define Version (2.4)
#ifndef R2D
#define R2D (180.0/M_PI)
#endif
#define SMALL 1.0e-10
#define MAX_L1_PATH 1000 /* Max path length in Level 1 implementations */
#define I_255 (1.0 / 255.0)
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef M_PI
#define M_PI            3.14159265358979323846
#endif
#ifndef M_SQRT2
#define M_SQRT2         1.41421356237309504880
#endif
#define VNULL		((void *)NULL)
#define PAGE_HEIGHT_IN_PTS 842
#ifndef CNULL
#define CNULL (char *)NULL
#endif
/*--------------------------------------------------------------------
 *			PSL FUNCTION MACRO DEFINITIONS
 *--------------------------------------------------------------------*/

#ifndef MIN
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#endif
#ifndef MAX
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#endif
#ifndef irint
#define irint(x) ((int)rint(x))
#endif
#define YIQ(rgb) irint (0.299 * (rgb[0]) + 0.587 * (rgb[1]) + 0.114 * (rgb[2]))	/* How B/W TV's convert RGB to Gray */
#define iscolor(rgb) (rgb[0] != rgb[1] || rgb[1] != rgb[2])

#define N_FONTS	39	/* Current no of fonts;  To add more, modify the file
			 * PS_font_names.h and PS_font_heights.h and set N_FONTS */

typedef int BOOLEAN;		/* BOOLEAN used for logical variables */

struct EPS {    /* Holds info for eps files */
        int x0, x1, y0, y1;     /* Bounding box values in points */
	int portrait;		/* TRUE if start of plot was portrait */
        char *font[N_FONTS];    /* Pointers to font names used */
        int fontno[N_FONTS];    /* Array with font ids */
        char *name;             /* User name */
        char *title;            /* Plot title */
};

struct rasterfile {
        int     ras_magic;              /* magic number */
        int     ras_width;              /* width (pixels) of image */
        int     ras_height;             /* height (pixels) of image */
        int     ras_depth;              /* depth (1, 8, or 24 bits) of pixel */
        int     ras_length;             /* length (bytes) of image */
        int     ras_type;               /* type of file; see RT_* below */
        int     ras_maptype;            /* type of colormap; see RMT_* below */
        int     ras_maplength;          /* length (bytes) of following map */
        /* color map follows for ras_maplength bytes, followed by image */
};

#define	RAS_MAGIC	0x59a66a95	/* Magic number for Sun rasterfile */
#define RT_OLD		0	/* Old-style, unencoded Sun rasterfile */
#define RT_STANDARD	1	/* Standard, unencoded Sun rasterfile */
#define RT_BYTE_ENCODED	2	/* Run-length-encoded Sun rasterfile */
#define RT_FORMAT_RGB	3	/* [X]RGB instead of [X]BGR Sun rasterfile */
#define RMT_NONE	0       /* ras_maplength is expected to be 0 */
#define RMT_EQUAL_RGB	1       /* red[ras_maplength/3], green[], blue[] follow */

/*--------------------------------------------------------------------
 *			PSL PARAMETERS DEFINITIONS
 *--------------------------------------------------------------------*/

/* Global structure used internally by pslib */

struct {
	FILE *fp;		/* PS output file pointer. NULL = stdout	*/
	int bb[4];		/* Boundingbox arguments			*/
	int font_no;		/* Current font number				*/
	int linewidth;		/* Current pen thickness			*/
	int rgb[3];		/* Current paint				*/
	BOOLEAN landscape;	/* TRUE = Landscape, FALSE = Portrait			*/
	int ix, iy;		/* Absolute coordinates of last point		*/
	int npath;		/* Length of current unstroked path		*/
	char bw_format[8];	/* Format used for grayshade value		*/
	char rgb_format[24];	/* Same, for color triplets			*/
	double xscl, yscl;	/* Global scale values [1.0]			*/
	double xoff, yoff;	/* Origin offset [1/1]				*/
	double scale;		/* Must be set through plotinit();		*/
	int p_width;		/* Paper width in points, set in plotinit();	*/
	int p_height;		/* Paper height in points, set in plotinit();	*/
	double points_pr_unit;	/* # of points pr measure unit (e.g., 72/inch	*/
	int v1_path_length_limit;	/* Implementation limit on path lengths	in Level 1 PostScript	*/
	int max_path_length;	/* Maximum path length encountered during execution	*/
	int clip_path_length;	/* Current length of clip path			*/
	int hex_image;		/* TRUE writes images in hex, FALSE uses binary	*/
	int absolute;		/* TRUE will reset origin, FALSE means relative position	*/
	int eps_format;		/* TRUE makes EPS file, FALSE means PS file	*/
} ps;

char *PSHOME = (char *)NULL;	/* Pointer to path of directory with lib subdirectory */

	/* Font information */
	

double ps_font_height[N_FONTS] = {	/* Based on the size of A devided by fontsize */
#include "PS_font_heights.h"
};

char *ps_font_name[N_FONTS] = {
#include "PS_font_names.h"
};

	/* Pattern information */
	
#define N_PATTERNS 91	/* Current number of predefined patterns + 1, # 91 is user-supplied */

char ps_pattern_status[N_PATTERNS][2];
int ps_pattern_nx[N_PATTERNS][2];
int ps_pattern_ny[N_PATTERNS][2];

int ps_n_userimages;
struct USERIMAGE {
	char *name;
	int nx, ny;
} ps_user_image[N_PATTERNS];

int no_rgb[3] = {-1, -1, -1};

#endif	/* _PSLIB_INC_H */
