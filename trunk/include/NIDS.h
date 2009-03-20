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

#ifndef _NIDS_H
#define _NIDS_H

/*******************************************************************************
1. Message Header Block

Halfword	Name										Description
01 				Message Code						See message code table below
02				Date of Message					Number of days since 1 Jan 1970
03				Time of Message (MSW)		Number of seconds since midnight
04				Time (LSW)
05				Length of Message (MSW) Number of bytes in message including header
06				Length (LSW) 	 
07				Source ID 	 
08				Destination ID 	 
09				Number of Blocks				1 (Header Block) + number of blocks in message
*******************************************************************************/

typedef struct {
	int code;
	time_t time;
	size_t len;
	int s_id;
	int d_id;
	int num_blocks;
} NIDS_msg_header;

/*******************************************************************************
2. Product Description Block

Halfword	Name											Description
10				Divider										Integer value -1
11				Latitude of Radar (MSW)		Latitude*1000, + is north
12				Latitude (LSW) 	 
13				Longitude of Radar (MSW)	Longitude*1000, + is east
14				Longitude (LSW) 	 
15				Height of Radar						Feet above sea level
16				Product Code							See message code table below
17				Operational Mode					0=Maintenance, 1=Clear air,
																		2=Precipitation/Severe weather
18				Volume Scan Pattern				RDA volume coverage pattern
19				Sequence Number						Sequence number of the request that
																		generated the product
20				Volume Scan Number				Counter, cycles 1 to 80
21				Volume Scan Date 	 
22				Volume Scan Time (MSW) 	 
23				Time (LSW) 	 
24				Product Generation Date 	 
25				Product Generation Time (MSW) 	 
26				Time (LSW) 	 
27				P1												Product specific codes, see product codes
																		below
28				P2 	 
29				Elevation Number					Elevation scan within volume scan
30				P3 	 
31				Data 1 Threshold					Data threshold values, see threshold
																		discussion below
32				Data 2 Threshold 	 
33				Data 3 Threshold 	 
34				Data 4 Threshold 	 
35				Data 5 Threshold 	 
36				Data 6 Threshold 	 
37				Data 7 Threshold 	 
38				Data 8 Threshold 	 
39				Data 9 Threshold 	 
40				Data 10 Threshold 	 
41				Data 11 Threshold 	 
42				Data 12 Threshold 	 
43				Data 13 Threshold 	 
44				Data 14 Threshold 	 
45				Data 15 Threshold
46				Data 16 Threshold 	 
47				P4 	 
48				P5 	 
49				P6 	 
50				P7 	 
51				P8 	 
52				P9 	 
53				P10 	 
54				Number of Maps 	 
55				Offset to Symbology Block (MSW) 	Number of halfwords (2 bytes) from
																						start of product to block
56				Offset (LSW) 	 
57				Offset to Graphic Block (MSW) 	 
58				Offset (LSW) 	 
59				Offset to Tabular Block (MSW) 	 
60b				Offset (LSW) 	 
*******************************************************************************/

typedef struct {
	float lat;
	float lon;
	int elev;
	int code;
	int mode;
	int vol_scan_pat;
	int seq_num;
	int scan_num;
	time_t scan_time;
	time_t prod_time;
	int codes[10];
	int elev_num;
	int thresholds[16];
	int num_maps;
	size_t symbol_off;
	size_t graph_off;
	size_t tab_off;
} NIDS_prod_desc;

/*******************************************************************************
Individual Radials

Each radial contains Run Length Encoded (RLE) values.
Halfword	Name											Description
01				Number of RLE Halfwords 	RLE data always padded to even halfword
																		boundary
02				Radial Start Angle				Angle*10, scan direction is always clockwise
03				Radial Angle Delta				Angle*10
04				Run0 	Code0 	Run1 	Code1	Run is 4 bit value for number of bins for
																		this value
05				Run2 	Code2 	Run3 	Code3	Code is 4 bit value (0-15) for the value
																		within the run
06-##			Run Codes									Repeated for entire radial
##				RunN 	CodeN								00 	00

*******************************************************************************/

typedef struct {
	int num_rle;
	float start;
	float delta;
	char *run;
	char *code;
} NIDS_radial;

/*******************************************************************************
3.1. Radial Data Packet

Radial data contains values for each bin within a particular radial.  Each
radial is defined by a start and end angle and by distance.  There are multiple
radials that define a full scan.

01 	Packet Code 	Hex "AF1F"
02 	Index of First Range Bin 	 
03 	Number of Range Bins 	Number of bins in each radial
04 	I Center of Sweep 	Center point location in a -2048 to 2047 coordinate region(mostly 0,0)
05 	J Center of Sweep 	 
06 	Scale Factor 	Number of pixels per range bin
07 	Number of Radials 	 
*******************************************************************************/

