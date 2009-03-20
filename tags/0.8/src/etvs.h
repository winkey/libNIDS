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
 
#ifndef _ETVS_H
#define _ETVS_H


/*******************************************************************************
	function to parse a etvs packet
	
args:					buf			the buffer etvsing to the start of the etvs packet
							p				the structure to store the etvss in

returns:
							etvser to the next byte in the buffer
*******************************************************************************/

char *parse_etvs_header(char *buf, NIDS_etvss *p);

/*******************************************************************************
	function to free any dynamicly alocated memory used in etvs storage

args:
						p				the structure the etvss are stored in

returns:
						nothing
*******************************************************************************/

void free_etvs_header(NIDS_etvss *p);

/*******************************************************************************
	function to print a etvs packet

args:
						p				the structure the etvss are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_etvs_header(NIDS_etvss *p, char *prefix);

/*******************************************************************************
	fuction to draw etvss in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the etvss

returns:
						nothing
*******************************************************************************/

void etvss_to_raster (
	NIDS_image *im,
	NIDS_etvss *p);

#endif /* _ETVS_H */

 
