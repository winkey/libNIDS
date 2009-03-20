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
#include "precip.h"

/*******************************************************************************
Individual Rows

Each row contains Run Length Encoded (RLE) values.
01 	Number of Bytes in Row 	RLE data always padded to even halfword boundary
02 	Run0 	Code0 	Run is 8 bit value for number of bins for this value
03 	Run1 	Code1 	Code is 8 bit value (0-255) for the value within the run
04-## 	Run Codes 	Repeated for entire row
## 	RunN 	CodeN 	  
*******************************************************************************/

/*******************************************************************************
	function to parse a single row
	
args:
							buf			the buffer pointing to the first byte of the row
							r				the struct to store the row in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_precip_row(char *buf, NIDS_precip_row *r) {
	char *p;
	int i;
	
	r->num_rle = GET2(buf);
	printf("num_rle = %i", r->num_rle);
	if (!(r->run = malloc(r->num_rle))) {
		fprintf(stderr, "ERROR: percip_parse_row : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (!(r->code = malloc(r->num_rle))) {
		fprintf(stderr, "ERROR: percip_parse_row : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	p = buf + 2;
	
	for (i = 0 ; i < r->num_rle ; i++, p += 2) {
		r->run[i] = *p;
		r->code[i] = *(p + 1);
	}
	
	if (!(i%2))
		p += 2;
	
	return p;
}	

/*******************************************************************************
3.5. Digital Precipitation Array Packet
01 	Packet Code = 17 	Hex "11"
02 	Op Flags 	Reserved
03 	Op Flags 	Reserved
04 	Number of LFM Boxes in Row 	 
05 	Number of Rows 	 
*******************************************************************************/

/*******************************************************************************
	function to parse a precip packet
	
args:					buf			the buffer pointing to the start of the precip packet
							r				the structure to store the precip in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_precip_header(char *buf, NIDS_precip *r) {
	int i;
	char *p;
	
	r->op_flags1 = GET2(buf);
	r->op_flags2 = GET2(buf + 2);
	r->lfm_per_row = GET2(buf + 4);
	r->num_rows = GET2(buf + 6);
	
	if (!(r->rows = malloc(r->num_rows * sizeof(NIDS_precip)))) {
		fprintf(stderr, "ERROR: parse_precip_header : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	p = buf + 8;
	
	for (i = 0 ; i < r->num_rows ; i++) {
		p = parse_precip_row(p, r->rows + i);
	}
	
	return p;
}

/*******************************************************************************
	function to free a precip row

args:
						r			the precip row to free

returns:
						nothing
*******************************************************************************/

void free_precip_row(NIDS_precip_row *r) {
	free(r->run);
	free(r->code);
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in precip storage

args:
						r				the structure the percip is stored in

returns:
						nothing
*******************************************************************************/

void free_precip_header(NIDS_precip *r) {
	int i;
	
	for (i = 0 ; i < r->num_rows ; i++)
		free_precip_row(r->rows + i);
	
	free(r->rows);
	
}

/*******************************************************************************
	function to print a single precip row

args:
						r				the structure the precip row is stored in
						ln			the layer number
						rn			the barb number

returns:
						nothing
*******************************************************************************/

void print_precip_row(NIDS_precip_row *r, int ln, int rn) {
	
	printf("data.symb.layers[%i].precip.rows[%i].num_rle %i\n", ln, rn, r->num_rle);
	
}
	
/*******************************************************************************
	function to print a percip packet

args:
						b				the structure the percip is stored in
						ln			the layer number

returns:
						nothing
*******************************************************************************/

void print_precip_header(NIDS_precip *r, int ln) {
	int i;
	
	printf("data.symb.layers[%i].precip.op_flags1 %i\n", ln, r->op_flags1);
	printf("data.symb.layers[%i].precip.op_flags2 %i\n", ln, r->op_flags2);
	printf("data.symb.layers[%i].precip.lfm_per_row %i\n", ln, r->lfm_per_row);
	printf("data.symb.layers[%i].precip.num_rows %i\n", ln, r->num_rows);
	
	for (i = 0 ; i < r->num_rows ; i++)
		print_precip_row(r->rows + i, ln, i);
	
}
