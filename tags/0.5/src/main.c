/*
 * libNIDS
 * Copyright (C) Brian Case 2008 <rush@gisweather.org>
 *
 * main.c is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * main.c is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <bzlib.h>

#include "../include/NIDS.h"
#include "image.h"
#include "msg_header.h"
#include "prod_desc.h"
#include "product_symbology.h"
#include "graphic_alphanumeric.h"
#include "tabular_alphanumeric.h"
#include "product_dependent_desc.h"
#include "color.h"
#include "error.h"

FILE *NIDS_open(char *filename) {
	FILE *result = NULL;
	
	if (!(result = fopen(filename, "r")))
		ERROR("NIDS_open");
	
	return result;
}

void NIDS_close(FILE *fp) {
	
	fclose(fp);
	
}

void NIDS_read (FILE *fp, NIDS *data) {
  char *buf;
	char *nbuf;
	char *p;
	char nws[100] = {};
	char *bzbuf = NULL;
	int bze;
	
	if (!(buf = malloc(1024)))
		ERROR("NIDS_read");
	
	/***** read the tacked on nws header *****/
	
	if (!fread(nws, 30, 1, fp))
		ERROR("NIDS_read");
		
	printf("%s\n", nws);
	
	/***** read the first 18 *****/
	
	if (!fread(buf, 18, 1, fp))
		ERROR("NIDS_read");
	
  p = parse_msg_header(buf, &(data->msg));
  
	

	
	
	if (data->msg.len > 1024) {
		if (!(nbuf = realloc(buf, data->msg.len)))
			ERROR("NIDS_read");
		
		buf = nbuf;
		p = buf + 18;
	}
	
	if (!fread(buf + 18, data->msg.len - 18, 1, fp))
		ERROR("NIDS_read");
	
	parse_product_dependent_desc(data->msg.code, p, &(data->pdd));
	
	p = parse_prod_desc(p, &(data->prod));
	
	if (data->pdd.compression) {
		data->pdd.uncompressed_size += 1000000;
		if (!(bzbuf = malloc(data->pdd.uncompressed_size)))
			ERROR("NIDS_read");

		if (0 > (bze = BZ2_bzBuffToBuffDecompress(bzbuf,
															 &(data->pdd.uncompressed_size),
															 p,
															 data->msg.len - (p - buf),
															 0,
															 0))) {
			fprintf (stderr, "bze = %i\n", bze);
			ERROR("NIDS_read : bzip error");
		}											 
		
		p = bzbuf;
	}

	switch (data->msg.code) {
		
		/***** stand alone tabular_alphanumeric *****/
		
		case 62:
			p = parse_tabular_alphanumeric(buf - 8, &(data->tab));
			break;
		
		default:
			if (is_product_symbology(p))
				p = parse_product_symbology(p, &(data->symb));
			if (data->prod.graph_off && is_graphic_alphanumeric(p))
				p = parse_graphic_alphanumeric(p, &(data->graphic));
			if (data->prod.tab_off && is_tabular_alphanumeric(p))
				p = parse_tabular_alphanumeric(p, &(data->tab));
			break;
	}
	
	
	if (bzbuf)
		free(bzbuf);
	free(buf);
  
}

void NIDS_free(NIDS *data) {
	free_msg_header(&(data->msg));
	free_prod_desc(&(data->prod));
	free_product_symbology(&(data->symb));
	free_graphic_alphanumeric(&(data->graphic));
	free_tabular_alphanumeric(&(data->tab));
}

void NIDS_print(NIDS *data) {
	char prefix[PREFIX_LEN];
	
	snprintf(prefix, PREFIX_LEN, "data");
	
	print_msg_header(&(data->msg), prefix);
	print_prod_desc(&(data->prod), prefix);
	print_product_symbology(&(data->symb), prefix);
	print_graphic_alphanumeric(&(data->graphic), prefix);
	print_tabular_alphanumeric(&(data->tab), prefix);
}

#define RASTER_X_SIZE 640
#define RASTER_Y_SIZE 640

char *NIDS_to_raster(
	NIDS *data,
	int *width,
	int *height)
{
	NIDS_image im;
	
	im.raster = NULL;
	im.x_center = RASTER_X_SIZE / 2;
	im.y_center = RASTER_Y_SIZE / 2;
	*width = im.width = RASTER_X_SIZE;
	*height = im.height = RASTER_Y_SIZE;
	
	if (!(im.raster = calloc(RASTER_X_SIZE, RASTER_Y_SIZE)))
		ERROR("NIDS_to_raster");

	product_symbology_to_raster(&im, &(data->symb));
	graphic_alphanumeric_to_raster(&im, &(data->graphic));
	
	return im.raster;
}

void NIDS_get_color(
	NIDS *data,
	NIDS_color **colors)
{
	
	get_product_dependent_color(data->msg.code, colors);
	
	return;
}
	
