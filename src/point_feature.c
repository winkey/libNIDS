/*
 * libNIDS
 * Copyright (C) Brian Case 2008 <rush@gisweather.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Library General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor Boston, MA 02110-1301,  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include "../include/NIDS.h"
#include "get.h"
#include "image.h"
#include "point_feature.h"
#include "error.h"

/*******************************************************************************

                                   MSB                  HALFWORD                       LSB
                                                   PACKET CODE (=20)
                                               LENGTH OF BLOCK (BYTES)
           REPEAT FOR                                   I POSITION
           EACH SYMBOL                                  J POSITION
                                                  POINT FEATURE TYPE
                                               POINT FEATURE ATTRIBUTE
FIELDNAME      TYPE      UNITS      RANGE      PRECISION/    REMARKS
                                               ACCURACY
Packet Code    INT*2     N/A        20         N/A           Packet Type (Note 1)
Length of      INT*2     Bytes      8 to       1             Number of bytes in block not including self
Block                               32760                    or packet code
I Position     INT*2      Km/4      -2048 to   1             I starting coordinate
                                    +2047
J Position     INT*2     Km/4       -2048 to   1             J starting coordinate
                                    +2047
Point          INT*2     N/A        1 to 4, 5  1             1 = mesocyclone (extrapolated)
Feature                             to 8, 9-11               2 = 3-D correlated shear (extrapolated)
Type                                                         3 = mesocyclone (persistent, new, or
                                                             increasing)
                                                             4 = 3-D correlated shear (persistent,
                                                             increasing, or new)
                                                             5 = TVS (extrapolated)
                                                             6 = ETVS (extrapolated)
                                                             7 = TVS (persistent, new, or increasing)
                                                             8 = ETVS (persistent, new, or increasing)
                                                             9 = MDA Circulation with Strength Rank >=
                                                             5 AND with a Base Height <= 1 km ARL or
                                                             with its Base on the lowest elevation angle.
                                                             10 = MDA Circulation with Strength Rank
                                                             >= 5 AND with a Base Height > 1 km ARL
                                                             AND that Base is not on the lowest elevation
                                                             angle.
                                                             11 = MDA Circulation with Strength Rank <
                                                             5
Point          INT*2     Type       Type       Type          For feature types 1-4, 9, 10, 11, radius in
Feature                  depende    depende    dependent,    km/4
Attribute                nt, see    nt, see    see
                         remarks.   remarks.   remarks.
             Figure 3-14. Special Graphic Symbol Packet - Packet Code 20 (Sheet 4)
*******************************************************************************/

/*******************************************************************************
	function to parse a single point feature
	
args:
							buf			the buffer pointing to the first byte of the point feature
							p				the struct to store the point feature in;

returns:
							pointer to the next byte in the buffer 
*******************************************************************************/

char *parse_point_feature (char *buf, NIDS_point_feature *p) {
	p->x_start = GET2(buf);
	p->y_start = GET2(buf + 2);
	p->type = GET2(buf + 4);
	p->attr = GET2(buf + 6);
	

	return buf + 8;
}

/*******************************************************************************
	function to parse a point feature packet
	
args:					buf			the buffer pointing to the start of the point featurepacket
							p				the structure to store the points in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_point_feature_header(char *buf, NIDS_point_features *p) {
	int i;
	char *ptr;
	
	p->length = GET2(buf);
	p->num_points = p->length / 8;
	
	if (!(p->points = malloc(p->num_points * sizeof(NIDS_point_feature))))
		ERROR("parse_point_feature_header");
	
	ptr = buf + 2;
	
	for (i = 0 ; i < p->num_points ; i++) {
		ptr = parse_point_feature(ptr, p->points + i);
	}
	
	return ptr;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in point feature storage

args:
						p				the structure the points are stored in

returns:
						nothing
*******************************************************************************/

void free_point_feature_header(NIDS_point_features *p) {

	free(p->points);
	
}

/*******************************************************************************
	function to print a single point feature

args:
						p				the structure the point is stored in
						prefix	the start of the line
						rn			the point number

returns:
						nothing
*******************************************************************************/

void print_point_feature(NIDS_point_feature *p, char *prefix, int rn) {
	
	printf("%s.point_feature.points[%i].x_start %i\n", prefix, rn, p->x_start);
	printf("%s.point_feature.points[%i].y_start %i\n", prefix, rn, p->y_start);
	printf("%s.point_feature.points[%i].type %i\n", prefix, rn, p->type);
	printf("%s.point_feature.points[%i].attr %i\n", prefix, rn, p->attr);
	
}
	
/*******************************************************************************
	function to print a point feature packet

args:
						p				the structure the points are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_point_feature_header(NIDS_point_features *p, char *prefix) {
	int i;
	
	printf("%s.point_feature.length %i\n", prefix, p->length);
	printf("%s.point_feature.num_points %i\n", prefix, p->num_points);
	
	for (i = 0 ; i < p->num_points ; i++)
		print_point_feature(p->points + i, prefix, i);
	
}

/*******************************************************************************
	fuction to draw a point feature in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the point feature

returns:
						nothing
*******************************************************************************/

void point_feature_to_raster (
	NIDS_image *im,
	NIDS_point_feature *p)
{
	int i, j;
	
	switch (p->type) {
		case 1:
			/***** dashed yellow *****/
			for (i = p->attr; i > p->attr + 4; i--) {
				draw_circle(im, p->x_start, p->y_start, i, 4);
			}
			break;
		case 2:
			/***** dahed yellow *****/
			draw_circle(im, p->x_start, p->y_start, p->attr, 4);
			break;
		case 3:
			/***** yellow *****/
			for (i = p->attr; i > p->attr + 4; i--) {
				draw_circle(im, p->x_start, p->y_start, i, 4);
			}
			break;
		case 4:
			/***** yellow *****/
			draw_circle(im, p->x_start, p->y_start, p->attr, 4);
			break;
		case 5:
		case 7:
			/***** red *****/
			for (i = 10, j = 14 ; i > 0; i--, j--) {
				draw_invert_triangle(im, p->x_start, p->y_start, i, j, 5);
			}
			break;
		case 6:
		case 8:
			/***** red *****/
			draw_invert_triangle(im, p->x_start, p->y_start, 10, 14, 5);
			break;
		case 9:
			/***** yellow spikes *****/
			for (i = p->attr; i > p->attr + 4; i--) {
				draw_circle(im, p->x_start, p->y_start, i, 4);
			}
			break;
		case 10:
			/***** yellow *****/
			for (i = p->attr; i > p->attr + 4; i--) {
				draw_circle(im, p->x_start, p->y_start, i, 4);
			}
			break;
		case 11:
			/***** yellow *****/
			draw_circle(im, p->x_start, p->y_start, p->attr, 4);
			break;
	
	}
	
}

/*******************************************************************************
	fuction to draw point feature in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the point features

returns:
						nothing
*******************************************************************************/

void point_features_to_raster (
	NIDS_image *im,
	NIDS_point_features *p)
{
	int i;
	
	for (i = 0; i < p->num_points ; i++)
		point_feature_to_raster(im, p->points + i);
	
}
