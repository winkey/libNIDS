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
#include "storm_id.h"
#include "error.h"

/*******************************************************************************
            MSB             HALFWORD               LSB
                        PACKET CODE (=15)
STORM ID             LENGTH OF BLOCK (BYTES)
REPEAT FOR                  I POSITION
EACH SYMBOL                 J POSITION
                CHARACTER 1            CHARACTER 2
*******************************************************************************/

/*******************************************************************************
	function to parse a single storm id
	
args:
							buf			the buffer pointing to the first byte of the storm id
							s				the struct to store the storm id in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_storm_id(char *buf, NIDS_storm_id *s) {
	char *p;
	
	s->x_pos = GET2(buf);
	s->y_pos = GET2(buf + 2);
	
	p = buf + 4;
	
	s->id[0] = p[0];
	s->id[1] = p[1];
	s->id[2] = 0;
	
	
	return p + 2;
}	



/*******************************************************************************
	function to parse a storm id packet
	
args:					buf			the buffer pointing to the start of the storm id packet
							s				the structure to store the storm ids in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_storm_id_header(char *buf, NIDS_storm_ids *s) {
	char *p;
	int i;
	
	s->length = GET2(buf);
	s->num_ids = s->length / 6;
	

	
	if (!(s->ids = malloc(s->num_ids * sizeof(NIDS_storm_id))))
		ERROR("parse_storm_id_header");
	
	p = buf + 2;
	
	for (i = 0 ; i < s->num_ids ; i++) {
		p = parse_storm_id(p, s->ids + i);
	}
	
	return p;
}
	
/*******************************************************************************
	function to free any dynamicly alocated memory used in storm_id storage

args:
						s				the structure the storm_ids are stored in

returns:
						nothing
*******************************************************************************/

void free_storm_id_header(NIDS_storm_ids *s) {
	
	free(s->ids);
	
}

/*******************************************************************************
	function to print a storm id

args:
						s				the structure the storm id is stored in
						prefix	the start of the line
						rn			the row number

returns:
						nothing
*******************************************************************************/

void print_storm_id(NIDS_storm_id *s, char *prefix, int rn) {
	
	printf("%s.storm_id.ids[%i].x_pos %i\n", prefix, rn, s->x_pos);
	printf("%s.storm_id.ids[%i].y_pos %i\n", prefix, rn, s->y_pos);
	printf("%s.storm_id.ids[%i].id %s\n", prefix, rn, s->id);
	
}
/*******************************************************************************
	function to print a storm id packet

args:
						s				the structure the storm ids are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_storm_id_header(NIDS_storm_ids *s, char *prefix) {
	int i;
	
	printf("%s.storm_id.length %i\n", prefix, s->length);
	printf("%s.storm_id.num_ids %i\n", prefix, s->num_ids);
	
	for (i = 0; i < s->num_ids; i++)
		print_storm_id(s->ids + i, prefix, i);
	
}
/*******************************************************************************
*******************************************************************************/

void storm_id_to_raster(
	NIDS_image *im,
	NIDS_storm_id *s)
{

	draw_string(im, s->x_pos, s->y_pos, s->id, 1);
	
}

/*******************************************************************************
	fuction to draw a storm id in an image

args:
						t				the structure that holds the storm id
						width		pointer to return the width of the raster in
						height	pointer to return the height of the raster in

returns:
						a char pointer to the raster data
*******************************************************************************/

void storm_ids_to_raster (
	NIDS_image *im,
	NIDS_storm_ids *s)
{
	int i;
	
	for (i = 0; i < s->num_ids; i++) {
		storm_id_to_raster(im, s->ids + i);
	}
	
}
