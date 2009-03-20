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
#include <tiffio.h>
#include <geotiffio.h>

#include "../include/NIDS.h"

unsigned char colors[16][4] = {
	{0x00, 0x00, 0x00, 0x00},
	{0x00, 0xff, 0x00, 0xff},
	{0x00, 0xf0, 0x00, 0xff},
	{0x00, 0xe0, 0x00, 0xff},
	{0x00, 0xd0, 0x00, 0xff},
	{0x00, 0xc0, 0x00, 0xff},
	{0x00, 0xb0, 0x00, 0xff},
	{0x00, 0xa0, 0x00, 0xff},
	{0xc0, 0xc0, 0xc0, 0xff},
	{0xb0, 0x00, 0x00, 0xff},
	{0xc0, 0x00, 0x00, 0xff},
	{0xd0, 0x00, 0x00, 0xff},
	{0xe0, 0x00, 0x00, 0xff},
	{0xf0, 0x00, 0x00, 0xff},
	{0xff, 0x00, 0x00, 0xff},
	{0xc0, 0x40, 0xc0, 0xff}
};

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
	char rast[460*460] = {};
	int i, j;
	char line[460 * 4];
	
	if (argc < 4 || argc > 4) {
		fprintf (stderr, "USAGE: %s <layernumber> <nidsfile> <tifffile>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	if (!sscanf(argv[1], "%i", &layer)) {
		fprintf (stderr, "USAGE: %s <layernumber> <nidsfile> <tifffile>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	nids_fp = NIDS_open(argv[2]);
	
	tiff = tiff_open(argv[3]);
	
	NIDS_read(nids_fp, &data);
	
	//print_nids(&data);
	
	NIDS_to_raster(&data, rast, 460, 460);
	
	tiff_setfields(tiff, 460, 460);
	
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
	TIFFSetField(tiff, TIFFTAG_ROWSPERSTRIP, TIFFDefaultStripSize(tiff, 460 * 4));
	
	for (i = 0; i < 460; i++) {
		for (j = 0; j < 460 ; j++) {
			line[j * 4 + 0] = colors[(int)rast[i + j * 460]][0];
			line[j * 4 + 1] = colors[(int)rast[i + j * 460]][1];
			line[j * 4 + 2] = colors[(int)rast[i + j * 460]][2];
			line[j * 4 + 3] = colors[(int)rast[i + j * 460]][3];
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
