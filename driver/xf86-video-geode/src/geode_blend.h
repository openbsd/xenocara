#ifndef AMD_BLEND_H_
#define AMD_BLEND_H_

#define GEODEPTR_FROM_PIXMAP(x)		\
		GEODEPTR(xf86Screens[(x)->drawable.pScreen->myNum])
#define GEODEPTR_FROM_SCREEN(x)		\
		GEODEPTR(xf86Screens[(x)->myNum])
#define GEODEPTR_FROM_PICTURE(x)	\
		GEODEPTR(xf86Screens[(x)->pDrawable->pScreen->myNum])

#define usesPasses(op) (((		\
   ( 1 << PictOpAtop ) | 		\
   ( 1 << PictOpAtopReverse ) | \
   ( 1 << PictOpXor ) | 		\
   0 ) >> (op)) & 1)

/* pass1 or pass2 */
#define usesSrcAlpha(op) ((( 		\
   ( 1 << PictOpOver ) | 			\
   ( 1 << PictOpInReverse  ) | 		\
   ( 1 << PictOpOutReverse  ) | 	\
   ( 1 << PictOpAtop ) | 			\
   ( 1 << PictOpAtopReverse ) | 	\
   ( 1 << PictOpXor ) | 			\
   0 ) >> (op)) & 1)

/* pass1 or pass2 */
#define usesDstAlpha(op) ((( 		\
   ( 1 << PictOpOverReverse ) | 	\
   ( 1 << PictOpIn ) | 				\
   ( 1 << PictOpOut ) | 			\
   ( 1 << PictOpAtop ) | 			\
   ( 1 << PictOpAtopReverse ) | 	\
   ( 1 << PictOpXor ) | 			\
   0 ) >> (op)) & 1)

/* non 2 pass ops */
#define usesChanB0(op) ((( 			\
   ( 1 << PictOpOver ) | 			\
   ( 1 << PictOpOverReverse ) | 	\
   ( 1 << PictOpIn ) | 				\
   ( 1 << PictOpInReverse ) | 		\
   ( 1 << PictOpOut ) | 			\
   ( 1 << PictOpOutReverse ) | 		\
   ( 1 << PictOpAdd ) | 			\
   0 ) >> (op)) & 1)

/* pass 1 ops */
#define usesChanB1(op) ((( 			\
   ( 1 << PictOpAtop ) | 			\
   ( 1 << PictOpAtopReverse ) | 	\
   ( 1 << PictOpXor ) | 			\
   0 ) >> (op)) & 1)

/* pass 2 ops */
#define usesChanB2(op) ((( 			\
   ( 1 << PictOpAtop ) | 			\
   ( 1 << PictOpAtopReverse ) | 	\
   ( 1 << PictOpXor ) | 			\
   0 ) >> (op)) & 1)

#endif
