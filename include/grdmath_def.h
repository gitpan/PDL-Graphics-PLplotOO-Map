/*--------------------------------------------------------------------
 *
 *   The GMT-system:	gmtmath_def.h [Generated by make_grdmath.x]
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
/*	grdmath_def.h is automatically generated by make_grdmath.x;
 *	Do NOT edit manually!
 *
 * For backward compatibility:
 */

#define ADD	3
#define DIV	24
#define MUL	54
#define RAISE	59	/* (POW) */
#define SUB	71

/* Declare all functions to return int */

void grd_ABS(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 00 */
void grd_ACOS(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 01 */
void grd_ACOSH(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 02 */
void grd_ADD(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 03 */
void grd_AND(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 04 */
void grd_ASIN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 05 */
void grd_ASINH(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 06 */
void grd_ATAN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 07 */
void grd_ATAN2(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 08 */
void grd_ATANH(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 09 */
void grd_BEI(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 10 */
void grd_BER(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 11 */
void grd_CDIST(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 12 */
void grd_CEIL(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 13 */
void grd_COS(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 14 */
void grd_COSD(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 15 */
void grd_COSH(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 16 */
void grd_CURV(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 17 */
void grd_D2DX2(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 18 */
void grd_D2DY2(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 19 */
void grd_D2R(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 20 */
void grd_DDX(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 21 */
void grd_DDY(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 22 */
void grd_DILOG(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 23 */
void grd_DIV(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 24 */
void grd_DUP(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 25 */
void grd_ERF(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 26 */
void grd_ERFC(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 27 */
void grd_ERFINV(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 28 */
void grd_EXCH(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 29 */
void grd_EXP(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 30 */
void grd_FLOOR(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 31 */
void grd_FMOD(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 32 */
void grd_GDIST(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 33 */
void grd_HYPOT(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 34 */
void grd_I0(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 35 */
void grd_I1(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 36 */
void grd_IN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 37 */
void grd_INV(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 38 */
void grd_J0(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 39 */
void grd_J1(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 40 */
void grd_JN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 41 */
void grd_K0(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 42 */
void grd_K1(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 43 */
void grd_KN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 44 */
void grd_KEI(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 45 */
void grd_KER(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 46 */
void grd_LOG(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 47 */
void grd_LOG10(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 48 */
void grd_LOG1P(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 49 */
void grd_MAX(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 50 */
void grd_MEAN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 51 */
void grd_MED(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 52 */
void grd_MIN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 53 */
void grd_MUL(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 54 */
void grd_NEG(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 55 */
void grd_OR(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 56 */
void grd_PLM(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 57 */
void grd_POP(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 58 */
void grd_POW(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 59 */
void grd_R2(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 60 */
void grd_R2D(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 61 */
void grd_RINT(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 62 */
void grd_SIGN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 63 */
void grd_SIN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 64 */
void grd_SIND(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 65 */
void grd_SINH(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 66 */
void grd_SQRT(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 67 */
void grd_STD(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 68 */
void grd_STEPX(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 69 */
void grd_STEPY(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 70 */
void grd_SUB(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 71 */
void grd_TAN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 72 */
void grd_TAND(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 73 */
void grd_TANH(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 74 */
void grd_Y0(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 75 */
void grd_Y1(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 76 */
void grd_YLM(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 77 */
void grd_YN(float *stack[], BOOLEAN *constant, double *factor, int last);		/* id = 78 */

/* Declare operator array */

char *operator[N_OPERATORS] = {
	"ABS",		/* id = 00 */
	"ACOS",		/* id = 01 */
	"ACOSH",		/* id = 02 */
	"ADD",		/* id = 03 */
	"AND",		/* id = 04 */
	"ASIN",		/* id = 05 */
	"ASINH",		/* id = 06 */
	"ATAN",		/* id = 07 */
	"ATAN2",		/* id = 08 */
	"ATANH",		/* id = 09 */
	"BEI",		/* id = 10 */
	"BER",		/* id = 11 */
	"CDIST",		/* id = 12 */
	"CEIL",		/* id = 13 */
	"COS",		/* id = 14 */
	"COSD",		/* id = 15 */
	"COSH",		/* id = 16 */
	"CURV",		/* id = 17 */
	"D2DX2",		/* id = 18 */
	"D2DY2",		/* id = 19 */
	"D2R",		/* id = 20 */
	"DDX",		/* id = 21 */
	"DDY",		/* id = 22 */
	"DILOG",		/* id = 23 */
	"DIV",		/* id = 24 */
	"DUP",		/* id = 25 */
	"ERF",		/* id = 26 */
	"ERFC",		/* id = 27 */
	"ERFINV",		/* id = 28 */
	"EXCH",		/* id = 29 */
	"EXP",		/* id = 30 */
	"FLOOR",		/* id = 31 */
	"FMOD",		/* id = 32 */
	"GDIST",		/* id = 33 */
	"HYPOT",		/* id = 34 */
	"I0",		/* id = 35 */
	"I1",		/* id = 36 */
	"IN",		/* id = 37 */
	"INV",		/* id = 38 */
	"J0",		/* id = 39 */
	"J1",		/* id = 40 */
	"JN",		/* id = 41 */
	"K0",		/* id = 42 */
	"K1",		/* id = 43 */
	"KN",		/* id = 44 */
	"KEI",		/* id = 45 */
	"KER",		/* id = 46 */
	"LOG",		/* id = 47 */
	"LOG10",		/* id = 48 */
	"LOG1P",		/* id = 49 */
	"MAX",		/* id = 50 */
	"MEAN",		/* id = 51 */
	"MED",		/* id = 52 */
	"MIN",		/* id = 53 */
	"MUL",		/* id = 54 */
	"NEG",		/* id = 55 */
	"OR",		/* id = 56 */
	"PLM",		/* id = 57 */
	"POP",		/* id = 58 */
	"POW",		/* id = 59 */
	"R2",		/* id = 60 */
	"R2D",		/* id = 61 */
	"RINT",		/* id = 62 */
	"SIGN",		/* id = 63 */
	"SIN",		/* id = 64 */
	"SIND",		/* id = 65 */
	"SINH",		/* id = 66 */
	"SQRT",		/* id = 67 */
	"STD",		/* id = 68 */
	"STEPX",		/* id = 69 */
	"STEPY",		/* id = 70 */
	"SUB",		/* id = 71 */
	"TAN",		/* id = 72 */
	"TAND",		/* id = 73 */
	"TANH",		/* id = 74 */
	"Y0",		/* id = 75 */
	"Y1",		/* id = 76 */
	"YLM",		/* id = 77 */
	"YN"		/* id = 78 */
};

