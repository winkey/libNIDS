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
 
#ifndef _TEXT_H
#define _TEXT_H

/*******************************************************************************
	function to parse a text packet
	
args:					buf			the buffer pointing to the start of the text packet
							t				the structure to store the text in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_text_header(char *buf, NIDS_text *t);

/*******************************************************************************
	function to free any dynamicly alocated memory used in text storage

args:
						t				the structure the text is stored in

returns:
						nothing
*******************************************************************************/

void free_text_header(NIDS_text *t);

/*******************************************************************************
	function to print a text packet

args:
						t				the structure the text is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_text_header(NIDS_text *t, char *prefix);

/*******************************************************************************
	fuction to draw a text in an image

args:
						raster	pointer to the raster
						t				the structure that holds the text
						xcenter	the x axis center in the raster
						ycenter	the y axis center in the raster

returns:
						nothing
*******************************************************************************/

void texts_to_raster (
	NIDS_image *im,
	NIDS_text *t);

#endif /* _TEXT_H */

 
