/*--------------------------------------------------------------------
 *
 *   The GMT-system:	grdmath_explain.h [Generated by make_grdmath.x]
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
/*	grdmath_explain.h is automatically generated by make_grdmath.x;
 *	Do NOT edit manually!
 *
 */
		fprintf (stderr, "	ABS	1	abs (A).\n");
		fprintf (stderr, "	ACOS	1	acos (A).\n");
		fprintf (stderr, "	ACOSH	1	acosh (A).\n");
		fprintf (stderr, "	ADD(+)	2	A + B.\n");
		fprintf (stderr, "	AND	2	NaN if A and B == NaN, B if A == NaN, else A.\n");
		fprintf (stderr, "	ASIN	1	asin (A).\n");
		fprintf (stderr, "	ASINH	1	asinh (A).\n");
		fprintf (stderr, "	ATAN	1	atan (A).\n");
		fprintf (stderr, "	ATAN2	2	atan2 (A, B).\n");
		fprintf (stderr, "	ATANH	1	atanh (A).\n");
		fprintf (stderr, "	BEI	1	bei (A).\n");
		fprintf (stderr, "	BER	1	ber (A).\n");
		fprintf (stderr, "	CDIST	2	Cartesian distance between grid nodes and stack x,y.\n");
		fprintf (stderr, "	CEIL	1	ceil (A) (smallest integer >= A).\n");
		fprintf (stderr, "	COS	1	cos (A) (A in radians).\n");
		fprintf (stderr, "	COSD	1	cos (A) (A in degrees).\n");
		fprintf (stderr, "	COSH	1	cosh (A).\n");
		fprintf (stderr, "	CURV	1	Curvature of A (Laplacian).\n");
		fprintf (stderr, "	D2DX2	1	d^2(A)/dx^2 2nd derivative.\n");
		fprintf (stderr, "	D2DY2	1	d^2(A)/dy^2 2nd derivative.\n");
		fprintf (stderr, "	D2R	1	Converts Degrees to Radians.\n");
		fprintf (stderr, "	DDX	1	d(A)/dx 1st derivative.\n");
		fprintf (stderr, "	DDY	1	d(A)/dy 1st derivative.\n");
		fprintf (stderr, "	DILOG	1	Dilog (A).\n");
		fprintf (stderr, "	DIV(/)	2	A / B.\n");
		fprintf (stderr, "	DUP	1	Places duplicate of A on the stack.\n");
		fprintf (stderr, "	ERF	1	Error function of A.\n");
		fprintf (stderr, "	ERFC	1	Complimentory Error function of A.\n");
		fprintf (stderr, "	ERFINV	1	Inverse error function of A.\n");
		fprintf (stderr, "	EXCH	2	Exchanges A and B on the stack.\n");
		fprintf (stderr, "	EXP	1	exp (A).\n");
		fprintf (stderr, "	FLOOR	1	floor (A) (greatest integer <= A).\n");
		fprintf (stderr, "	FMOD	2	A %% B (remainder).\n");
		fprintf (stderr, "	GDIST	2	Great distance (in degrees) between grid nodes and stack lon,lat.\n");
		fprintf (stderr, "	HYPOT	2	hypot (A, B).\n");
		fprintf (stderr, "	I0	1	Modified Bessel function of A (1st kind, order 0).\n");
		fprintf (stderr, "	I1	1	Modified Bessel function of A (1st kind, order 1).\n");
		fprintf (stderr, "	IN	2	Modified Bessel function of A (1st kind, order B).\n");
		fprintf (stderr, "	INV	1	1 / A.\n");
		fprintf (stderr, "	J0	1	Bessel function of A (1st kind, order 0).\n");
		fprintf (stderr, "	J1	1	Bessel function of A (1st kind, order 1).\n");
		fprintf (stderr, "	JN	2	Bessel function of A (1st kind, order B).\n");
		fprintf (stderr, "	K0	1	Modified Kelvin function of A (2nd kind, order 0).\n");
		fprintf (stderr, "	K1	1	Modified Bessel function of A (2nd kind, order 1).\n");
		fprintf (stderr, "	KN	2	Modified Bessel function of A (2nd kind, order B).\n");
		fprintf (stderr, "	KEI	1	kei (A).\n");
		fprintf (stderr, "	KER	1	ker (A).\n");
		fprintf (stderr, "	LOG	1	log (A) (natural log).\n");
		fprintf (stderr, "	LOG10	1	log10 (A).\n");
		fprintf (stderr, "	LOG1P	1	log (1+A) (accurate for small A).\n");
		fprintf (stderr, "	MAX	2	Maximum of A and B.\n");
		fprintf (stderr, "	MEAN	1	Mean value of A.\n");
		fprintf (stderr, "	MED	1	Median value of A.\n");
		fprintf (stderr, "	MIN	2	Minimum of A and B.\n");
		fprintf (stderr, "	MUL(x)	2	A * B.\n");
		fprintf (stderr, "	NEG	1	-A.\n");
		fprintf (stderr, "	OR	2	NaN if A or B == NaN, else A.\n");
		fprintf (stderr, "	PLM	3	Associated Legendre polynomial P(-1<A<+1) degree B order C.\n");
		fprintf (stderr, "	POP	1	Delete top element from the stack.\n");
		fprintf (stderr, "	POW(^)	2	A ^ B.\n");
		fprintf (stderr, "	R2	2	R2 = A^2 + B^2.\n");
		fprintf (stderr, "	R2D	1	Convert Radians to Degrees.\n");
		fprintf (stderr, "	RINT	1	rint (A) (nearest integer).\n");
		fprintf (stderr, "	SIGN	1	sign (+1 or -1) of A.\n");
		fprintf (stderr, "	SIN	1	sin (A) (A in radians).\n");
		fprintf (stderr, "	SIND	1	sin (A) (A in degrees).\n");
		fprintf (stderr, "	SINH	1	sinh (A).\n");
		fprintf (stderr, "	SQRT	1	sqrt (A).\n");
		fprintf (stderr, "	STD	1	Standard deviation of A.\n");
		fprintf (stderr, "	STEPX	1	Heaviside step function in x: H(x-A).\n");
		fprintf (stderr, "	STEPY	1	Heaviside step function in y: H(y-A).\n");
		fprintf (stderr, "	SUB(-)	2	A - B.\n");
		fprintf (stderr, "	TAN	1	tan (A) (A in radians).\n");
		fprintf (stderr, "	TAND	1	tan (A) (A in degrees).\n");
		fprintf (stderr, "	TANH	1	tanh (A).\n");
		fprintf (stderr, "	Y0	1	Bessel function of A (2nd kind, order 0).\n");
		fprintf (stderr, "	Y1	1	Bessel function of A (2nd kind, order 1).\n");
		fprintf (stderr, "	YLM	2	Re and Im normalized surface harmonics (degree A, order B).\n");
		fprintf (stderr, "	YN	2	Bessel function of A (2nd kind, order B).\n");
