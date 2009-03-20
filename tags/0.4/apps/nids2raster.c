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
#include <tiffio.h>
#include <geotiffio.h>

#include "../include/NIDS.h"


TIFF *tiff_open (char *file) {
	TIFF *result = NULL;
	
	if((result = TIFFOpen(file, "w")) == NULL) {
    fprintf(stderr, "ERROR: can't open tiff file: %s\n", file);
    exit(EXIT_FAILURE);
  }
	
	return result;
}

void tiff_setfields(TIFF *tiff, int width, int height) {
	
	TIFFSetField(tiff, TIFFTAG_IMAGEWIDTH, width);
  TIFFSetField(tiff, TIFFTAG_IMAGELENGTH, height);
  TIFFSetField(tiff, TIFFTAG_SAMPLESPERPIXEL, 4);
  TIFFSetField(tiff, TIFFTAG_BITSPERSAMPLE, 8);
  TIFFSetField(tiff, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);
	TIFFSetField(tiff, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
	TIFFSetField(tiff, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_RGB);
}

int main (int argc, char **argv) {
	FILE *nids_fp;
	TIFF *tiff;
	
	char mybuf[100] = {};
	int layer;
	NIDS data = {};
	NIDS_symbology_layer *slayer = NULL;
	char *rast = NULL;
	int i, j;
	char line[4097 * 4];
	int width = 0, height = 0;
	NIDS_color *colors = NULL;
	char pixel;

	if (argc < 3 || argc > 3) {
		fprintf (stderr, "USAGE: %s <nidsfile> <tifffile>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	nids_fp = NIDS_open(argv[1]);
	
	tiff = tiff_open(argv[2]);
	
	NIDS_read(nids_fp, &data);
	
	//print_nids(&data);
	
	rast = NIDS_to_raster(&data, &width, &height);
	
	tiff_setfields(tiff, width, height);
	
//	GTModelTypeGeoKey ModelTypeGeographic  = 2   /* Geographic latitude-longitude System */
/*	GTRasterTypeGeoKey RasterPixelIsArea.
	
Step 3: This step depends on the GTModelType:
      case GEOGRAPHIC:  Check the list of standard GCS's and use the
           corresponding code. To use a code both the Datum, Prime
           Meridian, and angular units must match those of the code.
                
           Store in:  GeographicTypeGeoKey and skip to Step 4.
           If none of the coded GCS's match exactly, then this is a
           user-defined GCS. Check the list of standard datums,
           Prime Meridians, and angular units to define your system.
           Store in: GeogGeodeticDatumGeoKey, GeogAngularUnitsGeoKey,
              GeogPrimeMeridianGeoKey and skip to Step 4. 
  
           If none of the datums match your system, you have a 
           user-defined datum, which is an odd system, indeed. Use 
           the GeogEllipsoidGeoKey to select the appropriate ellipsoid
           or use the GeogSemiMajorAxisGeoKey, GeogInvFlatteningGeoKey to 
           define, and give a reference using the GeogCitationGeoKey.
           Store in: GeogEllipsoidGeoKey, etc. and go to Step 4.
        
Step 4: Install the GeoKeys/codes into the GeoKeyDirectoryTag, and the
      DOUBLE and ASCII key values into the corresponding value-tags.
      
Step 5: Having completely defined the Raster & Model coordinate system,
      go to Cookbook  section 2.6.2 and use the Georeferencing Tags
      to tie the raster image down onto the Model space.

*/		
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tiff, width * 4));
	
	NIDS_get_color(&data, &colors);
	
	
	for (i = 0; i < height; i++) {
		for (j = 0; j < width ; j++) {
			pixel = rast[j + i * width];
			if (colors) {
				memcpy(&line[j * 4], colors[(int)pixel].codes, 4);
			}
			else if (pixel) {
				line[j * 4] = 0xff;
				line[j * 4 + 1] = 0xff;
				line[j * 4 + 2] = 0xff;
				line[j * 4 + 3] = 0xff;
			}
			else {
				line[j * 4] = 0x00;
				line[j * 4 + 1] = 0x00;
				line[j * 4 + 2] = 0x00;
				line[j * 4 + 3] = 0x00;
			}
			
				
			
		}
		if (0 > TIFFWriteScanline(tiff, line, i, 0)) {
			fprintf(stderr, "Could not write image\n");
			exit(EXIT_FAILURE);
		}
	}
				
	NIDS_free(&data);
	TIFFClose(tiff);	
	NIDS_close(nids_fp);

	return EXIT_SUCCESS;
}
