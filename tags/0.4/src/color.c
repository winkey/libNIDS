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
#include <time.h>

#include "../include/NIDS.h"
#include "color.h"
#include "error.h"

NIDS_color four_bit_reflectivity[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x9C, 0x9C, 0x9C, 0xFF}},
	{{0x76, 0x76, 0x76, 0xFF}},
	{{0xFF, 0xAA, 0xAA, 0xFF}},
	{{0xEE, 0x8C, 0x8C, 0xFF}},
	{{0xC9, 0x70, 0x70, 0xFF}},
	{{0x00, 0xFB, 0x90, 0xFF}},
	{{0x00, 0xBB, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0x70, 0xFF}},
	{{0xD0, 0xD0, 0x60, 0xFF}},
	{{0xFF, 0x60, 0x60, 0xFF}},
	{{0xDA, 0x00, 0x00, 0xFF}},
	{{0xAE, 0x00, 0x00, 0xFF}},
	{{0x00, 0x00, 0xFF, 0xFF}},
	{{0xFF, 0xFF, 0xFF, 0xFF}},
	{{0xE7, 0x00, 0xFF, 0xFF}}
};

NIDS_color three_bit_reflectivity[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0xFF, 0xAA, 0xAA, 0xFF}},
	{{0xC9, 0x70, 0x70, 0xFF}},
	{{0x00, 0xBB, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0x70, 0xFF}},
	{{0xDA, 0x00, 0x00, 0xFF}},
	{{0x00, 0x00, 0xFF, 0xFF}},
	{{0xFF, 0xFF, 0xFF, 0xFF}}
};

NIDS_color four_bit_velocity[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x00, 0xE0, 0xFF, 0xFF}},
	{{0x00, 0x80, 0xFF, 0xFF}},
	{{0x32, 0x00, 0x96, 0xFF}},
	{{0x00, 0xFB, 0x90, 0xFF}},
	{{0x00, 0xBB, 0x99, 0xFF}},
	{{0x00, 0x8F, 0x00, 0xFF}},
	{{0xCD, 0xC9, 0x9F, 0xFF}},
	{{0x76, 0x76, 0x76, 0xFF}},
	{{0xF8, 0x87, 0x00, 0xFF}},
	{{0xFF, 0xCF, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0x00, 0xFF}},
	{{0xAE, 0x00, 0x00, 0xFF}},
	{{0xD0, 0x70, 0x00, 0xFF}},
	{{0xFF, 0x00, 0x00, 0xFF}},
	{{0x77, 0x00, 0x7D, 0xFF}}
};

NIDS_color three_bit_velocity[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x00, 0xE0, 0xFF, 0xFF}},
	{{0x00, 0xBB, 0x00, 0xFF}},
	{{0x00, 0x8F, 0x00, 0xFF}},
	{{0xF8, 0x87, 0x00, 0xFF}},
	{{0xFF, 0xCF, 0x00, 0xFF}},
	{{0xFF, 0x00, 0x00, 0xFF}},
	{{0x77, 0x00, 0x7D, 0xFF}}
};

NIDS_color three_bit_spectrum[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x76, 0x76, 0x76, 0xFF}},
	{{0x9C, 0x9C, 0x9C, 0xFF}},
	{{0x00, 0xBB, 0x00, 0xFF}},
	{{0xFF, 0x00, 0x00, 0xFF}},
	{{0xD0, 0x70, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0x00, 0xFF}},
	{{0x77, 0x00, 0x7D, 0xFF}}
};

NIDS_color four_bit_shear[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x76, 0x76, 0x76, 0xFF}},
	{{0xAA, 0xAA, 0xAA, 0xFF}},
	{{0x32, 0x00, 0x96, 0xFF}},
	{{0x00, 0x80, 0xFF, 0xFF}},
	{{0x00, 0xFF, 0xFF, 0xFF}},
	{{0x00, 0x8F, 0x00, 0xFF}},
	{{0x00, 0xBB, 0x00, 0xFF}},
	{{0x00, 0xFF, 0x00, 0xFF}},
	{{0xC8, 0x64, 0x00, 0xFF}},
	{{0xFF, 0xAA, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0x00, 0xFF}},
	{{0xAE, 0x00, 0x00, 0xFF}},
	{{0xFF, 0x00, 0x00, 0xFF}},
	{{0xFF, 0x7D, 0x7D, 0xFF}},
	{{0x96, 0x00, 0x96, 0xFF}}
};

