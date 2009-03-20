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

#include "../include/NIDS.h"

int main (int argc, char **argv) {
	FILE *fp;
	NIDS data = {};
	
	if (argc < 2 || argc > 2) {
		fprintf (stderr, "USAGE: %s <nidsfile>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	fp = NIDS_open(argv[1]);
	
	NIDS_read(fp, &data);
	
	NIDS_print(&data);
	
	NIDS_close(fp);
	
	NIDS_free(&data);
	
	return EXIT_SUCCESS;
}
		
		
