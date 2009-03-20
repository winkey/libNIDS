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
#include "hail_probable.h"
#include "error.h"


/*******************************************************************************
	function to parse a hail_probable packet
	
args:					buf			the buffer pointing to the start of the hail_probable packet
							p				the structure to store the hail_probables in

returns:
							hail_probableer to the next byte in the buffer
*******************************************************************************/

char *parse_hail_probable_header(char *buf, NIDS_hail_probables *p) {
	int i;
	char *ptr;
	
	p->length = GET2(buf);
	p->num_points = p->length / 4;
	
	if (!(p->points = malloc(p->num_points * sizeof(NIDS_point))))
		ERROR("parse_hail_probable_header");
	
	ptr = buf + 2;
	
	for (i = 0 ; i < p->num_points ; i++) {
		ptr = parse_point(ptr, p->points + i);
	}
	
	return ptr;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in hail_probable storage

args:
						p				the structure the hail_probables are stored in

returns:
						nothing
*******************************************************************************/

void free_hail_probable_header(NIDS_hail_probables *p) {

	free(p->points);
	
}

/*******************************************************************************
	function to print a hail_probable packet

args:
						p				the structure the hail_probables are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_hail_probable_header(NIDS_hail_probables *p, char *prefix) {
	int i;
	char myprefix[PREFIX_LEN];
	
	printf("%s.hail_probable.length %i\n", prefix, p->length);
	printf("%s.hail_probable.num_points %i\n", prefix, p->num_points);
	
	for (i = 0 ; i < p->num_points ; i++) {
		snprintf(myprefix, PREFIX_LEN, "%s.hail_probable.points[%i]", prefix, i);
		print_point(p->points + i, prefix);
	}
	
}

/*******************************************************************************
	fuction to draw a hail_probable in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the hail_probable

returns:
						nothing
*******************************************************************************/

void hail_probable_to_raster (
	NIDS_image *im,
	NIDS_point *p)
{

	draw_triangle(im, p->x_start, p->y_start, 8, 12, 3);
	
}

/*******************************************************************************
	fuction to draw hail_probables in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the hail_probables

returns:
						nothing
*******************************************************************************/

void hail_probables_to_raster (
	NIDS_image *im,
	NIDS_hail_probables *p)
{
	int i;
	
	for (i = 0; i < p->num_points ; i++)
		hail_probable_to_raster(im, p->points + i);
	
}
