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
 
#ifndef _PRODUCT_SYMBOLOGY_H
#define _PRODUCT_SYMBOLOGY_H

/*******************************************************************************
	function to parse the Product Symbology
	
args:					buf			the buffer pointing to the start of the Product Symbology
							s				the structure to store the Product Symbology in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_product_symbology(char *buf, NIDS_product_symbology *s);

/*******************************************************************************
	function to check if a block is a product_symbology block
	
args:
					buf 				the buffer pointing to the start of the block

returns:
					1 	if the block is a product_symbology block
					0		if it is not
*******************************************************************************/

int is_product_symbology(char *buf);

/*******************************************************************************
	function to free any dynamicly alocated memory used in product symbology storage

args:
						h				the structure the product symbology is stored in

returns:
						nothing
*******************************************************************************/

void free_product_symbology(NIDS_product_symbology *s);

/*******************************************************************************
	function to print the product symbology

args:
						s				the structure the product desc is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_product_symbology(NIDS_product_symbology *s, char *prefix);

/*******************************************************************************
	function to convert the symbology block to a raster

args:
						s				the structure the product symbology is stored in

returns:
						nothing
*******************************************************************************/

void product_symbology_to_raster(
	NIDS_image *im,
	NIDS_product_symbology *s);

#endif /* _PRODUCT_SYMBOLOGY_H */

 
