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
#include <math.h>

#include "../include/NIDS.h"
#include "get.h"
#include "image.h"
#include "linked_vector.h"
#include "error.h"

/*******************************************************************************
       Figure 3-7 Linked Vector Packet - Packet Code 6 (Sheet 1)
                   MSB       Uniform Value      LSB
                         PACKET CODE (=9)
                  LENGTH OF DATA BLOCK (BYTES)
                     VALUE (LEVEL) OF VECTOR
                         I STARTING POINT             1/4 Km
                         J STARTING POINT             Screen Coordinates
 DATA                 END I VECTOR NUMBER 1
 BLOCK                END J VECTOR NUMBER 1
                      END I VECTOR NUMBER 2
                      END J VECTOR NUMBER 2
                                   •
                                   •
       Figure 3-7 Linked Vector Packet - Packet Code 9 (Sheet 2)
*******************************************************************************/

/*******************************************************************************
	function to parse a value linked vector
	
args:
							buf			the buffer pointing to the first byte of the vector
							v				the struct to store the vector in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_v_linked_vector(char *buf, NIDS_v_vector *v, int value, int x_start, int y_start) {
	
	v->value = value;
	v->x_start = x_start;
	v->y_start = y_start;
	v->x_end = GET2(buf);
	v->y_end = GET2(buf + 2);

	return buf + 6;
}

/*******************************************************************************
	function to parse a value linked vector packet
	
args:					buf			the buffer pointing to the start of the value linked vector packet
							v				the structure to store the value linked vectors in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_v_linked_vector_header(char *buf, NIDS_v_linked_vectors *v) {
	int i;
	char *p;
	int value;
	int x_start;
	int y_start;
	
	v->length = GET2(buf);
	v->num_vectors = (v->length - 6) / 4 ;
	value = GET2(buf + 2);
	x_start = GET2(buf + 4);
	y_start = GET2(buf + 6);
	
	if (!(v->vectors = malloc(v->num_vectors * sizeof(NIDS_v_vector))))
		ERROR("parse_v_linked_vector_header");
	
	p = buf + 6;
	
	for (i = 0 ; i < v->num_vectors ; i++) {
		p = parse_v_linked_vector(p, v->vectors + i, value, x_start, y_start);
	}
	
	return p;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in value linked vector storage

args:
						v				the structure the vector is stored in

returns:
						nothing
*******************************************************************************/

void free_v_linked_vector_header(NIDS_v_linked_vectors *v) {
	
	free(v->vectors);
	
}

/*******************************************************************************
	function to print a single value linked vector

args:
						v				the structure the vector is stored in
						prefix	the start of the line
						rn			the vector number

returns:
						nothing
*******************************************************************************/

void print_v_linked_vector(NIDS_v_vector *v, char *prefix, int rn) {
	
	printf("%s.v_linked_vector.vectors[%i].value %i\n", prefix, rn, v->value);
	printf("%s.v_linked_vector.vectors[%i].x_start %i\n", prefix, rn, v->x_start);
	printf("%s.v_linked_vector.vectors[%i].y_start %i\n", prefix, rn, v->y_start);
	printf("%s.v_linked_vector.vectors[%i].x_end %i\n", prefix, rn, v->x_end);
	printf("%s.v_linked_vector.vectors[%i].y_end %i\n", prefix, rn, v->y_end);
	
}


/*******************************************************************************
	function to print a value linked vector packet

args:
						v				the structure the vectors is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_v_linked_vector_header(NIDS_v_linked_vectors *v, char *prefix) {
	int i;
	
	printf("%s.v_linked_vector.length %i\n", prefix, v->length);
	printf("%s.v_linked_vector.num_vectors %i\n", prefix, v->num_vectors);
	
	for (i = 0 ; i < v->num_vectors ; i++)
		print_v_linked_vector(v->vectors + i, prefix, i);
}

/*******************************************************************************
	fuction to draw a vector in an image

args:
						raster	pointer to the raster
						v				the structure that holds the vectors
						xcenter	the x axis center in the raster
						ycenter	the y axis center in the raster

returns:
						nothing
*******************************************************************************/

void v_linked_vectors_to_raster (
	NIDS_image *im,
	NIDS_v_linked_vectors *v)
{
	int i;

	
	for (i = 0 ; i < v->num_vectors ; i++) {
		draw_line(im,
							v->vectors[i].x_start,
							v->vectors[i].x_end,
							v->vectors[i].y_start,
							v->vectors[i].y_end,
							1);
	}
	
}
