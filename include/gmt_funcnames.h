/*--------------------------------------------------------------------
 *    The GMT-system:	@(#)gmt_funcnames.h	3.77  11/29/99
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
 * ANSI-C prototypes of all GMT functions called outside the file they
 * are declared in.
 *
 * Author:	Paul Wessel
 * Date:	10-AUG-1995
 * Revised:	15-JUN-1999
 */

EXTERN_MSC void GMT_x_axis (double x0, double y0, double length, double va10, double val1, double anotation_int, double tickmark_int, char *label, char *unit, int axistype, int anottype, int below, int anotate);
EXTERN_MSC void GMT_y_axis (double x0, double y0, double length, double va10, double val1, double anotation_int, double tickmark_int, char *label, char *unit, int axistype, int anottype, int left_side, int anotate);
EXTERN_MSC void GMT_x_to_xx (double x, double *xx); 
EXTERN_MSC void GMT_xx_to_x (double *x, double xx); 
EXTERN_MSC void GMT_y_to_yy (double y, double *yy); 
EXTERN_MSC void GMT_yy_to_y (double *y, double yy); 
EXTERN_MSC void GMT_z_to_zz (double z, double *zz); 
EXTERN_MSC void GMT_zz_to_z (double *z, double zz);
EXTERN_MSC void *GMT_memory (void *prev_addr, size_t nelem, size_t size, char *progname);
EXTERN_MSC void GMT_free (void *addr);
EXTERN_MSC double GMT_great_circle_dist (double lon1, double lat1, double lon2, double lat2);
EXTERN_MSC double GMT_half_map_width (double y);
EXTERN_MSC double GMT_dot3v (double *a, double *b);
EXTERN_MSC double GMT_ddmmss_to_degree (char *text);
EXTERN_MSC struct EPS *GMT_epsinfo (char *program);
EXTERN_MSC int GMT_comp_double_asc (const void *p_1, const void *p_2);
EXTERN_MSC int GMT_comp_float_asc (const void *p_1, const void *p_2);
EXTERN_MSC int GMT_comp_int_asc (const void *p_1, const void *p_2);
EXTERN_MSC int GMT_begin (int argc, char **argv);
EXTERN_MSC void GMT_end (int argc, char **argv);
EXTERN_MSC void GMT_put_history (int argc, char **argv);
EXTERN_MSC int GMT_get_common_args (char *item, double *w, double *e, double *s, double *n);
EXTERN_MSC int GMT_getinc (char *line, double *dx, double *dy);
EXTERN_MSC void GMT_getdefaults (char *this_file);
EXTERN_MSC void GMT_setdefaults (int argc, char **argv);
EXTERN_MSC int GMT_savedefaults (char *file);
EXTERN_MSC void GMT_default_error (char option);
EXTERN_MSC void GMT_explain_option (char option);
EXTERN_MSC void GMT_map_setup (double west, double east, double south, double north);
EXTERN_MSC int GMT_map_getproject (char *args);
EXTERN_MSC void GMT_geoplot (double lon, double lat, int pen);
EXTERN_MSC void GMT_hash_init (struct GMT_HASH *hashnode , char **keys, int n_hash, int n_keys);
EXTERN_MSC int GMT_hash_lookup (char *key, struct GMT_HASH *hashnode, int n);
EXTERN_MSC int GMT_hash (char *v);
EXTERN_MSC void GMT_linearx_grid (double w, double e, double s, double n, double dval);
EXTERN_MSC int GMT_key_lookup (char *name, char **list, int n);
EXTERN_MSC void GMT_grdio_init (void);
EXTERN_MSC void GMT_geoz_to_xy (double x, double y, double z, double *x_out, double *y_out);
EXTERN_MSC void GMT_xy_do_z_to_xy (double x, double y, double z, double *x_out, double *y_out);
EXTERN_MSC int GMT_intpol (double *x, double *y, int n, int m, double *u, double *v, int mode);
EXTERN_MSC int GMT_map_outside (double lon, double lat);
EXTERN_MSC void GMT_get_plot_array (void);
EXTERN_MSC int GMT_graticule_path (double **x, double **y, int dir, double w, double e, double s, double n);
EXTERN_MSC int GMT_map_path (double lon1, double lat1, double lon2, double lat2, double **x, double **y);
EXTERN_MSC int GMT_latpath (double lat, double lon1, double lon2, double **x, double **y);
EXTERN_MSC int GMT_lonpath (double lon, double lat1, double lat2, double **x, double **y);
EXTERN_MSC int GMT_get_format (double interval, char *unit, char *format);
EXTERN_MSC void GMT_geo_to_xy (double lon, double lat, double *x, double *y);
EXTERN_MSC void GMT_xy_to_geo (double *lon, double *lat, double x, double y);
EXTERN_MSC int GMT_break_through (double x0, double y0, double x1, double y1);
EXTERN_MSC int GMT_map_crossing (double lon1, double lat1, double lon2, double lat2, double *xlon, double *xlat, double *xx, double *yy, int *sides);
EXTERN_MSC void GMT_vertical_axis (int mode);
EXTERN_MSC void GMT_project3D (double x, double y, double z, double *x_out, double *y_out, double *z_out);
EXTERN_MSC int GMT_map_clip_path (double **x, double **y, BOOLEAN *donut);
EXTERN_MSC int GMT_fix_up_path (double **a_lon, double **a_lat, int n, BOOLEAN greenwich, double step);
EXTERN_MSC int GMT_clip_to_map (double *lon, double *lat, int np, double **x, double **y);
EXTERN_MSC int GMT_compact_line (double *x, double *y, int n, BOOLEAN pen_flag, int *pen);
EXTERN_MSC void GMT_plot_line (double *x, double *y, int *pen, int n);
EXTERN_MSC void GMT_2D_to_3D (double *x, double *y, int n);
EXTERN_MSC void GMT_xyz_to_xy (double x, double y, double z, double *x_out, double *y_out);
EXTERN_MSC int GMT_geo_to_xy_line (double *lon, double *lat, int n);
EXTERN_MSC int GMT_grd_get_i_format (char *file, char *fname, double *scale, double *offset);
EXTERN_MSC int GMT_grd_get_o_format (char *file, char *fname, double *scale, double *offset);
EXTERN_MSC void GMT_grd_init (struct GRD_HEADER *header, int argc, char **argv, BOOLEAN update);
EXTERN_MSC int GMT_median (double *x, int n, double xmin, double xmax, double m_initial, double *med);
EXTERN_MSC int GMT_mode (double *x, int n, int j, int sort, double *mode_est);
EXTERN_MSC void GMT_geo_to_cart (double *alat, double *alon, double *a, int rads);
EXTERN_MSC void GMT_cart_to_geo (double *alat, double *alon, double *a, int rads);
EXTERN_MSC void GMT_normalize3v (double *a);
EXTERN_MSC void GMT_cross3v (double *a, double *b, double *c);
EXTERN_MSC int GMT_jacobi (double *a, int *n, int *m, double *d, double *v, double *b, double *z, int *nrots);
EXTERN_MSC int GMT_non_zero_winding (double xp, double yp, double *x, double *y, int n_path);
EXTERN_MSC void GMT_grd_setregion (struct GRD_HEADER *h, double *xmin, double *xmax, double *ymin, double *ymax);
EXTERN_MSC void GMT_init_pen (struct GMT_PEN *pen, double width);
EXTERN_MSC void GMT_pen_syntax (char option);
EXTERN_MSC int GMT_getrgb (char *line, int *rgb);
EXTERN_MSC int GMT_getpen (char *line, struct GMT_PEN *pen);
EXTERN_MSC char *GMT_convertpen (struct GMT_PEN *pen, int *width, int *offset, int rgb[]);
EXTERN_MSC void GMT_setpen (struct GMT_PEN *pen);
EXTERN_MSC void GMT_echo_command (int argc, char **argv);
EXTERN_MSC void GMT_timestamp (int argc, char **argv);
EXTERN_MSC void GMT_read_cpt (char *cpt_file);
EXTERN_MSC int GMT_contours (float *grd, struct GRD_HEADER *header, int smooth_factor, int int_scheme, int *side, int *edge, int first, double **x_array, double **y_array);
EXTERN_MSC void GMT_fill (double x[], double y[], int n, struct GMT_FILL *fill, BOOLEAN outline);
EXTERN_MSC void GMT_textbox3d (double x, double y, double z, int size, int font, char *label, double angle, int just, BOOLEAN outline, double dx, double dy, int rgb[]);
EXTERN_MSC void GMT_text3d (double x, double y, double z, int fsize, int fontno, char *text, double angle, int justify, int form);
EXTERN_MSC void GMT_vector3d (double x0, double y0, double x1, double y1, double z0, double tailwidth, double headlength, double headwidth, double shape, int rgb[], BOOLEAN outline);
EXTERN_MSC void GMT_map_basemap (void);
EXTERN_MSC void GMT_draw_map_scale (double x0, double y0, double lat, double length, char measure, BOOLEAN gave_xy, BOOLEAN fancy);
EXTERN_MSC void GMT_grd_shift (struct GRD_HEADER *header, float *grd, double shift);
EXTERN_MSC void GMT_fourt (float *data, int *nn, int ndim, int ksign, int iform, float *work);
EXTERN_MSC void GMT_grdproject_init (struct GRD_HEADER *head, double x_inc, double y_inc, int nx, int ny, int dpi, int offset);
EXTERN_MSC void GMT_grd_forward (float *geo, struct GRD_HEADER *g_head, float *rect, struct GRD_HEADER *r_head, double max_radius, BOOLEAN center);
EXTERN_MSC void GMT_grd_inverse (float *geo, struct GRD_HEADER *g_head, float *rect, struct GRD_HEADER *r_head, double max_radius, BOOLEAN center);
EXTERN_MSC void GMT_illuminate (double intensity, int *rgb);
EXTERN_MSC int GMT_get_rgb24 (double value, int *rgb);
EXTERN_MSC void GMT_grid_clip_on (struct GRD_HEADER *h, int rgb[], int flag);
EXTERN_MSC void GMT_map_clip_on (int rgb[], int flag);
EXTERN_MSC void GMT_map_clip_off (void);
EXTERN_MSC void GMT_grid_clip_off (void);
EXTERN_MSC void GMT_color_image (double x0, double y0, double x_side, double y_side, unsigned char *image, int nx, int ny);
EXTERN_MSC void GMT_init_fill (struct GMT_FILL *fill, int r, int g, int b);
EXTERN_MSC void GMT_fill_syntax (char option);
EXTERN_MSC void GMT_rgb_syntax (char option);
EXTERN_MSC int GMT_getfill (char *line, struct GMT_FILL *fill);
EXTERN_MSC int GMT_check_rgb (int *rgb);
EXTERN_MSC void GMT_azim_to_angle (double lon, double lat, double c, double azim, double *angle);
EXTERN_MSC int GMT_delaunay (double *x_in, double *y_in, int n, int **link);
EXTERN_MSC int GMT_sig_f (double chi1, int n1, double chi2, int n2, double level, double *prob);
EXTERN_MSC void GMT_grd_RI_verify (struct GRD_HEADER *h);
EXTERN_MSC double GMT_convert_units (char *from, int new_format);
EXTERN_MSC int GMT_get_unit (char c);
EXTERN_MSC BOOLEAN GMT_getpathname (char *name, char *path);
EXTERN_MSC char *GMT_getdefpath (int get);
EXTERN_MSC int GMT_getscale (char *text, double *x0, double *y0, double *scale_lat, double *length, char *measure, BOOLEAN *fancy, BOOLEAN *gave_xy);
