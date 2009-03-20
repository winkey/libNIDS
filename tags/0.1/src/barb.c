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
#include "barb.h"

/*******************************************************************************
Individual Barbs

Each barb is 5 halfwords and is repeated until byte length.
01 	Value 	Color value (0-5) representing RMS variation of wind speed
02 	I Coordinate 	 
03 	J Coordinate 	 
04 	Direction of Wind 	In degrees
05 	Wind Speed 	
*******************************************************************************/

/*******************************************************************************
	function to parse a single barb
	
args:
							buf			the buffer pointing to the first byte of the barb
							b				the struct to store the barb in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_barb (char *buf, NIDS_barb *b) {

	b->value = GET2(buf);
	b->x_start = GET2(buf + 2);
	b->y_start = GET2(buf + 4);
	b->heading = GET2(buf + 6);
	b->speed = GET2(buf + 8);
	
	return buf + 10;
}

/*******************************************************************************
3.4. Wind Barb Packet

These define the location and size of wind barbs to be plotted.
01 	Packet Code = 4 	Hex "4"
02 	Length of Data 	In bytes
*******************************************************************************/

/*******************************************************************************
	function to parse a barb packet
	
args:					buf			the buffer pointing to the start of the barb packet
							b				the structure to store the barbs in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_barb_header(char *buf, NIDS_barbs *b) {
	int i;
	char *p;
	
	b->length = GET2(buf);
	b->num_barbs = b->length / 10;
	if (!(b->barbs = malloc(b->num_barbs * sizeof(NIDS_barb)))) {
		fprintf(stderr, "ERROR: parse_barb_header : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	p = buf + 2;
	
	for (i = 0 ; i < b->num_barbs ; i++) {
		p = parse_barb(p, b->barbs + i);
	}
	
	return p;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in barb storage

args:
						b				the structure the barbs are stored in

returns:
						nothing
*******************************************************************************/

void free_barb_header(NIDS_barbs *b) {
	
	free(b->barbs);
	
}

/*******************************************************************************
	function to print a single barb

args:
						b				the structure the barb is stored in
						ln			the layer number
						rn			the barb number

returns:
						nothing
*******************************************************************************/

void print_barb(NIDS_barb *b, int ln, int rn) {
	
	printf("data.symb.layers[%i].barb.barbs[%i].value %i\n", ln, rn, b->value);
	printf("data.symb.layers[%i].barb.barbs[%i].x_start %i\n", ln, rn, b->x_start);
	printf("data.symb.layers[%i].barb.barbs[%i].y_start %i\n", ln, rn, b->y_start);
	printf("data.symb.layers[%i].barb.barbs[%i].heading %i\n", ln, rn, b->heading);
	printf("data.symb.layers[%i].barb.barbs[%i].speed %i\n", ln, rn, b->speed);
	
}

/*******************************************************************************
	function to print a barb packet

args:
						b				the structure the barbs are stored in
						ln			the layer number

returns:
						nothing
*******************************************************************************/

void print_barb_header(NIDS_barbs *b, int ln) {
	int i;
	
	printf("data.symb.layers[%i].barbs.length %i\n", ln, b->length);
	printf("data.symb.layers[%i].barbs.num_barbs %i\n", ln, b->num_barbs);
	
	for (i = 0 ; i < b->num_barbs ; i++)
		print_barb(b->barbs + i, ln, i);
	
}
