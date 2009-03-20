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

#include "../include/NIDS.h"
#include "get.h"
#include "radial.h"
#include "raster.h"
#include "arrow.h"
#include "barb.h"
#include "precip.h"
#include "vector.h"
#include "v_vector.h"
#include "d_radial.h"
#include "circle.h"
#include "text.h"
#include "v_text.h"
#include "storm_id.h"
#include "linked_vector.h"
#include "v_linked_vector.h"
#include "forecast.h"
#include "product_symbology.h"
#include "error.h"

/*******************************************************************************
Symbology Layer
Within the layer, there are a number of data packets which describe the type of 
information to be plotted.

01 	Divider 	 
02 	Length of Block (MSW) 	Length in bytes
03 	Length (LSW) 	 
04-## 	Display Data Packets 	 
*******************************************************************************/

/*******************************************************************************
	function to parse a symbology layer
	
args:
							buf			the buffer pointing to the first byte of the layer
							l				the struct to store the layer in;

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_symbology_layer (char *buf, NIDS_symbology_layer *l) {
	char *p;
	
	l->length = GET4(buf + 2);
	l->data_type = GET2(buf + 6);
		
	p = buf + 8;
		
	switch (l->data_type) {
		case RADIAL:
			p = parse_radial_header(p, &(l->rad));
			break;
		
		case D_RADIAL:
			p = parse_d_radial_header(p, &(l->d_radial));
			break;
		
		case RASTER1:
		case RASTER2:
			p = parse_raster_header(p, &(l->rast));
			break;
		
		case ARROW:
			p = parse_arrow_header(p, &(l->arrow));
			break;
		
		case BARB:
			p = parse_barb_header(p, &(l->barb));
			break;
		
		case PRECIP:
			p = parse_precip_header(p, &(l->precip));
			break;
	
		case VECTOR:
			p = parse_vector_header(p, &(l->vector));
			break;
		
		case V_VECTOR:
			p = parse_v_vector_header(p, &(l->v_vector));
			break;
		
		case CIRCLE1:
		case CIRCLE2:
		case CIRCLE3:
			p = parse_circle_header(p, &(l->circle));
			break;
		
		case TEXT1:
		case TEXT2:
			p = parse_text_header(p, &(l->text));
			break;
		
		case V_TEXT:
			p = parse_v_text_header(p, &(l->v_text));
			break;
		
		case STORM_ID:
			p = parse_storm_id_header(p, &(l->storm_id));
			break;
		
		case LINKED_VECTOR:
			p = parse_linked_vector_header(p, &(l->linked_vector));
			break;
		
		case V_LINKED_VECTOR:
			p = parse_v_linked_vector_header(p, &(l->v_linked_vector));
			break;
		
		case FORECAST1:
		case FORECAST2:
			p = parse_forecast_header(p, &(l->forecast));
			break;
		
		default:
			printf("unknown symbology layer 0x%04x\n", l->data_type);
						 
	}
	
	return p;
}

/*******************************************************************************
3. Product Symbology Block

The Symbology Block has a header plus a number of data layers. 
These layers can be graphical such as radial and rastor scan data or could be
textual or symbolic such as TVS locations.

01 	Divider 	 
02 	Block ID 	Integer value 1
03 	Length of Block (MSW) 	Length in bytes
04 	Length (LSW) 	 
05 	Number of Layers 	 
*******************************************************************************/

/*******************************************************************************
	function to parse the Product Symbology
	
args:					buf			the buffer pointing to the start of the Product Symbology
							s				the structure to store the Product Symbology in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_product_symbology(char *buf, NIDS_product_symbology *s) {
	int i;
	char *p;
	
	s->id = GET2(buf + 2);
	s->length = GET4(buf + 4);
	s->num_layers = GET2(buf + 8);
	
	if (!(s->layers = malloc(s->num_layers * sizeof(NIDS_symbology_layer))))
		ERROR("parse_product_symbology");
	
	p = buf + 10;
	
	for (i = 0 ; i < s->num_layers ; i++) {
		p = parse_symbology_layer(p, s->layers + i);
		//p += s->layers[i].length;
	}
			
	return p;
}

/*******************************************************************************
	function to free all the memory used in a symbology layer

args:
						l				the structure the layer is stored in

returns:
						nothing
*******************************************************************************/

