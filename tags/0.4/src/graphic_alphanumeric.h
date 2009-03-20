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
 
#ifndef _GRAPHIC_ALPHANUMERIC_H
#define _GRAPHIC_ALPHANUMERIC_H

/*******************************************************************************
	function to parse the graphic alphanumeric block
	
args:					buf			the buffer pointing to the start of the block
							g				the structure to store the block in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_graphic_alphanumeric(char *buf, NIDS_graphic_alphanumeric *g);

/*******************************************************************************
	function to check if a block is a graphic alphanumeric block
	
args:
					buf 				the buffer pointing to the start of the block

returns:
					1 	if the block is a graphic_alphanumeric block
					0		if it is not
*******************************************************************************/

int is_graphic_alphanumeric(char *buf);

/*******************************************************************************
	function to free any dynamicly alocated memory used in graphic alphanumeric storage

args:
						h				the structure the product symbology is stored in

returns:
						nothing
*******************************************************************************/

void free_graphic_alphanumeric(NIDS_graphic_alphanumeric *g);

/*******************************************************************************
	function to print the graphic_alphanumeric block

args:
						g				the structure the block is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_graphic_alphanumeric(NIDS_graphic_alphanumeric *g, char *prefix);

/*******************************************************************************
	function to convert the graphic_alphanumeric block to a raster

args:
						g				the structure the graphic_alphanumeric is stored in

returns:
						nothing
*******************************************************************************/

void graphic_alphanumeric_to_raster(
	NIDS_image *im,
	NIDS_graphic_alphanumeric *g);

#endif /* _GRAPHIC_ALPHANUMERIC_H */

 
