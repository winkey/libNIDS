/*
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
#include <time.h>
#include <errno.h>

#include "get.h"
#include "../include/NIDS.h"
#include "image.h"
#include "v_vector.h"
#include "error.h"



/*******************************************************************************
3.7. Valued Unlinked Vector Packet

This is a set of unlinked vectors that plot disconnected lines on the plot. This contains a value level for coloring.
01 	Packet Code = 10 	Hex "A"
02 	Length of Data 	In bytes
03 	Value 	Color level of vector
04 	Vector 1, I Start 	Vector 1 start and end coordinates
05 	Vector 1, J Start 	 
06 	Vector 1, I End 	 
07 	Vector 1, J End 	 
08 	Vector 2, I Start 	Vector 2 start and end coordinates
09 	Vector 2, J Start 	 
10 	Vector 2, I End 	 
11 	Vector 2, J End 	 
12-## 	Repeated Vectors 	
*******************************************************************************/

/*******************************************************************************
	function to parse a valued unlinked vector
	
args:
							buf			the buffer pointing to the first byte of the vector
							v				the struct to store the vector in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_v_vector(char *buf, NIDS_v_vector *v, int value) {
	
	v->value = value;
	v->x_start = GET2(buf);
	v->y_start = GET2(buf + 2);
	v->x_end = GET2(buf + 4);
	v->y_end = GET2(buf + 6);

	return buf + 8;
}

/*******************************************************************************
	function to parse a valued unlinked vector packet
	
args:					buf			the buffer pointing to the start of the vector packet
							v				the structure to store the vectors in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_v_vector_header(char *buf, NIDS_v_vectors *v) {
	int i;
	char *p;
	int value;
	
	v->length = GET2(buf);
	
	value = GET2(buf + 2);
	
	v->num_vectors = (v->length - 2) / 8;
	if (!(v->vectors = malloc(v->num_vectors * sizeof(NIDS_v_vector))))
		ERROR("parse_v_vector_header");
	
	p = buf + 4;
	
	for (i = 0 ; i < v->num_vectors ; i++) {
		p = parse_v_vector(p, v->vectors + i, value);
	}
	
	return p;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in vector storage

args:
						v				the structure the vector is stored in

returns:
						nothing
*******************************************************************************/

void free_v_vector_header(NIDS_v_vectors *v) {

	free(v->vectors);
	
}

/*******************************************************************************
	function to print a single unlinked valued vector

args:
						v				the structure the vector is stored in
						prefix	the start of the line
						rn			the vector number

returns:
						nothing
*******************************************************************************/

void print_v_vector(NIDS_v_vector *v, char *prefix, int rn) {
	
	printf("%s.v_vector.vectors[%i].value %i\n", prefix, rn, v->value);
	printf("%s.v_vector.vectors[%i].x_start %i\n", prefix, rn, v->x_start);
	printf("%s.v_vector.vectors[%i].y_start %i\n", prefix, rn, v->y_start);
	printf("%s.v_vector.vectors[%i].x_end %i\n", prefix, rn, v->x_end);
	printf("%s.v_vector.vectors[%i].y_end %i\n", prefix, rn, v->y_end);

}

/*******************************************************************************
	function to print a unlinked valued vector packet

args:
						v				the structure the vectors is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_v_vector_header(NIDS_v_vectors *v, char *prefix) {
	int i;
	
	printf("%s.v_vector.length %i\n", prefix, v->length);
	printf("%s.v_vector.num_vectors %i\n", prefix, v->num_vectors);
	
	for (i = 0 ; i < v->num_vectors ; i++)
		print_v_vector(v->vectors + i, prefix, i);

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

void v_vectors_to_raster (
	NIDS_image *im,
	NIDS_v_vectors *v)
{
	int i;
	
	for (i = 0 ; i < v->num_vectors ; i++) {
		draw_line(im,
							v->vectors[i].x_start,
							v->vectors[i].x_end,
							v->vectors[i].y_start,
							v->vectors[i].y_end,
							v->vectors[i].value);
	}
	
}


