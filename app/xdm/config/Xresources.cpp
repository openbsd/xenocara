! $Xorg: Xresources,v 1.3 2000/08/17 19:54:17 cpqbld Exp $
! $OpenBSD: Xresources.cpp,v 1.4 2007/09/15 15:22:59 matthieu Exp $
!
!
!
!
! $XFree86: xc/programs/xdm/config/Xres.cpp,v 1.6 2003/01/04 03:11:31 dawes Exp $
!

#define BS \ /* cpp can be trickier than m4 */
#define NLBS \n\ /* don't remove these comments */
xlogin*login.translations: #override BS
	Ctrl<Key>R: abort-display()NLBS
	<Key>F1: set-session-argument(failsafe) finish-field()NLBS
	<Key>Left: move-backward-character()NLBS
	<Key>Right: move-forward-character()NLBS
	<Key>Home: move-to-begining()NLBS
	<Key>End: move-to-end()NLBS
	Ctrl<Key>KP_Enter: set-session-argument(failsafe) finish-field()NLBS
	<Key>KP_Enter: set-session-argument() finish-field()NLBS
	Ctrl<Key>Return: set-session-argument(failsafe) finish-field()NLBS
	<Key>Return: set-session-argument() finish-field()

xlogin*greeting: Welcome to CLIENTHOST
xlogin*namePrompt: \040\040\040\040\040\040\040Login:
xlogin*fail: Login incorrect

XHASHif WIDTH > 800
xlogin*greetFont: -adobe-helvetica-bold-o-normal--24-240-75-75-p-138-iso8859-1
xlogin*font: -adobe-helvetica-medium-r-normal--18-180-75-75-p-98-iso8859-1
xlogin*promptFont: -adobe-helvetica-bold-r-normal--18-180-75-75-p-103-iso8859-1
xlogin*failFont: -adobe-helvetica-bold-r-normal--18-180-75-75-p-103-iso8859-1
xlogin*greetFace:	Sans-24:bold:italic
xlogin*face: 		Sans-18
xlogin*promptFace: 	Sans-18:bold
xlogin*failFace: 	Sans-18:bold
XHASHelse
xlogin*greetFont: -adobe-helvetica-bold-o-normal--17-120-100-100-p-92-iso8859-1
xlogin*font: -adobe-helvetica-medium-r-normal--12-120-75-75-p-67-iso8859-1
xlogin*promptFont: -adobe-helvetica-bold-r-normal--12-120-75-75-p-70-iso8859-1
xlogin*failFont: -adobe-helvetica-bold-o-normal--14-140-75-75-p-82-iso8859-1
xlogin*greetFace:	Sans-16:bold:italic
xlogin*face: 		Sans-12
xlogin*promptFace: 	Sans-12:bold
xlogin*failFace: 	Sans-12:bold
XHASHendif

XHASHif !(defined(bpp1) || defined(bpp4) || defined(bpp8) || defined(bpp15))
XHASH if PLANES < 4 || defined(Hp300Architecture)
XHASH  ifndef bpp1
XHASH   define bpp1
XHASH  endif
XHASH else
XHASH  if PLANES > 4
XHASH   if PLANES > 8
XHASH    ifndef bpp15
XHASH     define bpp15
XHASH    endif
XHASH   else
XHASH    ifndef bpp8
XHASH     define bpp8
XHASH    endif bpp8
XHASH   endif
XHASH  else
XHASH   ifndef bpp4
XHASH    define bpp4
XHASH   endif
XHASH  endif
XHASH endif
XHASHendif  //**/* If manual override */**//

XHASHifndef bpp1
xlogin*borderWidth: 2
xlogin*frameWidth: 5
xlogin*innerFramesWidth: 2

! top/left border
xlogin*hiColor: #6d9df2

! bottom/right border
xlogin*shdColor: #384c70

! 'Welcome to..' text color
xlogin*greetColor: #000000
XHASHif defined(bpp4) || defined(bpp8) || defined(bpp15)
! flood fill
!xlogin*background: #2559a5
xlogin*background: #5272b6
XHASHendif
xlogin*failColor: red

! 'Login:' and 'Password:'
*Foreground: #fbfeff

! border/shadow
*Background: #000000
XHASHelse
xlogin*borderWidth: 3
xlogin*frameWidth: 5
xlogin*innerFramesWidth: 1
xlogin*shdColor: white
xlogin*hiColor: white
xlogin*greetColor: white
xlogin*background: black
xlogin*failColor: white
xlogin*promptColor: white
*Foreground: white
*Background: black
XHASHendif
XHASHifdef bpp1
xlogin*logoFileName: BITMAPDIR/**//OpenBSD_1bpp.xpm
XHASHendif
#if ! defined(SparcArchitecture)
XHASHif defined(bpp4) || defined(bpp8)
xlogin*logoFileName: BITMAPDIR/**//OpenBSD_4bpp.xpm
XHASHendif
#else /* sparc */
XHASHifdef bpp4
xlogin*logoFileName: BITMAPDIR/**//OpenBSD_4bpp.xpm
XHASHendif
XHASHifdef bpp8
xlogin*logoFileName: BITMAPDIR/**//OpenBSD_8bpp.xpm
XHASHendif
#endif
XHASHifdef bpp15
xlogin*logoFileName: BITMAPDIR/**//OpenBSD_15bpp.xpm
XHASHendif
#if ! defined(Hp300Architecture)
xlogin*useShape: true
xlogin*logoPadding: 10
#endif /* Hp300Architecture */

! comment out to disable root logins
xlogin.Login.allowRootLogin:	true

XConsole*background:	black
XConsole*foreground:	white
XConsole*borderWidth:	2
XConsole*borderColor:   grey
XConsole.text.geometry:	480x130
XConsole.verbose:	true
XConsole*iconic:	true
XConsole*font:		fixed

Chooser*geometry:		700x500+300+200
Chooser*allowShellResize:	false
Chooser*viewport.forceBars:	true
Chooser*label.font:		*-new century schoolbook-bold-i-normal-*-240-*
Chooser*label.label:		XDMCP Host Menu from CLIENTHOST
Chooser*list.font:		-*-*-medium-r-normal-*-*-230-*-*-c-*-iso8859-1
Chooser*Command.font:		*-new century schoolbook-bold-r-normal-*-180-*
