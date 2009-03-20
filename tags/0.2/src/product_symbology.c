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
#include <time.h>
#include <errno.h>

#include "get.h"
#include "../include/NIDS.h"
#include "product_symbology.h"
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
		
		default:
			printf("unknown symbology layer %04x\n", l->data_type);
						 
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
	
	if (!(s->layers = malloc(s->num_layers * sizeof(NIDS_symbology_layer)))) {
		fprintf(stderr, "ERROR: parse_product_symbology : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
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
	
	printf ("data.symb.layers[%i].length %i\n", n, l->length);
	printf("data.symb.layers[%i].data_type %04x\n", n, l->data_type);
	
	switch (l->data_type) {
		case RADIAL:
			print_radial_header(&(l->rad), n);
			break;
		
		case D_RADIAL:
			print_d_radial_header(&(l->d_radial), n);
			break;
		
		case RASTER1:
		case RASTER2:
			print_raster_header(&(l->rast), n);
			break;
		
		case ARROW:
			print_arrow_header(&(l->arrow), n);
			break;
		
		case BARB:
			print_barb_header(&(l->barb), n);
			break;
		
		case PRECIP:
			print_precip_header(&(l->precip), n);
			break;
		
		case VECTOR:
			print_vector_header(&(l->vector), n);
			break;
		
		case V_VECTOR:
			print_v_vector_header(&(l->v_vector), n);
			break;
		
		case CIRCLE1:
		case CIRCLE2:
		case CIRCLE3:
			print_circle_header(&(l->circle), n);
			break;
		
		case TEXT1:
		case TEXT2:
			print_text_header(&(l->text), n);
			break;
		
		default:
			printf("unknown symbology layer %04x\n", l->data_type);
							 
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

void product_symbology_layer_to_raster (
	NIDS_symbology_layer *l,
	char *raster,
	int width,
	int height)
{
	
	switch (l->data_type) {
		case RADIAL:
			radials_to_raster(&(l->rad), raster, width, height);
			break;
		
		case D_RADIAL:
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
}	

/*******************************************************************************
	function to convert the symbology block to a raster

args:
						s				the structure the product symbology is stored in

returns:
						nothing
*******************************************************************************/

void product_symbology_to_raster(
	NIDS_product_symbology *s,
	char *raster,
	int width,
	int height)
{
	int i;
	
	for (i = 0 ; i < s->num_layers ; i++)
		product_symbology_layer_to_raster(s->layers + i, raster, width, height);
	
}
NIDS_symbology_layer *get_symbology_layer(NIDS_product_symbology *s, int layer) {
	NIDS_symbology_layer *result = NULL;
	
	if (layer < s->num_layers || layer > s->num_layers)
		fprintf (stderr, "WARNING: symbology layer %i not found\n", layer);
	else
		result = s->layers + (layer - 1);
	
	return result;
}

