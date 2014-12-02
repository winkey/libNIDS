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
	
	
	
	
	GDALDatasetH out_DS;
	OGRSpatialReferenceH out_SRS;
	
	GDALRasterBandH hBand;
	
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
		format = "HFA";
	
	NIDS_fp = NIDS_open(nidsfile);
	NIDS_read(NIDS_fp, &data);
	printf("data.info=%x", data.info);
	
	rast = NIDS_to_raster(&data, &width, &height);
	
	
	GDALAllRegister();
	
	/***** create the raster *****/
	
	out_DS = gdal_create(format, outfile, width, height, 1);
	
	/***** get the band *****/
	
	hBand = get_band(out_DS, 1);
	
	/***** set the projection *****/
	
	printf ("xres=%lg yres=%lg\n", data.info->xres, data.info->yres);
	out_SRS = set_projection(out_DS, &data, width / 2, height / 2, data.info->xres, data.info->yres );
	
	/***** create the color table *****/
	
	GDALColorTableH hColorTable = GDALCreateColorTable(GPI_RGB);
	
	/***** fill the color table *****/
	NIDS_color *colors = NULL;
	if (!scalename)
		get_product_dependent_color(data.msg.code, data.prod.mode, &colors);
	else
		colors = color_getscale(scalename);
	
	NIDS_color *color = NULL;
	i = 0;
	for (color = colors ; *(color->color) ; color++) {
		GDALColorEntry ce = {};
		
		sscanf(color->color, "%2x%2x%2x", &(ce.c1), &(ce.c2), &(ce.c3));
		if (!ce.c1 && !ce.c2 && !ce.c3)
			ce.c4 = 0;
		else
			ce.c4 = 255;
		
		GDALSetColorEntry (hColorTable, i, &ce);
		i++;
	}
	
	/***** set the color table *****/
	
	GDALSetRasterColorTable(hBand, hColorTable);
	
	/***** write the raster *****/
	
	GDALRasterIO(hBand, GF_Write, 0, 0,
								 width, height,
								 rast,
								 width, height,
								 GDT_Byte, 0, 0);
	
	
	
	GDALClose(out_DS);
	OSRDestroySpatialReference(out_SRS);
	GDALDestroyDriverManager();
	
	NIDS_free(&data);
	NIDS_close(NIDS_fp);
	free(colors);
	free(rast);
	
	return EXIT_SUCCESS;
}
