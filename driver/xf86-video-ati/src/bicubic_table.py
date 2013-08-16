#!/usr/bin/python

import struct

def half(i):
 fs, fe, fm = ((i >> 31) & 0x1, (i >> 23) & 0xff, i & 0x7fffff)
 s, e, m = (fs, 0, 0)

 if (fe == 0x0):
  pass
 if ((fe == 0xff) and (fm == 0x0)):
  e = 31
 elif (fe == 0xff):
  m = 1
  e = 31
 else:
  exp = fe - 127;
  if (exp < -24):
   pass
  elif (exp < -14):
   temp = 10 - (-14 - exp)
   m = 2**temp + (m >> (23 - temp))
  elif (exp > 15):
   e = 31
  else:
   e = exp + 15
   m = fm >> 13

 return ((s << 15) | (e << 10) | m)

def texgen(pix):

 tex = []

 for i in range(0,pix,4):

  a = i / float(pix)
  a2 = a ** 2
  a3 = a ** 3

  w0 = 1 / 6.0 * (-a3 + 3 * a2 + -3 * a + 1)
  w1 = 1 / 6.0 * (3 * a3 + -6 * a2 + 4)
  w2 = 1 / 6.0 * (-3 * a3 + 3 * a2 + 3 * a + 1)
  w3 = 1 / 6.0 * a3

  tex.append(-(1 - (w1 / (w0 + w1)) + a))
  tex.append(1 + (w3 / (w2 + w3)) - a)
  tex.append(w0 + w1)
  tex.append(w2 + w3)

 return tex

def printrow(l, offset):

 seq = [ struct.unpack('<I',struct.pack('f',i))[0] for i in l[offset:offset+4] ]
 seq = [ hex(half(i)) for i in seq ]
 return "\t" + ", ".join(seq) + ","

def maketable(pix):

 l = texgen(pix)

 print "static const uint16_t bicubic_tex_" + str(pix) + "[] = {"

 for i in range(0, pix, 4):

  print printrow(l, i)

 print "\t0 };\n"

maketable(512)
maketable(2048)
