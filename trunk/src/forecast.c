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
#include "circle.h"
#include "text.h"
#include "linked_vector.h"
#include "forecast.h"
#include "error.h"

/*******************************************************************************
              MSB       HALFWORD           LSB
SCIT PAST/         PACKET CODE (=23 or 24)
FORECAST DATA     LENGTH OF BLOCK (BYTES)
                   DISPLAY DATA PACKETS
                             •
                             •
*******************************************************************************/




/*******************************************************************************
	function to parse a forecast or past data layer
	
args:					buf			the buffer pointing to the start of the forecast
							f				the structure to store the forecast in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_forecast(char *buf, NIDS_forecast *f) {
	char *p;
	
	f->data_type = GET2(buf);
	
	p = buf + 2;
	
	switch(f->data_type) {
		case TEXT2:
			p = parse_text_header(p, &(f->text));
			break;
		
		case LINKED_VECTOR:
			p = parse_linked_vector_header(p, &(f->linked_vector));
			break;
		
		case CIRCLE3:
			p = parse_circle_header(p, &(f->circle));
			break;
		
		default:
			printf("unknown forecast layer %04x\n", f->data_type);
	}
	
	return p;
}
	


/*******************************************************************************
	function to parse a forecast or past data packet
	
args:					buf			the buffer pointing to the start of the forecast packet
							f				the structure to store the forecast in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_forecast_header(char *buf, NIDS_forecasts *f) {
	int i;
	char *p;
	NIDS_forecast *temp;
	
	f->length = GET2(buf);
	
	p = buf + 2;
	
	for (i = 0; p - buf + 2 < f->length; i++) {
		f->num_forecasts++;
		
		if (!(temp = realloc(f->forecasts, f->num_forecasts * sizeof(NIDS_forecast))))
			ERROR("parse_forecast_header");
		f->forecasts = temp;
		
		p = parse_forecast(p, f->forecasts + i);
	}
	
	return p;
};

/*******************************************************************************
	function to free all the memory used in a forecast layer

args:
						f				the structure the layer is stored in

returns:
						nothing
*******************************************************************************/

void free_forecast (NIDS_forecast *f) {
	
	switch (f->data_type) {

		case CIRCLE3:
			free_circle_header(&(f->circle));
			break;
		
		case TEXT1:
			free_text_header(&(f->text));
			break;
		
		case LINKED_VECTOR:
			free_linked_vector_header(&(f->linked_vector));
			break;
		
		default:
			printf("unknown forecast layer 0x%04x\n", f->data_type);
						 
	}
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in forecast storage

args:
						f				the structure the forecast is stored in

returns:
						nothing
*******************************************************************************/

void free_forecast_header(NIDS_forecasts *f) {
	int i;
	
	for (i = 0 ; i < f->num_forecasts ; i++)
		free_forecast (f->forecasts + i);

	free(f->forecasts);
}

/*******************************************************************************
	function to print a single forecast layer

args:
						v				the structure the layer is stored in
						prefix	the start of the line
						rn			the layer number

returns:
						nothing
*******************************************************************************/

void print_forecast(NIDS_forecast *f, char *prefix, int rn) {

	printf("%s.data_type %04x\n", prefix, f->data_type);
	
	switch (f->data_type) {
		case CIRCLE3:
			print_circle_header(&(f->circle), prefix);
			break;
		
		case TEXT1:
			print_text_header(&(f->text), prefix);
			break;
		
		case LINKED_VECTOR:
			print_linked_vector_header(&(f->linked_vector), prefix);
			break;
		
		default:
			printf("unknown symbology layer 0x%04x\n", f->data_type);
							 
	}
	
}

/*******************************************************************************
	function to print the forecast packet

args:
						f				the structure the forecasts are stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_forecast_header(NIDS_forecasts *f, char *prefix) {
	int i;
	char myprefix[PREFIX_LEN];
	
	printf("%s.forecast.length %i\n", prefix, f->length);
	printf("%s.forecast.num_forecasts %i\n", prefix, f->num_forecasts);
	
	for (i = 0 ; i < f->num_forecasts ; i++) {
		snprintf(myprefix, PREFIX_LEN, "%s.forecast.forecasts[%i]", prefix, i);
		print_forecast(f->forecasts + i, prefix, i);
	}
	
}
