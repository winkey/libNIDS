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
#include "raster.h"


/*******************************************************************************

Individual Rows

Each row contains Run Length Encoded (RLE) values.
01 			number of RLE halfwords			RLE data always padded to even halfword boundary
02 			Run0 	Code0 	Run1 	Code1 	Run is 4 bit value for number of bins for this value
03 			Run2 	Code2 	Run3 	Code3 	Code is 4 bit value (0-15) for the value within the run
04-## 	Run Codes										Repeated for entire row
##			RunN 	CodeN									00 	00

*******************************************************************************/

/*******************************************************************************
	function to parse a single row
	
args:
							buf			the buffer pointing to the first byte of the row
							r				the struct to store the row in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_raster_row(char *buf, NIDS_raster_row *r) {
	char *p;
	int i;
	
	r->num_rle = GET2(buf);
		
	if (!(r->run = malloc(r->num_rle * 2))) {
		fprintf(stderr, "ERROR: parse_raster_row : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (!(r->code = malloc(r->num_rle * 2))) {
		fprintf(stderr, "ERROR: parse_raster_row : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	p = buf + 2;
	
	for (i = 0 ; i < r->num_rle ; i++) {
		r->run[i] = (p[i] >> 4) & 0x0f;
		r->code[i] = p[i] & 0x0f;
	}
	
	return p + i;
}	


 
/*******************************************************************************
3.2. Raster Data Packet

Raster data is pixelized data defined by rows and columns.
01 	Packet Code 	Hex "BA0F" or "BA07"
02 	Op Flags 	Hex "8000"
03 	Op Flags 	Hex "00C0"
04 	I Start Coordinate 	Start coordinate on -2048 to 2047 coordinate system
05 	J Start Coordinate 	 
06 	X Scale (INT) 	Reserved
07 	X Scale (FRACT) 	 
08 	Y Scale (INT) 	Reserved
09 	Y Scale (FRACT) 	 
10 	Number of Rows 	 
11 	Packing Descriptor 	Always 2
*******************************************************************************/

/*******************************************************************************
	function to parse a raster packet
	
args:					buf			the buffer pointing to the start of the raster packet
							r				the structure to store the raster in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_raster_header(char *buf, NIDS_raster *r) {
	int i;
	char *p;
	
	r->op_flags1 = GET2(buf);
	r->op_flags2 = GET2(buf + 2);
	r->x_start = GET2(buf + 4);
	r->y_start = GET2(buf + 6);
	r->x_scale_int = GET2(buf + 8);
	r->x_scale_fract = GET2(buf + 10);
	r->y_scale_int = GET2(buf + 12);
	r->y_scale_fract = GET2(buf + 14);
	r->num_rows = GET2(buf + 16);
	r->packing = GET2(buf + 18);
	
	if (!(r->rows = malloc(r->num_rows * sizeof(NIDS_raster_row)))) {
		fprintf(stderr, "ERROR: parse_radial_header : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	p = buf + 20;
	
	for (i = 0 ; i < r->num_rows ; i++) {
		p = parse_raster_row(p, r->rows + i);
	}
	
	return p;
}

/*******************************************************************************
	function to free a raster row

args:
						r			the row to free

returns:
						nothing
*******************************************************************************/

void free_row(NIDS_raster_row *r) {
	free(r->run);
	free(r->code);
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in raster storage

args:
						r				the structure the raster is stored in

returns:
						nothing
*******************************************************************************/

void free_raster_header(NIDS_raster *r) {
	int i;
	
	for (i = 0 ; i < r->num_rows ; i++)
		free_row(r->rows + i);
	
	free(r->rows);
	
}

/*******************************************************************************
	function to print a single raster row

args:
						r				the structure the row is stored in
						ln			the layer number
						rn			the row number

returns:
						nothing
*******************************************************************************/

void print_row(NIDS_raster_row *r, int ln, int rn) {
	
	printf("data.symb.layers[%i].rast.rows[%i].num_rle %i\n", ln, rn, r->num_rle);
	
}

/*******************************************************************************
	function to print a raster packet

args:
						r				the structure the raster is stored in
						ln			the layer number

returns:
						nothing
*******************************************************************************/

void print_raster_header(NIDS_raster *r, int ln) {
	int i;
	
	printf("data.symb.layers[%i].rast.x_start %i\n", ln, r->x_start);
	printf("data.symb.layers[%i].rast.y_start %i\n", ln, r->y_start);
	printf("data.symb.layers[%i].rast.num_rows %i\n", ln, r->num_rows);
	
	for (i = 0 ; i < r->num_rows ; i++)
		print_row(r->rows + i, ln, i);

}


