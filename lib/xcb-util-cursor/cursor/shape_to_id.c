/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf --includes --struct-type --language=ANSI-C --switch=1 /share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf  */
/* Computed positions: -k'1-2,4,$' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
struct shape_mapping { const char *name; int number; };
int cursor_shape_to_id(const char *name);
#include <string.h>

#define TOTAL_KEYWORDS 77
#define MIN_WORD_LENGTH 3
#define MAX_WORD_LENGTH 19
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 165
/* maximum key range = 162, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register size_t len)
{
  static unsigned char asso_values[] =
    {
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166,   5,
        0, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166,  30, 166,
      166, 166, 166, 166, 166,   0, 166,  20,   5,  20,
       65,   5,  50,   5,  30,  25, 166,   5,  15,  35,
       25,   0,  40,   0,   0,   0,   0,  40,   0,  25,
       50,  40, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166, 166, 166, 166, 166,
      166, 166, 166, 166, 166, 166
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[3]];
      /*FALLTHROUGH*/
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
      /*FALLTHROUGH*/
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval + asso_values[(unsigned char)str[len - 1]];
}

struct shape_mapping *
in_word_set (register const char *str, register size_t len)
{
  static struct shape_mapping wordlist[] =
    {
#line 67 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"star",63},
#line 55 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"rtl_logo",51},
#line 8 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"boat",4},
#line 74 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"top_tee",70},
#line 73 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"top_side",69},
#line 75 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"trek",71},
#line 71 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"top_left_corner",67},
#line 72 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"top_right_corner",68},
#line 31 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"gobbler",27},
#line 13 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"bottom_tee",9},
#line 12 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"bottom_side",8},
#line 10 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"bottom_left_corner",6},
#line 11 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"bottom_right_corner",7},
#line 19 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"cross",15},
#line 69 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"tcross",65},
#line 38 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"left_ptr",34},
#line 21 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"crosshair",17},
#line 14 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"box_spiral",10},
#line 68 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"target",64},
#line 40 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"left_tee",36},
#line 39 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"left_side",35},
#line 15 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"center_ptr",11},
#line 20 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"cross_reverse",16},
#line 70 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"top_left_arrow",66},
#line 35 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"heart",31},
#line 63 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"shuttle",59},
#line 56 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"sailboat",52},
#line 60 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"sb_right_arrow",56},
#line 46 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"mouse",42},
#line 62 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"sb_v_double_arrow",58},
#line 43 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"lr_angle",39},
#line 5 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"arrow",1},
#line 9 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"bogosity",5},
#line 41 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"leftbutton",37},
#line 59 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"sb_left_arrow",55},
#line 49 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"plus",45},
#line 37 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"iron_cross",33},
#line 64 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"sizing",60},
#line 42 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"ll_angle",38},
#line 51 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"right_ptr",47},
#line 17 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"clock",13},
#line 23 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"dot",19},
#line 53 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"right_tee",49},
#line 52 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"right_side",48},
#line 6 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"based_arrow_down",2},
#line 78 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"ur_angle",74},
#line 16 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"circle",12},
#line 58 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"sb_h_double_arrow",54},
#line 4 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"X_cursor",0},
#line 50 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"question_arrow",46},
#line 61 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"sb_up_arrow",57},
#line 44 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"man",40},
#line 7 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"based_arrow_up",3},
#line 18 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"coffee_mug",14},
#line 47 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"pencil",43},
#line 76 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"ul_angle",72},
#line 80 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"xterm",76},
#line 54 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"rightbutton",50},
#line 66 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"spraycan",62},
#line 32 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"gumby",28},
#line 48 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"pirate",44},
#line 29 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"exchange",25},
#line 36 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"icon",32},
#line 79 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"watch",75},
#line 77 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"umbrella",73},
#line 25 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"double_arrow",21},
#line 57 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"sb_down_arrow",53},
#line 30 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"fleur",26},
#line 65 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"spider",61},
#line 34 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"hand2",30},
#line 33 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"hand1",29},
#line 24 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"dotbox",20},
#line 26 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"draft_large",22},
#line 22 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"diamond_cross",18},
#line 27 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"draft_small",23},
#line 45 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"middlebutton",41},
#line 28 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"
      {"draped_box",24}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= MIN_HASH_VALUE)
        {
          register struct shape_mapping *resword;

          switch (key - 4)
            {
              case 0:
                resword = &wordlist[0];
                goto compare;
              case 4:
                resword = &wordlist[1];
                goto compare;
              case 5:
                resword = &wordlist[2];
                goto compare;
              case 8:
                resword = &wordlist[3];
                goto compare;
              case 9:
                resword = &wordlist[4];
                goto compare;
              case 10:
                resword = &wordlist[5];
                goto compare;
              case 11:
                resword = &wordlist[6];
                goto compare;
              case 12:
                resword = &wordlist[7];
                goto compare;
              case 13:
                resword = &wordlist[8];
                goto compare;
              case 16:
                resword = &wordlist[9];
                goto compare;
              case 17:
                resword = &wordlist[10];
                goto compare;
              case 19:
                resword = &wordlist[11];
                goto compare;
              case 20:
                resword = &wordlist[12];
                goto compare;
              case 21:
                resword = &wordlist[13];
                goto compare;
              case 22:
                resword = &wordlist[14];
                goto compare;
              case 24:
                resword = &wordlist[15];
                goto compare;
              case 25:
                resword = &wordlist[16];
                goto compare;
              case 26:
                resword = &wordlist[17];
                goto compare;
              case 27:
                resword = &wordlist[18];
                goto compare;
              case 29:
                resword = &wordlist[19];
                goto compare;
              case 30:
                resword = &wordlist[20];
                goto compare;
              case 31:
                resword = &wordlist[21];
                goto compare;
              case 34:
                resword = &wordlist[22];
                goto compare;
              case 35:
                resword = &wordlist[23];
                goto compare;
              case 36:
                resword = &wordlist[24];
                goto compare;
              case 38:
                resword = &wordlist[25];
                goto compare;
              case 39:
                resword = &wordlist[26];
                goto compare;
              case 40:
                resword = &wordlist[27];
                goto compare;
              case 41:
                resword = &wordlist[28];
                goto compare;
              case 43:
                resword = &wordlist[29];
                goto compare;
              case 44:
                resword = &wordlist[30];
                goto compare;
              case 46:
                resword = &wordlist[31];
                goto compare;
              case 49:
                resword = &wordlist[32];
                goto compare;
              case 51:
                resword = &wordlist[33];
                goto compare;
              case 54:
                resword = &wordlist[34];
                goto compare;
              case 55:
                resword = &wordlist[35];
                goto compare;
              case 56:
                resword = &wordlist[36];
                goto compare;
              case 57:
                resword = &wordlist[37];
                goto compare;
              case 59:
                resword = &wordlist[38];
                goto compare;
              case 60:
                resword = &wordlist[39];
                goto compare;
              case 61:
                resword = &wordlist[40];
                goto compare;
              case 64:
                resword = &wordlist[41];
                goto compare;
              case 65:
                resword = &wordlist[42];
                goto compare;
              case 66:
                resword = &wordlist[43];
                goto compare;
              case 67:
                resword = &wordlist[44];
                goto compare;
              case 69:
                resword = &wordlist[45];
                goto compare;
              case 72:
                resword = &wordlist[46];
                goto compare;
              case 73:
                resword = &wordlist[47];
                goto compare;
              case 74:
                resword = &wordlist[48];
                goto compare;
              case 75:
                resword = &wordlist[49];
                goto compare;
              case 77:
                resword = &wordlist[50];
                goto compare;
              case 79:
                resword = &wordlist[51];
                goto compare;
              case 80:
                resword = &wordlist[52];
                goto compare;
              case 81:
                resword = &wordlist[53];
                goto compare;
              case 82:
                resword = &wordlist[54];
                goto compare;
              case 84:
                resword = &wordlist[55];
                goto compare;
              case 86:
                resword = &wordlist[56];
                goto compare;
              case 87:
                resword = &wordlist[57];
                goto compare;
              case 89:
                resword = &wordlist[58];
                goto compare;
              case 91:
                resword = &wordlist[59];
                goto compare;
              case 92:
                resword = &wordlist[60];
                goto compare;
              case 94:
                resword = &wordlist[61];
                goto compare;
              case 95:
                resword = &wordlist[62];
                goto compare;
              case 96:
                resword = &wordlist[63];
                goto compare;
              case 99:
                resword = &wordlist[64];
                goto compare;
              case 103:
                resword = &wordlist[65];
                goto compare;
              case 104:
                resword = &wordlist[66];
                goto compare;
              case 106:
                resword = &wordlist[67];
                goto compare;
              case 107:
                resword = &wordlist[68];
                goto compare;
              case 116:
                resword = &wordlist[69];
                goto compare;
              case 121:
                resword = &wordlist[70];
                goto compare;
              case 122:
                resword = &wordlist[71];
                goto compare;
              case 127:
                resword = &wordlist[72];
                goto compare;
              case 134:
                resword = &wordlist[73];
                goto compare;
              case 137:
                resword = &wordlist[74];
                goto compare;
              case 158:
                resword = &wordlist[75];
                goto compare;
              case 161:
                resword = &wordlist[76];
                goto compare;
            }
          return 0;
        compare:
          {
            register const char *s = resword->name;

            if (*str == *s && !strcmp (str + 1, s + 1))
              return resword;
          }
        }
    }
  return 0;
}
#line 81 "/share/OpenBSD/xenocara/lib/xcb-util-cursor/cursor/shape_to_id.gperf"

int cursor_shape_to_id(const char *name) {
	struct shape_mapping *mapping = in_word_set(name, strlen(name));
	return (mapping ? (mapping->number * 2) : -1);
}
