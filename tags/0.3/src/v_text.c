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
#include "v_text.h"
#include "error.h"

/*
Write Text (Uniform Value)
                                                         PRECISION/
FIELDNAME               TYPE     UNITS    RANGE          ACCURACY       REMARKS
Packet Code             INT*2    N/A      8              N/A            Packet Type 8
Length of Block         INT*2    Bytes    1 to 32767     1              Number of bytes in
                                                                        block not including self
                                                                        or packet code
Value (Level) of Text   INT*2    N/A      0 to 15        1              Color Level of text
I Starting Point        INT*2    Km/4     -2048 to +2047 1              I coordinate for text
                                 or                                     starting point
                                 Pixels
J Starting Point        INT*2    Km/4     -2048 to +2047 1              J coordinate for text
                                 or                                     starting point
                                 Pixels
Character 1 to N        Char     8 bit    ASCII          N/A            Characters are ASCII
                                 ASCII    Character Set

*/

/*******************************************************************************
	function to parse a valued text packet
	
args:					buf			the buffer pointing to the start of the valued text packet
							t				the structure to store the text in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_v_text_header(char *buf, NIDS_v_text *t) {
	char *p;
	int i;
	
	t->length = GET2(buf);
	t->num_chars = t->length - 6;
	t->value = GET2(buf + 2);
	t->x_start = GET2(buf + 4);
	t->x_start = GET2(buf + 6);
	
	if (!(t->chars = malloc(t->num_chars)))
		ERROR("parse_v_text_header");
	
	p = buf + 6;
	
	for (i = 0 ; i < t->num_chars ; i++, p++) {
		t->chars[i] = *p;
	}
	t->chars[i] = 0;
	
	if (i % 2)
		p++;
	
	return p;
}
	
/*******************************************************************************
	function to free any dynamicly alocated memory used in valued text storage

args:
						t				the structure the text is stored in

returns:
						nothing
*******************************************************************************/

void free_v_text_header(NIDS_v_text *t) {
	
	free(t->chars);
	
}

/*******************************************************************************
	function to print a valued text packet

args:
						t				the structure the text is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_v_text_header(NIDS_v_text *t, char *prefix) {
	
	printf("%s.v_text.length %i\n", prefix, t->length);
	printf("%s.v_text.num_chars %i\n", prefix, t->num_chars);
	printf("%s.v_text.value %i\n", prefix, t->value);
	printf("%s.v_text.x_start %i\n", prefix, t->x_start);
	printf("%s.v_text.y_start %i\n", prefix, t->y_start);
	printf("%s.v_text.chars %s\n", prefix, t->chars);
	
}
