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

/*******************************************************************************
GRAPHIC ALPHANUMERIC BLOCK
                                              PRECISION/
FIELDNAME        TYPE    UNITS    RANGE       ACCURACY   REMARKS
Block Divider    INT*2   N/A      -1          N/A        Integer value of -1
                                                         used to delineate the
                                                         Graphic Alphanumeric
                                                         Block
Block ID         INT*2   N/A      2           N/A        Constant value of 2
                                                         which identifies this
                                                         block
Length of Block  INT*4   Bytes    1 to 65535  1          Length of block in
                                                         bytes (includes
                                                         preceding divider and
                                                         block id) from the
                                                         divider to the end of
                                                         message
Number of Pages  INT*2   N/A      1 to 48     1          Total number of pages
Page Number      INT*2   N/A      1 to 48     1          Current page number
Length of Page   INT*2   Bytes    4 to 1360   1          Number of bytes in
                                                         Text Packet 1 through
                                                         Text Packet N
Text Packet (N)  N/A     N/A      N/A         N/A        The format of these
                                                         text packets are Packet
                                                         Code 8, shown in
                                                         Figure 3-8b, and
                                                         Packet Code 10, shown
                                                         in Figure 3-8
*******************************************************************************/


