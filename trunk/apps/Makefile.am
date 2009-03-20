
bin_PROGRAMS = \
	nids2raster\
	nidsinfo \
	NIDS2gdal

NIDS2gdal_SOURCES = \
	NIDS2gdal.c\
	gdalcode.c \
	gdalcode.h

NIDS2gdal_LDFLAGS = \
	-L../src/.libs \
	-lNIDS \
	-lz \
	-lgdal

nidsinfo_SOURCES = \
	nidsinfo.c

nidsinfo_LDFLAGS = \
	-L../src/.libs \
	-lNIDS \
	-lz

## File created by the gnome-build tools

