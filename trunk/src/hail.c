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

#include "get.h"
#include "image.h"
#include "../include/NIDS.h"
#include "hail.h"
#include "error.h"

/*******************************************************************************
            MSB        HALFWORD         LSB
                    PACKET CODE (=19)
HDA HAIL        LENGTH OF BLOCK (BYTES)
REPEAT FOR             I POSITION
EACH SYMBOL            J POSITION
                      PROB. OF HAIL
                  PROB. OF SEVERE HAIL
                      MAX HAIL SIZE
*******************************************************************************/

/*******************************************************************************
	function to parse a single hail
	
args:
							buf			the buffer pointing to the first byte of the hail
							h				the struct to store the hail in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_hail (char *buf, NIDS_hail *h) {

	h->x_start = GET2(buf);
	h->y_start = GET2(buf + 2);
	h->prob = GET2(buf + 4);
	h->severe = GET2(buf + 6);
	h->max_size = GET2(buf + 8);
	
	return buf + 10;
}

/*******************************************************************************
	function to parse a hail packet
	
args:					buf			the buffer pointing to the start of the hail packet
							b				the structure to store the hails in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_hail_header(char *buf, NIDS_hails *h) {
	int i;
	char *p;
	
	h->length = GET2(buf);
	h->num_hails = h->length / 10;
	
	if (!(h->hails = malloc(h->num_hails * sizeof(NIDS_hail))))
		ERROR("parse_hail_header");
	
	p = buf + 2;
	
	for (i = 0 ; i < h->num_hails ; i++) {
		p = parse_hail(p, h->hails + i);
	}
	
	return p;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in hail storage

args:
						h				the structure the hails are stored in

returns:
						nothing
*******************************************************************************/

void free_hail_header(NIDS_hails *h) {
	
	free(h->hails);
	
}

/*******************************************************************************
	function to print a single hail

args:
						h				the structure the hail is stored in
						prefix	the start of the line
						rn			the barb number

returns:
						nothing
*******************************************************************************/

void print_hail(NIDS_hail *h, char *prefix, int rn) {
	
	printf("%s.hail.hails[%i].x_start %i\n", prefix, rn, h->x_start);
	printf("%s.hail.hails[%i].y_start %i\n", prefix, rn, h->y_start);
	printf("%s.hail.hails[%i].prob %i\n", prefix, rn, h->prob);
	printf("%s.hail.hails[%i].severe %i\n", prefix, rn, h->severe);
	printf("%s.hail.hails[%i].max_size %i\n", prefix, rn, h->max_size);
	
}

/*******************************************************************************
	function to print a hail packet

args:
						h				the structure the hails are stored in
						prefix	the start of the line
returns:
						nothing
*******************************************************************************/

void print_hail_header(NIDS_hails *h, char *prefix) {
	int i;
	
	printf("%s.hail.length %i\n", prefix, h->length);
	printf("%s.hail.num_hails %i\n", prefix, h->num_hails);
	
	for (i = 0 ; i < h->num_hails ; i++)
		print_hail(h->hails + i, prefix, i);
	
}

/*******************************************************************************
	fuction to draw a hail in an image

args:
						im			pointer to the raster struct
						h				the structure that holds the hail

returns:
						nothing
*******************************************************************************/

void hail_to_raster (
	NIDS_image *im,
	NIDS_hail *h)
{
	int i, j;
	
	if (h->severe >= 50) {
		for (i = 16, j = 20 ; i > 0; i--, j--) {
			draw_triangle(im, h->x_start, h->y_start, i, j, 3);
		}
	}
	else if (h->severe >= 30) {
		draw_triangle(im, h->x_start, h->y_start, 16, 20, 3);
	}
	else if (h->prob >= 50) {
		for (i = 8, j = 12 ; i > 0; i--, j--) {
			draw_triangle(im, h->x_start, h->y_start, i, j, 3);
		}
	}
	else if (h->severe >= 30) {
		draw_triangle(im, h->x_start, h->y_start, 8, 12, 3);
	}
	
}

/*******************************************************************************
	fuction to draw hails in an image

args:
						im			pointer to the raster struct
						h				the structure that holds the hails

returns:
						nothing
*******************************************************************************/

void hails_to_raster (
	NIDS_image *im,
	NIDS_hails *h)
{
	int i;
	
	for (i = 0; i < h->num_hails ; i++)
		hail_to_raster(im, h->hails + i);
	
}
