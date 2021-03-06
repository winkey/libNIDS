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
 
#ifndef _RADIAL_H
#define _RADIAL_H



/*******************************************************************************
	function to parse a radial packet
	
args:					buf			the buffer pointing to the start of the radial packet
							r				the structure to store the radials in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_radial_header(char *buf, NIDS_radials *r);

/*******************************************************************************
	function to free any dynamicly alocated memory used in radial storage

args:
						r				the structure the radials is stored in

returns:
						nothing
*******************************************************************************/

void free_radial_header(NIDS_radials *r);

/*******************************************************************************
	function to print a radial packet

args:
						r				the structure the radials are stored in
						prefix	the start of the line


returns:
						nothing
*******************************************************************************/

void print_radial_header(NIDS_radials *r, char *prefix);

/*******************************************************************************
	function to convert radials to a raster

args:
						r				the structure that holds the radials
						width		pointer to return the width of the raster in
						height	pointer to return the height of the raster in

returns:
						a char pointer to the raster data

*******************************************************************************/

void radials_to_raster(
	NIDS_image *im,
	NIDS_radials *r);


#endif /* _RADIAL_H */

 