void free_symbology_layer (NIDS_symbology_layer *l) {
	
	switch (l->data_type) {
		case RADIAL:
			free_radial_header(&(l->rad));
			break;
		
		case D_RADIAL:
			free_d_radial_header(&(l->d_radial));
			break;
		
		case RASTER1:
		case RASTER2:
			free_raster_header(&(l->rast));
			break;
		
		case ARROW:
			free_arrow_header(&(l->arrow));
			break;
		
		case BARB:
			free_barb_header(&(l->barb));
			break;
		
		case PRECIP:
			free_precip_header(&(l->precip));
			break;
		
		case VECTOR:
			free_vector_header(&(l->vector));
			break;
		
		case V_VECTOR:
			free_v_vector_header(&(l->v_vector));
			break;
		
		case CIRCLE1:
		case CIRCLE2:
		case CIRCLE3:
			free_circle_header(&(l->circle));
			break;
		
		case TEXT1:
		case TEXT2:
			free_text_header(&(l->text));
			break;
		
		case V_TEXT:
			free_v_text_header(&(l->v_text));
			break;
		
		case STORM_ID:
			free_storm_id_header(&(l->storm_id));
			break;
		
		case LINKED_VECTOR:
			free_linked_vector_header(&(l->linked_vector));
			break;
		
		case V_LINKED_VECTOR:
			free_v_linked_vector_header(&(l->v_linked_vector));
			break;
		
		case FORECAST1:
		case FORECAST2:
			free_forecast_header(&(l->forecast));
			break;
		
		default:
			printf("unknown symbology layer %04x\n", l->data_type);
						 
	}
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in product symbology storage

args:
						h				the structure the product symbology is stored in

returns:
						nothing
*******************************************************************************/

void free_product_symbology(NIDS_product_symbology *s) {
	int i;
	
	for (i = 0 ; i < s->num_layers ; i++)
		free_symbology_layer(s->layers + i);

	free(s->layers);
}

/*******************************************************************************
	function to print a symbology layer

args:
						l				the structure the layer is stored in
						n			the layer number

returns:
						nothing
*******************************************************************************/

void print_symbology_layer (NIDS_symbology_layer *l, int n) {
	char prefix[PREFIX_LEN];
	
	printf ("data.symb.layers[%i].length %i\n", n, l->length);
	printf("data.symb.layers[%i].data_type %04x\n", n, l->data_type);
	
	snprintf(prefix, PREFIX_LEN, "data.symb.layers[%i]", n);
	
	switch (l->data_type) {
		case RADIAL:
			print_radial_header(&(l->rad), prefix);
			break;
		
		case D_RADIAL:
			print_d_radial_header(&(l->d_radial), prefix);
			break;
		
		case RASTER1:
		case RASTER2:
			print_raster_header(&(l->rast), prefix);
			break;
		
		case ARROW:
			print_arrow_header(&(l->arrow), prefix);
			break;
		
		case BARB:
			print_barb_header(&(l->barb), prefix);
			break;
		
		case PRECIP:
			print_precip_header(&(l->precip), prefix);
			break;
		
		case VECTOR:
			print_vector_header(&(l->vector), prefix);
			break;
		
		case V_VECTOR:
			print_v_vector_header(&(l->v_vector), prefix);
			break;
		
		case CIRCLE1:
		case CIRCLE2:
		case CIRCLE3:
			print_circle_header(&(l->circle), prefix);
			break;
		
		case TEXT1:
		case TEXT2:
			print_text_header(&(l->text), prefix);
			break;
		
		case V_TEXT:
			print_v_text_header(&(l->v_text), prefix);
			break;
		
		case STORM_ID:
			print_storm_id_header(&(l->storm_id), prefix);
			break;
		
		case LINKED_VECTOR:
			print_linked_vector_header(&(l->linked_vector), prefix);
			break;
		
		case V_LINKED_VECTOR:
			print_v_linked_vector_header(&(l->v_linked_vector), prefix);
			break;
		
		case FORECAST1:
		case FORECAST2:
			print_forecast_header(&(l->forecast), prefix);
			break;
		
		default:
			printf("unknown symbology layer 0x%04x\n", l->data_type);
							 
	}
	
}

/*******************************************************************************
	function to print the product symbology

args:
						s				the structure the product desc is stored in

returns:
						nothing
*******************************************************************************/

void print_product_symbology(NIDS_product_symbology *s) {
	int i;
	
	printf("data.symb.id %i\n", s->id);
	printf("data.symb.length %u\n", s->length);
	printf("data.symb.num_layers %i\n", s->num_layers);
	
	
	for (i = 0 ; i < s->num_layers ; i++)
		print_symbology_layer(s->layers + i, i);
}

/*******************************************************************************
	function to convert a symbology layer to a raster
*******************************************************************************/

char *product_symbology_layer_to_raster (
	NIDS_symbology_layer *l,
	int *width,
	int *height)
{
	char *result = NULL;
	
	switch (l->data_type) {
		case RADIAL:
			result = radials_to_raster(&(l->rad), width, height);
			break;
		
		case D_RADIAL:
			result = d_radials_to_raster(&(l->d_radial), width, height);
			break;
		
		case RASTER1:
		case RASTER2:
			break;
		
		case ARROW:
			break;
		
		case BARB:
			break;
		
		case PRECIP:
			break;
		
		case VECTOR:
			break;
		
		case V_VECTOR:
			break;
		
		case CIRCLE1:
		case CIRCLE2:
		case CIRCLE3:
			break;
		
		case TEXT1:
		case TEXT2:
			break;
		
		default:
			printf("unknown symbology layer %04x\n", l->data_type);
							 
	}
	
	return result;
}	

/*******************************************************************************
	function to convert the symbology block to a raster

args:
						s				the structure the product symbology is stored in

returns:
						nothing
*******************************************************************************/

char *product_symbology_to_raster(
	NIDS_product_symbology *s,
	int layer,
	int *width,
	int *height)
{
	char *result = NULL;
	
	if (layer - 1 < 0 || layer > s->num_layers) {
		fprintf(stderr, "ERROR Layer %i not found\n", layer);
		exit (EXIT_FAILURE);
	}
	
	result = product_symbology_layer_to_raster(s->layers + (layer - 1), width, height);
	
	return result;
}

