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


/*******************************************************************************
	function to parse a mesocyclone packet
	
args:					buf			the buffer pointing to the start of the mesocyclone packet
							c				the structure to store the mesocyclones in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_mesocyclone_header(char *buf, NIDS_mesocyclones *c);

/*******************************************************************************
	function to free any dynamicly alocated memory used in mesocyclone storage

args:
						c			the structure the mesocyclones are stored in

returns:
						nothing
*******************************************************************************/

void free_mesocyclone_header(NIDS_mesocyclones *c);

/*******************************************************************************
	function to print a mesocyclone packet

args:
						c				the structure the mesocyclones are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_mesocyclone_header(NIDS_mesocyclones *c, char *prefix);

/*******************************************************************************
	fuction to draw a mesocyclone in an image

args:
						raster	pointer to the raster
						v				the structure that holds the mesocyclones
						xcenter	the x axis center in the raster
						ycenter	the y axis center in the raster

returns:
						nothing
*******************************************************************************/

void mesocyclones_to_raster (
	NIDS_image *im,
	NIDS_mesocyclones *c);
