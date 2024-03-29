/*--------------------------------------------------------------------
 *    The GMT-system:	@(#)gmt_init.c	2.185  12/03/99
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
 *--------------------------------------------------------------------
 *
 * gmt_init.c contains code which is used by all GMT programs
 *
 * Author:	Paul Wessel
 * Date:	20-OCT-1999
 * Version:	3.3.3
 *
 *
 * The PUBLIC functions are:
 *
 *	GMT_explain_option ()	:	Prints explanations for the common options
 *	GMT_get_common_args ()	:	Interprets -B -H -J -K -O -P -R -U -V -X -Y -: -c
 *	GMT_getdefaults ()	:	Initializes the GMT global parameters
 *	GMT_free_plot_array	:	Free plot memory
 *	GMT_key_lookup ()	:	Linear Key - id lookup function
 *	GMT_savedefaults ()	:	Writes the GMT global parameters to .gmtdefaults
 *	GMT_hash_init ()	: 	Initializes a hash
 *	GMT_hash_lookup ()	:	Key - id lookup using hashing
 *	GMT_hash ()		:	Key - id lookup using hashing
 *	GMT_begin ()		:	Gets history and init parameters
 *	GMT_end ()		:	Cleans up and exits
 *	GMT_get_history ()	:	Read the .gmtcommand file
 *	GMT_putpen		:	Encode pen argument into textstring
 *	GMT_put_history ()	:	Writes updates to the .gmtcommand file
 *	GMT_map_getproject ()	:	Scans the -Jstring to set projection
 *
 * The INTERNAL functions are:
 *
 *	GMT_loaddefaults ()	:	Reads the GMT global parameters from .gmtdefaults
 *	GMT_map_getframe ()	:	Scans the -Bstring to set tickinfo
 *	GMT_setparameter ()	:	Sets a default value given keyord,value-pair
 *	GMT_setshorthand ()	:	Reads and initializes the suffix shorthands
 *	GMT_get_ellipse()	:	Returns ellipse id based on name
 *	GMT_prepare_3D ()	:	Initialize 3-D parameters
 */
 
#include "gmt.h"
#include "gmt_init.h"

#define USER_MEDIA_OFFSET 1000

int GMT_setparameter(char *keyword, char *value);
int GMT_get_ellipse(char *name);
int GMT_load_user_media (void);
void GMT_set_home (void);
BOOLEAN true_false_or_error (char *value, int *answer);
void str_tolower (char *value);
void GMT_get_history(int argc, char **argv);
void GMT_prepare_3D(void);
void GMT_free_plot_array(void);
int GMT_map_getframe(char *args);
char *GMT_putpen (struct GMT_PEN *pen);
int GMT_check_region (double w, double e, double s, double n);
char *GMT_getdefpath (int get);

/* Local variables to gmt_init.c */

struct GMT_HASH hashnode[HASH_SIZE];
BOOLEAN GMT_x_abs = FALSE, GMT_y_abs = FALSE;
BOOLEAN GMT_got_frame_rgb;
FILE *GMT_fp_history;	/* For .gmtcommands file */

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
	
