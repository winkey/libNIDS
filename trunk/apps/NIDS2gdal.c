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
#include <unistd.h>

#include <gdal.h>
#include <ogr_srs_api.h>
#include "../include/NIDS.h"
#include "../src/error.h"

#include "gdalcode.h"


	

int main (int argc, char **argv) {
	FILE *NIDS_fp;
	
	char *nidsfile = NULL;
	char *format = NULL;
	char *outfile = NULL;
	int threshhold = 0;
	int i, j;
	int width = 0, height = 0;
	NIDS data = {};
	char *rast = NULL;
	char *scalename = NULL;
	
	NIDS_color *colors = NULL;
	
	
	GDALDatasetH out_DS;
	OGRSpatialReferenceH out_SRS;
	
	GDALRasterBandH r_Band;
	GDALRasterBandH g_Band;
	GDALRasterBandH b_Band;
	GDALRasterBandH a_Band;
	
	unsigned char *rline = NULL;
	unsigned char *gline = NULL;
	unsigned char *bline = NULL;
	unsigned char *aline = NULL;
	
	unsigned int r, g, b, a;
	
	int opt;
	
	while (0 < (opt = getopt(argc, argv, "n:f:o:s:h"))) {
		
		switch (opt) {
			case 'n':
				nidsfile = optarg;
				break;
			
			case 'f':
				format = optarg;
				break;
			
			case 'o':
				outfile = optarg;
				break;
			
			case 's':
				scalename = optarg;
				break;
			/*
			case 't':
				threshhold = atoi(optarg);
				break;
			*/
			case 'h':
			case '?':
			default:
				fprintf(stderr,
								"USAGE: %s <-n nids file> <-o outfile> [-f format>]\n", argv[0]);
				exit(EXIT_FAILURE);
		}
	}
	
	if (!outfile || !nidsfile || threshhold < 0 || threshhold > 255) {
		fprintf(stderr,	"USAGE: %s <-n nids file> <-o outfile> [-f format>]\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if (!format)
		format = "gtiff";
	
	NIDS_fp = NIDS_open(nidsfile);
	NIDS_read(NIDS_fp, &data);
	printf("data.info=%x", data.info);
	
	rast = NIDS_to_raster(&data, &width, &height);
	
	
	GDALAllRegister();
	
	out_DS = gdal_create(format, outfile, width, height);
	
	printf ("xres=%lg yres=%lg\n", data.info->xres, data.info->yres);
	out_SRS = set_projection(out_DS, &data, width / 2, height / 2, data.info->xres, data.info->yres );
	
	r_Band = get_band(out_DS, 1);
	g_Band = get_band(out_DS, 2);
	b_Band = get_band(out_DS, 3);
	a_Band = get_band(out_DS, 4);
	
	if (!scalename)
		get_product_dependent_color(data.msg.code, &colors);
	else
		colors = color_getscale(scalename);

	/***** alocate line memmory *****/
	
	if (!(rline = malloc(width * sizeof(unsigned char))))
		ERROR("main");
	if (!(gline = malloc(width * sizeof(unsigned char))))
		ERROR("main");
	if (!(bline = malloc(width * sizeof(unsigned char))))
		ERROR("main");
	if (!(aline = malloc(width * sizeof(unsigned char))))
		ERROR("main");
	

	
	
	for (i = 0; i < height; i++) {
		for (j = 0; j < width ; j++) {
						
			if (!scalename)
				sscanf(colors[(unsigned char)rast[i * width + j]].color, "%2x%2x%2x",
							 &r, &g, &b);
			else
				sscanf(color_checkscale(colors, data.prod.thresholds[rast[i * width + j]]),
							 "%2x%2x%2x", &r, &g, &b);
			
			if (rast[i * width + j])
				a = 255;
			else
				a = 0;
			
			*(rline + j) = r;
			*(gline + j) = g;
			*(bline + j) = b;
			*(aline + j) = a;
			
		}
		
		/***** output lines *****/
		
		GDALRasterIO(r_Band, GF_Write, 0, i,
								 width, 1,
								 rline,
								 width, 1,
								 GDT_Byte, 0, 0);
		GDALRasterIO(g_Band, GF_Write, 0, i,
								 width, 1,
								 gline,
								 width, 1,
								 GDT_Byte, 0, 0);
		GDALRasterIO(b_Band, GF_Write, 0, i,
								 width, 1,
								 bline,
								 width, 1,
								 GDT_Byte, 0, 0);
		GDALRasterIO(a_Band, GF_Write, 0, i,
								 width, 1,
								 aline,
								 width, 1,
								 GDT_Byte, 0, 0);
	}
	
	/***** set threshold *****/
	/*
	for (i = 0; i < data.pdb.pixels_per_line * data.pdb.num_lines ; i++) {
		if ((unsigned int) data.ids[i] <= threshhold)
			data.ids[i] = 0;
	}
	*/
	
		
	
	GDALClose(out_DS);
	OSRDestroySpatialReference(out_SRS);
	GDALDestroyDriverManager();
	
	NIDS_free(&data);
	NIDS_close(NIDS_fp);
	free(rline);
	free(gline);
	free(bline);
	free(aline);
	free(colors);
	free(rast);
	
	return EXIT_SUCCESS;
}
