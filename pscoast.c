 /*--------------------------------------------------------------------
 *    THE GMT-system:	@(#)pscoast.c	3.83  03/20/00
 *
 *	Copyright (c) 1991-2000 by P. Wessel and W. H. F. Smith
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
 *
 *--------------------------------------------------------------------*/
/*
 * pscoast (the expurgated version c/o Doug Hunt) can only dump binary
 * map data.  Three
 * data sets are considered: shores, borders, and rivers.  Each of these
 * data sets come in 5 different resolutions.  The lower resolution files
 * are derived from the full resolution data using the Douglas-Peucker (DP)
 * line reduction algorithm.  By giving a tolerance in km, the algorithm
 * will remove points that do not depart more than the tolerance from the
 * straight line that would connect the points if the point in question was
 * removed.  The resolutions are:
 *
 * full		- The complete World Vector Shoreline + CIA data base
 * high		- DP reduced with tolerance = 0.2 km
 * intermediate	- DP reduced with tolerance = 1 km
 * low		- DP reduced with tolerance = 5 km
 * crude	- DP reduced with tolerance = 25 km
 *
 * Author:	Paul Wessel
 * Date:	24-MAR-1984
 * Version:	3.1	Added clipping and dumping as options
 *
 * Excerpted by Doug Hunt to form the core of the PDL::Graphics::Map
 * interface to the GMT maps.  7/27/2000
 *
 */

#include "EXTERN.h"   /* std perl include */
#include "perl.h"     /* std perl include */
#include "gmt.h"

#ifdef DEBUG
int debug_bin = -1;
#endif

struct GMT_SHORE c;
struct GMT_BR b, r;

char *shore_resolution[5] = {"full", "high", "intermediate", "low", "crude"};

