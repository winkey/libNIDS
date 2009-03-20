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
 
#ifndef _POINT_H
#define _POINT_H

/*******************************************************************************
	function to parse a single point
	
args:
							buf			the buffer pointing to the first byte of the point
							p				the struct to store the point in;

returns:
							pointer to the next byte in the buffer 
*******************************************************************************/

char *parse_point (char *buf, NIDS_point *p);

/*******************************************************************************
	function to parse a point packet
	
args:					buf			the buffer pointing to the start of the point packet
							p				the structure to store the points in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_point_header(char *buf, NIDS_points *p);

/*******************************************************************************
	function to free any dynamicly alocated memory used in point storage

args:
						p				the structure the points are stored in

returns:
						nothing
*******************************************************************************/

void free_point_header(NIDS_points *p);

/*******************************************************************************
	function to print a single point

args:
						p				the structure the point is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_point(NIDS_point *p, char *prefix);

/*******************************************************************************
	function to print a point packet

args:
						p				the structure the points are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_point_header(NIDS_points *p, char *prefix);

#endif /* _POINT_H */

 