NIDS_color four_bit_echo_tops[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x76, 0x76, 0x76, 0xFF}},
	{{0x00, 0xE0, 0xFF, 0xFF}},
	{{0x00, 0xB0, 0xFF, 0xFF}},
	{{0x00, 0x90, 0xCC, 0xFF}},
	{{0x32, 0x00, 0x96, 0xFF}},
	{{0x00, 0xFB, 0x90, 0xFF}},
	{{0x00, 0xBB, 0x00, 0xFF}},
	{{0x00, 0xEF, 0x00, 0xFF}},
	{{0xFE, 0xBF, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0x00, 0xFF}},
	{{0xAE, 0x00, 0x00, 0xFF}},
	{{0xFF, 0x00, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0xFF, 0xFF}},
	{{0xE7, 0x00, 0xFF, 0xFF}}
};



NIDS_color two_bit_severe_weather_probability[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x00, 0xE0, 0xFF, 0xFF}},
	{{0xFF, 0xFF, 0x00, 0xFF}},
	{{0xFF, 0x00, 0x00, 0xFF}}
};

NIDS_color three_bit_vwp[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x00, 0xFF, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0x00, 0xFF}},
	{{0xFF, 0x00, 0x00, 0xFF}},
	{{0x00, 0xE0, 0xFF, 0xFF}},
	{{0xFF, 0x70, 0xFF, 0xFF}}
};

NIDS_color three_bit_vad[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0x77, 0x77, 0x90, 0xFF}},
	{{0xFF, 0xAA, 0xAA, 0xFF}},
	{{0xC9, 0x70, 0x70, 0xFF}},
	{{0x00, 0xBB, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0x70, 0xFF}},
	{{0xDA, 0x00, 0x00, 0xFF}},
	{{0x00, 0x00, 0xFF, 0xFF}}
};


NIDS_color four_bit_precip[] = {
	{{0x00, 0x00, 0x00, 0x00}},
	{{0xAA, 0xAA, 0xAA, 0xFF}},
	{{0x76, 0x76, 0x76, 0xFF}},
	{{0x00, 0xFF, 0xFF, 0xFF}},
	{{0x00, 0xAF, 0xAF, 0xFF}},
	{{0x00, 0xFF, 0x00, 0xFF}},
	{{0x00, 0x8F, 0x00, 0xFF}},
	{{0xFF, 0x00, 0xFF, 0xFF}},
	{{0xAF, 0x32, 0x7D, 0xFF}},
	{{0x00, 0x00, 0xFF, 0xFF}},
	{{0x32, 0x00, 0x96, 0xFF}},
	{{0xFF, 0xFF, 0x00, 0xFF}},
	{{0xFF, 0xAA, 0x00, 0xFF}},
	{{0xFF, 0x00, 0x00, 0xFF}},
	{{0xAE, 0x00, 0x00, 0xFF}},
	{{0xFF, 0xFF, 0xFF, 0xFF}}
};


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
			*result = three_bit_reflectivity;
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
			*result = four_bit_reflectivity;
			break;
		
		case 22:
		case 23:
		case 24:
		case 86:
			*result = three_bit_velocity;
			break;
		
		case 25:
		case 26:
		case 27:
		case 44:
		case 51:
		case 55:
		case 56:
			*result = four_bit_velocity;
			break;

		case 28:
		case 29:
		case 30:
		case 45:
			*result = three_bit_spectrum;
			break;
		
		case 41:
			*result = four_bit_echo_tops;
			break;

		case 46:
		case 87:
			*result = four_bit_shear;
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
			*result = four_bit_precip;
			break;

		case 47:
			*result = two_bit_severe_weather_probability;
			break;
		
		case 48:
			*result = three_bit_vad;
			break;

		case 59:
		case 141:
			*result = three_bit_reflectivity;
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