void pscoast (double west, double east, double south, double north, char res, int rlevels[N_RLEVELS], int blevels[N_BLEVELS], int draw_coast, SV *lon, SV *lat)
{
	int i, n, np, ind, bin, anti_bin = -1, level, flag;
	int level_to_be_painted, max_level = MAX_LEVEL, direction, np_new, k, last_k;
	int n_blevels = 0, n_rlevels = 0, bin_trouble;
	int start_direction, stop_direction, lowest_level = 1, min_level = 0, n_pieces = 0;
	
	BOOLEAN	error = FALSE, set_lake_fill = FALSE, draw_river = FALSE, shift = FALSE, need_coast_base, recursive;
	BOOLEAN	fill_ocean = FALSE, fill_land = FALSE, possibly_donut_hell = FALSE;
	BOOLEAN clobber_background, draw_border = FALSE, paint_polygons = FALSE, donut, fill_in_use = FALSE;
	BOOLEAN draw_scale = FALSE, gave_xy = FALSE, fancy = FALSE, donut_hell = FALSE, world_map_save;
	BOOLEAN clip_ocean = FALSE, clip_land = FALSE, end_of_clip = FALSE, clipping = FALSE;
	
	double	bin_x[5], bin_y[5];
	double west_border, east_border, anti_lon, anti_lat, edge = 720.0, left, right;
	double *xtmp, *ytmp, min_area = 0.0, x0, y0, scale_lat, length, step = 0.01;
	double anti_x, anti_y, x_0, y_0, x_c, y_c, dist, out[2], mylon;
	
        char *myptr;
        int len, base, latlonlen = 0;

	char key[5], *string, measure = '\0', comment[32];

	struct POL *p;

	for (i=0;i<N_BLEVELS;i++) if (blevels[i]) n_blevels++;
	for (i=0;i<N_RLEVELS;i++) if (rlevels[i]) n_rlevels++;
	if (n_rlevels) draw_river  = TRUE;
	if (n_blevels) draw_border = TRUE;

	my_GMT_begin (); 

	base = GMT_set_resolution (&res, 'D');

	need_coast_base = (draw_coast);

	if (east > 360.0) {
		west -= 360.0;
		east -= 360.0;
	}
	
	GMT_map_getproject ("x1d");

	GMT_map_setup (west, east, south, north);

	world_map_save = GMT_world_map;
	
	if (need_coast_base && GMT_init_shore (res, &c, project_info.w, project_info.e, project_info.s, project_info.n))  {
		fprintf (stderr, "%s: %s resolution shoreline data base not installed\n", GMT_program, shore_resolution[base]);
		need_coast_base = FALSE;
	}
	
	if (draw_border && GMT_init_br ('b', res, &b, project_info.w, project_info.e, project_info.s, project_info.n)) {
		fprintf (stderr, "%s: %s resolution political boundary data base not installed\n", GMT_program, shore_resolution[base]);
		draw_border = FALSE;
	}
	
	if (draw_river && GMT_init_br ('r', res, &r, project_info.w, project_info.e, project_info.s, project_info.n)) {
		fprintf (stderr, "%s: %s resolution river data base not installed\n", GMT_program, shore_resolution[base]);
		draw_river = FALSE;
	}
	
	if (! (need_coast_base || draw_border || draw_river || end_of_clip)) {
		fprintf (stderr, "%s: No databases available - aborts\n", GMT_program);
		exit (EXIT_FAILURE);
	}

	start_direction = -1;
	stop_direction = 1;
	lowest_level = (fill_ocean) ? 0 : 1;

	if ((360.0 - fabs (project_info.e - project_info.w) ) < c.bsize)
		GMT_world_map = TRUE;
	
	/* west_border = project_info.w;	east_border = project_info.e; */

	/* Temporarily shift boundaries */

	/*
	if (project_info.w < 0.0 && project_info.e <= 0.0) {	
		project_info.w += 360.0;
		project_info.e += 360.0;
		if (project_info.central_meridian < 0.0) project_info.central_meridian += 360.0;
	}
	*/

	/* west_border = project_info.w;	east_border = project_info.e; */
	west_border = floor (project_info.w / c.bsize) * c.bsize;
	east_border = ceil (project_info.e / c.bsize) * c.bsize;
		
	for (ind = 0; need_coast_base && ind < c.nb; ind++) {	/* Loop over necessary bins only */
		
		bin = c.bins[ind];
#ifdef DEBUG
		if (debug_bin >= 0 && bin != debug_bin) continue;
#endif
		GMT_get_shore_bin (ind, &c, min_area, min_level, max_level);
		
		if (gmtdefs.verbose) fprintf (stderr, "%s: Working on block # %5d\r", GMT_program, bin);

		bin_trouble = (anti_bin == bin) ? anti_bin : -1;
			
		if (draw_coast && c.ns) {	/* Draw or dump shorelines, no need to assemble polygons */
			/* if ((np = GMT_assemble_shore (&c, direction, FALSE, shift, edge, &p)) == 0) continue; */
			if ((np = GMT_assemble_shore (&c, direction, min_level, FALSE, shift, west_border, east_border, &p)) == 0) continue;

			for (i = 0; i < np; i++) {
			  latlonlen += (p[i].n + 2)*sizeof(double);
			}
			SvGROW(lon, latlonlen);  /* pregrow for efficiency */
			SvGROW(lat, latlonlen);  /* pregrow for efficiency */

			for (i = 0; i < np; i++) {
			  sv_catpvn(lon, (char *) &GMT_d_NaN, sizeof(double)); /* separator */
			  sv_catpvn(lat, (char *) &GMT_d_NaN, sizeof(double)); /* separator */
			  for (k = 0; k < p[i].n; k++) {
                            mylon = p[i].lon[k];
			    sv_catpvn(lon, (char *) &(mylon), sizeof(double)); 
			    sv_catpvn(lat, (char *) &(p[i].lat[k]), sizeof(double)); 
			  }
			}
			
			GMT_free_polygons (p, np);
			GMT_free ((void *)p);
		}
		
		GMT_free_shore (&c);
	
	}

	if (need_coast_base) GMT_shore_cleanup (&c);

	if (gmtdefs.verbose) fprintf (stderr, "\n");
	
	if (draw_river) {	/* Read rivers file and plot as lines */
		
		if (gmtdefs.verbose) fprintf (stderr, "%s: Adding Rivers...", GMT_program);
		last_k = -1;

		for (ind = 0; ind < r.nb; ind++) {	/* Loop over necessary bins only */
		
			bin = r.bins[ind];
			GMT_get_br_bin (ind, &r, rlevels, n_rlevels);
			
			if (r.ns == 0) continue;
			
			if ((np = GMT_assemble_br (&r, shift, edge, &p)) == 0) continue;

			for (i = 0; i < np; i++) {
			  latlonlen += (p[i].n + 2)*sizeof(double);
			}
			SvGROW(lon, latlonlen);  /* pregrow for efficiency */
			SvGROW(lat, latlonlen);  /* pregrow for efficiency */

			for (i = 0; i < np; i++) {
			  sv_catpvn(lon, (char *) &GMT_d_NaN, sizeof(double)); /* separator */
			  sv_catpvn(lat, (char *) &GMT_d_NaN, sizeof(double)); /* separator */
			  for (k = 0; k < p[i].n; k++) {
                            mylon = p[i].lon[k];
			    sv_catpvn(lon, (char *) &(mylon), sizeof(double)); 
			    sv_catpvn(lat, (char *) &(p[i].lat[k]), sizeof(double)); 
			  }
			}
			
			/* Free up memory */
		
			GMT_free_br (&r);
			for (k = 0; k < np; k++) {
				GMT_free ((void *)p[k].lon);
				GMT_free ((void *)p[k].lat);
			}
			GMT_free ((void *)p);
		}
		GMT_br_cleanup (&r);
		
		if (gmtdefs.verbose) fprintf (stderr, "\n");
	}

	if (draw_border) {	/* Read borders file and plot as lines */
		
		if (gmtdefs.verbose) fprintf (stderr, "%s: Adding Borders...", GMT_program);
		
		/* Must resample borders because some points may be too far apart and look like 'jumps' */
		
		step = MAX (fabs(project_info.w - project_info.e), fabs (project_info.n - project_info.s)) / 4.0;

		last_k = -1;

		for (ind = 0; ind < b.nb; ind++) {	/* Loop over necessary bins only */
		
			bin = b.bins[ind];
			GMT_get_br_bin (ind, &b, blevels, n_blevels);
			
			if (b.ns == 0) continue;
			
			if ((np = GMT_assemble_br (&b, shift, edge, &p)) == 0) continue;

			for (i = 0; i < np; i++) {
			  latlonlen += (p[i].n + 2)*sizeof(double);
			}
			SvGROW(lon, latlonlen);  /* pregrow for efficiency */
			SvGROW(lat, latlonlen);  /* pregrow for efficiency */

			for (i = 0; i < np; i++) {
			  sv_catpvn(lon, (char *) &GMT_d_NaN, sizeof(double)); /* separator */
			  sv_catpvn(lat, (char *) &GMT_d_NaN, sizeof(double)); /* separator */
			  for (k = 0; k < p[i].n; k++) {
                            mylon = p[i].lon[k];
			    sv_catpvn(lon, (char *) &(mylon), sizeof(double)); 
			    sv_catpvn(lat, (char *) &(p[i].lat[k]), sizeof(double)); 
			  }
			}
			
			/* Free up memory */
		
			GMT_free_br (&b);
			for (k = 0; k < np; k++) {
				GMT_free ((void *)p[k].lon);
				GMT_free ((void *)p[k].lat);
			}
			GMT_free ((void *)p);
		}
		GMT_br_cleanup (&b);
		
		if (gmtdefs.verbose) fprintf (stderr, "\n");
	}
	
}

