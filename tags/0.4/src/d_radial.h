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
 
#ifndef _D_RADIAL_H
#define _D_RADIAL_H

/*******************************************************************************
	function to parse a Digital Radial Data Array packet
	
args:					buf			the buffer pointing to the start of the radial packet
							r				the structure to store the radials in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_d_radial_header(char *buf, NIDS_d_radials *r);

/*******************************************************************************
	function to free any dynamicly alocated memory used in Digital Radial Data
	Array storage

args:
						r				the structure the radials is stored in

returns:
						nothing
*******************************************************************************/

void free_d_radial_header(NIDS_d_radials *r);

/*******************************************************************************
	function to print a Digital Radial Data Array packet

args:
						r				the structure the radials are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_d_radial_header(NIDS_d_radials *r, char *prefix);



#endif /* _D_RADIAL_H */

 
