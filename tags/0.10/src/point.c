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
#include "point.h"
#include "error.h"

/*******************************************************************************
                             MSB               HALFWORD                      LSB
                                         PACKET CODE (=12 or 26)
   TVS or ETVS                         LENGTH OF BLOCK (BYTES)
   REPEAT FOR                                  I POSITION
   EACH SYMBOL                                 J POSITION
                             MSB               HALFWORD                      LSB
                                           PACKET CODE (=13)
   HAIL POSITIVE
   (FILLED)                            LENGTH OF BLOCK (BYTES)
   REPEAT FOR                                  I POSITION
   EACH SYMBOL                                 J POSITION
                             MSB               HALFWORD                      LSB
                                           PACKET CODE (=14)
   HAIL PROBABLE                       LENGTH OF BLOCK (BYTES)
   REPEAT FOR                                  I POSITION
   EACH SYMBOL                                 J POSITION
Figure 3-14. Special Graphic Symbol Packet - Packet Code 3 or 11, 12 or 26, 13 and 14
                                     (Sheet 1)

*******************************************************************************/

/*******************************************************************************
	function to parse a single point
	
args:
							buf			the buffer pointing to the first byte of the point
							p				the struct to store the point in;

returns:
							pointer to the next byte in the buffer 
*******************************************************************************/

char *parse_point (char *buf, NIDS_point *p) {
	p->x_start = GET2(buf);
	p->y_start = GET2(buf + 2);

	return buf + 4;
}

/*******************************************************************************
	function to parse a point packet
	
args:					buf			the buffer pointing to the start of the point packet
							p				the structure to store the points in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_point_header(char *buf, NIDS_points *p) {
	int i;
	char *ptr;
	
	p->length = GET2(buf);
	p->num_points = p->length / 4;
	
	if (!(p->points = malloc(p->num_points * sizeof(NIDS_point))))
		ERROR("parse_point_header");
	
	ptr = buf + 2;
	
	for (i = 0 ; i < p->num_points ; i++) {
		ptr = parse_point(ptr, p->points + i);
	}
	
	return ptr;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in point storage

args:
						p				the structure the points are stored in

returns:
						nothing
*******************************************************************************/

void free_point_header(NIDS_points *p) {

	free(p->points);
	
}

/*******************************************************************************
	function to print a single point

args:
						p				the structure the point is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_point(NIDS_point *p, char *prefix) {
	
	printf("%s.x_start %i\n", prefix, p->x_start);
	printf("%s.y_start %i\n", prefix, p->y_start);
	
}
	
/*******************************************************************************
	function to print a point packet

args:
						p				the structure the points are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_point_header(NIDS_points *p, char *prefix) {
	int i;
	char myprefix[PREFIX_LEN];
	
	printf("%s.point.length %i\n", prefix, p->length);
	printf("%s.point.num_points %i\n", prefix, p->num_points);
	
	for (i = 0 ; i < p->num_points ; i++) {
		snprintf(myprefix, PREFIX_LEN, "%s.point.points[%i]", prefix, i);
		print_point(p->points + i, myprefix);
	}
	
}

