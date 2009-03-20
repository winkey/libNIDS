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
TABULAR ALPHANUMERIC BLOCK (see Note 3)
                                                                      PRECISION/
FIELDNAME                          TYPE         UNITS   RANGE         ACCURACY      REMARKS
Block Divider                      INT*2        N/A     -1            N/A            Integer value of -1
                                                                                    used to delineate the
                                                                                    Tabular Alphanumeric
                                                                                    Block
Block ID                           INT*2        N/A     3             N/A            Constant value of 3
                                                                                    which identifies this
                                                                                    block
Length of Block                    INT*4        Bytes   1 to 65535    1             Length of block in
                                                                                    bytes from the divider
                                                                                    to the end of message
------------------------------------------ SECOND MESSAGE HEADER BLOCK ---------------------------------------------
--------------------------------------------- SECOND PRODUCT DESCRIPTION BLOCK ----------------------------------
Block Divider               INT*2        N/A     -1            N/A            Integer value of -1
                                                                             used to delineate the
                                                                             data from the Product
                                                                             Description Block
Number of Pages             INT*2        N/A     1 to 48       1             Total number of pages
Number of Characters INT*2               N/A     0 to 80       1             Number of characters
                                                                             in a line
Character Data              CHAR         8 Bit   ASCII         N/A           Characters are ASCII
                                         ASCII   Character Set               when the MSB is set
                                                                                    to zero. When the
                                                                                    MSB is set to one, the
                                                                                    remaining 7 bits
                                                                                    define the special
                                                                                    symbol
End of Page Flag                   INT*2        N/A     -1            N/A           Integer value of -1 to
                                                                                    delineate the end of
                                                                                    page
Note 3. Tabular Alphanumeric Block must be the last block in a product message. Maximum lines
per page = 17. Alphanumeric Products containing RPG Site Adaptable Parameters must have the
Site Adaptable Parameters formatted as the last page(s) of the Product.
                                 Figure 3-6. Graphic Product Message (Sheet 10)
*******************************************************************************/
