/*************************************************************************************
 * Copyright (C) 2005 Bogdan D. bogdand@users.sourceforge.net
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this 
 * software and associated documentation files (the "Software"), to deal in the Software 
 * without restriction, including without limitation the rights to use, copy, modify, 
 * merge, publish, distribute, sublicense, and/or sell copies of the Software, 
 * and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
 * INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the author shall not be used in advertising or 
 * otherwise to promote the sale, use or other dealings in this Software without prior written 
 * authorization from the author.
 *
 * $Log: theatre_detect.h,v $
 * Revision 1.1  2012/01/25 21:27:07  matthieu
 * Add radeon driver 6.12.2 renamed as 'radeonold'.
 *
 * This is the last known working version in 'zaphod' mode (one X
 * screen per head in multi-head configs). This makes it possible
 * to update the "real" radeon driver in xf86-video-ati, without
 * breaking existing zaphod setups.
 *
 * Revision 1.3  2009/08/25 18:51:45  matthieu
 * update do xf86-video-ati 6.12.2
 *
 * This has been in snapshots for weeks. ok oga@, todd@.
 *
 * Revision 1.3  2005/08/28 18:00:23  bogdand
 * Modified the licens type from GPL to a X/MIT one
 *
 * Revision 1.2  2005/07/01 22:43:11  daniels
 * Change all misc.h and os.h references to <X11/foo.h>.
 *
 *
 ************************************************************************************/

#ifndef __THEATRE_DETECT_H__
#define __THEATRE_DETECT_H__

/*
 * Created by Bogdan D. bogdand@users.sourceforge.net
 */


#define xf86_DetectTheatre	DetectTheatre
_X_EXPORT TheatrePtr DetectTheatre(GENERIC_BUS_Ptr b);



#endif
