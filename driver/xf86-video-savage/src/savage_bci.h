
#ifndef S3BCI_H
#define S3BCI_H


/* BCI Control Register */
#define   S3_BCI_CONTROL                0x816C

/*
 *  High 8 bit of primary bitmap descriptor 2 register.
 *  Bits 25-24: Tile Format  00 = Linear ; 10 = 16 bits; 11 = 32 bits (Savage4)
 *            00 = Linear ; 01 = Tile; 10 = Texture tiling format/ Destination Tiling format      
 *            11 = Reserved/Destination tiling format. 
 */


#define   TILE_LINEAR                   0
#define   TILE_FORMAT_LINEAR            0
#define   TILE_TEXTURE                  2
#define   TILE_DESTINATION              1
#define   TILE_FORMAT_16BPP     	2
#define   TILE_FORMAT_32BPP     	3

/* BD - BCI enable */
/* savage4, MX, IX, 3D */
#define   BCI_ENABLE                    8
/* twister, prosavage, DDR, supersavage, 2000 */
#define   BCI_ENABLE_TWISTER            0

#define   S3_BIG_ENDIAN                    4
#define   S3_LITTLE_ENDIAN                 0
#define   S3_BD64                          1


 
/*  Global Bitmap Descriptor */
#define   S3_BCI_GLB_BD_LOW             0x8168
#define   S3_BCI_GLB_BD_HIGH            0x816C

#define REVERSE_BYTE_ORDER32(dword) {\
    unsigned int temp; \
    dword =  (temp & 0xFF) << 24; \
    dword |= (temp & 0xFF00) << 8; \
    dword |= (temp & 0xFF0000) >> 8; \
    dword |= (temp & 0xFF000000) >> 24; }

#define BCI_GET_PTR volatile CARD32 * bci_ptr = (CARD32 *) psav->BciMem
#define BCI_RESET bci_ptr = (CARD32 *) psav->BciMem

#define BCI_SEND(dw)   (*bci_ptr++ = (CARD32)(dw))

#define BCI_CMD_NOP                  0x40000000
#define BCI_CMD_RECT                 0x48000000
#define BCI_CMD_RECT_XP              0x01000000
#define BCI_CMD_RECT_YP              0x02000000
#define BCI_CMD_SCANLINE             0x50000000
#define BCI_CMD_LINE                 0x5C000000
#define BCI_CMD_LINE_LAST_PIXEL      0x58000000
#define BCI_CMD_BYTE_TEXT            0x63000000
#define BCI_CMD_NT_BYTE_TEXT         0x67000000
#define BCI_CMD_BIT_TEXT             0x6C000000
#define BCI_CMD_GET_ROP(cmd)         (((cmd) >> 16) & 0xFF)
#define BCI_CMD_SET_ROP(cmd, rop)    ((cmd) |= ((rop & 0xFF) << 16))
#define BCI_CMD_SEND_COLOR           0x00008000

#define BCI_CMD_CLIP_NONE            0x00000000
#define BCI_CMD_CLIP_CURRENT         0x00002000
#define BCI_CMD_CLIP_LR              0x00004000
#define BCI_CMD_CLIP_NEW             0x00006000

#define BCI_CMD_DEST_GBD             0x00000000
#define BCI_CMD_DEST_PBD             0x00000800
#define BCI_CMD_DEST_PBD_NEW         0x00000C00
#define BCI_CMD_DEST_SBD             0x00001000
#define BCI_CMD_DEST_SBD_NEW         0x00001400

