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
 
#ifndef _GDALCODE_H
#define _GDALCODE_H



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
	int offy);

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
	int band_num);


/*******************************************************************************
	function to get a gdal driver by its name
	
	args:
						name			the name of the driver
	
	returns:
						the gdal driver
*******************************************************************************/

GDALDriverH gdal_get_driver_by_name (
	char *name);

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
	int ysize);




#endif /* GDALCODE_H */