typedef struct {
	int index_first_bin;
	int num_bins;
	int x_center;
	int y_center;
	int scale;
	int num_radials;
	NIDS_radial *radials;
} NIDS_radials;

/*******************************************************************************
	digital radial data array
*******************************************************************************/

typedef struct {
	int num_bytes;
	float start;
	float delta;
	char *level;
} NIDS_d_radial;

typedef struct {
	int index_first_bin;
	int num_bins;
	int x_center;
	int y_center;
	int scale;
	int num_radials;
	NIDS_d_radial *radials;
} NIDS_d_radials;

/*******************************************************************************

Individual Rows

Each row contains Run Length Encoded (RLE) values.
01 			number of RLE halfwords			RLE data always padded to even halfword boundary
02 			Run0 	Code0 	Run1 	Code1 	Run is 4 bit value for number of bins for this value
03 			Run2 	Code2 	Run3 	Code3 	Code is 4 bit value (0-15) for the value within the run
04-## 	Run Codes										Repeated for entire row
##			RunN 	CodeN									00 	00

*******************************************************************************/

typedef struct {
	short num_rle;
	char *run;
	char *code;
} NIDS_raster_row;

/*******************************************************************************
3.2. Raster Data Packet

Raster data is pixelized data defined by rows and columns.
01 	Packet Code 	Hex "BA0F" or "BA07"
02 	Op Flags 	Hex "8000"
03 	Op Flags 	Hex "00C0"
04 	I Start Coordinate 	Start coordinate on -2048 to 2047 coordinate system
05 	J Start Coordinate 	 
06 	X Scale (INT) 	Reserved
07 	X Scale (FRACT) 	 
08 	Y Scale (INT) 	Reserved
09 	Y Scale (FRACT) 	 
10 	Number of Rows 	 
11 	Packing Descriptor 	Always 2
*******************************************************************************/

typedef struct {
	int op_flags1;
	int op_flags2;
	int x_start;
	int y_start;
	int x_scale_int;
	int x_scale_fract;
	int y_scale_int;
	int y_scale_fract;
	int num_rows;
	int packing;
	NIDS_raster_row *rows;
} NIDS_raster;

/*******************************************************************************
Individual Barbs

Each barb is 5 halfwords and is repeated until byte length.
01 	Value 	Color value (0-5) representing RMS variation of wind speed
02 	I Coordinate 	 
03 	J Coordinate 	 
04 	Direction of Wind 	In degrees
05 	Wind Speed 	
*******************************************************************************/

typedef struct {
	int value;
	int x_start;
	int y_start;
	int heading;
	int speed;
} NIDS_barb;

/*******************************************************************************
3.4. Wind Barb Packet

These define the location and size of wind barbs to be plotted.
01 	Packet Code = 4 	Hex "4"
02 	Length of Data 	In bytes
*******************************************************************************/

typedef struct {
	size_t length;
	int num_barbs;
	NIDS_barb *barbs;
} NIDS_barbs;

/*******************************************************************************

Individual Vectors

Each arrow is 5 halfwords and is repeated until byte length.
01 	I Coordinate 	 
02 	J Coordinate 	 
03 	Direction of Arrow 	In degrees
04 	Arrow Length 	In pixels
05 	Arrow Head Length 	In pixels
*******************************************************************************/

typedef struct {
	int x_start;
	int y_start;
	int heading;
	int length;
	int head_length;
} NIDS_arrow;

/*******************************************************************************
3.3. Vector Arrow Packet

These define the location and size of vector arrows to be plotted.
01 	Packet Code = 5 	Hex "5"
02 	Length of Data 	In bytes
*******************************************************************************/

typedef struct {
	size_t length;
	int num_arrows;
	NIDS_arrow *arrows;
} NIDS_arrows;

/*******************************************************************************
3.6. Unlinked Vector Packet

This is a set of unlinked vectors that plot disconnected lines on the plot.
01 	Packet Code = 7 	Hex "7"
02 	Length of Data 	In bytes
03 	Vector 1, I Start 	Vector 1 start and end coordinates
04 	Vector 1, J Start 	 
05 	Vector 1, I End 	 
06 	Vector 1, J End 	 
07 	Vector 2, I Start 	Vector 2 start and end coordinates
08 	Vector 2, J Start 	 
09 	Vector 2, I End 	 
10 	Vector 2, J End 	 
11-## 	Repeated Vectors 	 
*******************************************************************************/

typedef struct {
	int x_start;
	int y_start;
	int x_end;
	int y_end;
} NIDS_vector;

