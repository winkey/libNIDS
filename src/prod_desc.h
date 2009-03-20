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
 
#ifndef _PROD_DESC_H
#define _PROD_DESC_H

/*******************************************************************************
	function to parse the product desc
	
args:					buf			the buffer pointing to the start of the product desc
							d				the structure to store the product desc in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_prod_desc(char *buf, NIDS_prod_desc *d);

/*******************************************************************************
	function to free any dynamicly alocated memory used in product desc storage

args:
						d				the structure the product desc is stored in

returns:
						nothing
*******************************************************************************/

void free_prod_desc(NIDS_prod_desc *d);

/*******************************************************************************
	function to print the product desc

args:
						d				the structure the product desc is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_prod_desc(NIDS_prod_desc *d, char *prefix);

#endif /* _PROD_DESC_H */

 
