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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <math.h>

#include "../include/NIDS.h"
#include "get.h"
#include "radial.h"
#include "error.h"

#define RASTER_X_SIZE 4096
#define RASTER_Y_SIZE 4096

/*******************************************************************************
	function to parse a single radial
	
args:
							buf			the buffer pointing to the first byte of the radial
							r				the struct to store the radial in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_d_radial(char *buf, NIDS_d_radial *r) {
	char *p;
	int i;
	
	r->num_bytes = GET2(buf);
	r->start = GET2(buf + 2);
	r->start /= 10;
	r->delta = GET2(buf + 4);
	r->delta /= 10;
	
	if (!(r->level = malloc(r->num_bytes)))
		ERROR("parse_d_radial");
	
	p = buf + 6;
	
	for (i = 0 ; i < r->num_bytes ; i++)
		r->level[i] = p[i];
	
	if ((i % 2))
		i++;
	
	return p + i;
}	


/*******************************************************************************
3.1. Digital Radial Data Array

Radial data contains values for each bin within a particular radial.  Each
radial is defined by a start and end angle and by distance.  There are multiple
radials that define a full scan.

01 	Packet Code 16	Hex "0x10"
02 	Index of First Range Bin 	 
03 	Number of Range Bins 	Number of bins in each radial
04 	I Center of Sweep 	Center point location in a -2048 to 2047 coordinate region(mostly 0,0)
05 	J Center of Sweep 	 
06 	Scale Factor 	Number of pixels per range bin
07 	Number of Radials 	 
*******************************************************************************/

/*******************************************************************************
	function to parse a Digital Radial Data Array packet
	
args:					buf			the buffer pointing to the start of the radial packet
							r				the structure to store the radials in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_d_radial_header(char *buf, NIDS_d_radials *r) {
	int i;
	char *p;
	
	r->index_first_bin = GET2(buf);
	r->num_bins = GET2(buf + 2);
	r->x_center = GET2(buf + 4);
	r->y_center = GET2(buf + 6);
	r->scale = GET2(buf + 8);
	r->num_radials = GET2(buf + 10);
	
	if (!(r->radials = malloc(r->num_radials * sizeof(NIDS_d_radial))))
		ERROR("parse_d_radial_header");
	
	p = buf + 12;
	
	for (i = 0 ; i < r->num_radials ; i++) {
		p = parse_d_radial(p, r->radials + i);
	}
	
	return p;
}

/*******************************************************************************
	function to free a radial

args:
						r			the radial to free

returns:
						nothing
*******************************************************************************/

void free_d_radial(NIDS_d_radial *r) {
	
	free(r->level);
	
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in Digital Radial Data
	Array storage

args:
						r				the structure the radials is stored in

returns:
						nothing
*******************************************************************************/

void free_d_radial_header(NIDS_d_radials *r) {
	int i;
	
	for (i = 0 ; i < r->num_radials ; i++)
		free_d_radial(r->radials + i);
	
	free(r->radials);
	
}

/*******************************************************************************
	function to print a single radial

args:
						r				the structure the radial is stored in
						prefix	the start of the line
						rn			the radial number

returns:
						nothing
*******************************************************************************/

void print_d_radial(NIDS_d_radial *r, char *prefix, int rn) {
	
	printf("%s.d_radial.radials[%i].num_bytes %i\n", prefix, rn, r->num_bytes);
	printf("%s.d_radial.radials[%i].start %f\n", prefix, rn, r->start);
	printf("%s.d_radial.radials[%i].delta %f\n", prefix, rn, r->delta);
	
}

/*******************************************************************************
	function to print a Digital Radial Data Array packet

args:
						r				the structure the radials are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_d_radial_header(NIDS_d_radials *r, char *prefix) {
	int i;
	printf("%s.d_radial.index_first_bin %i\n", prefix, r->index_first_bin);
	printf("%s.d_radial.num_bins %i\n", prefix, r->num_bins);
	printf("%s.d_radial.x_center %i\n", prefix, r->x_center);
	printf("%s.d_radial.y_center %i\n", prefix, r->y_center);
	printf("%s.d_radial.num_radials %i\n", prefix, r->num_radials);
	
	for (i = 0 ; i < r->num_radials ; i++)
		print_d_radial(r->radials + i, prefix, i);
}

#define PI 3.14159265

void d_raidial_convert(float angle, float delta, int bin, int *x, int *y) {
	float r_angle = angle * (PI / 180);
	
	*x = bin * cos(r_angle);
	*y = bin * sin(r_angle);

}

/*******************************************************************************
	function to convert a single radial to a raster
*******************************************************************************/

void d_radial_to_raster (NIDS_d_radial *r, char *raster, int x_center, int y_center) {
	int x, y;
	int i;
	float k, angle;
	int bin = 1;
	
	for (i = 0 ; i < r->num_bytes ; i++) {
		for (k = -(r->delta / 2) ; k <= r->delta / 2; k += 0.1) {
				
			if (r->start + k > 360)
				angle = k;
			else
				angle = r->start + k;
			
			d_raidial_convert(angle , r->delta, bin, &x, &y);
				
			if (x_center + x >= RASTER_X_SIZE || x_center + x < 0)
				fprintf(stderr, "WARNING: raster x value %i out of range, skipping\n", x + x_center);
			else if (y_center + -y >= RASTER_Y_SIZE || y_center + -y < 0)
				fprintf(stderr, "WARNING: raster y value %i out of range, skipping\n", y + y_center);
			
			else {
				raster[(-x + x_center) + (RASTER_X_SIZE * (y + y_center))] = r->level[i];
			}
		}
	}
	
}

/*******************************************************************************
	function to convert a Digital Radial Data Array to a raster

args:
						r				the structure that holds the radials
						width		pointer to return the width of the raster in
						height	pointer to return the height of the raster in

returns:
						a char pointer to the raster data

*******************************************************************************/

char *d_radials_to_raster (
	NIDS_d_radials *r,
	int *width,
	int *height)
{
	int i;
	char *raster = NULL;
	
	if (!(raster = calloc(RASTER_X_SIZE, RASTER_Y_SIZE)))
		ERROR("d_radials_to_raster");
	
	for (i = 0 ; i < r->num_radials ; i++)
		d_radial_to_raster(r->radials + i, raster, r->x_center + 2048, r->y_center + 2048);
	
	*width = RASTER_X_SIZE;
	*height = RASTER_Y_SIZE;
	
	return raster;
}
