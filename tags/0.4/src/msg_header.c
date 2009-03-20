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

#include "../include/NIDS.h"
#include "get.h"
#include "msg_header.h"
#include "error.h"

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

/*******************************************************************************
	function to parse the msg header
	
args:					buf			the buffer pointing to the start of the msg header
							h				the structure to store the msg header in

returns:
							pointer to the next byte in the buffer
*******************************************************************************/

char *parse_msg_header(char *buf, NIDS_msg_header *h) {
	
	h->code = GET2(buf);
	h->time = GET2(buf + 2);
	h->time *= 24 * 60 * 60;
	h->time += GET4(buf + 4);
	h->len = GET4(buf + 8);
	h->s_id = GET2(buf + 12);
	h->d_id = GET2(buf + 14);
	h->num_blocks = GET2(buf + 16);
	
	return buf + 18;
}

/*******************************************************************************
	function to free any dynamicly alocated memory used in msg header storage

args:
						h				the structure the msg header is stored in

returns:
						nothing
*******************************************************************************/

void free_msg_header(NIDS_msg_header *h) {

}

/*******************************************************************************
	function to print the msg header

args:
						h				the structure the msg header is stored in
						prefix	the start of the line

returns:
						nothing
*******************************************************************************/

void print_msg_header(NIDS_msg_header *h, char *prefix) {
	
	printf("%s.msg.code %i\n", prefix, h->code);
	printf("%s.msg.time %lu\n", prefix, h->time);
	printf("%s.msg.len %u\n", prefix, h->len);
	printf("%s.msg.s_id %i\n", prefix, h->s_id);
	printf("%s.msg.d_id %i\n", prefix, h->d_id);
	printf("%s.msg.num_blocks %i\n", prefix, h->num_blocks);
	
}

