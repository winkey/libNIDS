/*
 * main.c
 * Copyright (C) winkey 2008 <rush@winkey.org>
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
#include "msg_header.h"
#include "prod_desc.h"
#include "product_symbology.h"
#include "product_dependent_desc.h"

FILE *NIDS_open(char *filename) {
	FILE *result = NULL;
	
	if (!(result = fopen(filename, "r"))) {
		fprintf(stderr, "ERROR: NIDS_open : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
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
	
	if (!(buf = malloc(1024))) {

		fprintf(stderr, "ERROR: NIDS_read : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	/***** read the tacked on nws header *****/
	
	if (!fread(nws, 30, 1, fp)) {
		fprintf(stderr, "ERROR: NIDS_read : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
		
	printf("%s\n", nws);
	
	/***** read the first 18 *****/
	
	if (!fread(buf, 18, 1, fp)) {
		fprintf(stderr, "ERROR: NIDS_read : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
  p = parse_msg_header(buf, &(data->msg));
  
	

	
	
	if (data->msg.len > 1024) {
		if (!(nbuf = realloc(buf, data->msg.len))) {
			fprintf(stderr, "ERROR: NIDS_read : %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}
		
		buf = nbuf;
		p = buf + 18;
	}
	
	if (!fread(buf + 18, data->msg.len - 18, 1, fp)) {
		fprintf(stderr, "ERROR: NIDS_read : %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	parse_product_dependent_desc(data->msg.code, p, &(data->pdd));
	
	p = parse_prod_desc(p, &(data->prod));
	
	if (data->pdd.compression) {
		if (!(bzbuf = malloc(data->pdd.uncompressed_size))) {
			fprintf(stderr, "ERROR: NIDS_read : %s\n", strerror(errno));
			exit(EXIT_FAILURE);
		}

		if (0 > BZ2_bzBuffToBuffDecompress(bzbuf,
															 &(data->pdd.uncompressed_size),
															 p,
															 data->msg.len - (p - buf),
															 0,
															 0)) {
			fprintf(stderr, "ERROR: NIDS_read : bzip error\n");
			exit(EXIT_FAILURE);
		}
		parse_product_symbology(bzbuf, &(data->symb));
		free(bzbuf);
	}
	
	else 
   parse_product_symbology(p, &(data->symb));
	
	
	free(buf);
  
}

void NIDS_free(NIDS *data) {
	free_msg_header(&(data->msg));
	free_prod_desc(&(data->prod));
	free_product_symbology(&(data->symb));
}

void NIDS_print(NIDS *data) {
	print_msg_header(&(data->msg));
	print_prod_desc(&(data->prod));
	print_product_symbology(&(data->symb));
}

void NIDS_to_raster(
	NIDS *data,
	char *raster,
	int width,
	int height)
{

	product_symbology_to_raster(&(data->symb), raster, width, height);
	
}
