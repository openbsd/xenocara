/*
 * Integrated TV out support based on the GATOS code by
 * Federico Ulivi <fulivi@lycos.com>
 */


/*
 * Limits of h/v positions (hPos & vPos)
 */
#define MAX_H_POSITION 5 /* Range: [-5..5], negative is on the left, 0 is default, positive is on the right */
#define MAX_V_POSITION 5 /* Range: [-5..5], negative is up, 0 is default, positive is down */

/*
 * Unit for hPos (in TV clock periods)
 */
#define H_POS_UNIT 10

/*
 * Indexes in h. code timing table for horizontal line position adjustment
 */
#define H_TABLE_POS1 6
#define H_TABLE_POS2 8

/*
 * Limits of hor. size (hSize)
 */
#define MAX_H_SIZE 5 /* Range: [-5..5], negative is smaller, positive is larger */

/* tv standard constants */
#define NTSC_TV_PLL_M 22
#define NTSC_TV_PLL_N 175
#define NTSC_TV_PLL_P 5
#define NTSC_TV_CLOCK_T 233
#define NTSC_TV_VFTOTAL 1
#define NTSC_TV_LINES_PER_FRAME 525
#define NTSC_TV_ZERO_H_SIZE 479166
#define NTSC_TV_H_SIZE_UNIT 9478

#define PAL_TV_PLL_M 113
#define PAL_TV_PLL_N 668
#define PAL_TV_PLL_P 3
#define PAL_TV_CLOCK_T 188
#define PAL_TV_VFTOTAL 3
#define PAL_TV_LINES_PER_FRAME 625
#define PAL_TV_ZERO_H_SIZE 473200
#define PAL_TV_H_SIZE_UNIT 9360


#define VERT_LEAD_IN_LINES 2
#define FRAC_BITS 0xe
#define FRAC_MASK 0x3fff
