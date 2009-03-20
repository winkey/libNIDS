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
#include <time.h>
#include "get.h"
#include "../include/NIDS.h"

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

/*******************************************************************************
	function to parse the product desc
	
args:					buf			the buffer pointing to the start of the product desc
							d				the structure to store the product desc in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_prod_desc(char *buf, NIDS_prod_desc *d) {

	d->lat = GET4(buf + 2);
	d->lat /= 1000;
	d->lon = GET4(buf + 6);
	d->lon /= 1000;
	d->elev = GET2(buf + 10);
	d->code = GET2(buf + 12);
	d->mode = GET2(buf + 14);
	d->vol_scan_pat = GET2(buf + 16);
	d->seq_num = GET2(buf + 18);
	d->scan_num = GET2(buf + 20);
	d->scan_time = GET2(buf + 22);
	d->scan_time *= 24 * 60 * 60;
	d->scan_time += GET4(buf + 24);
	d->prod_time = GET2(buf + 28);
	d->prod_time *= 24 * 60 * 60;
	d->prod_time += GET4(buf + 30);
	d->codes[0] = GET2(buf + 34);
	d->codes[1] = GET2(buf + 36);
	d->elev_num = GET2(buf + 38);
	d->codes[2] = GET2(buf + 40);
	d->thresholds[0] = GET2(buf + 42);
	d->thresholds[1] = GET2(buf + 44);
	d->thresholds[2] = GET2(buf + 46);
	d->thresholds[3] = GET2(buf + 48);
	d->thresholds[4] = GET2(buf + 50);
	d->thresholds[5] = GET2(buf + 52);
	d->thresholds[6] = GET2(buf + 54);
	d->thresholds[7] = GET2(buf + 56);
	d->thresholds[8] = GET2(buf + 58);
	d->thresholds[9] = GET2(buf + 60);
	d->thresholds[10] = GET2(buf + 62);
	d->thresholds[11] = GET2(buf + 64);
	d->thresholds[12] = GET2(buf + 66);
	d->thresholds[13] = GET2(buf + 68);
	d->thresholds[14] = GET2(buf + 70);
	d->thresholds[15] = GET2(buf + 72);
	d->codes[3] = GET2(buf + 74);
	d->codes[4] = GET2(buf + 76);
	d->codes[5] = GET2(buf + 78);
	d->codes[6] = GET2(buf + 80);
	d->codes[7] = GET2(buf + 82);
	d->codes[8] = GET2(buf + 84);
	d->codes[9] = GET2(buf + 86);
	d->num_maps = GET2(buf + 88);
	d->symbol_off = GET4(buf + 90);
	d->graph_off = GET4(buf + 94);
	d->tab_off = GET4(buf + 98);
	
	return buf + 102;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in product desc storage

args:
						d				the structure the product desc is stored in

returns:
						nothing
*******************************************************************************/

void free_prod_desc(NIDS_prod_desc *d) {

}

/*******************************************************************************
	function to print the product desc

args:
						d				the structure the product desc is stored in

returns:
						nothing
*******************************************************************************/

void print_prod_desc(NIDS_prod_desc *d) {
	int i;

	printf("data.prod.lat %f\n", d->lat);
	printf("data.prod.lon %f\n", d->lon);
	printf("data.prod.elev %i\n", d->elev);
	printf("data.prod.codes %i\n", d->code);
	printf("data.prod.mode %i\n", d->mode);
	printf("data.prod.vol_scan_pat %i\n", d->vol_scan_pat);
	printf("data.prod.seq_num %i\n", d->seq_num);
	printf("data.prod.scan_num %i\n", d->scan_num);
	printf("data.prod.scan_time %lu\n", d->scan_time);
	printf("data.prod.prod_time %lu\n", d->prod_time);
	
	for (i = 0 ; i < 10 ; i++)
		printf("data.prod.codes[%i] %i\n", i, d->codes[i]);
	
	printf("data.prod.elev_num %i\n", d->elev_num);
				 
	for (i = 0 ; i < 16 ; i++)
		printf("data.prod.thresholds[%i] %i\n", i, d->thresholds[i]);
	
	printf("data.prod.num_maps %i\n", d->num_maps);
	printf("data.prod.sybol_off %u\n", d->symbol_off);
	printf("data.prod.graph_off %u\n", d->graph_off);
	printf("data.prod.tab_off %u\n", d->tab_off);
}

