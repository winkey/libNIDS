/*
 * libNIDS
 * Copyright (C) Brian Case 2008 <rush@gisweather.org>
 *
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
 
#ifndef _BARBS_H
#define _BARBS_H

/*******************************************************************************
	function to parse a barb packet
	
args:					buf			the buffer pointing to the start of the barb packet
							b				the structure to store the barbs in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_barb_header(char *buf, NIDS_barbs *b);

/*******************************************************************************
	function to free any dynamicly alocated memory used in barb storage

args:
						b				the structure the barbs are stored in

returns:
						nothing
*******************************************************************************/

void free_barb_header(NIDS_barbs *b);

/*******************************************************************************
	function to print a barb packet

args:
						b				the structure the barbs are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_barb_header(NIDS_barbs *b, char *prefix);

/*******************************************************************************
	fuction to draw barbs in an image

args:
						raster	pointer to the raster
						v				the structure that holds the circles
						xcenter	the x axis center in the raster
						ycenter	the y axis center in the raster

returns:
						nothing
*******************************************************************************/

void barbs_to_raster (
	NIDS_image *im,
	NIDS_barbs *b);

#endif /* _BARBS_H */

 
