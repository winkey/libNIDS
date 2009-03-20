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
#include <errno.h>
#include <time.h>
#include <math.h>

#include "../include/NIDS.h"
#include "get.h"
#include "linked_vector.h"
#include "error.h"

#define RASTER_X_SIZE 4096
#define RASTER_Y_SIZE 4096

/*******************************************************************************
                    MSB       HALFWORD         LSB
                               No Value
                         PACKET CODE (=6)
                   LENGTH OF DATA BLOCK (BYTES)
                         I STARTING POINT             1/4 Km or
                         J STARTING POINT             Screen Coordinates
DATA                  END I VECTOR NUMBER 1
BLOCK                 END J VECTOR NUMBER 1
                      END I VECTOR NUMBER 2
                      END J VECTOR NUMBER 2
                                   •
                                   •
*******************************************************************************/


/*******************************************************************************
	function to parse a linked vector
	
args:
							buf			the buffer pointing to the first byte of the vector
							v				the struct to store the vector in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_linked_vector(char *buf, NIDS_vector *v, int x_start, int y_start) {
	
	v->x_start = x_start;
	v->y_start = y_start;
	v->x_end = GET2(buf);
	v->y_end = GET2(buf + 2);

	return buf + 4;
}

/*******************************************************************************
	function to parse a linked vector packet
	
args:					buf			the buffer pointing to the start of the linked vector packet
							v				the structure to store the linked vectors in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_linked_vector_header(char *buf, NIDS_linked_vectors *v) {
	int i;
	char *p;
	int x_start;
	int y_start;
	
	v->length = GET2(buf);
	v->num_vectors = (v->length - 4) / 4;
	x_start = GET2(buf + 2);
	y_start = GET2(buf + 4);
	
	if (!(v->vectors = malloc(v->num_vectors * sizeof(NIDS_vector))))
		ERROR("parse_linked_vector_header");
	
	p = buf + 6;
	
	for (i = 0 ; i < v->num_vectors ; i++) {
		p = parse_linked_vector(p, v->vectors + i, x_start, y_start);
	}
	
	return p;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in linked vector storage

args:
						v				the structure the vector is stored in

returns:
						nothing
*******************************************************************************/

void free_linked_vector_header(NIDS_linked_vectors *v) {
	
	free(v->vectors);
	
}

/*******************************************************************************
	function to print a single unlinked vector

args:
						v				the structure the vector is stored in
						prefix	the start of the line
						rn			the vector number

returns:
						nothing
*******************************************************************************/

void print_linked_vector(NIDS_vector *v, char *prefix, int rn) {
	
	printf("%s.linked_vector.vectors[%i].x_start %i\n", prefix, rn, v->x_start);
	printf("%s.linked_vector.vectors[%i].y_start %i\n", prefix, rn, v->y_start);
	printf("%s.linked_vector.vectors[%i].x_end %i\n", prefix, rn, v->x_end);
	printf("%s.linked_vector.vectors[%i].y_end %i\n", prefix, rn, v->y_end);
	
}


/*******************************************************************************
	function to print a linked vector packet

args:
						v				the structure the vectors is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_linked_vector_header(NIDS_linked_vectors *v, char *prefix) {
	int i;
	
	printf("%s.linked_vector.length %i\n", prefix, v->length);
	printf("%s.linked_vector.num_vectors %i\n", prefix, v->num_vectors);
	
	for (i = 0 ; i < v->num_vectors ; i++)
		print_linked_vector(v->vectors + i, prefix, i);
}