void GMT_explain_option (char option)
{

	/* The function print to stderr a short explanation for the option indicated by
	 * the variable <option>.  Only the common parameter options are covered
	 */
	 
	char *GMT_choice[2] = {"OFF", "ON"};

	switch (option) {
	
		case 'B':	/* Tickmark option */
		
			fprintf (stderr, "	-B specifies Boundary info.  <tickinfo> is a textstring made up of one or\n");
			fprintf (stderr, "	   more substrings of the form [t]<tick>[m|c].  The optional [t] can be\n");
			fprintf (stderr, "	   either: a for anotation interval, f for frame interval, or g for\n");
			fprintf (stderr, "	   gridline interval.  If the [t] is not given, a<tick> AND f<tick>\n");
			fprintf (stderr, "	   (but no g<tick>) are set.  The frame interval = anotation interval\n");
			fprintf (stderr, "	   if no separate f<tick> is given.  <tick> is the desired tick-interval.\n");
			fprintf (stderr, "	   The optional [m|s] indicates minutes or seconds.  To specify separate x and y tick-.\n");
			fprintf (stderr, "	   info, separate the strings with a slash [/].  E.g., 5 degree ticks for\n");
			fprintf (stderr, "	   frame AND anotation, 30 minutes grid lines, use\n");
			fprintf (stderr, "	        -B5g30m.   For different y ticks try -B5g30m/2g15m\n");
			fprintf (stderr, "	   [If -Jz is selected, use slashes to separate x, y, and z-tickinfo.]\n");
			fprintf (stderr, "	   Add labels by surrounding them with colons.  If first character is a\n");
			fprintf (stderr, "	   period, then the text is used as the plot title (e.g. :.Plot_Title:).\n");
			fprintf (stderr, "	   If it is a comma, then the text is used as unit anotation (e.g. :,%%:).\n");
			fprintf (stderr, "	   If unit starts with - there will be no space between unit and anotation\n");
			fprintf (stderr, "	   Append any combination of w, e, s, n to plot those axes\n");
			fprintf (stderr, "	   only [Default is all].  Append l to anotate log10 (value), p for\n");
			fprintf (stderr, "	   10^(log10(value)).  (See -J for log10 scaling).  For -Jx with power\n");
			fprintf (stderr, "	   scaling, append p to annotate value at equidistant pow increments\n");
			fprintf (stderr, "	   See psbasemap man pages for more details and examples.\n");
			break;
			
		case 'b':	/* Condensed tickmark option */
		
			fprintf (stderr, "	-B Boundary anotation, give -B<xinfo>[/<yinfo>[/<zinfo>]][.:\"title\":][wesnzWESNZ+]\n");
			fprintf (stderr, "	   <?info> is 1-3 substring(s) of form [a|f|g]<tick>[m][l|p] and optionally :\"label\": and/or :,[-]\"unit\":\n");
			fprintf (stderr, "	   (See psbasemap man pages for more details and examples.)\n");
			break;
			
		case 'H':	/* Header */
		
			fprintf (stderr, "	-H means input/output file has %d Header record(s) [%s]\n",
				gmtdefs.n_header_recs, GMT_choice[gmtdefs.io_header]);
			fprintf (stderr, "	   Optionally, append number of header records\n");
			break;
			
		case 'J':	/* Map projection option */
		
			fprintf (stderr, "	-J Selects the map proJection system. (<mapwidth> is in %s)\n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Ja<lon0>/<lat0>/<scale> OR -JA<lon0>/<lat0>/<mapwidth> (Lambert Azimuthal Equal Area)\n");
			fprintf (stderr, "	     lon0/lat0 is the center or the projection.\n");
			fprintf (stderr, "	     Scale is either <1:xxxx> or <radius>/<lat>, where <radius> distance\n");
			fprintf (stderr, "	     is in %s to the oblique parallel <lat>.\n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Jb<lon0>/<lat0>/<lat1>/<lat2>/<scale> OR -JB<lon0>/<lat0>/<lat1>/<lat2>/<mapwidth> (Albers Equal-Area Conic)\n");
			fprintf (stderr, "	     Give origin, 2 standard parallels, and true scale in %s/degree\n",
				GMT_unit_names[gmtdefs.measure_unit]);
				
			fprintf (stderr, "	   -Jc<lon0>/<lat0><scale> OR -JC<lon0>/<lat0><mapwidth> (Cassini)\n");
			fprintf (stderr, "	     Give central point and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Jd<lon0>/<lat0>/<lat1>/<lat2>/<scale> OR -JD<lon0>/<lat0>/<lat1>/<lat2>/<mapwidth> (Equidistant Conic)\n");
			fprintf (stderr, "	     Give origin, 2 standard parallels, and true scale in %s/degree\n",
				GMT_unit_names[gmtdefs.measure_unit]);
				
			fprintf (stderr, "	   -Je<lon0>/<lat0>/<scale> OR -JE<lon0>/<lat0>/<mapwidth> (Azimuthal Equidistant)\n");
			fprintf (stderr, "	     lon0/lat0 is the center or the projection.\n");
			fprintf (stderr, "	     Scale is either <1:xxxx> or <radius>/<lat>, where <radius> is distance\n");
			fprintf (stderr, "	     in %s to the oblique parallel <lat>. \n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Jf<lon0>/<lat0>/<horizon>/<scale> OR -JF<lon0>/<lat0>/<horizon>/<mapwidth> (Gnomonic)\n");
			fprintf (stderr, "	     lon0/lat0 is the center or the projection.\n");
			fprintf (stderr, "	     horizon is max distance from center of the projection (< 90).\n");
			fprintf (stderr, "	     Scale is either <1:xxxx> or <radius>/<lat>, where <radius> is distance\n");
			fprintf (stderr, "	     in %s to the oblique parallel <lat>. \n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Jg<lon0>/<lat0>/<scale> OR -JG<lon0>/<lat0>/<mapwidth> (Orthographic)\n");
			fprintf (stderr, "	     lon0/lat0 is the center or the projection.\n");
			fprintf (stderr, "	     Scale is either <1:xxxx> or <radius>/<lat>, where <radius> is distance\n");
			fprintf (stderr, "	     in %s to the oblique parallel <lat>. \n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Jh<lon0>/<scale> OR -JH<lon0>/<mapwidth> (Hammer-Aitoff)\n");
			fprintf (stderr, "	     Give central meridian and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Ji<lon0>/<scale> OR -JI<lon0>/<mapwidth> (Sinusoidal)\n");
			fprintf (stderr, "	     Give central meridian and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);

			fprintf (stderr, "	   -Jj<lon0>/<scale> OR -JJ<lon0>/<mapwidth> (Miller projection)\n");
			fprintf (stderr, "	     Give central meridian and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);

			fprintf (stderr, "	   -Jk[f|s]<lon0>/<scale> OR -JK[f|s]<lon0>/<mapwidth> (Eckert IV (f) or VI (s))\n");
			fprintf (stderr, "	     Give central meridian and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);

			fprintf (stderr, "	   -Jl<lon0>/<lat0>/<lat1>/<lat2>/<scale> OR -JL<lon0>/<lat0>/<lat1>/<lat2>/<mapwidth> (Lambert Conformal Conic)\n");
			fprintf (stderr, "	     Give origin, 2 standard parallels,  and true scale in %s/degree\n",
				GMT_unit_names[gmtdefs.measure_unit]);
				
			fprintf (stderr, "	   -Jm | -JM (Mercator).  Specify one of two definitions:\n");
			fprintf (stderr, "	      -Jm<scale> OR -JM<mapwidth>\n");
			fprintf (stderr, "	       Give true scale at Equator in %s/degree\n",
				GMT_unit_names[gmtdefs.measure_unit]);
			fprintf (stderr, "	      -Jm<lon0>/<lat0>/<scale> OR -JM<lon0>/<lat0>/<mapwidth>\n");
			fprintf (stderr, "	       Give true scale at parallel lat0 in %s/degree\n",
				GMT_unit_names[gmtdefs.measure_unit]);
				
			fprintf (stderr, "	   -Jn<lon0>/<scale> OR -JN<lon0>/<mapwidth> (Robinson projection)\n");
			fprintf (stderr, "	     Give central meridian and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);

			fprintf (stderr, "	   -Jo | -JO (Oblique Mercator).  Specify one of three definitions:\n");
			fprintf (stderr, "	      -Joa<orig_lon>/<orig_lat>/<azimuth>/<scale> OR -JOa<orig_lon>/<orig_lat>/<azimuth>/<mapwidth>\n");
			fprintf (stderr, "	      		Give origin and azimuth of oblique equator\n");
			fprintf (stderr, "	      -Job<orig_lon>/<orig_lat>/<b_lon>/<b_lat>/<scale> OR -JOb<orig_lon>/<orig_lat>/<b_lon>/<b_lat>/<mapwidth>\n");
			fprintf (stderr, "	      		Give origin and second point on oblique equator\n");
			fprintf (stderr, "	      -Joc<orig_lon>/<orig_lat>/<pole_lon>/<pole_lat>/<scale> OR -JOc<orig_lon>/<orig_lat>/<pole_lon>/<pole_lat>/<mapwidth>\n");
			fprintf (stderr, "	      		Give origin and pole of projection\n");
			fprintf (stderr, "	        Scale is true scale at oblique equator in %s/degree\n",
				GMT_unit_names[gmtdefs.measure_unit]);
			fprintf (stderr, "	        Specify region in oblique degrees OR use -R<>r\n");
			
			fprintf (stderr, "	   -Jq<lon0>/<scale> OR -JQ<lon0>/<mapwidth> (Equidistant Cylindrical)\n");
			fprintf (stderr, "	     Give central meridian and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Jr<lon0>/<scale> OR -JR<lon0>/<mapwidth> (Winkel Tripel)\n");
			fprintf (stderr, "	     Give central meridian and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);

			fprintf (stderr, "	   -Js<lon0>/<lat0>/<scale> OR -JS<lon0>/<lat0>/<mapwidth> (Stereographic)\n");
			fprintf (stderr, "	     lon0/lat0 is the center or the projection.\n");
			fprintf (stderr, "	     Scale is either <1:xxxx> or <radius>/<lat>, where <radius> distance\n");
			fprintf (stderr, "	     is in %s to the oblique parallel <lat>.\n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Jt | -JT (Transverse Mercator).  Specify one of two definitions:\n");
			fprintf (stderr, "	      -Jt<lon0>/<scale> OR -JT<lon0>/<mapwidth>\n");
			fprintf (stderr, "	         Give central meridian and scale as 1:xxxx or %s/degree\n",
				GMT_unit_names[gmtdefs.measure_unit]);
			fprintf (stderr, "	      -Jt<lon0>/<lat0>/<scale> OR -JT<lon0>/<lat0>/<mapwidth>\n");
			fprintf (stderr, "	         Give lon/lat of origin, and scale as 1:xxxx or %s/degree\n",
				GMT_unit_names[gmtdefs.measure_unit]);
				
			fprintf (stderr, "	   -Ju<zone>/<scale> OR -JU<zone>/<mapwidth> (UTM)\n");
			fprintf (stderr, "	     Give zone (negative for S hemisphere) and scale as 1:xxxx or %s/degree\n",
				GMT_unit_names[gmtdefs.measure_unit]);
				
			fprintf (stderr, "	   -Jv<lon0>/<scale> OR -JV<lon0>/<mapwidth> (van der Grinten)\n");
			fprintf (stderr, "	     Give central meridian and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Jw<lon0>/<scale> OR -JW<lon0>/<mapwidth> (Mollweide)\n");
			fprintf (stderr, "	     Give central meridian and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Jy<lon0>/<lats>/<scale> OR -JY<lon0>/<lats>/<mapwidth> (Cylindrical Equal-area)\n");
			fprintf (stderr, "	     Give central meridian, standard parallel and scale as 1:xxxx or %s/degree\n", GMT_unit_names[gmtdefs.measure_unit]);
			fprintf (stderr, "	     <slat> = 45 (Peters), 37.4 (Trystan Edwards), 30 (Behrmann), 0 (Lambert)\n");

			fprintf (stderr, "	   -Jp<scale>[/<origin>] OR -JP<mapwidth>[/<origin>] (Polar (theta,radius))\n");
			fprintf (stderr, "	     Linear scaling for polar coordinates.  Give scale in %s/units\n",
				GMT_unit_names[gmtdefs.measure_unit]);
			fprintf (stderr, "	     Optionally, append theta value for origin [0]\n");
				
			fprintf (stderr, "	   -Jx OR -JX for non-map projections.  Scale in %s/units.  Specify one:\n",
				GMT_unit_names[gmtdefs.measure_unit]);
			fprintf (stderr, "	      -Jx<x-scale>		Linear projection\n");
			fprintf (stderr, "	      -Jx<x-scale>l		Log10 projection\n");
			fprintf (stderr, "	      -Jx<x-scale>p<power>	x^power projection\n");
			fprintf (stderr, "	      Use / to specify separate x/y scaling (e.g., -Jx0.5/0.3.)\n");
			fprintf (stderr, "	      If -JX is used then give axes lengths rather than scales\n");
			break;
			
		case 'j':	/* Condensed version of J */
		
			fprintf (stderr, "	-J Selects map proJection. (<scale> in %s/degree, <mapwidth> in %s)\n", GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
			
			fprintf (stderr, "	   -Ja|A<lon0>/<lat0>/<scale (or radius/lat)|mapwidth> (Lambert Azimuthal Equal Area)\n");
			
			fprintf (stderr, "	   -Jb|B<lon0>/<lat0>/<lat1>/<lat2>/<scale|mapwidth> (Albers Equal-Area Conic)\n");
			fprintf (stderr, "	   -Jc|C<lon0>/<lat0><scale|mapwidth> (Cassini)\n");
			
			fprintf (stderr, "	   -Jd|D<lon0>/<lat0>/<lat1>/<lat2>/<scale|mapwidth> (Equidistant Conic)\n");

			fprintf (stderr, "	   -Je|E<lon0>/<lat0>/<scale (or radius/lat)|mapwidth>  (Azimuthal Equidistant)\n");
			
			fprintf (stderr, "	   -Jf|F<lon0>/<lat0>/<horizon>/<scale (or radius/lat)|mapwidth>  (Gnomonic)\n");

			fprintf (stderr, "	   -Jg|G<lon0>/<lat0>/<scale (or radius/lat)|mapwidth>  (Orthographic)\n");
			
			fprintf (stderr, "	   -Jh|H<lon0>/<scale|mapwidth> (Hammer-Aitoff)\n");
			
			fprintf (stderr, "	   -Ji|I<lon0>/<scale|mapwidth> (Sinusoidal)\n");

			fprintf (stderr, "	   -Jj|J<lon0>/<scale|mapwidth> (Miller)\n");

			fprintf (stderr, "	   -Jk|K[f|s]<lon0>/<scale/mapwidth> (Eckert IV (f) or VI (s))\n");


			fprintf (stderr, "	   -Jl|L<lon0>/<lat0>/<lat1>/<lat2>/<scale|mapwidth> (Lambert Conformal Conic)\n");
				
			fprintf (stderr, "	   -Jm|M (Mercator).  Specify one of two definitions:\n");
			fprintf (stderr, "	      -Jm|M<scale|mapwidth>\n");
			fprintf (stderr, "	      -Jm|M<lon0>/<lat0>/<scale|mapwidth>\n");
				
			fprintf (stderr, "	   -Jn|N<lon0>/<scale|mapwidth> (Robinson projection)\n");

			fprintf (stderr, "	   -Jo|O (Oblique Mercator).  Specify one of three definitions:\n");
			fprintf (stderr, "	      -Jo|Oa<orig_lon>/<orig_lat>/<azimuth>/<scale|mapwidth>\n");
			fprintf (stderr, "	      -Jo|Ob<orig_lon>/<orig_lat>/<b_lon>/<b_lat>/<scale|mapwidth>\n");
			fprintf (stderr, "	      -Jo|Oc<orig_lon>/<orig_lat>/<pole_lon>/<pole_lat>/<scale|mapwidth>\n");
			
			fprintf (stderr, "	   -Jq|Q<lon0>/<scale|mapwidth> (Equidistant Cylindrical)\n");
			
			fprintf (stderr, "	   -Jr|R<lon0>/<scale|mapwidth> (Winkel Tripel)\n");

			fprintf (stderr, "	   -Js|S<lon0>/<lat0>/<scale (or radius/lat)|mapwidth> (Stereographic)\n");
			
			fprintf (stderr, "	   -Jt|T (Transverse Mercator).  Specify one of two definitions:\n");
			fprintf (stderr, "	      -Jt|T<lon0>/<scale|mapwidth>\n");
			fprintf (stderr, "	      -Jt|T<lon0>/<lat0>/<scale|mapwidth>\n");
				
			fprintf (stderr, "	   -Ju|U<zone>/<scale|mapwidth> (UTM)\n");
				
			fprintf (stderr, "	   -Jv|V<lon0>/<scale/mapwidth> (van der Grinten)\n");

			fprintf (stderr, "	   -Jw|W<lon0>/<scale|mapwidth> (Mollweide)\n");
			
			fprintf (stderr, "	   -Jy|Y<lon0>/<lats>/<scale|mapwidth> (Cylindrical Equal-area)\n");

			fprintf (stderr, "	   -Jp|P<scale|mapwidth>[/<origin>] (Polar (theta,radius))\n");
				
			fprintf (stderr, "	   -Jx|X<x-scale|mapwidth>[l|p<power>][/<y-scale|mapheight>[l|p<power>]] (Linear projections)\n");
			fprintf (stderr, "	   (See psbasemap for more details on projection syntax)\n");
			break;
			
		case 'K':	/* Append-more-PostScript-later */

			fprintf (stderr, "	-K means allow for more plot code to be appended later [%s].\n",
				GMT_choice[!gmtdefs.last_page]);
			break;
			
		case 'M':	/* Multisegment option */

			fprintf (stderr, "	-M Input file(s) contain multiple segments separated by a record\n");
			fprintf (stderr, "	   whose first character is <flag> [%c]\n", GMT_io.EOF_flag);
			break;
			
		case 'O':	/* Overlay plot */

			fprintf (stderr, "	-O means Overlay plot mode [%s].\n",
				GMT_choice[gmtdefs.overlay]);
			break;
			
		case 'P':	/* Portrait or landscape */
		
			fprintf (stderr, "	-P means Portrait page orientation [%s].\n",
				GMT_choice[(gmtdefs.page_orientation & 1)]);
			break;
			
		case 'R':	/* Region option */
		
			fprintf (stderr, "	-R specifies the min/max coordinates of data region in user units.\n");
			fprintf (stderr, "	   Use dd:mm[:ss] format for regions given in degrees and minutes [and seconds].\n");
			fprintf (stderr, "	   Append r if -R specifies the longitudes/latitudes of the lower left\n");
			fprintf (stderr, "	   and upper right corners of a rectangular area\n");
			break;
			
		case 'r':	/* Region option for 3-D */
		
			fprintf (stderr, "	-R specifies the xyz min/max coordinates of the plot window in user units.\n");
			fprintf (stderr, "	   Use dd:mm[:ss] format for regions given in degrees and minutes [and seconds].\n");
			fprintf (stderr, "	   Append r if first 4 arguments to -R specifies the longitudes/latitudes\n");
			fprintf (stderr, "	   of the lower left and upper right corners of a rectangular area\n");
			break;
			
		case 'U':	/* Plot time mark and [optionally] command line */
		
			fprintf (stderr, "	-U to plot Unix System Time stamp [and optionally appended text].\n");
			fprintf (stderr, "	   You may also set the lower left corner position of stamp [%lg/%lg].\n",
				gmtdefs.unix_time_pos[0], gmtdefs.unix_time_pos[1]);
			fprintf (stderr, "	   Give -Uc to have the command line plotted [%s].\n",
				GMT_choice[gmtdefs.unix_time]);
			break;
			
		case 'V':	/* Verbose */
		
			fprintf (stderr, "	-V Run in verbose mode [%s].\n", GMT_choice[gmtdefs.verbose]);
			break;
			
		case 'X':
		case 'Y':	/* Reset plot origin option */
		
			fprintf (stderr, "	-X -Y to shift origin of plot to (<xshift>, <yshift>) [a%lg,a%lg].\n",
				gmtdefs.x_origin, gmtdefs.y_origin);
			fprintf (stderr, "	   Prepend a for absolute [Default r is relative]\n");
			fprintf (stderr, "	   (Note that for overlays (-O), the default is [r0,r0].)\n");
			break;
			
		case 'Z':	/* Vertical scaling for 3-D plots */
		
			fprintf (stderr, "	   -Jz for z component of 3-D projections.  Same syntax as -Jx.\n");
			break;
			
		case 'c':	/* Set number of plot copies option */
		
			fprintf (stderr, "	-c specifies the number of copies [%d].\n", gmtdefs.n_copies);
			break;

		case 'i':	/* -b binary option with input only */
		
			fprintf (stderr, "\t-bi for binary input.  Append s for single precision [Default is double]\n");
			break;

		case 'n':	/* -bi addendum when input format is unknown */
		
			fprintf (stderr, "\t    Append <n> for the number of columns in binary file(s).\n");
			break;

		case 'o':	/* -b binary option with output only */
		
			fprintf (stderr, "\t-bo for binary output. Append s for single precision [Default is double]\n");
			break;

		case ':':	/* lon/lat or lat/lon */

			fprintf (stderr, "	-: Expect lat/lon input rather than lon/lat [%s].\n",
				GMT_choice[gmtdefs.xy_toggle]);
			break;
			
		case '.':	/* Trailer message */
		
			fprintf (stderr, "	(See gmtdefaults man page for hidden GMT default parameters)\n");
			break;

		default:
			break;
	}
}

void GMT_fill_syntax (char option)
{
	fprintf (stderr, "%s: GMT SYNTAX ERROR -%c option.  Correct syntax:\n", GMT_program, option);
	fprintf (stderr, "\t-%cP|p<dpi>/<pattern>[:F<rgb>B<rgb>], dpi of pattern, pattern from 1-90 or a filename, optionally add fore/background colors (use - for transparency)\n", option);
	fprintf (stderr, "\t-%c<red>/<green>/<blue> or -%c<gray>, all in the 0-255 range\n", option, option);
}

void GMT_pen_syntax (char option)
{
	fprintf (stderr, "%s: GMT SYNTAX ERROR -%c option.  Correct syntax:\n", GMT_program, option);
	fprintf (stderr, "\t-%c[<width>][/<red>/<green>/<blue> | <gray>][to | ta | t<texture>:<offset>][p]\n", option);
	fprintf (stderr, "\t  <width> >= 0, <red>/<green>/<blue> or <gray> all in the 0-255 range\n");
}

void GMT_rgb_syntax (char option)
{
	fprintf (stderr, "%s: GMT SYNTAX ERROR -%c option.  Correct syntax:\n", GMT_program, option);
	fprintf (stderr, "\t-%c<red>/<green>/<blue> or -%c<gray>, all in the 0-255 range\n", option, option);
}

void GMT_syntax (char option)
{
	/* The function print to stderr the syntax for the option indicated by
	 * the variable <option>.  Only the common parameter options are covered
	 */
	 
	fprintf (stderr, "%s: GMT SYNTAX ERROR -%c option.  Correct syntax:\n", GMT_program, option);

	switch (option) {
	
		case 'B':	/* Tickmark option */
		
			fprintf (stderr, "\t-B[a|f|g]<tick>[m][l|p][:\"label\":][:,\"unit\":[/.../...]:.\"Title\":[W|w|E|e|S|s|N|n][Z|z]\n");
			break;
			
		case 'H':	/* Header */
		
			fprintf (stderr, "\t-H[n-header-records]\n");
			break;
			
		case 'J':	/* Map projection option */
		
			switch (project_info.projection) {
				case LAMB_AZ_EQ:
					fprintf (stderr, "\t-Ja<lon0>/<lat0>/<scale> OR -Ja<lon0>/<lat0>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or <radius> (in %s)/<lat>, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case ALBERS:
					fprintf (stderr, "\t-Jb<lon0>/<lat0>/<lat1>/<lat2>/<scale> OR -Jb<lon0>/<lat0>/<lat1>/<lat2>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case ECONIC:
					fprintf (stderr, "\t-Jd<lon0>/<lat0>/<lat1>/<lat2>/<scale> OR -JD<lon0>/<lat0>/<lat1>/<lat2>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case CASSINI:
					fprintf (stderr, "\t-Jc<lon0>/<lat0><scale> OR -JC<lon0>/<lat0><mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree ,or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case GNOMONIC:
					fprintf (stderr, "\t-Jf<lon0>/<lat0>/<horizon>/<scale> OR -JF<lon0>/<lat0>/<horizon>/<mapwidth>\n");
					fprintf (stderr, "\t   <horizon> is distance from center to perimeter (< 90)\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or <radius> (in %s)/<lat>, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case ORTHO:
					fprintf (stderr, "\t-Jg<lon0>/<lat0>/<scale> OR -JG<lon0>/<lat0>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or <radius> (in %s)/<lat>, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case HAMMER:
					fprintf (stderr, "\t-Jh<lon0>/<scale> OR -JH<lon0>/<mapwidth\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case SINUSOIDAL:
					fprintf (stderr, "\t-Ji<lon0>/<scale> OR -JI<lon0>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case LAMBERT:
					fprintf (stderr, "\t-Jl<lon0>/<lat0>/<lat1>/<lat2>/<scale> OR -JL<lon0>/<lat0>/<lat1>/<lat2>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case MERCATOR:
					fprintf (stderr, "\t-Jm<scale> OR -JM<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case ROBINSON:
					fprintf (stderr, "\t-Jn<lon0>/<scale> OR -JN<lon0>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case OBLIQUE_MERC:
					fprintf (stderr, "\t-JOa<lon0>/<lat0>/<azimuth>/<scale> OR -JOa<lon0>/<lat0>/<azimuth>/<mapwidth>\n");
					fprintf (stderr, "\t-Job<lon0>/<lat0>/<b_lon>/<b_lat>/<scale> OR -JOb<lon0>/<lat0>/<b_lon>/<b_lat>/<mapwidth>\n");
					fprintf (stderr, "\t-Joc<lon0>/<lat0>/<lonp>/<latp>/<scale> OR -JOc<lon0>/<lat0>/<lonp>/<latp>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/oblique degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case WINKEL:
					fprintf (stderr, "\t-Jr<lon0>/<scale> OR -JR<lon0><mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case CYL_EQDIST:
					fprintf (stderr, "\t-Jq<lon0>/<scale> OR -JQ<lon0><mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case MILLER:
					fprintf (stderr, "\t-Jj<lon0>/<scale> OR -JJ<lon0><mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case STEREO:
					fprintf (stderr, "\t-Js<lon0>/<lat0>/<scale> OR -JS<lon0>/<lat0>/<mapwidth>\n"); 
					fprintf (stderr, "\t  <scale is <1:xxxx> or <radius> (in %s)/<lat>, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case TM:
					fprintf (stderr, "\t-Jt<lon0>/<scale> OR -JT<lon0>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case UTM:
					fprintf (stderr, "\t-Ju<zone>/<scale> OR -JU<zone>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case GRINTEN:
					fprintf (stderr, "\t-Jv<lon0>/<scale> OR -JV<lon0>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case MOLLWEIDE:
					fprintf (stderr, "\t-Jw<lon0>/<scale> OR -JW<lon0>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case ECKERT4:
					fprintf (stderr, "\t-Jkf<lon0>/<scale> OR -JKf<lon0>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case ECKERT6:
					fprintf (stderr, "\t-Jk[s]<lon0>/<scale> OR -JK[s]<lon0>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case CYL_EQ:
					fprintf (stderr, "\t-Jy<lon0>/<lats>/<scale> OR -JY<lon0>/<lats>/<mapwidth>\n");
					fprintf (stderr, "\t  <scale is <1:xxxx> or %s/degree, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					break;
				case POLAR:
					fprintf (stderr, "\t-Jp<scale>[/<origin>] OR -JP<mapwidth>[/<origin>]\n");
					fprintf (stderr, "\t  <scale is %s/units, or use <mapwidth> in %s\n",
						GMT_unit_names[gmtdefs.measure_unit], GMT_unit_names[gmtdefs.measure_unit]);
					fprintf (stderr, "\t  Optionally, append theta value for origin [0]\n");
				case LINEAR:
					fprintf (stderr, "\t-Jx<x-scale>[l|p<power>][/<y-scale>[l|p<power>]], scale in %s/units\n",
						GMT_unit_names[gmtdefs.measure_unit]);
					fprintf (stderr, "\t-Jz<z-scale>[l|p<power>], scale in %s/units\n",
						GMT_unit_names[gmtdefs.measure_unit]);
					fprintf (stderr, "\tUse -JX (and/or -JZ) to give axes lengths rather than scales\n");
					break;
				default:
					fprintf (stderr, "\tProjection not recognized!\n");
					break;
			}
			break;
	
		case 'R':	/* Region option */
		
			fprintf (stderr, "\t-R<xmin>/<xmax>/<ymin>/<ymax>[/<zmin>/<zmax>], dd:mm format ok\n");
			fprintf (stderr, "\tAppend r if giving lower left and upper right coordinates\n");
			break;
			
		case 'U':	/* Set time stamp option */
		
			fprintf (stderr, "\t-U[/<dx>/<dy>/][<string> | c], c will plot command line.\n");
			break;
			
		case 'c':	/* Set number of plot copies option */
		
			fprintf (stderr, "\t-c<copies>, copies is number of copies\n");
			break;

		default:
			break;
	}
}

void GMT_default_error (char option)
{
	fprintf (stderr, "%s: GMT SYNTAX ERROR:  Unrecognized option -%c\n", GMT_program, option);
}

int GMT_get_common_args (char *item, double *w, double *e, double *s, double *n)
{
	char *text, string[100], txt_a[32], txt_b[32];
	
	/* GMT_get_common_args interprets the command line for the common, unique options
	 * -B, -H, -J, -K, -O, -P, -R, -U, -V, -X, -Y, -c, -:, -
	 */
	 
	int i, j, nn, n_slashes, error = 0;
	double *p[6];
	
	switch (item[1]) {
		case '\0':
			GMT_quick = TRUE;
			break;
		case 'B':
			error += (i = GMT_map_getframe (&item[2]));
			if (i) GMT_syntax ('B');
			break;
		case 'H':
			if (item[2]) {
				i = atoi (&item[2]);
				if (i < 0) {
					GMT_syntax ('H');
					error++;
				}
				else
					gmtdefs.n_header_recs = i;
			}
			gmtdefs.io_header = (gmtdefs.n_header_recs > 0);
			break;
		case 'J':
			error += (i = GMT_map_getproject (&item[2]));
			if (i) GMT_syntax ('J');
			break;
		case 'K':
			gmtdefs.last_page = FALSE;
			break;
		case 'O':
			gmtdefs.overlay = TRUE;
			break;
		case 'P':
			gmtdefs.page_orientation |= 1;	/* Bit arith because eurofont bit may be set */
			break;
		case 'R':
			p[0] = w;	p[1] = e;	p[2] = s;	p[3] = n;
			p[4] = &project_info.z_bottom;	p[5] = &project_info.z_top;
	 		project_info.region_supplied = TRUE;
			
			i = 0;
			strcpy (string, &item[2]);
			text = strtok (string, "/");
			while (text) {
				*p[i] = GMT_ddmmss_to_degree (text);
				i++;
				text = strtok (CNULL, "/");
			}
	 		if (item[strlen(item)-1] == 'r') {	/* Rectangular box given */
	 			project_info.region = FALSE;
	 			d_swap (*p[2], *p[1]);		/* So w/e/s/n makes sense */
	 		}
			if ((i < 4 || i > 6) || (GMT_check_region (*p[0], *p[1], *p[2], *p[3]) || (i == 6 && *p[4] >= *p[5]))) {
				error++;
				GMT_syntax ('R');
			}
			break;
		case 'U':
			gmtdefs.unix_time = TRUE;
			for (i = n_slashes = 0; item[i]; i++) if (item[i] == '/') {
				n_slashes++;
				if (n_slashes < 4) j = i;
			}
			if (item[2] == '/' && n_slashes == 2) {	/* Gave -U/<dx>/<dy> */
				nn = sscanf (&item[3], "%[^/]/%s", txt_a, txt_b);
				gmtdefs.unix_time_pos[0] = GMT_convert_units (txt_a, GMT_INCH);
				gmtdefs.unix_time_pos[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			else if (item[2] == '/' && n_slashes > 2) {	/* Gave -U/<dx>/<dy>/<string> */
				nn = sscanf (&item[3], "%[^/]/%[^/]/%*s", txt_a, txt_b);
				gmtdefs.unix_time_pos[0] = GMT_convert_units (txt_a, GMT_INCH);
				gmtdefs.unix_time_pos[1] = GMT_convert_units (txt_b, GMT_INCH);
				strcpy (gmtdefs.unix_time_label, &item[j+1]);
			}
			else if (item[2] && item[2] != '/')	/* Gave -U<string> */
				strcpy (gmtdefs.unix_time_label, &item[2]);
			if ((item[2] == '/' && n_slashes == 1) || (item[2] == '/' && n_slashes >= 2 && nn != 2)) {
				error++;
				GMT_syntax ('U');
			}
			break;
		case 'V':
			gmtdefs.verbose = TRUE;
			break;
		case 'X':
		case 'x':
			i = 2;
			if (item[2] == 'r') i++;	/* Relative mode is default anyway */
			if (item[2] == 'a') i++, GMT_x_abs = TRUE;
			gmtdefs.x_origin = GMT_convert_units (&item[i], GMT_INCH);
			project_info.x_off_supplied = TRUE;
			break;
		case 'Y':
		case 'y':
			i = 2;
			if (item[2] == 'r') i++;	/* Relative mode is default anyway */
			if (item[2] == 'a') i++, GMT_y_abs = TRUE;
			gmtdefs.y_origin = GMT_convert_units (&item[i], GMT_INCH);
			project_info.y_off_supplied = TRUE;
			break;
		case 'c':
			i = atoi (&item[2]);
			if (i < 1) {
				error++;
				GMT_syntax ('c');
			}
			else
				gmtdefs.n_copies = i;
			break;
		case ':':	/* Toggle lon/lat - lat/lon */
			gmtdefs.xy_toggle = TRUE;
			break;
		default:	/* Should never get here, but... */
			error++;
			fprintf (stderr, "GMT: Warning: bad case in GMT_get_common_args\n");
			break;
	}

	return (error);
}

double GMT_ddmmss_to_degree (char *text)
{
	int i, colons = 0;
	double degree, minute, degfrac, second;

	for (i = 0; text[i]; i++) if (text[i] == ':') colons++;
	if (colons == 2) {	/* dd:mm:ss format */
		sscanf (text, "%lf:%lf:%lf", &degree, &minute, &second);
		degfrac = degree + copysign (minute / 60.0, degree) + copysign (second / 3600.0, degree);
	}
	else if (colons == 1) {	/* dd:mm format */
		sscanf (text, "%lf:%lf", &degree, &minute);
		degfrac = degree + copysign (minute / 60.0, degree);
	}
	else
		degfrac = atof (text);
	return (degfrac);
}

void str_tolower (char *value)
{
	/* Convert entire string to lower case */
	int i, c;
	for (i = 0; value[i]; i++) {
		c = (int)value[i];
		value[i] = (char) tolower (c);
	}
}

BOOLEAN true_false_or_error (char *value, int *answer)
{
	/* Assigns 1 or 0 to answer if value is true or false and return FALSE.
	 * If not given true or false, return error TRUE */

	if (!strcmp (value, "true")) {	/* TRUE */
		*answer = 1;
		return (FALSE);
	}
	if (!strcmp (value, "false")) {	/* FALSE */
		*answer = 0;
		return (FALSE);
	}

	/* Got neither true or false.  Make no assignement and return TRUE for error */

	return (TRUE);
}

int GMT_savedefaults (char *file)
{
	FILE *fp;
	char u, abbrev[4] = {'c', 'i', 'm', 'p'};
	double s;
	
	if (!file)
		fp = GMT_stdout;
	else if ((fp = fopen (file, "w")) == NULL) {
		fprintf (stderr, "GMT: Could not create file %s\n", file);
		return (-1);
	}

	u = abbrev[gmtdefs.measure_unit];
	s = GMT_u2u[GMT_INCH][gmtdefs.measure_unit];	/* Convert from internal inch to users unit */

	fprintf (fp, "#\n#	GMT-SYSTEM %s Defaults file\n#\n", GMT_VERSION);
	fprintf (fp, "ANOT_MIN_ANGLE		= %lg\n", gmtdefs.anot_min_angle);
	fprintf (fp, "ANOT_FONT		= %s\n", GMT_font_name[gmtdefs.anot_font]);
	fprintf (fp, "ANOT_FONT_SIZE		= %dp\n", gmtdefs.anot_font_size);
	fprintf (fp, "ANOT_OFFSET		= %lg%c\n", gmtdefs.anot_offset * s, u);
	fprintf (fp, "BASEMAP_AXES		= %s\n", gmtdefs.basemap_axes);
	fprintf (fp, "BASEMAP_FRAME_RGB	= %d/%d/%d\n", gmtdefs.basemap_frame_rgb[0],
		gmtdefs.basemap_frame_rgb[1], gmtdefs.basemap_frame_rgb[2]);
	(gmtdefs.basemap_type) ? fprintf (fp, "BASEMAP_TYPE		= plain\n") : fprintf (fp, "BASEMAP_TYPE		= fancy\n");
	fprintf (fp, "COLOR_BACKGROUND	= %d/%d/%d\n", gmtdefs.background_rgb[0], gmtdefs.background_rgb[1], gmtdefs.background_rgb[2]);
	fprintf (fp, "COLOR_FOREGROUND	= %d/%d/%d\n", gmtdefs.foreground_rgb[0], gmtdefs.foreground_rgb[1], gmtdefs.foreground_rgb[2]);
	fprintf (fp, "COLOR_NAN		= %d/%d/%d\n", gmtdefs.nan_rgb[0], gmtdefs.nan_rgb[1], gmtdefs.nan_rgb[2]);
	fprintf (fp, "COLOR_IMAGE		= ");
	if (gmtdefs.color_image == 0)
		fprintf (fp, "adobe\n");
	else if (gmtdefs.color_image == 1)
		fprintf (fp, "tiles\n");
	(gmtdefs.color_model == GMT_HSV) ? fprintf (fp, "COLOR_MODEL		= hsv\n") : fprintf (fp, "COLOR_MODEL		= rgb\n");
	fprintf (fp, "D_FORMAT		= %s\n", gmtdefs.d_format);
	fprintf (fp, "DEGREE_FORMAT		= %d\n", gmtdefs.degree_format);
	fprintf (fp, "DOTS_PR_INCH		= %d\n", gmtdefs.dpi);
	fprintf (fp, "ELLIPSOID		= %s\n", gmtdefs.ellipse[gmtdefs.ellipsoid].name);
	fprintf (fp, "FRAME_PEN		= %s\n", GMT_putpen (&gmtdefs.frame_pen));
	fprintf (fp, "FRAME_WIDTH		= %lg%c\n", gmtdefs.frame_width * s, u);
	fprintf (fp, "GLOBAL_X_SCALE		= %lg\n", gmtdefs.global_x_scale);
	fprintf (fp, "GLOBAL_Y_SCALE		= %lg\n", gmtdefs.global_y_scale);
	fprintf (fp, "GRID_CROSS_SIZE		= %lg%c\n", gmtdefs.grid_cross_size * s, u);
	fprintf (fp, "GRID_PEN		= %s\n", GMT_putpen (&gmtdefs.grid_pen));
	(gmtdefs.gridfile_shorthand) ? fprintf (fp, "GRIDFILE_SHORTHAND	= TRUE\n") : fprintf (fp, "GRIDFILE_SHORTHAND	= FALSE\n");
	fprintf (fp, "HEADER_FONT		= %s\n", GMT_font_name[gmtdefs.header_font]);
	fprintf (fp, "HEADER_FONT_SIZE	= %dp\n", gmtdefs.header_font_size);
	fprintf (fp, "HSV_MIN_SATURATION	= %lg\n", gmtdefs.hsv_min_saturation);
	fprintf (fp, "HSV_MAX_SATURATION	= %lg\n", gmtdefs.hsv_max_saturation);
	fprintf (fp, "HSV_MIN_VALUE		= %lg\n", gmtdefs.hsv_min_value);
	fprintf (fp, "HSV_MAX_VALUE		= %lg\n", gmtdefs.hsv_max_value);
	fprintf (fp, "INTERPOLANT		= ");
	if (gmtdefs.interpolant == 0)
		fprintf (fp, "linear\n");
	else if (gmtdefs.interpolant == 1)
		fprintf (fp, "akima\n");
	else if (gmtdefs.interpolant == 2)
		fprintf (fp, "cubic\n");
	(gmtdefs.io_header) ? fprintf (fp, "IO_HEADER		= TRUE\n") : fprintf (fp, "IO_HEADER		= FALSE\n");
	fprintf (fp, "N_HEADER_RECS		= %d\n", gmtdefs.n_header_recs);
	fprintf (fp, "LABEL_FONT		= %s\n", GMT_font_name[gmtdefs.label_font]);
	fprintf (fp, "LABEL_FONT_SIZE		= %dp\n", gmtdefs.label_font_size);
	fprintf (fp, "LINE_STEP		= %lg%c\n", gmtdefs.line_step * s, u);
	fprintf (fp, "MAP_SCALE_FACTOR	= %lg\n", gmtdefs.map_scale_factor);
	fprintf (fp, "MAP_SCALE_HEIGHT	= %lg%c\n", gmtdefs.map_scale_height * s, u);
	fprintf (fp, "MEASURE_UNIT		= %s\n", GMT_unit_names[gmtdefs.measure_unit]);
	fprintf (fp, "N_COPIES		= %d\n", gmtdefs.n_copies);
	fprintf (fp, "OBLIQUE_ANOTATION	= %d\n", gmtdefs.oblique_anotation);
	fprintf (fp, "PAGE_COLOR		= %d/%d/%d\n", gmtdefs.page_rgb[0], gmtdefs.page_rgb[1], gmtdefs.page_rgb[2]);
	(gmtdefs.page_orientation & 1) ? fprintf (fp, "PAGE_ORIENTATION	= portrait\n") : fprintf (fp, "PAGE_ORIENTATION	= landscape\n");
	if (gmtdefs.media >= USER_MEDIA_OFFSET)
		fprintf (fp, "PAPER_MEDIA		= %s", GMT_user_media_name[gmtdefs.media-USER_MEDIA_OFFSET]);
	else
		fprintf (fp, "PAPER_MEDIA		= %s", GMT_media_name[gmtdefs.media]);
	if (gmtdefs.paper_width[0] < 0)
		fprintf (fp, "-\n");
	else if (gmtdefs.paper_width[1] < 0)
		fprintf (fp, "+\n");
	else
		fprintf (fp, "\n");
	(gmtdefs.ps_heximage) ? fprintf (fp, "PSIMAGE_FORMAT		= hex\n") : fprintf (fp, "PSIMAGE_FORMAT		= bin\n");
	fprintf (fp, "TICK_LENGTH		= %lg%c\n", gmtdefs.tick_length * s, u);
	fprintf (fp, "TICK_PEN		= %s\n", GMT_putpen (&gmtdefs.tick_pen));
	(gmtdefs.unix_time) ? fprintf (fp, "UNIX_TIME		= TRUE\n") : fprintf (fp, "UNIX_TIME		= FALSE\n");
	fprintf (fp, "UNIX_TIME_POS		= %lg%c/%lg%c\n", gmtdefs.unix_time_pos[0] * s, u, gmtdefs.unix_time_pos[1] * s, u);
	fprintf (fp, "VECTOR_SHAPE		= %lg\n", gmtdefs.vector_shape);
	(gmtdefs.verbose) ? fprintf (fp, "VERBOSE			= TRUE\n") : fprintf (fp, "VERBOSE			= FALSE\n");
	(gmtdefs.want_euro_font) ? fprintf (fp, "WANT_EURO_FONT		= TRUE\n") : fprintf (fp, "WANT_EURO_FONT		= FALSE\n");
	fprintf (fp, "X_AXIS_LENGTH		= %lg%c\n", gmtdefs.x_axis_length * s, u);
	fprintf (fp, "Y_AXIS_LENGTH		= %lg%c\n", gmtdefs.y_axis_length * s, u);
	fprintf (fp, "X_ORIGIN		= %lg%c\n", gmtdefs.x_origin * s, u);
	fprintf (fp, "Y_ORIGIN		= %lg%c\n", gmtdefs.y_origin * s, u);
	(gmtdefs.xy_toggle) ? fprintf (fp, "XY_TOGGLE	= TRUE\n") : fprintf (fp, "XY_TOGGLE		= FALSE\n");
	(gmtdefs.y_axis_type == 1) ? fprintf (fp, "Y_AXIS_TYPE		= ver_text\n") : fprintf (fp, "Y_AXIS_TYPE		= hor_text\n");

	if (fp != GMT_stdout) fclose (fp);
	
	return (0);
}


char *GMT_getdefpath (int get)
{
	/* Return the full path to the chosen .gmtdefaults system file
	 * depending on the value of get:
	 * get = 0:	Use gmt.conf to set get to 1 or 2.
	 * get = 1:	Use the SI settings.
	 * get = 2:	Use the US settings. */

	int id;
	char line[BUFSIZ], *path, *suffix[2] = {"SI", "US"};
	FILE *fp;
	
	GMT_set_home ();

	if (get == 0) {	/* Must use GMT system defaults via gmt.conf */
	
		sprintf (line, "%s%cshare%cgmt.conf\0", GMTHOME, DIR_DELIM, DIR_DELIM);
		if ((fp = fopen (line, "r")) == NULL) {
			fprintf (stderr, "GMT Fatal Error: Cannot open/find GMT configuration file %s\n", line);
			exit (EXIT_FAILURE);
		}
		
		while (fgets (line, BUFSIZ, fp) && (line[0] == '#' || line[0] == '\n'));	/* Scan to first real line */
		fclose (fp);
		if (!strncmp (line, "US", 2))
			id = 2;
		else if (!strncmp (line, "SI", 2))
			id = 1;
		else {
			fprintf (stderr, "GMT Fatal Error: No SI/US keyword in GMT configuration file ($GMTHOME/share/gmt.conf)\n");
			exit (EXIT_FAILURE);
		}
	}
	else
		id = get;
	
	id--;	/* Get 0 or 1 */
	sprintf (line, "%s%cshare%c.gmtdefaults_%s\0", GMTHOME, DIR_DELIM, DIR_DELIM, suffix[id]);

	path = (char *) GMT_memory (VNULL, (size_t)(strlen (line) + 1), (size_t)1, GMT_program);
	
	strcpy (path, line);
	
	return (path);
}
	
double GMT_convert_units (char *from, int new_format)
{
	/* Converts the input value to new units indicated by way */

	int c = 0, len, old_format;
	BOOLEAN have_unit = FALSE;
	double value;

	if ((len = strlen(from))) {
		c = from[len-1];
		if ((have_unit = isalpha (c))) from[len-1] = '\0';	/* Temporarily remove unit */
	}

	switch (c) {
		case 'C':	/* Centimeters */
		case 'c':
			old_format = 0;
			break;
		case 'I':	/* Inches */
		case 'i':
			old_format = 1;
			break;
		case 'M':	/* meters */
		case 'm':
			old_format = 2;
			break;
		case 'P':	/* points */
		case 'p':
			old_format = 3;
			break;
		default:
			old_format = gmtdefs.measure_unit;
			break;
	}

	value = atof (from) * GMT_u2u[old_format][new_format];
	if (have_unit) from[len-1] = c;	/* Put back what we took out temporarily */
	
	return (value);
	
}

int GMT_hash_lookup (char *key, struct GMT_HASH *hashnode, int n)
{
	int i;
	struct GMT_HASH *this;
	
	i = GMT_hash (key);
	
	if (i >= n || i < 0 || !hashnode[i].next) return (-1);	/* Bad key */
	this = hashnode[i].next;
	while (this && strcmp (this->key, key)) this = this->next;
	
	return ((this) ? this->id : -1);
}

void GMT_hash_init (struct GMT_HASH *hashnode, char **keys, int n_hash, int n_keys)
{
	int i, entry;
	struct GMT_HASH *this;

	/* Set up hash table */
	
	for (i = 0; i < n_hash; i++) hashnode[i].next = (struct GMT_HASH *)0;
	for (i = 0; i < n_keys; i++) {
		entry = GMT_hash (keys[i]);
		this = &hashnode[entry];
		while (this->next) this = this->next;
		this->next = (struct GMT_HASH *)GMT_memory (VNULL, (size_t)1, sizeof (struct GMT_HASH), GMT_program);
		this->next->key = keys[i];
		this->next->id = i;
	}
}

int GMT_hash (char *v)
{
        int h;
        for (h = 0; *v != '\0'; v++) h = (64 * h + (*v)) % HASH_SIZE;
        return (h);
}

int GMT_get_ellipse (char *name)
{
	int i, n;
	
	for (i = 0; i < N_ELLIPSOIDS && strcmp (name, gmtdefs.ellipse[i].name); i++);

	if (i == N_ELLIPSOIDS) {	/* Try to open as file */
		FILE *fp;
		char line[BUFSIZ];
			
		if ((fp = fopen (name, "r")) == NULL)
			i = -1;	/* Failed, give error */
		else {	/* Found file, now get parameters */
			i = N_ELLIPSOIDS - 1;
			while (fgets (line, BUFSIZ, fp) && (line[0] == '#' || line[0] == '\n'));
			fclose (fp);
			n = sscanf (line, "%s %d %lf %lf %lf", gmtdefs.ellipse[i].name, 
				&gmtdefs.ellipse[i].date, &gmtdefs.ellipse[i].eq_radius,
				&gmtdefs.ellipse[i].pol_radius, &gmtdefs.ellipse[i].flattening);
			if (n != 5) {
				fprintf (stderr, "GMT: Error decoding user ellipsoid parameters (%s)\n", line);
				exit (EXIT_FAILURE);
			}
		}
	}
			
	return (i);
}

int GMT_key_lookup (char *name, char **list, int n)
{
	int i;
	
	for (i = 0; i < n && strcmp (name, list[i]); i++);
	return (i);
}

int GMT_load_user_media (void) {	/* Load any user-specified media formats */
	int n, n_alloc, w, h;
	char line[BUFSIZ], media[80];
	FILE *fp;

	GMT_set_home ();

	sprintf (line, "%s%cshare%cgmtmedia.d\0", GMTHOME, DIR_DELIM, DIR_DELIM);
	if ((fp = fopen (line, "r")) == NULL) return (0);

	n_alloc = GMT_TINY_CHUNK;
	GMT_user_media = (struct GMT_MEDIA *) GMT_memory (VNULL, (size_t)n_alloc, sizeof (struct GMT_MEDIA), GMT_program);
	GMT_user_media_name = (char **) GMT_memory (VNULL, (size_t)n_alloc, sizeof (char *), GMT_program);
 
	n = 0;
	while (fgets (line, BUFSIZ, fp)) {
		if (line[0] == '#' || line[0] == '\n') continue;

		sscanf (line, "%s %d %d", media, &w, &h);

		/* Convert string to lower case */

		str_tolower (media);

		GMT_user_media_name[n] = (char *) GMT_memory (VNULL, (size_t)(strlen(media)+1), sizeof (char), GMT_program);
		strcpy (GMT_user_media_name[n], media);
		GMT_user_media[n].width  = w;
		GMT_user_media[n].height = h;
		n++;
		if (n == n_alloc) {
			n_alloc += GMT_TINY_CHUNK;
			GMT_user_media = (struct GMT_MEDIA *) GMT_memory ((void *)GMT_user_media, (size_t)n_alloc, sizeof (struct GMT_MEDIA), GMT_program);
			GMT_user_media_name = (char **) GMT_memory ((void *)GMT_user_media_name, (size_t)n_alloc, sizeof (char *), GMT_program);
		}
	}
	fclose (fp);

	GMT_user_media = (struct GMT_MEDIA *) GMT_memory ((void *)GMT_user_media, (size_t)n, sizeof (struct GMT_MEDIA), GMT_program);
	GMT_user_media_name = (char **) GMT_memory ((void *)GMT_user_media_name, (size_t)n, sizeof (char *), GMT_program);

	return (n);
}

void GMT_setshorthand (void) {/* Read user's .gmt_io file and initialize shorthand notation */
	int n = 0, n_alloc;
	char file[BUFSIZ], line[BUFSIZ], *homedir, a[10], b[20], c[20], d[20], e[20];
        static char home [] = "HOME";
	FILE *fp;
	
	if ((homedir = getenv (home)) == NULL) {
		fprintf (stderr, "GMT Warning: Could not determine home directory!\n");
		return;
	}
	sprintf (file, "%s%c.gmt_io\0", homedir, DIR_DELIM);
	if ((fp = fopen (file, "r")) == NULL) return;

	n_alloc = GMT_SMALL_CHUNK;
	GMT_file_id = (int *) GMT_memory (VNULL, (size_t)n_alloc, sizeof (int), GMT_program);
	GMT_file_scale = (double *) GMT_memory (VNULL, (size_t)n_alloc, sizeof (double), GMT_program);
	GMT_file_offset = (double *) GMT_memory (VNULL, (size_t)n_alloc, sizeof (double), GMT_program);
	GMT_file_nan = (double *) GMT_memory (VNULL, (size_t)n_alloc, sizeof (double), GMT_program);
	GMT_file_suffix = (char **) GMT_memory (VNULL, (size_t)n_alloc, sizeof (char *), GMT_program);
 
	while (fgets (line, BUFSIZ, fp)) {
		if (line[0] == '#' || line[0] == '\n') continue;
		sscanf (line, "%s %s %s %s %s", a, b, c, d, e);
		GMT_file_suffix[n] = (char *) GMT_memory (VNULL, (size_t)(strlen(a)+1), sizeof (char), GMT_program);
		strcpy (GMT_file_suffix[n], a);
		GMT_file_id[n] = atoi (b);
		GMT_file_scale[n] = (strcmp (c, "-")) ? atof (c) : 1.0;
		GMT_file_offset[n] = (strcmp (d, "-")) ? atof (d) : 0.0;
		GMT_file_nan[n] = (strcmp (e, "-")) ? atof (e) : GMT_d_NaN;
		n++;
		if (n == n_alloc) {
			n_alloc += GMT_SMALL_CHUNK;
			GMT_file_id = (int *) GMT_memory ((void *)GMT_file_id, (size_t)n_alloc, sizeof (int), GMT_program);
			GMT_file_scale = (double *) GMT_memory ((void *)GMT_file_scale, (size_t)n_alloc, sizeof (double), GMT_program);
			GMT_file_offset = (double *) GMT_memory ((void *)GMT_file_offset, (size_t)n_alloc, sizeof (double), GMT_program);
			GMT_file_nan = (double *) GMT_memory ((void *)GMT_file_nan, (size_t)n_alloc, sizeof (double), GMT_program);
			GMT_file_suffix = (char **) GMT_memory ((void *)GMT_file_suffix, (size_t)n_alloc, sizeof (char *), GMT_program);
		}
	}
	fclose (fp);
	GMT_n_file_suffix = n;
	GMT_file_id = (int *) GMT_memory ((void *)GMT_file_id, (size_t)GMT_n_file_suffix, sizeof (int), GMT_program);
	GMT_file_scale = (double *) GMT_memory ((void *)GMT_file_scale, (size_t)GMT_n_file_suffix, sizeof (double), GMT_program);
	GMT_file_offset = (double *) GMT_memory ((void *)GMT_file_offset, (size_t)GMT_n_file_suffix, sizeof (double), GMT_program);
	GMT_file_nan = (double *) GMT_memory ((void *)GMT_file_nan, (size_t)GMT_n_file_suffix, sizeof (double), GMT_program);
	GMT_file_suffix = (char **) GMT_memory ((void *)GMT_file_suffix, (size_t)GMT_n_file_suffix, sizeof (char *), GMT_program);
}

void GMT_end (int argc, char **argv)
{
	/* GMT_end will clean up after us. */
       
	int i;

	for (i = 0; i < N_UNIQUE; i++) if (GMT_oldargv[i]) GMT_free ((void *)GMT_oldargv[i]);
	if (GMT_lut) GMT_free ((void *)GMT_lut);
	GMT_free_plot_array ();

#ifdef __FreeBSD__
	fpresetsticky (FP_X_DZ | FP_X_INV);
	fpsetmask (FP_X_DZ | FP_X_INV);
#endif

	exit (EXIT_SUCCESS);
}

void GMT_set_home (void)
{
	char *this;

	if (GMTHOME) return;	/* Already set elsewhere */

	if ((this = getenv ("GMTHOME")) == CNULL) {	/* Use default GMT path */
		GMTHOME = (char *) GMT_memory (VNULL, (size_t)(strlen (GMT_DEFAULT_PATH) + 1), (size_t)1, "GMT");
 		strcpy (GMTHOME, GMT_DEFAULT_PATH);
	}
	else {	/* Use user's default path */
		GMTHOME = (char *) GMT_memory (VNULL, (size_t)(strlen (this) + 1), (size_t)1, "GMT");
		strcpy (GMTHOME, this);
	}
}

void GMT_put_history (int argc, char **argv)
{
	/* GMT_put_history will update the .gmtcommands
	 * file and write out the common parameters.
	 */
       
	int i, j, k, found_new, found_old;
#ifndef NO_LOCK
	struct flock lock;
#endif

	/* First check that -X -Y was done correctly */

	if ((project_info.x_off_supplied && project_info.y_off_supplied) && GMT_x_abs != GMT_y_abs) {
		fprintf (stderr, "%s: GMT SYNTAX ERROR: -X -Y must both be absolute or relative\n", GMT_program);
		exit (EXIT_FAILURE);
	}
	if (GMT_x_abs && GMT_y_abs) gmtdefs.page_orientation |= 8;

	/* Ok, now update history, first rewind file pointer */

	rewind (GMT_fp_history);

	fprintf (GMT_fp_history, "# GMT common arguments shelf\n");

	for (i = 0; i < N_UNIQUE; i++) {        /* Loop over GMT_unique_option parameters */

		/* First see if an updated value exist for this common parameter */

		for (j = 1, found_new = FALSE; !found_new && j < argc; j++) {
			if (argv[j][0] != '-') continue;
			if (GMT_unique_option[i][0] == 'J') /* Range of -J? options */
				found_new = !strncmp (&argv[j][1], GMT_unique_option[i], 2);
			else
				found_new = (argv[j][1] == GMT_unique_option[i][0]);
		}

		if (found_new) { /* Need to store this updated value */
			fprintf (GMT_fp_history, "%s\n", argv[j-1]);
		}
		else  {	 	/* Need to find and store the old value if any */
			for (k = 0, found_old = FALSE; !found_old && k < GMT_oldargc; k++) {
				if (GMT_oldargv[k][0] != '-') continue;
				if (GMT_unique_option[i][0] == 'J') /* Range of -J? options */
					found_old = !strncmp (&GMT_oldargv[k][1], GMT_unique_option[i], 2);
				else
					found_old = (GMT_oldargv[k][1] == GMT_unique_option[i][0]);
			}

			if (found_old)  /* Keep old value */
				fprintf (GMT_fp_history, "%s\n", GMT_oldargv[k-1]);
		}
	}
	fprintf (GMT_fp_history, "EOF\n");	/* Logical end of file marker (since old file may be longer) */
	fflush (GMT_fp_history);		/* To ensure all is written when lock is released */

#ifndef NO_LOCK
	lock.l_type = F_UNLCK;		/* Release lock and close file */
	lock.l_whence = SEEK_SET;	/* These three apply lock to entire file */
	lock.l_start = lock.l_len = 0;
	
	if (GMT_lock && fcntl (GMT_fd_history, F_SETLK, &lock)) {
		fprintf (stderr, "%s: Error returned by fcntl [F_UNLCK]\n", GMT_program);
		exit (EXIT_FAILURE);
	}
#endif

	fclose (GMT_fp_history);
}

void GMT_get_history (int argc, char ** argv)
{
	int i, j;
	BOOLEAN nothing_to_do = FALSE, need_xy = FALSE, overlay = FALSE, found, done;
	char line[BUFSIZ];
#ifndef NO_LOCK
	struct flock lock;
#endif

	/* Open .gmtcommands file and retrive old argv (if any)
	 * This is tricky since GMT programs are often hooked together
	 * with pipes so it actually has happened that the first program
	 * is updating the .gmtdefaults file while the second tries to
	 * read.  The result is that the second program often cannot expand
	 * the shorthand and fails with error message.  In GMT 3.1 we introduced
	 * Advisory File Locking and also update the .gmtcommands file as soon
	 * as possible so that commands down the pipe will see the new options
	 * already have taken effect.
	 */


	if (access (".gmtcommands", R_OK)) {    /* No .gmtcommands file in current directory, try to make one */
		if ((GMT_fp_history = fopen (".gmtcommands", "w")) == NULL) {
			fprintf (stderr, "GMT Warning: Could not create .gmtcommands file [permission problem?]\n");
			return;
		}
		nothing_to_do = TRUE;
	}
	else if ((GMT_fp_history = fopen (".gmtcommands", "r+")) == NULL) {
		fprintf (stderr, "GMT Warning: Could not update .gmtcommands file [permission problem?]\n");
		return;
	}

	/* When we get here the file exists */

#ifndef NO_LOCK
	/* now set exclusive lock */

	lock.l_type = F_WRLCK;		/* Lock for [exclusive] reading/writing */
	lock.l_whence = SEEK_SET;	/* These three apply lock to entire file */
	lock.l_start = lock.l_len = 0;
	
	GMT_fd_history = fileno (GMT_fp_history);	/* Get file descriptor */

	if (GMT_lock && fcntl (GMT_fd_history, F_SETLKW, &lock)) {	/* Will wait for file to be ready for reading */
		fprintf (stderr, "%s: Error returned by fcntl [F_WRLCK]\n", GMT_program);
		exit (EXIT_FAILURE);
	}

#endif

	if (nothing_to_do) return;	/* No processing, use original args */

	/* If we get here there is an old file to process */
	/* Get the common arguments and copy them to array GMT_oldargv */

	done = FALSE;
	while (!done && fgets (line, BUFSIZ, GMT_fp_history)) {

		if (line[0] == '#' || line[0] == '\n') continue;	/* Skip comments or blank lines */
		if (!strncmp (line, "EOF", 3)) {	/* Logical end of .gmtcommands file */
			done = TRUE;
			continue;
		}
		if (line[0] != '-') continue;	/* Possibly reading old .gmtcommands format or junk */
		line[strlen(line)-1] = 0;
		GMT_oldargv[GMT_oldargc] = (char *) GMT_memory (VNULL, (size_t)(strlen (line) + 1), 1, "GMT");
		strcpy (GMT_oldargv[GMT_oldargc], line);
		GMT_oldargc++;
		if (GMT_oldargc > N_UNIQUE) {
			fprintf (stderr, "GMT Fatal Error: Failed while decoding common arguments\n");
			exit (EXIT_FAILURE);
		}
	}

	/* See if (1) We need abs/rel shift and (2) if we have an overlay */
      
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') continue;
		if (argv[i][1] == 'X' || argv[i][1] == 'Y' || argv[i][1] == 'x' || argv[i][1] == 'y') need_xy = TRUE;
		if (argv[i][1] == 'O' || argv[i][1] == 'o') overlay = TRUE;
	}
      
	overlay = (need_xy && overlay); /* -O means overlay only if -X or -Y has been specified */
      
	/* Change x/y to upper case if no overlay and change X/Y to lower case if overlay */
 
	for (i = 1; i < argc; i++) {
		if (argv[i][0] != '-') continue;
		if (overlay) {
			if (argv[i][1] == 'X') argv[i][1] = 'x';
			if (argv[i][1] == 'Y') argv[i][1] = 'y';
		}
		else {
			if (argv[i][1] == 'x') argv[i][1] = 'X';
			if (argv[i][1] == 'y') argv[i][1] = 'Y';
		}
	}

	/* Loop over argv and merge argv and GMT_oldargv */
      
	for (i = 1; i < argc; i++) {

		/* Skip if filename or option has modifiers */
              
		if (argv[i][0] != '-') continue;
		if (argv[i][1] != 'J' && argv[i][2] != 0) continue;
		if (argv[i][1] == 'J' && argv[i][3] != 0) continue;

		for (j = 0, found = FALSE; !found && j < GMT_oldargc; j++) {
			if (argv[i][1] == 'J')
				found = (GMT_oldargv[j][1] == argv[i][1] && GMT_oldargv[j][2] == argv[i][2]);
			else
				found = (GMT_oldargv[j][1] == argv[i][1]);
		}
		j--;
                              
		/* Skip if not found or GMT_oldargv has no modifiers */
              
		if (!found) continue;
		if (argv[i][1] != 'J' && GMT_oldargv[j][2] == 0) continue;
		if (argv[i][1] == 'J' && GMT_oldargv[j][3] == 0) continue;
      
		/* Here, GMT_oldargv has modifiers and argv dont, set pointer */
              
		argv[i] = GMT_oldargv[j];
	}
}

int GMT_map_getframe (char *args)
{
	/* GMT_map_getframe scans an argument string and extract parameters that
	 * set the interval for  tickmars and anotations on the boundary.
	 * The string must be continous, i.e. no whitespace must be present
	 * The string may have 1, 2,  or 3 parts, separated by a slash '/'. All
	 * info after the first slash are assigned to the y-axis.  Info after
	 * the second slash are assigned to the z-axis.  If there is no
	 * slash, x-values are copied to y-values.
	 * A substring looks like [t][value][m|c]. The [t] and [m|c] are optional
	 * ([ and ] are NOT part of the string and are just used to clarify)
	 * [t] can be any of [a](anotation int), [f](frame int), or [g](gridline int).
	 * Default is a AND f. The [m], if present indicates value is in minutes.
	 * The [c], if present indicates value is in seConds (s already used for south...).
	 * Text between : and : are labels for the respective axes. If the first
	 * character of the text is a period, then the rest of the text is used
	 * as the plot title.  If it is a comma, then the rest is used as anotation unit.
	 * For LINEAR axes: If the first characters in args are one or more of w,e,s,n
	 * only those axes will be drawn. Upper case letters means the chosen axes
	 * also will be annotated. Default is all 4 axes drawn/annotated.
	 * For logscale plots:  l will cause log10(x) to be plotted
	 *			p will cause 10 ^ log10(x) to be plotted
	 *	anot/tick/grid interval can here be either:
	 *		1.0	-> Only powers of 10 are anotated
	 *		2.0	-> powers of 10 times (1, 2, 5) are anotated
	 *		3.0	-> powers of 10 times (1,2,3,..9) are anotated
	 */
	 
	int i, i1, i2, xyz_side = 0, n_colons = 0, side[5], flag;
	BOOLEAN xyz_set[3], error = FALSE, set_sides = FALSE;
	double value;
	
	for (i = 0; i < 3; i++) {
		frame_info.anot_int[i] = 0.0;
		frame_info.frame_int[i] = 0.0;
		frame_info.grid_int[i] = 0.0;
		frame_info.label[i][0] = 0;
		frame_info.unit[i][0] = 0;
		frame_info.anot_type[i] = 0;
		xyz_set[i] = FALSE;
	}
	frame_info.plot = TRUE;
	frame_info.draw_box = FALSE;
	frame_info.header[0] = 0;
	for (i = 0; i < 5; i++) side[i] = 0;
	
	i1 = 0;
	while (args[i1]) {
		if (args[i1] == '/') {
			xyz_side++;
			i1++;
		}
		flag = '*';	/* If a,f, or g is not appended, we set all to the same value */
		if (isalpha((int)args[i1])) {
			flag = args[i1];
			i1++;
		}
		if (flag == 'w' || flag == 'W') {
			side[3] = (flag == 'W') ? 2 : 1;
			set_sides = TRUE;
			continue;
		}
		if (flag == 'e' || flag == 'E') {
			side[1] = (flag == 'E') ? 2 : 1;
			set_sides = TRUE;
			continue;
		}
		if (flag == 's' || flag == 'S') {
			side[0] = (flag == 'S') ? 2 : 1;
			set_sides = TRUE;
			continue;
		}
		if (flag == 'n' || flag == 'N') {
			side[2] = (flag == 'N') ? 2 : 1;
			set_sides = TRUE;
			continue;
		}
		if (flag == 'z' || flag == 'Z') {
			side[4] = (flag == 'Z') ? 2 : 1;
			if (args[i1] == '+') {
				frame_info.draw_box = TRUE;
				i1++;
			}
			set_sides = TRUE;
			continue;
		}
		if (isupper(flag)) flag = tolower (flag);
		if (flag == 'l') {	/* log10(val) anotations */
			frame_info.anot_type[xyz_side] = 1;
			continue;
		}
		if (flag == 'p') {	/* Powers of 10 anotations */
			frame_info.anot_type[xyz_side] = 2;
			continue;
		}
		if (args[i1] == ':') {	/* Label string */
			n_colons++;
			flag = ' ';
			i2 = ++i1;
			while (args[i2] && args[i2] != ':') i2++;
			if (args[i2] == ':') n_colons++;
			if (args[i1] == '.') {
				i1++;
				strncpy (frame_info.header, &args[i1], (size_t)i2-i1);
				frame_info.header[i2-i1] = 0;
			}
			else if (args[i1] == ',') {
				i1++;
				strncpy (frame_info.unit[xyz_side], &args[i1], (size_t)i2-i1);
				frame_info.unit[xyz_side][i2-i1] = 0;
			}
			else {
				strncpy (frame_info.label[xyz_side], &args[i1], (size_t)i2-i1);
				frame_info.label[xyz_side][i2-i1] = 0;
			}
			i1 = i2 + 1;
		}
		i2 = i1;
		while (args[i2] && strchr ("WESNacefglmnpswz/:", (int)args[i2]) == NULL) i2++;
		i = i2 + 1;
		if ((args[i2] == 'e' || args[i2] == 'E') && args[i] && (args[i] == '-' || args[i] == '+' || isdigit ((int)args[i]))) {	/* Hit exponential notation, keep skipping */
			i2++;
			while (args[i2] && strchr ("WESNacefglmnpswz/:", (int)args[i2]) == NULL) i2++;
		}
		if (flag != ' ') {	/* Decode the numerical value */
			value = atof (&args[i1]);
			if (args[i2] == 'm') {
				value /= 60.0;
				i2++;
			}
			else if (args[i2] == 'c') {
				value /= 3600.0;
				i2++;
			}
			xyz_set[xyz_side] = TRUE;
		}
		if (flag == '*')
			frame_info.anot_int[xyz_side] = frame_info.frame_int[xyz_side] = value;
		else if (flag == 'a')
			frame_info.anot_int[xyz_side] = value;
		else if (flag == 'f')
			frame_info.frame_int[xyz_side] = value;
		else if (flag == 'g')
			frame_info.grid_int[xyz_side] = value;
		i1 = i2;
	}
	if (!xyz_set[1]) frame_info.anot_type[1] = frame_info.anot_type[0];
	for (i = 0; i < 3; i++) {	/* Set both a and f if only one of them is set */
		if (frame_info.anot_int[i] != 0.0 && frame_info.frame_int[i] == 0.0)
			frame_info.frame_int[i] = frame_info.anot_int[i];
		else if (frame_info.frame_int[i] != 0.0 && frame_info.anot_int[i] == 0.0)
			frame_info.anot_int[i] = frame_info.frame_int[i];
	}
	if (!xyz_set[1]) {	/* No separate y-axis info given, copy x values */
		frame_info.anot_int[1] = frame_info.anot_int[0];
		frame_info.frame_int[1] = frame_info.frame_int[0];
		frame_info.grid_int[1] = frame_info.grid_int[0];
	}
	if (set_sides) for (i = 0; i < 5; i++) frame_info.side[i] = side[i];	/* Override default */

	if (n_colons%2) error++;
	
	return (error);
}

int GMT_map_getproject (char *args)
{
	/* GMT_map_getproject scans the arguments given and extracts the parameters needed
	 * for the specified map projection. These parameters are passed through the
	 * project_info structure.  The function returns TRUE if an error is encountered.
	 */
	 
	int i, j, k, n, slash, l_pos[2], p_pos[2], id, project = -1, n_slashes = 0;
	BOOLEAN error = FALSE, skip = FALSE;
	double o_x, o_y, b_x, b_y, c, az;
	double GMT_units[3] = {0.01, 0.0254, 1.0};      /* No of meters in a cm, inch, m */
	char type, args_cp[80], txt_a[32], txt_b[32], txt_c[32], txt_d[32], txt_e[32];
	
	l_pos[0] = l_pos[1] = p_pos[0] = p_pos[1] = 0;
	type = args[0];

	if (strchr ("AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz", (int)type) == NULL) return (TRUE);	/* NO valid projection specified */
	args++;

	for (j = 0; args[j]; j++) if (args[j] == '/') n_slashes++;
	 
	if (!(type == 'z' || type == 'Z')) {
		/* Check to see if scale is specified in 1:xxxx */
		for (j = n = strlen (args), k = -1; j > 0 && k < 0 && args[j] != '/'; j--) if (args[j] == ':') k = j + 1; 
		project_info.units_pr_degree = (k == -1) ? TRUE : FALSE;
	}
	 
	project_info.unit = GMT_units[GMT_INCH];	/* No of meters in an inch */

	 switch (type) {
	 	case 'X':
	 		project_info.compute_scale[0] = project_info.compute_scale[1] = TRUE;
	 		if (args[0] == 'v') {
	 			project_info.pars[0] = gmtdefs.y_axis_length;
	 			project_info.pars[1] = gmtdefs.x_axis_length;
	 			skip = TRUE;
	 		}
	 		else if (args[0] == 'h') {
	 			project_info.pars[0] = gmtdefs.x_axis_length;
	 			project_info.pars[1] = gmtdefs.y_axis_length;
	 			skip = TRUE;
	 		}
	 		
	 	case 'x':		/* Linear x/y scaling */
	 		project_info.pars[4] = (strchr (args, 'd')) ? 1.0 : 0.0;	/* TRUE if input is degrees and d is appended */
			error = (n_slashes > 1);
	 		
	 		/* Find occurrences of /, l, or p */
	 		for (j = 0, slash = 0; args[j] && slash == 0; j++) if (args[j] == '/') slash = j;
	 		for (j = id = 0; args[j]; j++) {
	 			if (args[j] == '/') id = 1;
	 			if (args[j] == 'L' || args[j] == 'l') l_pos[id] = j;
	 			if (args[j] == 'P' || args[j] == 'p') p_pos[id] = j;
	 		}
	 		
			/* Distinguish between p for points and p<power> for scaling */

			n = strlen (args);
			for (j = 0; j < 2; j++) {
				if (!p_pos[j]) continue;
				i = p_pos[j] + 1;
				if (i == n || (args[i] == '/' || args[i] == 'd'))	/* This p is for points since no power is following */
					p_pos[j] = 0;
				else if (args[i] == 'p')	/* The 2nd p is the p for power */
					p_pos[j]++;
			}

	 		/* Get x-arguments */

			strcpy (args_cp, args);	/* Since GMT_convert_units modifies the string */
			if (slash) args_cp[slash] = 0;	/* Chop off y part */
			if ((i = MAX (l_pos[0], p_pos[0])) > 0)
				args_cp[i] = 0;	/* Chop off log or power part */
			else if (project_info.pars[4] == 1.0 && !slash)	/* Chop of trailing 'd' */
				args_cp[strlen(args_cp)-1] = 0;
	 		if (!skip) project_info.pars[0] = GMT_convert_units (args_cp, GMT_INCH);	/* x-scale */
	 		
	 		if (l_pos[0] > 0)
	 			project_info.xyz_projection[0] = LOG10;
	 		else if (p_pos[0] > 0) {
	 			project_info.xyz_projection[0] = POW;
	 			project_info.pars[2] = atof (&args[p_pos[0]+1]);	/* pow to raise x */
	 		}
	 		
	 		if (slash) {	/* Separate y-scaling desired */
				strcpy (args_cp, &args[slash+1]);	/* Since GMT_convert_units modifies the string */
				if ((i = MAX (l_pos[1], p_pos[1])) > 0)
					args_cp[i-slash-1] = 0;	/* Chop off log or power part */
				else if (project_info.pars[4] == 1.0)
					args_cp[strlen(args_cp)-1] = 0;	/* Chop of trailing 'd' part */
	 			if (!skip) project_info.pars[1] = GMT_convert_units (args_cp, GMT_INCH);	/* y-scale */
	 			
	 			if (l_pos[1] > 0)
	 				project_info.xyz_projection[1] = LOG10;
	 			else if (p_pos[1] > 0) {
	 				project_info.xyz_projection[1] = POW;
	 				project_info.pars[3] = atof (&args[p_pos[1]+1]);	/* pow to raise y */
	 			}
	 		}
	 		else {	/* Just copy x parameters */
	 			project_info.xyz_projection[1] = project_info.xyz_projection[0];
	 			if (!skip) project_info.pars[1] = project_info.pars[0];
	 			project_info.pars[3] = project_info.pars[2];
	 		}
	 		project = LINEAR;
	 		if (project_info.pars[0] == 0.0 || project_info.pars[1] == 0.0) error = TRUE;
	 		break;
	 	case 'Z':
			project_info.compute_scale[2] = TRUE;
	 	case 'z':
	 		
			error = (n_slashes > 0);

	 		/* Find occurrences of l or p */
	 		for (j = 0; args[j]; j++) if (args[j] == 'L' || args[j] == 'l') l_pos[0] = j;
	 		for (j = 0; args[j]; j++) if (args[j] == 'P' || args[j] == 'p') p_pos[0] = j;
	 		
			/* Distinguish between p for points and p<power> for scaling */

			n = strlen (args);
			if (p_pos[0]) {
				i = p_pos[0] + 1;
				if (i == n || (args[i] == 'd'))	/* This p is for points since no power is following */
					p_pos[0] = 0;
				else if (args[i] == 'p')	/* The 2nd p is the p for power */
					p_pos[0]++;
			}
	 		/* Get arguments */
	 		
			strcpy (args_cp, args);	/* Since GMT_convert_units modifies the string */
			if ((i = MAX (l_pos[0], p_pos[0])) > 0) args_cp[i] = 0;
	 		project_info.z_pars[0] = GMT_convert_units (args_cp, GMT_INCH);	/* z-scale */
	 		
	 		if (l_pos[0] > 0)
	 			project_info.xyz_projection[2] = LOG10;
	 		else if (p_pos[0] > 0) {
	 			project_info.xyz_projection[2] = POW;
	 			project_info.z_pars[1] = atof (&args[p_pos[0]+1]);	/* pow to raise z */
	 		}
	 		if (project_info.z_pars[0] == 0.0) error = TRUE;
	 		break;
	 	case 'P':		/* Polar (theta,r) */
	 		project_info.gave_map_width = TRUE;
	 	case 'p':
	 		if (n_slashes == 1) {	/* Gave optional zero-base angle [0] */
	 		 	n = sscanf (args, "%[^/]/%lf", txt_a, &project_info.pars[1]);
				project_info.pars[0] = GMT_convert_units (txt_a, GMT_INCH);
				error = (project_info.pars[0] <= 0.0 || n != 2);
			}
			else if (n_slashes == 0) {
	 			project_info.pars[0] = GMT_convert_units (args, GMT_INCH);
	 			n = (args) ? 1 : 0;
				error = (project_info.pars[0] <= 0.0 || n != 1);
	 		}
	 		else
	 			error = TRUE;
	 		project = POLAR;
	 		break;
	 		
	 	/* Map projections */

	 	case 'A':	/* Lambert Azimuthal Equal-Area */
	 		project_info.gave_map_width = TRUE;
	 	case 'a':
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/1:%lf", txt_a, txt_b, &project_info.pars[2]);
	 			if (project_info.pars[2] != 0.0) project_info.pars[2] = 1.0 / (project_info.pars[2] * project_info.unit);
				error = (!(n_slashes == 2 && n == 3));
	 		}
	 		else if (project_info.gave_map_width) {
	 			n = sscanf (args, "%[^/]/%[^/]/%s", txt_a, txt_b, txt_c);
				project_info.pars[2]= GMT_convert_units (txt_c, GMT_INCH);
				error = (!(n_slashes == 2 && n == 3));
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d);
				project_info.pars[2]= GMT_convert_units (txt_c, GMT_INCH);
				project_info.pars[3] = GMT_ddmmss_to_degree (txt_d);
				error = (!(n_slashes == 3 && n == 4));
	 		}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			error += (project_info.pars[2] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = LAMB_AZ_EQ;
	 		break;
	 	case 'B':		/* Albers Equal-area Conic */
	 		project_info.gave_map_width = TRUE;
	 	case 'b':
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%[^/]/1:%lf", txt_a, txt_b, txt_c, txt_d, &project_info.pars[4]);
	 			if (project_info.pars[4] != 0.0) project_info.pars[4] = 1.0 / (project_info.pars[4] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d, txt_e);
				project_info.pars[4]= GMT_convert_units (txt_e, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			project_info.pars[2] = GMT_ddmmss_to_degree (txt_c);
			project_info.pars[3] = GMT_ddmmss_to_degree (txt_d);
			error = !(n_slashes == 4 && n == 5);
			error += (project_info.pars[4] <= 0.0 || project_info.pars[2] == project_info.pars[3]);
			error += (k >= 0 && project_info.gave_map_width);
	 		project = ALBERS;
	 		break;

	 	case 'C':	/* Cassini */
	 		project_info.gave_map_width = TRUE;
	 	case 'c':
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/1:%lf", txt_a, txt_b, &project_info.pars[2]);
	 			if (project_info.pars[2] != 0.0) project_info.pars[2] = 1.0 / (project_info.pars[2] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%s", txt_a, txt_b, txt_c);
				project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			error = !(n_slashes == 2 && n == 3);
			error += (project_info.pars[2] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = CASSINI;
	 		break;

	 	case 'D':		/* Equidistant Conic */
	 		project_info.gave_map_width = TRUE;
	 	case 'd':
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%[^/]/1:%lf", txt_a, txt_b, txt_c, txt_d, &project_info.pars[4]);
	 			if (project_info.pars[4] != 0.0) project_info.pars[4] = 1.0 / (project_info.pars[4] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d, txt_e);
				project_info.pars[4]= GMT_convert_units (txt_e, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			project_info.pars[2] = GMT_ddmmss_to_degree (txt_c);
			project_info.pars[3] = GMT_ddmmss_to_degree (txt_d);
			error = !(n_slashes == 4 && n == 5);
			error += (project_info.pars[4] <= 0.0 || project_info.pars[2] == project_info.pars[3]);
			error += (k >= 0 && project_info.gave_map_width);
	 		project = ECONIC;
	 		break;

	 	case 'E':		/* Azimuthal equal-distant */
	 		project_info.gave_map_width = TRUE;
	 	case 'e':
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/1:%lf", txt_a, txt_b, &project_info.pars[2]);
	 			if (project_info.pars[2] != 0.0) project_info.pars[2] = 1.0 / (project_info.pars[2] * project_info.unit);
				error = (!(n_slashes == 2 && n == 3));
	 		}
	 		else if (project_info.gave_map_width) {
		 		n = sscanf (args, "%[^/]/%[^/]/%s", txt_a, txt_b, txt_c);
				project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
				error = (!(n_slashes == 2 && n == 3));
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d);
				project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
				project_info.pars[3] = GMT_ddmmss_to_degree (txt_d);
				error = (!(n_slashes == 3 && n == 4));
	 		}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			error += (project_info.pars[2] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = AZ_EQDIST;
	 		break;

	 	case 'F':		/* Gnomonic */
	 		project_info.gave_map_width = TRUE;
	 	case 'f':		/* Gnomonic */
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/1:%lf", txt_a, txt_b, txt_c, &project_info.pars[3]);
	 			if (project_info.pars[3] != 0.0) project_info.pars[3] = 1.0 / (project_info.pars[3] * project_info.unit);
				error = (!(n_slashes == 3 && n == 4));
	 		}
	 		else if (project_info.gave_map_width) {
		 		n = sscanf (args, "%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d);
				project_info.pars[3] = GMT_convert_units (txt_d, GMT_INCH);
				error = (!(n_slashes == 3 && n == 4));
			}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d, txt_e);
				project_info.pars[3] = GMT_convert_units (txt_d, GMT_INCH);
				project_info.pars[4] = GMT_ddmmss_to_degree (txt_e);
				error = (!(n_slashes == 4 && n == 5));
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			project_info.pars[2] = GMT_ddmmss_to_degree (txt_c);
			error += (project_info.pars[3] <= 0.0 || (k >= 0 && project_info.gave_map_width) || (project_info.pars[2] >= 90.0));
	 		project = GNOMONIC;
	 		break;

	 	case 'G':		/* Orthographic */
	 		project_info.gave_map_width = TRUE;
	 	case 'g':		/* Orthographic */
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/1:%lf", txt_a, txt_b, &project_info.pars[2]);
	 			if (project_info.pars[2] != 0.0) project_info.pars[2] = 1.0 / (project_info.pars[2] * project_info.unit);
				error = (!(n_slashes == 2 && n == 3));
	 		}
	 		else if (project_info.gave_map_width) {
		 		n = sscanf (args, "%[^/]/%[^/]/%s", txt_a, txt_b, txt_c);
				project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
				error = (!(n_slashes == 2 && n == 3));
			}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d);
				project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
				project_info.pars[3] = GMT_ddmmss_to_degree (txt_d);
				error = (!(n_slashes == 3 && n == 4));
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			error += (project_info.pars[2] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = ORTHO;
	 		break;

	 	case 'H':	/* Hammer-Aitoff Equal-Area */
	 		project_info.gave_map_width = TRUE;
	 	case 'h':
	 		if (k >= 0) {
	 			n = sscanf (args, "%[^/]/1:%lf", txt_a, &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%s", txt_a, txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = HAMMER;
	 		break;

	 	case 'I':	/* Sinusoidal Equal-Area */
	 		project_info.gave_map_width = TRUE;
	 	case 'i':
	 		if (k >= 0) {
	 			n = sscanf (args, "%[^/]/1:%lf", txt_a, &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%s", txt_a, txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = SINUSOIDAL;
	 		break;

	 	case 'J':	/* Miller cylindrical */
	 		project_info.gave_map_width = TRUE;
	 	case 'j':
	 		if (k >= 0) {
	 			n = sscanf (args, "%[^/]/1:%lf", txt_a, &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%s", txt_a, txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = MILLER;
	 		break;

	 	case 'K':	/* Eckert IV or VI projection */
	 		project_info.gave_map_width = TRUE;
	 	case 'k':
			if (args[0] == 'f' || args[0] == 'F') {
				project = ECKERT4;
				j = 1;
			}
			else if (args[0] == 's' || args[0] == 'S') {
				project = ECKERT6;
				j = 1;
			}
			else {	/* Default is Eckert VI */
				project = ECKERT6;
				j = 0;
			}

	 		if (k >= 0) {
	 			n = sscanf (&args[j], "%[^/]/1:%lf", txt_a, &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (&args[j], "%[^/]/%s", txt_a, txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		break;

	 	case 'L':		/* Lambert Conformal Conic */
	 		project_info.gave_map_width = TRUE;
	 	case 'l':
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%[^/]/1:%lf", txt_a, txt_b, txt_c, txt_d, &project_info.pars[4]);
	 			if (project_info.pars[4] != 0.0) project_info.pars[4] = 1.0 / (project_info.pars[4] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d, txt_e);
				project_info.pars[4] = GMT_convert_units (txt_e, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			project_info.pars[2] = GMT_ddmmss_to_degree (txt_c);
			project_info.pars[3] = GMT_ddmmss_to_degree (txt_d);
			error = !(n_slashes == 4 && n == 5);
			error += (project_info.pars[4] <= 0.0 || project_info.pars[2] == project_info.pars[3]);
			error += (k >= 0 && project_info.gave_map_width);
	 		project = LAMBERT;
	 		break;
	 	case 'M':		/* Mercator */
	 		project_info.gave_map_width = TRUE;
	 	case 'm':
			if (n_slashes == 2) {	/* -JM|m<lon0/lat0/width|scale>, store w/s in [2] */
				project_info.m_got_parallel = TRUE;
	 			if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 				n = sscanf (args, "%[^/]/%[^/]/1:%lf", txt_a, txt_b, &project_info.pars[2]);
	 				if (project_info.pars[2] != 0.0) project_info.pars[2] = 1.0 / (project_info.pars[2] * project_info.unit);
	 			}
	 			else {
	 				n = sscanf (args, "%[^/]/%[^/]/%s", txt_a, txt_b, txt_c);
					project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
				}
				project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
				project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
				if (n != 3 || project_info.pars[2] <= 0.0 || fabs (project_info.pars[1]) >= 90.0) error = TRUE;
			}
			else if (n_slashes == 0) {
				project_info.m_got_parallel = FALSE;
	 			if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 				n = sscanf (args, "1:%lf", &project_info.pars[0]);
	 				if (project_info.pars[0] != 0.0) project_info.pars[0] = 1.0 / (project_info.pars[0] * project_info.unit);
	 			}
	 			else {
	 				n = sscanf (args, "%s", txt_a);
					project_info.pars[0] = GMT_convert_units (txt_a, GMT_INCH);
				}
				if (n != 1 || project_info.pars[0] <= 0.0) error = TRUE;
			}
			else
				error = TRUE;
	 		project = MERCATOR;
	 		break;

	 	case 'N':	/* Robinson Projection */
	 		project_info.gave_map_width = TRUE;
	 	case 'n':
	 		if (k >= 0) {
	 			n = sscanf (args, "%[^/]/1:%lf", txt_a, &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%s", txt_a, txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = ROBINSON;
	 		break;

	 	case 'O':		/* Oblique Mercator */
	 		project_info.gave_map_width = TRUE;
	 	case 'o':
	 		if (args[0] == 'a') {	/* Origin and azimuth specified */
	 			if (k >= 0) {
					n = sscanf (&args[1], "%[^/]/%[^/]/%lf/1:%lf", txt_a, txt_b, &az, &project_info.pars[4]);
	 				if (project_info.pars[4] != 0.0) project_info.pars[4] = 1.0 / (project_info.pars[4] * project_info.unit);
	 			}
	 			else {
					n = sscanf (&args[1], "%[^/]/%[^/]/%lf/%s", txt_a, txt_b, &az, txt_c);
					project_info.pars[4] = GMT_convert_units (txt_c, GMT_INCH);
				}

				o_x = GMT_ddmmss_to_degree (txt_a);
				o_y = GMT_ddmmss_to_degree (txt_b);
	 			c = 10.0;	/* compute point 10 degrees from origin along azimuth */
	 			b_x = o_x + R2D * atan (sind (c) * sind (az) / (cosd (o_y) * cosd (c) - sind (o_y) * sind (c) * cosd (az)));
	 			b_y = R2D * d_asin (sind (o_y) * cosd (c) + cosd (o_y) * sind (c) * cosd (az));
	 			project_info.pars[6] = 0.0;
				error = !(n_slashes == 3 && n == 4);
	 		}
	 		else if (args[0] == 'b') {	/* Origin and second point specified */
	 			if (k >= 0) {
	 				n = sscanf (&args[1], "%[^/]/%[^/]/%[^/]/%[^/]/1:%lf", txt_a, txt_b, txt_c, txt_d, &project_info.pars[4]);
	 				if (project_info.pars[4] != 0.0) project_info.pars[4] = 1.0 / (project_info.pars[4] * project_info.unit);
	 			}
	 			else {
	 				n = sscanf (&args[1], "%[^/]/%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d, txt_e);
					project_info.pars[4] = GMT_convert_units (txt_e, GMT_INCH);
				}
				o_x = GMT_ddmmss_to_degree (txt_a);
				o_y = GMT_ddmmss_to_degree (txt_b);
				b_x = GMT_ddmmss_to_degree (txt_c);
				b_y = GMT_ddmmss_to_degree (txt_d);
	 			project_info.pars[6] = 0.0;
				error = !(n_slashes == 4 && n == 5);
	 		}
	 		else if (args[0] == 'c') {	/* Origin and Pole specified */
	 			if (k >= 0) {
	 				n = sscanf (&args[1], "%[^/]/%[^/]/%[^/]/%[^/]/1:%lf", txt_a, txt_b, txt_c, txt_d, &project_info.pars[4]);
	 				if (project_info.pars[4] != 0.0) project_info.pars[4] = 1.0 / (project_info.pars[4] * project_info.unit);
	 			}
	 			else {
	 				n = sscanf (&args[1], "%[^/]/%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d, txt_e);
					project_info.pars[4] = GMT_convert_units (txt_e, GMT_INCH);
				}
				o_x = GMT_ddmmss_to_degree (txt_a);
				o_y = GMT_ddmmss_to_degree (txt_b);
				b_x = GMT_ddmmss_to_degree (txt_c);
				b_y = GMT_ddmmss_to_degree (txt_d);
	 			if (b_y < 0.0) {	/* Flip from S hemisphere to N */
	 				b_y = -b_y;
	 				b_x += 180.0;
	 				if (b_x >= 360.0) b_x -= 360.0;
	 			}
	 			project_info.pars[6] = 1.0;
				error = !(n_slashes == 4 && n == 5);
	 		}
	 		else
	 			project = -1;
			error += (project_info.pars[4] <= 0.0);
			error += (k >= 0 && project_info.gave_map_width);
	 		project_info.pars[0] = o_x;	project_info.pars[1] = o_y;
	 		project_info.pars[2] = b_x;	project_info.pars[3] = b_y;
	 		
	 		/* see if wesn is in oblique degrees or just diagonal corners */
	 		
	 		project = OBLIQUE_MERC;
	 		break;

	 	case 'Q':	/* Equidistant Cylindrical (Plate Carree) */
	 		project_info.gave_map_width = TRUE;
	 	case 'q':
	 		if (k >= 0) {
	 			n = sscanf (args, "%[^/]/1:%lf", txt_a, &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%s", txt_a, txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = CYL_EQDIST;
	 		break;

	 	case 'R':	/* Winkel Tripel Modified azimuthal */
	 		project_info.gave_map_width = TRUE;
	 	case 'r':
	 		if (k >= 0) {
	 			n = sscanf (args, "%[^/]/1:%lf", txt_a, &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%s", txt_a, txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = WINKEL;
	 		break;

	 	case 'S':		/* Stereographic */
	 		project_info.gave_map_width = TRUE;
	 	case 's':
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/1:%lf", txt_a, txt_b, &project_info.pars[2]);
	 			if (project_info.pars[2] != 0.0) project_info.pars[2] = 1.0 / (project_info.pars[2] * project_info.unit);
				error = (!(n_slashes == 2 && n == 3));
	 		}
	 		else if (project_info.gave_map_width) {
		 		n = sscanf (args, "%[^/]/%[^/]/%s", txt_a, txt_b, txt_c);
				project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
				error = (!(n_slashes == 2 && n == 3));
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%[^/]/%s", txt_a, txt_b, txt_c, txt_d);
				project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
				project_info.pars[3] = GMT_ddmmss_to_degree (txt_d);
				error = (!(n_slashes == 3 && n == 4));
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			error += (project_info.pars[2] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = STEREO;
	 		break;

	 	case 'T':	/* Transverse Mercator */
	 		project_info.gave_map_width = TRUE;
	 	case 't':
			if (n_slashes == 1) {	/* -JT<lon>/<width> */
	 			if (k >= 0) {
	 				n = sscanf (args, "%[^/]/1:%lf", txt_a, &project_info.pars[2]);
	 				if (project_info.pars[2] != 0.0) project_info.pars[2] = 1.0 / (project_info.pars[2] * project_info.unit);
	 			}
	 			else {
	 				n = sscanf (args, "%[^/]/%s", txt_a, txt_b);
					project_info.pars[2] = GMT_convert_units (txt_b, GMT_INCH);
				}
				project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
				project_info.pars[1] = 0.0;	/* Default latitude of origin */
				error = !(n_slashes == 1 && n == 2);
			}
			else {	/* -JT<lon>/<lat>/<width> */
	 			if (k >= 0) {
	 				n = sscanf (args, "%[^/]/%[^/]/1:%lf", txt_a, txt_b, &project_info.pars[2]);
	 				if (project_info.pars[2] != 0.0) project_info.pars[2] = 1.0 / (project_info.pars[2] * project_info.unit);
	 			}
	 			else {
	 				n = sscanf (args, "%[^/]/%[^/]/%s", txt_a, txt_b, txt_c);
					project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
				}
				project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
				project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
				error = !(n_slashes == 2 && n == 3);
			}
			error += (project_info.pars[2] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = TM;
	 		break;

	 	case 'U':	/* Universal Transverse Mercator */
	 		project_info.gave_map_width = TRUE;
	 	case 'u':
	 		if (k >= 0) {
	 			n = sscanf (args, "%lf/1:%lf", &project_info.pars[0], &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%lf/%s", &project_info.pars[0], txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
	 		project_info.north_pole = (project_info.pars[0] > 0.0);
	 		project_info.pars[0] = fabs (project_info.pars[0]);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = UTM;
	 		break;
	 	case 'V':	/* Van der Grinten */
	 		project_info.gave_map_width = TRUE;
	 	case 'v':
	 		if (k >= 0) {
	 			n = sscanf (args, "%[^/]/1:%lf", txt_a, &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%s", txt_a, txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = GRINTEN;
	 		break;

	 	case 'W':	/* Mollweide Equal-Area */
	 		project_info.gave_map_width = TRUE;
	 	case 'w':
	 		if (k >= 0) {
	 			n = sscanf (args, "%[^/]/1:%lf", txt_a, &project_info.pars[1]);
	 			if (project_info.pars[1] != 0.0) project_info.pars[1] = 1.0 / (project_info.pars[1] * project_info.unit);
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%s", txt_a, txt_b);
				project_info.pars[1] = GMT_convert_units (txt_b, GMT_INCH);
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			error = !(n_slashes == 1 && n == 2);
			error += (project_info.pars[1] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = MOLLWEIDE;
	 		break;
	 		
	 	case 'Y':		/* Cylindrical Equal Area */
	 		project_info.gave_map_width = TRUE;
	 	case 'y':
	 		if (k >= 0) {	/* Scale entered as 1:mmmmm */
	 			n = sscanf (args, "%[^/]/%[^/]/1:%lf", txt_a, txt_b, &project_info.pars[2]);
	 			if (project_info.pars[2] != 0.0) project_info.pars[2] = 1.0 / (project_info.pars[2] * project_info.unit);
				error = (!(n_slashes == 2 && n == 3));
	 		}
	 		else {
	 			n = sscanf (args, "%[^/]/%[^/]/%s", txt_a, txt_b, txt_c);
				project_info.pars[2] = GMT_convert_units (txt_c, GMT_INCH);
				error = (!(n_slashes == 2 && n == 3));
			}
			project_info.pars[0] = GMT_ddmmss_to_degree (txt_a);
			project_info.pars[1] = GMT_ddmmss_to_degree (txt_b);
			error += (fabs(project_info.pars[1]) >= 90.0);
			error += (project_info.pars[2] <= 0.0 || (k >= 0 && project_info.gave_map_width));
	 		project = CYL_EQ;
	 		break;

	 	default:
	 		error = TRUE;
	 		project = -1;
	 		break;
	 }
	 
	 if (!project_info.units_pr_degree && project_info.gave_map_width) {
	 	fprintf (stderr, "%s: GMT SYNTAX ERROR -J%c option: Cannot specify map width with 1:xxxx format\n", GMT_program, type);
	 	error++;
	 }
	 
	 if (!(type == 'z' || type == 'Z')) project_info.projection = project;
	 return (error);
}

void GMT_free_plot_array (void) {
	if (GMT_n_alloc) {
		GMT_free ((void *)GMT_x_plot);
		GMT_free ((void *)GMT_y_plot);
		GMT_free ((void *)GMT_pen);
	}
}

void GMT_prepare_3D (void) {	/* Initialize 3-D parameters */
	project_info.z_pars[0] = project_info.z_pars[1] = 0.0;
	project_info.xyz_pos[2] = TRUE;
	project_info.zmin = project_info.zmax = 0.0;
	GMT_z_forward = (PFI) NULL;
	GMT_z_inverse = (PFI) NULL;
	z_project.view_azimuth = 180.0;
	z_project.view_elevation = 90.0;
	project_info.z_bottom = project_info.z_top = 0.0;
}

char *GMT_putpen (struct GMT_PEN *pen)
{
	/* GMT_putpen creates a GMT textstring equivalent of the specified pen */

	static char text[BUFSIZ];
	int i;

	if (pen->texture[0]) {

		if (pen->rgb[0] == 0 && pen->rgb[0] == pen->rgb[1] && pen->rgb[1] == pen->rgb[2]) /* Default black pen */
			sprintf (text, "%.5lgt%s:%.5lgp\0", pen->width, pen->texture, pen->offset);
		else
			sprintf (text, "%.5lg/%d/%d/%dt%s:%.5lgp\0", pen->width, pen->rgb[0], pen->rgb[1], pen->rgb[2], pen->texture, pen->offset);
		for (i = 0; text[i]; i++) if (text[i] == ' ') text[i] = '_';
	}
	else {
		if (pen->rgb[0] == 0 && pen->rgb[0] == pen->rgb[1] && pen->rgb[1] == pen->rgb[2]) /* Default black pen */
			sprintf (text, "%.5lgp\0", pen->width);
		else
			sprintf (text, "%.5lg/%d/%d/%dp\0", pen->width, pen->rgb[0], pen->rgb[1], pen->rgb[2]);
	}

	return (text);
}

int GMT_check_region (double w, double e, double s, double n)
{	/* If region is given then we must have w < e and s < n */
	return ((w >= e || s >= n) && project_info.region);
}

int GMT_get_unit (char c)
{
	/* Converts cC, iI, mM, and pP into 0-3 */

	int i;
	switch ((int)c) {
		case 'C':	/* cm */
		case 'c':
			i = 0;
			break;
		case 'I':	/* inch */
		case 'i':
			i = 1;
			break;
		case 'M':	/* meter */
		case 'm':
			i = 2;
			break;
		case 'P':	/* point */
		case 'p':
			i = 3;
			break;
		default:	/* error */
			i = -1;
			break;
	}
	return (i);
}

#ifdef WIN32

/* Make dummy functions so GMT will link under WIN32 */

struct passwd *getpwuid (const int uid)
{
	return ((struct passwd *)NULL);
}

int getuid (void) {
	return (0);
}

#endif

#ifdef SET_IO_MODE

/* Under non-Unix operating systems running on the PC, such as
 * Windows and OS/2, files are opened in either TEXT or BINARY
 * mode.  This difference does not exist under UNIX, but is important
 * on the PC.  Specifically, it causes a problem when a program
 * that writes/reads standard i/o wants to use binary data.
 * In those situations we must change the default (TEXT) mode of
 * the file handle to BINARY via a call to "setmode".
 *
 * Under OS/2, using the EMX compilation system, one can change
 * the mode of input/output from TEXT to BINARY using _fsetmode.
 * Suggested by Alan Cogbill, Los Alamos National Laboratory
 *
 * This can also be done under Win32 with the Microsoft VC++
 * compiler which supports ANSI-C (P. Wessel).  This may be true
 * of other Win32 compilers as well.  Until we know if _setmode
 * would work in the same way under OS/2 we choose to take two
 * different routes, hence the #ifdefs below
 */

void GMT_setmode (int i_or_o)
{
	/* Changes the stream to deal with BINARY rather than TEXT data */
	
	FILE *fp;
	
	if (GMT_io.binary[i_or_o]) {	/* User wants binary */
	
		fp = (i_or_o == 0) ? GMT_stdin : GMT_stdout;
		fflush (fp);	/* Should be untouched but anyway... */
#ifdef _WIN32
		setmode (fileno (fp), _O_BINARY);
#else
		_fsetmode (fp, "b");
#endif		
	}
}

#endif	/* SET_IO_MODE */
