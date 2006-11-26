#define MODULE_BIT 0x00004000

/* Maven bus address */
#define MAVEN_WRITE (0x1B<<1)
#define MAVEN_READ ((0x1B<<1)|1)

#define I2C_CLOCK 0x20
#define I2C_DATA 0x10

/* MGA-TVO-C I2C (G200), Maven (G400) */
#define I2C_CLOCK 0x20
#define I2C_DATA 0x10
/* primary head DDC for Mystique(?), G100, G200, G400 */
#define DDC1_CLK        0x08
#define DDC1_DATA       0x02
/* primary head DDC for Millennium, Millennium II */
#define DDC1B_CLK       0x10
#define DDC1B_DATA      0x04
/* secondary head DDC for G400, G450 and G550 */
#define DDC2_CLK        0x04
#define DDC2_DATA       0x01

/*MAVEN registers (<= G400) */
#define MGAMAV_PGM            0x3E
#define MGAMAV_PIXPLLM        0x80
#define MGAMAV_PIXPLLN        0x81
#define MGAMAV_PIXPLLP        0x82
#define MGAMAV_GAMMA1         0x83
#define MGAMAV_GAMMA2         0x84
#define MGAMAV_GAMMA3         0x85
#define MGAMAV_GAMMA4         0x86
#define MGAMAV_GAMMA5         0x87
#define MGAMAV_GAMMA6         0x88
#define MGAMAV_GAMMA7         0x89
#define MGAMAV_GAMMA8         0x8A
#define MGAMAV_GAMMA9         0x8B
#define MGAMAV_MONSET         0x8C
#define MGAMAV_TEST           0x8D
#define MGAMAV_WREG_0X8E_L    0x8E
#define MGAMAV_WREG_0X8E_H    0x8F
#define MGAMAV_HSCALETV       0x90
#define MGAMAV_TSCALETVL      0x91
#define MGAMAV_TSCALETVH      0x92
#define MGAMAV_FFILTER        0x93
#define MGAMAV_MONEN          0x94
#define MGAMAV_RESYNC         0x95
#define MGAMAV_LASTLINEL      0x96
#define MGAMAV_LASTLINEH      0x97
#define MGAMAV_WREG_0X98_L    0x98
#define MGAMAV_WREG_0X98_H    0x99
#define MGAMAV_HSYNCLENL      0x9A
#define MGAMAV_HSYNCLENH      0x9B
#define MGAMAV_HSYNCSTRL      0x9C
#define MGAMAV_HSYNCSTRH      0x9D
#define MGAMAV_HDISPLAYL      0x9E
#define MGAMAV_HDISPLAYH      0x9F
#define MGAMAV_HTOTALL        0xA0
#define MGAMAV_HTOTALH        0xA1
#define MGAMAV_VSYNCLENL      0xA2
#define MGAMAV_VSYNCLENH      0xA3
#define MGAMAV_VSYNCSTRL      0xA4
#define MGAMAV_VSYNCSTRH      0xA5
#define MGAMAV_VDISPLAYL      0xA6
#define MGAMAV_VDISPLAYH      0xA7
#define MGAMAV_VTOTALL        0xA8
#define MGAMAV_VTOTALH        0xA9
#define MGAMAV_HVIDRSTL       0xAA
#define MGAMAV_HVIDRSTH       0xAB
#define MGAMAV_VVIDRSTL       0xAC
#define MGAMAV_VVIDRSTH       0xAD
#define MGAMAV_VSOMETHINGL    0xAE
#define MGAMAV_VSOMETHINGH    0xAF
#define MGAMAV_OUTMODE        0xB0
#define MGAMAV_LOCK           0xB3
#define MGAMAV_LUMA           0xB9
#define MGAMAV_VDISPLAYTV     0xBE
#define MGAMAV_STABLE         0xBF
#define MGAMAV_HDISPLAYTV     0xC2
#define MGAMAV_BREG_0XC6      0xC6

