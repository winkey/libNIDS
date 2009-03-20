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
 
#ifndef _RASTER_H
#define _RASTER_H

/*******************************************************************************
	function to parse a raster packet
	
args:					buf			the buffer pointing to the start of the raster packet
							r				the structure to store the raster in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_raster_header(char *buf, NIDS_raster *r);

/*******************************************************************************
	function to free any dynamicly alocated memory used in raster storage

args:
						r				the structure the raster is stored in

returns:
						nothing
*******************************************************************************/

void free_raster_header(NIDS_raster *r);

/*******************************************************************************
	function to print a raster packet

args:
						r				the structure the raster is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_raster_header(NIDS_raster *r, char *prefix);

#endif /* _RASTER_H */

 
