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
 
#ifndef _FORECAST_H
#define _FORECAST_H

/*******************************************************************************
	function to parse a forecast or past data packet
	
args:					buf			the buffer pointing to the start of the forecast packet
							f				the structure to store the forecast in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_forecast_header(char *buf, NIDS_forecasts *f);

/*******************************************************************************
	function to free any dynamicly alocated memory used in forecast storage

args:
						f				the structure the forecast is stored in

returns:
						nothing
*******************************************************************************/

void free_forecast_header(NIDS_forecasts *f);

/*******************************************************************************
	function to print the forecast packet

args:
						f				the structure the forecasts are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_forecast_header(NIDS_forecasts *f, char *prefix);

/*******************************************************************************
	fuction to draw forcasts in an image

args:
						raster	pointer to the raster
						f				the structure that holds the forcasts
						xcenter	the x axis center in the raster
						ycenter	the y axis center in the raster

returns:
						nothing
*******************************************************************************/

void forecasts_to_raster (
	NIDS_image *im,
	NIDS_forecasts *f);

#endif /* _FORECAST_H */

 
