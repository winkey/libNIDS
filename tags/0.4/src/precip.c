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

#include "../include/NIDS.h"
#include "get.h"
#include "precip.h"
#include "error.h"

/*******************************************************************************
                        MSB               HALFWORD                 LSB
                                          PACKET CODE (=18)
                                                SPARE
                                                SPARE
                                   NUMBER OF LFM BOXES IN ROW
                                          NUMBER OF ROWS
REPEAT FOR                            NUMBER OF BYTES IN ROW
EACH ROW                  RUN (0)      LEVEL (0)      RUN (1)       LEVEL (1)
                          RUN (2)      LEVEL (2)      RUN (3)       LEVEL (3)
                                                 •••
                                                 •••
                          RUN (N)      LEVEL (N)         0000         0000
    Figure 3-11b. Precipitation Rate Data Array Packet - Packet Code 18 (Sheet 1)
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
		
	if (!(r->run = malloc(r->num_rle)))
		ERROR("parse_precip_row");
	
	if (!(r->code = malloc(r->num_rle)))
		ERROR("parse_precip_row");
	
	p = buf + 2;
	
	for (i = 0 ; i < r->num_rle ; i++) {
		r->run[i] = (p[i] >> 4) & 0x0f;
		r->code[i] = p[i] & 0x0f;
	}
	
	return p + i;
}


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
	
	if (!(r->rows = malloc(r->num_rows * sizeof(NIDS_precip))))
		ERROR("parse_precip_header");
	
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
						prefix	the start of the line
						rn			the barb number

returns:
						nothing
*******************************************************************************/

void print_precip_row(NIDS_precip_row *r, char *prefix, int rn) {
	
	printf("%s.precip.rows[%i].num_rle %i\n", prefix, rn, r->num_rle);
	
}
	
/*******************************************************************************
	function to print a percip packet

args:
						b				the structure the percip is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_precip_header(NIDS_precip *r, char *prefix) {
	int i;
	
	printf("%s.precip.op_flags1 %i\n", prefix, r->op_flags1);
	printf("%s.precip.op_flags2 %i\n", prefix, r->op_flags2);
	printf("%s.precip.lfm_per_row %i\n", prefix, r->lfm_per_row);
	printf("%s.precip.num_rows %i\n", prefix, r->num_rows);
	
	for (i = 0 ; i < r->num_rows ; i++)
		print_precip_row(r->rows + i, prefix, i);
	
}