int my_GMT_begin ()
{
	/* GMT_begin will merge the command line arguments with the arguments
	 * that were used the last time this programs was called (if any).  This
	 * way one only have to give -R -J to repeat previous map projection instead
	 * of spelling out the wesn and projection parameters every time.
	 * The information is stored in the first line of the file .gmtcommands
	 * in the current directory [or optionally a provided filename] and will
	 * contain the last arguments to the common parameters like
	 * -B, -H, -J, -K, -O, -P, -R, -U, -V, -X, -Y, -c
	 * Since the meaning of -X/-Y depends on weather we have an overlay or not
	 * we maintain -X -Y for absolute shifts and -x -y for relative shifts.
	 * If the argument +file is encountered then file is used in lieu of the
	 * usual .gmtdefaults file and this argument is chopped from argv
	 */
       
	int i, j;
	char *this;

#ifdef __FreeBSD__
	/* allow divide by zero -- Inf */
	fpsetmask (fpgetmask () & ~(FP_X_DZ | FP_X_INV));
#endif 
	/* Initialize parameters */
	
	GMT_stdin  = stdin;
	GMT_stdout = stdout;

	GMT_set_home ();

	this = CNULL;
	GMT_make_fnan (GMT_f_NaN);
	GMT_make_dnan (GMT_d_NaN);
	GMT_oldargc = 0;
	frame_info.plot = FALSE;
	project_info.projection = -1;
	project_info.gave_map_width = FALSE;
	project_info.region = TRUE;
	project_info.compute_scale[0] =  project_info.compute_scale[1] = project_info.compute_scale[2] = FALSE;
	project_info.x_off_supplied = project_info.y_off_supplied = FALSE;
	project_info.region_supplied = FALSE;
	for (j = 0; j < 10; j++) project_info.pars[j] = 0.0;
	project_info.xmin = project_info.ymin = 0.0;
	project_info.z_level = DBL_MAX;	/* Will be set in map_setup */
	project_info.xyz_pos[0] = project_info.xyz_pos[1] = TRUE;
	GMT_prepare_3D ();
	gmtdefs.dlon = (project_info.e - project_info.w) / gmtdefs.n_lon_nodes;
	gmtdefs.dlat = (project_info.n - project_info.s) / gmtdefs.n_lat_nodes;
	for (i = 0; i < 4; i++) project_info.edge[i] = TRUE;
	for (i = 0; i < N_UNIQUE; i++) GMT_oldargv[i] = CNULL;
	GMT_program = "pscoast";
	GMT_grd_in_nan_value = GMT_grd_out_nan_value = GMT_d_NaN;
	
	/* Set the gmtdefault parameters from the $HOME/.gmtdefaults (if any) */
	
        /* See if user specified +optional_defaults_file */
        
	if (gmtdefs.gridfile_shorthand) GMT_setshorthand ();
	
	return;
}











