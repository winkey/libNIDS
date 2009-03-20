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
 
#ifndef _HAIL_PROBABLE_H
#define _HAIL_PROBABLE_H

/*******************************************************************************
	function to parse a hail_probable packet
	
args:					buf			the buffer hail_probableing to the start of the hail_probable packet
							p				the structure to store the hail_probables in

returns:
							hail_probableer to the next byte in the buffer
*******************************************************************************/

char *parse_hail_probable_header(char *buf, NIDS_hail_probables *p);

/*******************************************************************************
	function to free any dynamicly alocated memory used in hail_probable storage

args:
						p				the structure the hail_probables are stored in

returns:
						nothing
*******************************************************************************/

void free_hail_probable_header(NIDS_hail_probables *p);

/*******************************************************************************
	function to print a hail_probable packet

args:
						p				the structure the hail_probables are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_hail_probable_header(NIDS_hail_probables *p, char *prefix);

/*******************************************************************************
	fuction to draw hail_probables in an image

args:
						im			pointer to the raster struct
						p				the structure that holds the hail_probables

returns:
						nothing
*******************************************************************************/

void hail_probables_to_raster (
	NIDS_image *im,
	NIDS_hail_probables *p);

#endif /* _HAIL_PROBABLE_H */

 
