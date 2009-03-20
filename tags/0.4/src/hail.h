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
 
#ifndef _HAIL_H
#define _HAIL_H

/*******************************************************************************
	function to parse a hail packet
	
args:					buf			the buffer pointing to the start of the hail packet
							b				the structure to store the hails in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_hail_header(char *buf, NIDS_hails *h);

/*******************************************************************************
	function to free any dynamicly alocated memory used in hail storage

args:
						h				the structure the hails are stored in

returns:
						nothing
*******************************************************************************/

void free_hail_header(NIDS_hails *h);

/*******************************************************************************
	function to print a hail packet

args:
						h				the structure the hails are stored in
						prefix	the start of the line
returns:
						nothing
*******************************************************************************/

void print_hail_header(NIDS_hails *h, char *prefix);

/*******************************************************************************
	fuction to draw hails in an image

args:
						im			pointer to the raster struct
						h				the structure that holds the hails

returns:
						nothing
*******************************************************************************/

void hails_to_raster (
	NIDS_image *im,
	NIDS_hails *h);

#endif /* _HAIL_H */

 
