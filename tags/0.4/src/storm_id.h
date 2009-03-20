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
 
#ifndef _STORM_ID_H
#define _STORM_ID_H

/*******************************************************************************
	function to parse a storm id packet
	
args:					buf			the buffer pointing to the start of the storm id packet
							s				the structure to store the storm ids in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_storm_id_header(char *buf, NIDS_storm_ids *s);

/*******************************************************************************
	function to free any dynamicly alocated memory used in storm_id storage

args:
						s				the structure the storm_ids are stored in

returns:
						nothing
*******************************************************************************/

void free_storm_id_header(NIDS_storm_ids *s);

/*******************************************************************************
	function to print a storm id packet

args:
						s				the structure the storm ids are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_storm_id_header(NIDS_storm_ids *s, char *prefix);

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
	NIDS_storm_ids *s);

#endif /* _STORM_ID_H */

 
