#!/bin/sh
# $XTermId: pointer-shapes.sh,v 1.2 2021/11/11 17:47:07 tom Exp $
# -----------------------------------------------------------------------------
# this file is part of xterm
#
# Copyright 2021 by Thomas E. Dickey
# 
#                         All Rights Reserved
# 
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, sublicense, and/or sell copies of the Software, and to
# permit persons to whom the Software is furnished to do so, subject to
# the following conditions:
# 
# The above copyright notice and this permission notice shall be included
# in all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
# IN NO EVENT SHALL THE ABOVE LISTED COPYRIGHT HOLDER(S) BE LIABLE FOR ANY
# CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
# TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
# SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
# 
# Except as contained in this notice, the name(s) of the above copyright
# holders shall not be used in advertising or otherwise to promote the
# sale, use or other dealings in this Software without prior written
# authorization.
# -----------------------------------------------------------------------------
# exercise xterm's pointer-shape control

map() {
	printf '\033]22;%s\033\\' "$1"
	printf "** %s\n" "$1"
	sleep 2
}

map X_cursor
map arrow
map based_arrow_down
map based_arrow_up
map boat
map bogosity
map bottom_left_corner
map bottom_right_corner
map bottom_side
map bottom_tee
map box_spiral
map center_ptr
map circle
map clock
map coffee_mug
map cross
map cross_reverse
map crosshair
map diamond_cross
map dot
map dotbox
map double_arrow
map draft_large
map draft_small
map draped_box
map exchange
map fleur
map gobbler
map gumby
map hand1
map hand2
map heart
map icon
map iron_cross
map left_ptr
map left_side
map left_tee
map leftbutton
map ll_angle
map lr_angle
map man
map middlebutton
map mouse
map pencil
map pirate
map plus
map question_arrow
map right_ptr
map right_side
map right_tee
map rightbutton
map rtl_logo
map sailboat
map sb_down_arrow
map sb_h_double_arrow
map sb_left_arrow
map sb_right_arrow
map sb_up_arrow
map sb_v_double_arrow
map shuttle
map sizing
map spider
map spraycan
map star
map target
map tcross
map top_left_arrow
map top_left_corner
map top_right_corner
map top_side
map top_tee
map trek
map ul_angle
map umbrella
map ur_angle
map watch
map xterm