#define BCI_CMD_SRC_TRANSPARENT      0x00000200
#define BCI_CMD_SRC_SOLID            0x00000000
#define BCI_CMD_SRC_GBD              0x00000020
#define BCI_CMD_SRC_COLOR            0x00000040
#define BCI_CMD_SRC_MONO             0x00000060
#define BCI_CMD_SRC_PBD_COLOR        0x00000080
#define BCI_CMD_SRC_PBD_MONO         0x000000A0
#define BCI_CMD_SRC_PBD_COLOR_NEW    0x000000C0
#define BCI_CMD_SRC_PBD_MONO_NEW     0x000000E0
#define BCI_CMD_SRC_SBD_COLOR        0x00000100
#define BCI_CMD_SRC_SBD_MONO         0x00000120
#define BCI_CMD_SRC_SBD_COLOR_NEW    0x00000140
#define BCI_CMD_SRC_SBD_MONO_NEW     0x00000160

#define BCI_CMD_PAT_TRANSPARENT      0x00000010
#define BCI_CMD_PAT_NONE             0x00000000
#define BCI_CMD_PAT_COLOR            0x00000002
#define BCI_CMD_PAT_MONO             0x00000003
#define BCI_CMD_PAT_PBD_COLOR        0x00000004
#define BCI_CMD_PAT_PBD_MONO         0x00000005
#define BCI_CMD_PAT_PBD_COLOR_NEW    0x00000006
#define BCI_CMD_PAT_PBD_MONO_NEW     0x00000007
#define BCI_CMD_PAT_SBD_COLOR        0x00000008
#define BCI_CMD_PAT_SBD_MONO         0x00000009
#define BCI_CMD_PAT_SBD_COLOR_NEW    0x0000000A
#define BCI_CMD_PAT_SBD_MONO_NEW     0x0000000B

#define BCI_BD_BW_DISABLE            0x10000000
#define BCI_BD_TILE_MASK             0x03000000
#define BCI_BD_TILE_NONE             0x00000000
#define BCI_BD_TILE_16               0x02000000
#define BCI_BD_TILE_32               0x03000000
#define BCI_BD_TILE_DESTINATION      0x01000000

#define BCI_BD_GET_BPP(bd)           (((bd) >> 16) & 0xFF)
#define BCI_BD_SET_BPP(bd, bpp)      ((bd) |= (((bpp) & 0xFF) << 16))
#define BCI_BD_GET_STRIDE(bd)        ((bd) & 0xFFFF)
#define BCI_BD_SET_STRIDE(bd, st)    ((bd) |= ((st) & 0xFFFF))

#define BCI_SET_REGISTER             0x96000000
#define BCI_SET_REGISTER_COUNT(count) ((count) << 16)
#define BCI_BITPLANE_WRITE_MASK	     0xD7
#define BCI_BITPLANE_READ_MASK	     0xD8
#define BCI_GBD_1		     0xE0
#define BCI_GBD_2		     0xE1
#define BCI_PBD_1		     0xE2
#define BCI_PBD_2		     0xE3
#define BCI_SBD_1		     0xE4
#define BCI_SBD_2		     0xE5

#define BCI_W_H(w, h)                ((((h) << 16) | (w)) & 0x0FFF0FFF)
#define BCI_X_Y(x, y)                ((((y) << 16) | (x)) & 0x0FFF0FFF)
#define BCI_X_W(x, y)                ((((w) << 16) | (x)) & 0x0FFF0FFF)
#define BCI_CLIP_LR(l, r)            ((((r) << 16) | (l)) & 0x0FFF0FFF)
#define BCI_CLIP_TL(t, l)            ((((t) << 16) | (l)) & 0x0FFF0FFF)
#define BCI_CLIP_BR(b, r)            ((((b) << 16) | (r)) & 0x0FFF0FFF)

#define BCI_LINE_X_Y(x, y)           (((y) << 16) | ((x) & 0xFFFF))
#define BCI_LINE_STEPS(diag, axi)    (((axi) << 16) | ((diag) & 0xFFFF))
#define BCI_LINE_MISC(maj, ym, xp, yp, err) \
	(((maj) & 0x1FFF) | \
	((ym) ? 1<<13 : 0) | \
	((xp) ? 1<<14 : 0) | \
	((yp) ? 1<<15 : 0) | \
	((err) << 16))

#endif /* S3BCI_H */