typedef struct {
	size_t length;
	int num_vectors;
	NIDS_vector *vectors;
} NIDS_vectors;

/*******************************************************************************
3.7. Valued Unlinked Vector Packet

This is a set of unlinked vectors that plot disconnected lines on the plot. This contains a value level for coloring.
01 	Packet Code = 7 	Hex "7"
02 	Length of Data 	In bytes
03 	Value 	Color level of vector
04 	Vector 1, I Start 	Vector 1 start and end coordinates
05 	Vector 1, J Start 	 
06 	Vector 1, I End 	 
07 	Vector 1, J End 	 
08 	Vector 2, I Start 	Vector 2 start and end coordinates
09 	Vector 2, J Start 	 
10 	Vector 2, I End 	 
11 	Vector 2, J End 	 
12-## 	Repeated Vectors 	
*******************************************************************************/


typedef struct {
	int value;
	int x_start;
	int y_start;
	int x_end;
	int y_end;
} NIDS_v_vector;

typedef struct {
	size_t length;
	int num_vectors;
	NIDS_v_vector *vectors;
} NIDS_v_vectors;

/*******************************************************************************
	special graphic symbol circle
*******************************************************************************/

typedef struct {
	int x_start;
	int y_start;
	int radius;
} NIDS_circle;

typedef struct {
	size_t length;
	int num_circles;
	NIDS_circle *circles;
} NIDS_circles;

/*******************************************************************************
Individual Rows

Each row contains Run Length Encoded (RLE) values.
01 	Number of Bytes in Row 	RLE data always padded to even halfword boundary
02 	Run0 	Code0 	Run is 8 bit value for number of bins for this value
03 	Run1 	Code1 	Code is 8 bit value (0-255) for the value within the run
04-## 	Run Codes 	Repeated for entire row
## 	RunN 	CodeN 	  
*******************************************************************************/

typedef struct {
	short num_rle;
	char *run;
	char *code;
} NIDS_precip_row;

/*******************************************************************************
3.5. Digital Precipitation Array Packet
01 	Packet Code = 17 	Hex "11"
02 	Op Flags 	Reserved
03 	Op Flags 	Reserved
04 	Number of LFM Boxes in Row 	 
05 	Number of Rows 	 
*******************************************************************************/

typedef struct {
	int op_flags1;
	int op_flags2;
	int lfm_per_row;
	int num_rows;
	NIDS_precip_row *rows;
} NIDS_precip;

/*******************************************************************************
	text packet
*******************************************************************************/

typedef struct {
	int num_chars;
	int x_start;
	int y_start;
	char *chars;
} NIDS_text;

/*******************************************************************************
	Symbology data type codes
*******************************************************************************/

#define TEXT1			0x0001
#define TEXT2			0x0002
#define CIRCLE1		0x0003
#define BARB			0x0004
#define ARROW			0x0005
#define VECTOR		0x0007
#define V_TEXT		0x0008
#define V_VECTOR	0x000A
#define CIRCLE2		0x000B
#define D_RADIAL	0x0010
#define PRECIP		0x0011
#define CIRCLE3		0x0019

#define RADIAL		0xAF1F
#define RASTER1		0xBA0F
#define RASTER2		0xBA07




/*******************************************************************************
Symbology Layer
Within the layer, there are a number of data packets which describe the type of 
information to be plotted.

01 	Divider 	 
02 	Length of Block (MSW) 	Length in bytes
03 	Length (LSW) 	 
04-## 	Display Data Packets 	 
*******************************************************************************/

typedef struct {
	size_t length;
	int data_type;
	union {
		NIDS_radials rad;
		NIDS_d_radials d_radial;
		NIDS_raster rast;
		NIDS_arrows arrow;
		NIDS_barbs barb;
		NIDS_precip precip;
		NIDS_vectors vector;
		NIDS_v_vectors v_vector;
		NIDS_circles circle;
		NIDS_text text;
	};
} NIDS_symbology_layer;


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

typedef struct {
	int id;
	size_t length;
	int num_layers;
	NIDS_symbology_layer *layers;
} NIDS_product_symbology;


/*******************************************************************************
	master struct to whole the whole thing
********************************************************************************/

typedef struct {
	NIDS_msg_header msg;
	NIDS_prod_desc prod;
	NIDS_product_symbology symb;
} NIDS;

FILE *NIDS_open(char *filename);

void NIDS_close(FILE *fp);

void NIDS_read(FILE *fp, NIDS *data);

void NIDS_free(NIDS *data);

void radials2raster (NIDS_radials *r, char *raster, int width, int height);

void NIDS_print(NIDS *data);

NIDS_symbology_layer *get_symbology_layer(NIDS_product_symbology *s, int layer);

#endif /* _NIDS_H */

 
