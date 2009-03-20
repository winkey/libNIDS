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
 
#ifndef _PRECIP_H
#define _PRECIP_H

/*******************************************************************************
	function to parse a precip packet
	
args:					buf			the buffer pointing to the start of the precip packet
							r				the structure to store the precip in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_precip_header(char *buf, NIDS_precip *r);

/*******************************************************************************
	function to free any dynamicly alocated memory used in precip storage

args:
						r				the structure the percip is stored in

returns:
						nothing
*******************************************************************************/

void free_precip_header(NIDS_precip *r);

/*******************************************************************************
	function to print a percip packet

args:
						b				the structure the percip is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_precip_header(NIDS_precip *r, char *prefix);

#endif /* _PRECIP_H */

 
