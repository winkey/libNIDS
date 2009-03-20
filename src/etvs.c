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
#include "etvs.h"
#include "error.h"


/*******************************************************************************
	function to parse a etvs packet
	
args:					buf			the buffer pointing to the start of the etvs packet
							p				the structure to store the etvss in

returns:
							etvser to the next byte in the buffer
*******************************************************************************/

char *parse_etvs_header(char *buf, NIDS_etvss *p) {
	int i;
	char *ptr;
	
	p->length = GET2(buf);
	p->num_points = p->length / 4;
	
	if (!(p->points = malloc(p->num_points * sizeof(NIDS_point))))
		ERROR("parse_etvs_header");
	
	ptr = buf + 2;
	
	for (i = 0 ; i < p->num_points ; i++) {
		ptr = parse_point(ptr, p->points + i);
	}
	
	return ptr;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in etvs storage

args:
						p				the structure the etvss are stored in

returns:
						nothing
*******************************************************************************/

void free_etvs_header(NIDS_etvss *p) {

	free(p->points);
	
}

/*******************************************************************************
	function to print a etvs packet

args:
						p				the structure the etvss are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_etvs_header(NIDS_etvss *p, char *prefix) {
	int i;
	char myprefix[PREFIX_LEN];
	
	printf("%s.etvs.length %i\n", prefix, p->length);
	printf("%s.etvs.num_points %i\n", prefix, p->num_points);
	
	for (i = 0 ; i < p->num_points ; i++) {
		snprintf(myprefix, PREFIX_LEN, "%s.etvs.points[%i]", prefix, i);
		print_point(p->points + i, prefix);
	}
	
}

/*******************************************************************************
	fuction to draw a etvs in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the etvs

returns:
						nothing
*******************************************************************************/

void etvs_to_raster (
	NIDS_image *im,
	NIDS_point *p)
{
	
	draw_invert_triangle(im, p->x_start, p->y_start, 10, 14, 5);
	
}

/*******************************************************************************
	fuction to draw etvss in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the etvss

returns:
						nothing
*******************************************************************************/

void etvss_to_raster (
	NIDS_image *im,
	NIDS_etvss *p)
{
	int i;
	
	for (i = 0; i < p->num_points ; i++)
		etvs_to_raster(im, p->points + i);
	
}
