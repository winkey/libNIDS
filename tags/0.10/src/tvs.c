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
#include "point.h"
#include "tvs.h"
#include "error.h"


/*******************************************************************************
	function to parse a tvs packet
	
args:					buf			the buffer pointing to the start of the tvs packet
							p				the structure to store the tvss in

returns:
							tvser to the next byte in the buffer
*******************************************************************************/

char *parse_tvs_header(char *buf, NIDS_tvss *p) {
	int i;
	char *ptr;
	
	p->length = GET2(buf);
	p->num_points = p->length / 4;
	
	if (!(p->points = malloc(p->num_points * sizeof(NIDS_point))))
		ERROR("parse_tvs_header");
	
	ptr = buf + 2;
	
	for (i = 0 ; i < p->num_points ; i++) {
		ptr = parse_point(ptr, p->points + i);
	}
	
	return ptr;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in tvs storage

args:
						p				the structure the tvss are stored in

returns:
						nothing
*******************************************************************************/

void free_tvs_header(NIDS_tvss *p) {

	free(p->points);
	
}

/*******************************************************************************
	function to print a tvs packet

args:
						p				the structure the tvss are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_tvs_header(NIDS_tvss *p, char *prefix) {
	int i;
	char myprefix[PREFIX_LEN];
	
	printf("%s.tvs.length %i\n", prefix, p->length);
	printf("%s.tvs.num_points %i\n", prefix, p->num_points);
	
	for (i = 0 ; i < p->num_points ; i++) {
		snprintf(myprefix, PREFIX_LEN, "%s.tvs.points[%i]", prefix, i);
		print_point(p->points + i, prefix);
	}
	
}

/*******************************************************************************
	fuction to draw a tvs in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the tvs

returns:
						nothing
*******************************************************************************/

void tvs_to_raster (
	NIDS_image *im,
	NIDS_point *p)
{
	int i, j;
	
	for (i = 10, j = 14 ; i > 0; i--, j--) {
		draw_invert_triangle(im, p->x_start, p->y_start, i, j, 5);
	}
	
}

/*******************************************************************************
	fuction to draw tvss in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the tvss

returns:
						nothing
*******************************************************************************/

void tvss_to_raster (
	NIDS_image *im,
	NIDS_tvss *p)
{
	int i;
	
	for (i = 0; i < p->num_points ; i++)
		tvs_to_raster(im, p->points + i);
	
}
