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
#include <gdal.h>
#include <gdal_alg.h>
#include <cpl_conv.h>
#include <ogr_api.h>
#include <ogr_srs_api.h>

#include "../include/NIDS.h"
#include "gdalcode.h"
#include "../src/error.h"

#define DEBUG 1


/*******************************************************************************
	function to transform the origins to the units the projection uses
*******************************************************************************/

void transform_origin(
	OGRSpatialReferenceH hSrcSRS,
	OGRSpatialReferenceH hDstSRS,
	double *MinX,
	double *MaxY)
{
											
	OGRCoordinateTransformationH transform = NULL;
	
	if (!(transform = OCTNewCoordinateTransformation(hSrcSRS, hDstSRS)))
		fprintf(stderr, "WARNING: failed to create Transformation\n");
	else {
		if (!OCTTransform(transform, 1, MinX, MaxY, NULL))
			fprintf(stderr, "WARNING: Transformation failed\n");
		
		OCTDestroyCoordinateTransformation(transform);
	}
	
	return;
}

/*******************************************************************************
	function to set the projection in the gdal dataset
*******************************************************************************/

void gdal_set_projection(
	GDALDatasetH hDS,
	OGRSpatialReferenceH hSRS)
{
	char *wkt;
	
	OSRExportToWkt(hSRS, &wkt);
	if (DEBUG) fprintf (stderr, "%s\n", wkt);
	GDALSetProjection(hDS, wkt);
	
	free (wkt);
	return;
}

/*******************************************************************************
	function to set the raster geotransform
	
	Args:
						ds				gdal dataset
						xo				x origin
						xsize			x pixel size
						xrot			x rotation
						yo				y origin
						ysize			y pixel size
						yrot			y rotation
	
	returns:
						nothing
*******************************************************************************/

void set_geotransform(
	GDALDatasetH hDS,
	double xo,
	double xsize,
	double xrot,
	double yo,
	double ysize,
	double yrot)


{
	double padfTransform[6];
	
	padfTransform[0] = xo;     /* X Origin (top left corner) */
	padfTransform[1] = xsize;      /* X Pixel size */
	padfTransform[2] = xrot;
	padfTransform[3] = yo;     /* Y Origin (top left corner) */
	padfTransform[4] = yrot;     
	padfTransform[5] = ysize;      /* Y Pixel Size */
	
	GDALSetGeoTransform(hDS, padfTransform);
	
	if (DEBUG) {
		if(padfTransform[2] == 0.0 && padfTransform[4] == 0.0) {
			fprintf(stderr, "Origin = (%.15f,%.15f)\n",
							padfTransform[0], padfTransform[3]);
			fprintf(stderr, "Pixel Size = (%.15f,%.15f)\n",
							padfTransform[1], padfTransform[5]);
		}
		else
			fprintf(stderr, "GeoTransform =\n  %.16g, %.16g, %.16g\n  %.16g, %.16g, %.16g\n",
							padfTransform[0], padfTransform[1], padfTransform[2],
							padfTransform[3], padfTransform[4], padfTransform[5]);
	}
	return;
}

/*******************************************************************************
	function to set the raster projection
	
	args:
						hDS			the gdal dataset
						data		the NIDS struct
						offx		the x center offset
						offy		the y center offset

	returns:
						nothing
				
*******************************************************************************/

OGRSpatialReferenceH set_projection(
	GDALDatasetH *hDS,
	NIDS *data,
	int offx,
	int offy)
{
	OGRSpatialReferenceH hSRS = OSRNewSpatialReference(NULL);
	OGRSpatialReferenceH hSrsSRS = OSRNewSpatialReference(NULL);
	double MinX, MinY, Dx, Dy, Ny, Nx, MaxX, MaxY;
	
	OSRSetWellKnownGeogCS(hSRS, "WGS84");
	OSRSetWellKnownGeogCS(hSrsSRS, "WGS84");
	
	OSRSetPS(hSRS, data->prod.lat, data->prod.lon, 1.0, 0.0, 0.0);
	
	MinX = data->prod.lon;
	MinY = data->prod.lat;
	
	
	transform_origin(hSrsSRS, hSRS, &MinX, &MinY);
	set_geotransform(hDS, MinX - 250.0 * offx, 250.0, 0, MinY + 250.0 * offy, -250.0, 0);
	
	

	gdal_set_projection(hDS, hSRS);
	
	/***** cleanup *****/
	
	OSRDestroySpatialReference(hSrsSRS);

	return hSRS;
}

/*******************************************************************************
	function to get the raster band
	
	args:
						hDS				gdal dataset
						band_num	the band number
	
	returns:
						the gdal raster band
*******************************************************************************/

GDALRasterBandH get_band(
	GDALDatasetH hDS,
	int band_num)
{
	GDALRasterBandH band = NULL;
	
	if (!(band = GDALGetRasterBand(hDS, band_num))) {
		fprintf (stderr, "Band %d does not exist on dataset.\n", band_num);
		exit(EXIT_FAILURE);
	}
	
	return band;
}

/*******************************************************************************
	function to create a new gdal dataset
	
	args:
						drivername	the format to create
						filename		the file to create
						xsize				the width of the raster
						ysize				the height of the raster
	
	returns:
						the gdal datasource
*******************************************************************************/

/*******************************************************************************
	function to create a new gdal dataset
	
	args:
						drivername	the format to create
						filename		the file to create
						xsize				the width of the raster
						ysize				the height of the raster
	
	returns:
						the gdal datasource
*******************************************************************************/

GDALDatasetH gdal_create (
	char *drivername,
	char *filename,
	int xsize,
	int ysize)
{
	char **papszMetadata;
	GDALDriverH hDriver;
	GDALDatasetH hDstDS;        
	char **papszOptions = NULL;
	
	if(!(hDriver = GDALGetDriverByName(drivername)))
		ERROR("GDALGetDriverByName");
	
	papszMetadata = GDALGetMetadata(hDriver, NULL);
	
	//if(!CSLFetchBoolean(papszMetadata, GDAL_DCAP_CREATE, FALSE)) {
		if (!(hDstDS = GDALCreate(hDriver, filename, xsize, ysize, 4, GDT_Byte, papszOptions)))
			ERROR("GDALCreate");
	//.}
	
	return hDstDS;
}
