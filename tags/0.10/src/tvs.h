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
 
#ifndef _TVS_H
#define _TVS_H


/*******************************************************************************
	function to parse a tvs packet
	
args:					buf			the buffer tvsing to the start of the tvs packet
							p				the structure to store the tvss in

returns:
							tvser to the next byte in the buffer
*******************************************************************************/

char *parse_tvs_header(char *buf, NIDS_tvss *p);

/*******************************************************************************
	function to free any dynamicly alocated memory used in tvs storage

args:
						p				the structure the tvss are stored in

returns:
						nothing
*******************************************************************************/

void free_tvs_header(NIDS_tvss *p);

/*******************************************************************************
	function to print a tvs packet

args:
						p				the structure the tvss are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_tvs_header(NIDS_tvss *p, char *prefix);

/*******************************************************************************
	fuction to draw tvss in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the tvss

returns:
						nothing
*******************************************************************************/

void tvss_to_raster (
	NIDS_image *im,
	NIDS_tvss *p);

#endif /* _TVS_H */

 
