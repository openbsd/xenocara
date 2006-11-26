/* dynlist.h: Dynamic lists and buffers in C
 * created 1999-Jan-06 15:04 jmk
 * autodate: 2000-Aug-28 01:29
 * 
 * by Jim Knoble <jmknoble@jmknoble.cx>
 * Copyright (C) 1999,2000 Jim Knoble
 * 
 * Disclaimer:
 * 
 * The software is provided "as is", without warranty of any kind,
 * express or implied, including but not limited to the warranties of
 * merchantability, fitness for a particular purpose and
 * noninfringement. In no event shall the author(s) be liable for any
 * claim, damages or other liability, whether in an action of
 * contract, tort or otherwise, arising from, out of or in connection
 * with the software or the use or other dealings in the software.
 * 
 * Permission to use, copy, modify, distribute, and sell this software
 * and its documentation for any purpose is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation.
 */

#ifndef H_DYNLIST
#define H_DYNLIST 1

#define APPEND_SUCCESS	1
#define APPEND_FAILURE	0

int append_to_list(char ***list_ptr, int *list_len, int *i, char *item);
int append_to_buf(char **buf, int *buflen, int *i, int c);

#endif /* H_DYNLIST */
