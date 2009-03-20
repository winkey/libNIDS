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
 
#ifndef _IMAGE_H
#define _IMAGE_H

typedef struct {
	char *raster;
	int x_center;
	int y_center;
	int width;
	int height;
	int scale;			//divisor
} NIDS_image;

void plot(
	NIDS_image *im,
	int x,
	int y,
	int value);

void draw_line(
	NIDS_image *im,
	int x0,
	int x1,
	int y0,
	int y1,
	int value);

void draw_circle(
	NIDS_image *im,
	int x0,
	int y0,
	int radius,
	int value);

void draw_barb(
	NIDS_image *im,
	int xstart,
	int ystart,
	float heading,
	int speed);

void draw_triangle(
	NIDS_image *im,
	int x,
	int y,
	int height,
	int width,
	int value);

void draw_invert_triangle(
	NIDS_image *im,
	int x,
	int y,
	int height,
	int width,
	int value);

void draw_string(
	NIDS_image *im,
	int x_start,
	int y_start,
	char *s,
	int value);

#endif /* _IMAGE_H */

 
