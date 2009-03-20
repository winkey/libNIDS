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
#include "circle.h"



/*******************************************************************************
	function to parse a single circle
	
args:
							buf			the buffer pointing to the first byte of the circle
							c				the struct to store the circle in;

returns:
							pointer to the next byte in the buffer 
*******************************************************************************/

char *parse_circle (char *buf, NIDS_circle *c) {
	c->x_start = GET2(buf);
	c->y_start = GET2(buf + 2);
	c->radius = GET2(buf + 4);
	
	return buf + 6;
}

/*******************************************************************************
	function to parse a circle packet
	
args:					buf			the buffer pointing to the start of the circle packet
							c				the structure to store the circles in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_circle_header(char *buf, NIDS_circles *c) {
	int i;
	char *p;
	
	c->length = GET2(buf);
	c->num_circles = c->length / 6;
	if (!(c->circles = malloc(c->num_circles * sizeof(NIDS_circle)))) {
		fprintf(stderr, "ERROR: parse_circle_header : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	p = buf + 2;
	
	for (i = 0 ; i < c->num_circles ; i++) {
		p = parse_circle(p, c->circles + i);
	}
	
	return p;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in circle storage

args:
						c			the structure the circles are stored in

returns:
						nothing
*******************************************************************************/

void free_circle_header(NIDS_circles *c) {

	free(c->circles);
	
}

/*******************************************************************************
	function to print a single circle

args:
						c				the structure the circle is stored in
						ln			the layer number
						rn			the arrow number

returns:
						nothing
*******************************************************************************/

void print_circle(NIDS_circle *c, int ln, int rn) {
	
	printf("data.symb.layers[%i].circle.circles[%i].x_start %i\n", ln, rn, c->x_start);
	printf("data.symb.layers[%i].circle.circles[%i].y_start %i\n", ln, rn, c->y_start);
	printf("data.symb.layers[%i].circle.circles[%i].radius %i\n", ln, rn, c->radius);
	
}
	
/*******************************************************************************
	function to print a circle packet

args:
						c				the structure the circles are stored in
						ln			the layer number

returns:
						nothing
*******************************************************************************/

void print_circle_header(NIDS_circles *c, int ln) {
	int i;
	
	printf("data.symb.layers[%i].circle.length %i\n", ln, c->length);
	printf("data.symb.layers[%i].circle.num_circles %i\n", ln, c->num_circles);
	
	for (i = 0 ; i < c->num_circles ; i++)
		print_circle(c->circles + i, ln, i);
	
}
