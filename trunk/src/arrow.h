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
 
#ifndef _ARROW_H
#define _ARROW_H

/*******************************************************************************
	function to parse an arrow packet
	
args:					buf			the buffer pointing to the start of the arrow packet
							a				the structure to store the arrows in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_arrow_header(char *buf, NIDS_arrows *a);

/*******************************************************************************
	function to free any dynamicly alocated memory used in arrow storage

args:
						a				the structure the arrows are stored in

returns:
						nothing
*******************************************************************************/

void free_arrow_header(NIDS_arrows *a);

/*******************************************************************************
	function to print an arrow packet

args:
						a				the structure the arrows are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_arrow_header(NIDS_arrows *a, char *prefix);

#endif /* _ARROW_H */

