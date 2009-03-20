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
 
#ifndef _TABULAR_ALPHANUMERIC_H
#define _TABULAR_ALPHANUMERIC_H

/*******************************************************************************
	function to parse the tabular alphanumeric block

args:					buf			the buffer pointing to the start of the block
							t				the structure to store the block in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_tabular_alphanumeric(char *buf, NIDS_tabular_alphanumeric *t);

/*******************************************************************************
	function to check if a block is a tabular alphanumeric block
	
args:
					buf 				the buffer pointing to the start of the block

returns:
					1 	if the block is a graphic_alphanumeric block
					0		if it is not
*******************************************************************************/

int is_tabular_alphanumeric(char *buf);

/*******************************************************************************
	function to free the tabular alphanumeric block

							t				pointer to the block

returns:
							nothing
*******************************************************************************/

void free_tabular_alphanumeric(NIDS_tabular_alphanumeric *t);

/*******************************************************************************
	function to print the tabular_alphanumeric block

args:
						t				the structure the block is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_tabular_alphanumeric(NIDS_tabular_alphanumeric *t, char *prefix);

#endif /* _TABULAR_ALPHANUMERIC_H */

 
