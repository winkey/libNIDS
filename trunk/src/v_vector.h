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
 
#ifndef _V_VECTOR_H
#define _V_VECTOR_H

/*******************************************************************************
	function to parse a valued unlinked vector packet
	
args:					buf			the buffer pointing to the start of the vector packet
							v				the structure to store the vectors in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_v_vector_header(char *buf, NIDS_v_vectors *v);

/*******************************************************************************
	function to free any dynamicly alocated memory used in vector storage

args:
						v				the structure the vector is stored in

returns:
						nothing
*******************************************************************************/

void free_v_vector_header(NIDS_v_vectors *v);

/*******************************************************************************
	function to print a unlinked valued vector packet

args:
						v				the structure the vectors is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_v_vector_header(NIDS_v_vectors *v, char *prefix);

/*******************************************************************************
	fuction to draw a vector in an image

args:
						raster	pointer to the raster
						v				the structure that holds the vectors
						xcenter	the x axis center in the raster
						ycenter	the y axis center in the raster

returns:
						nothing
*******************************************************************************/

void v_vectors_to_raster (
	NIDS_image *im,
	NIDS_v_vectors *v);

#endif /* _V_VECTOR_H */

 
