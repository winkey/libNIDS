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
#include "arrow.h"

/*******************************************************************************

Individual Vectors

Each arrow is 5 halfwords and is repeated until byte length.
01 	I Coordinate 	 
02 	J Coordinate 	 
03 	Direction of Arrow 	In degrees
04 	Arrow Length 	In pixels
05 	Arrow Head Length 	In pixels
*******************************************************************************/

/*******************************************************************************
	function to parse a single arrow
	
args:
							buf			the buffer pointing to the first byte of the arrow
							a				the struct to store the arrow in;

returns:
							pointer to the next byte in the buffer 
*******************************************************************************/

char *parse_arrow (char *buf, NIDS_arrow *a) {
	a->x_start = GET2(buf);
	a->y_start = GET2(buf + 2);
	a->heading = GET2(buf + 4);
	a->length = GET2(buf + 6);
	a->head_length = GET2(buf + 8);

	return buf + 10;
}


/*******************************************************************************
3.3. Vector Arrow Packet

These define the location and size of vector arrows to be plotted.
01 	Packet Code = 5 	Hex "5"
02 	Length of Data 	In bytes
*******************************************************************************/

/*******************************************************************************
	function to parse an arrow packet
	
args:					buf			the buffer pointing to the start of the arrow packet
							a				the structure to store the arrows in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_arrow_header(char *buf, NIDS_arrows *a) {
	int i;
	char *p;
	
	a->length = GET2(buf);
	a->num_arrows = a->length / 10;
	if (!(a->arrows = malloc(a->num_arrows * sizeof(NIDS_arrow)))) {
		fprintf(stderr, "ERROR: parse_arrow_header : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	p = buf + 2;
	
	for (i = 0 ; i < a->num_arrows ; i++) {
		p = parse_arrow(p, a->arrows + i);
	}
	
	return p;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in arrow storage

args:
						a				the structure the arrows are stored in

returns:
						nothing
*******************************************************************************/

void free_arrow_header(NIDS_arrows *a) {

	free(a->arrows);
	
}

/*******************************************************************************
	function to print a single arrow

args:
						a				the structure the arrow is stored in
						ln			the layer number
						rn			the arrow number

returns:
						nothing
*******************************************************************************/

void print_arrow(NIDS_arrow *a, int ln, int rn) {
	
	printf("data.symb.layers[%i].arrow.arrows[%i].x_start %i\n", ln, rn, a->x_start);
	printf("data.symb.layers[%i].arrow.arrows[%i].y_start %i\n", ln, rn, a->y_start);
	printf("data.symb.layers[%i].arrow.arrows[%i].heading %i\n", ln, rn, a->heading);
	printf("data.symb.layers[%i].arrow.arrows[%i].length %i\n", ln, rn, a->length);
	printf("data.symb.layers[%i].arrow.arrows[%i].head_length %i\n", ln, rn, a->head_length);
	
}
	
/*******************************************************************************
	function to print an arrow packet

args:
						a				the structure the arrows are stored in
						ln			the layer number

returns:
						nothing
*******************************************************************************/

void print_arrow_header(NIDS_arrows *a, int ln) {
	int i;
	
	printf("data.symb.layers[%i].arr.length %i\n", ln, a->length);
	printf("data.symb.layers[%i].arr.num_arrows %i\n", ln, a->num_arrows);
	
	for (i = 0 ; i < a->num_arrows ; i++)
		print_arrow(a->arrows + i, ln, i);
	
}

