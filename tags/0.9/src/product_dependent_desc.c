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
#include "get.h"
#include "error.h"


void parse_product_dependent_desc(int msgcode, char *buf, prod_dep_desc *pdd) {
	
	switch (msgcode) {
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		case 22:
		case 23:
		case 24:
		case 25:
		case 26:
		case 27:
		case 28:
		case 29:
		case 30:
		case 43:
		case 44:
		case 45:
		case 46:
		case 55:
		case 56:
		case 87:
		case 93:
		case 132:
		case 133:
		case 139:
		case 143:
			pdd->elevation_angle = GET2(buf + 40);
			pdd->elevation_angle /= 10;
			break;
		
		case 32:
		case 134:
		case 135:
		case 136:
			pdd->compression = GET2(buf + 82);
			pdd->uncompressed_size = GET4(buf + 84);
			break;

		case 84:
			pdd->elevation_angle = GET2(buf + 78);
			pdd->elevation_angle /= 10;
			break;
		
		case 94:
		case 99:
		case 149:
			pdd->elevation_angle = GET2(buf + 40);
			pdd->elevation_angle /= 10;
			pdd->compression = GET2(buf + 82);
			pdd->uncompressed_size = GET4(buf + 84);
			break;

		case 152:
			pdd->compression = GET2(buf + 82);
			pdd->uncompressed_size = GET2(buf + 84);
			break;
	}
	
	
}

