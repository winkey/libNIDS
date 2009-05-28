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

#include <stdio.h>
#include <time.h>

#include "../include/NIDS.h"
#include "color.h"
#include "error.h"

/*******************************************************************************
	function to get the scale

	args:
							name				the name of the scale

	returns:
							the scale
	
	NOTE: you must free() the result when your done with it
*******************************************************************************/

NIDS_color *color_getscale(
	char *name)
{
	char filename[1024];
	FILE *fp;
	char line[1024];
	
	size_t used = 0, alloced = 0;
	
	NIDS_color *scales = NULL;
	NIDS_color *temp = NULL;
	
	snprintf(filename, sizeof(filename), "%s/%s.scale", PACKAGE_PKG_DATA_DIR, name);
	
	if (!(fp = fopen(filename, "r")))
		ERROR("color_getscale");
	
	while (fgets(line, sizeof(line), fp)) {
		
		/***** if no memory alocate *****/

		if (!alloced) {
			alloced = 30;
			if (!(scales = malloc (alloced * sizeof(NIDS_color))))
				ERROR("color_getscale");
		}

		/***** if not enough memory realocate *****/

		else {
			if (alloced < used + 2) {
				alloced *= 2;
				if (!(temp = realloc (scales, alloced * sizeof(NIDS_color))))
					ERROR("buffer_alloc");
			
				scales = temp;
			}
		}
		
		if (2 == sscanf(line, "%f %6[0-9a-fA-F]", &((scales[used]).value), (scales[used]).color)) {
			scales[used].color[6] = 0;
			printf("scales[%i].value = %f scales[%i].color = %s\n", used, scales[used].value, used, scales[used].color);
			used++;
		}
		
		
		scales[used].value = 0;
		scales[used].color[0] = 0;
	}
	
		 
	fclose(fp);
	
	return scales;
}


void get_product_dependent_color(int msgcode, NIDS_color **result) {
	
	switch (msgcode) {
		case 16:
		case 17:
		case 18:
		case 35:
		case 36:
		case 63:
		case 64:
		case 65:
		case 66:
		case 67:
		case 85:
		case 89:
		case 90:
		case 95:
		case 96:
			*result = color_getscale("3bit_reflectivity");
			break;
		
		case 19:
		case 20:
		case 21:
		case 33:
		case 37:
		case 38:
		case 43:
		case 50:
		case 57:
		case 97:
		case 98:
		case 137:
			*result = color_getscale("4bit_reflectivity");
			break;
		
		case 22:
		case 23:
		case 24:
		case 86:
			*result = color_getscale("3bit_velocity");
			break;
		
		case 25:
		case 26:
		case 27:
		case 44:
		case 51:
		case 55:
		case 56:
			*result = color_getscale("4bit_velocity");
			break;

		case 28:
		case 29:
		case 30:
		case 45:
			*result = color_getscale("3bit_spectrum");
			break;
		
		case 41:
			*result = color_getscale("4bit_echotops");
			break;

		case 46:
		case 87:
			*result = color_getscale("4bit_shear");
			break;
		
		case 31:
		case 78:
		case 79:
		case 80:
		case 144:
		case 145:
		case 146:
		case 147:
		case 150:
		case 151:
			*result = color_getscale("4bit_precip");
			break;

		case 47:
			*result = color_getscale("2bit_severeweather");
			break;
		
		case 48:
			*result = color_getscale("3bit_vad");
			break;

		case 59:
		case 60:
		case 141:
			*result = color_getscale("3bit_reflectivity");
			break;
		
/*
47 11           Severe Weather Probability                       2.2 x 2.2 Nmi x Nmi                                    124                  N/A           Geographic
                                                                                                                                                           Alphanumeric
48 12           VAD Wind Profile                                 5 Knots                                                N/A                  5             Non-geographic

74 26           Radar Coded Message                              1/16 LFM                                        248                        9           Alphanumeric

84 12           Velocity Azimuth Display                         5 Knots                                       N/A                   8                  Non-geographic
                                                                                                                                                        Alphanumeric

132     36 Clutter Likelihood Reflectivity .54 x 1 Nmi. x Deg                124        11  Radial Image
133     37 Clutter Likelihood Doppler      .54 x 1 Nmi. x Deg                124        12  Radial Image
134     39 High Resolution VIL             .54 x 1 Nmi x Deg                 248        256 Radial Image
136     38 SuperOb                         Adaptable, default = 5 km x 6 deg Adaptable, N/A Latitude, Longitude
                                                                             default =      (ICD packet code 27)
                                                                             100 km
149       20        Digital Mesocyclone Detection      N/A                                      124             N/A   Generic Data Format
*/
	}

	return;
}
	
/*******************************************************************************
	function to get a color from a scale

	args:
							scales			the scale to check
							value				the value to check for in the scale

	returns:
							the color for the value
*******************************************************************************/
 
char *color_checkscale(
	NIDS_color *scales,
	float value)
{
	NIDS_color *scale;
	
	for (scale = scales ;
			 value > scale->value && *((scale + 1)->color) != 0;
			 scale++);
	
	return scale->color;
}


