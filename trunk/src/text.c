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
#include "image.h"
#include "text.h"
#include "error.h"

/*
3.8. Text and Symbol Packet

This is for plotting text and symbols.
01 	Packet Code = 1 	Hex "1"
02 	Length of Data 	In bytes
03 	I Start 	Vector 1 start and end coordinates
04 	J Start 	 
05 	Char1 	Char2 	Each character in string
06 	Char3 	Char4 	 
07-## 	Repeated 	 
## 	CharN-1 	CharN 	 

*/

/*******************************************************************************
	function to parse a text packet
	
args:					buf			the buffer pointing to the start of the text packet
							t				the structure to store the text in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_text_header(char *buf, NIDS_text *t) {
	char *p;
	int i;
	
	t->length = GET2(buf);
	t->num_chars = t->length - 4;
	t->x_start = GET2(buf + 2);
	t->y_start = GET2(buf + 4);
	
	if (!(t->chars = malloc(t->num_chars + 1)))
		ERROR("parse_text_header");
	
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
	function to free any dynamicly alocated memory used in text storage

args:
						t				the structure the text is stored in

returns:
						nothing
*******************************************************************************/

void free_text_header(NIDS_text *t) {
	
	free(t->chars);
	
}

/*******************************************************************************
	function to print a text packet

args:
						t				the structure the text is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_text_header(NIDS_text *t, char *prefix) {
	
	printf("%s.v_text.length %i\n", prefix, t->length);
	printf("%s.text.num_chars %i\n", prefix, t->num_chars);
	printf("%s.text.x_start %i\n", prefix, t->x_start);
	printf("%s.text.y_start %i\n", prefix, t->y_start);
	printf("%s.text.chars %s\n", prefix, t->chars);
	
}

/*******************************************************************************
	fuction to draw a text in an image

args:
						raster	pointer to the raster
						t				the structure that holds the text
						xcenter	the x axis center in the raster
						ycenter	the y axis center in the raster

returns:
						nothing
*******************************************************************************/

void texts_to_raster (
	NIDS_image *im,
	NIDS_text *t)
{
	
	draw_string(im, t->x_start, t->y_start, t->chars, 1);
	
}
