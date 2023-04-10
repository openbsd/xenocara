/*
 * Copyright 1995, 1999 by Patrick Lecoanet, France. <lecoanet@cena.dgac.fr>
 *
 * Permission to use, copy, modify, distribute, and sell this software and its
 * documentation for any purpose is  hereby granted without fee, provided that
 * the  above copyright   notice appear  in   all  copies and  that both  that
 * copyright  notice   and   this  permission   notice  appear  in  supporting
 * documentation, and that   the  name of  Patrick  Lecoanet not  be  used  in
 * advertising or publicity pertaining to distribution of the software without
 * specific,  written      prior  permission.     Patrick Lecoanet   makes  no
 * representations about the suitability of this software for any purpose.  It
 * is provided "as is" without express or implied warranty.
 *
 * PATRICK LECOANET DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE,
 * INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO
 * EVENT  SHALL PATRICK LECOANET BE LIABLE FOR ANY SPECIAL, INDIRECT OR
 * CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE,
 * DATA  OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER
 * TORTIOUS  ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
 * PERFORMANCE OF THIS SOFTWARE.
 *
 */

/*
 *******************************************************************************
 *******************************************************************************
 *
 * This driver is able to deal with Elographics SmartSet serial controllers.
 * It uses only a subset of the functions provided through the protocol.
 *
 * SUPPORT FOR E281-2310 and compatible controllers added with help of:
 *   1996/01/17 Juergen P. Meier (jpm@mailserv.rz.fh-muenchen.de) and
 *   1998/03/25 G.Felkel@edelmann.de
 *
 *   The E281-2310 is a somewhat lobotomized 2210.
 *   It does not support the c,g,h,k,l,p,q,s and t commands.
 *   Especially the P command, which is used to check the baud rate.
 *   The E281-2310 however semms to use always 9600bps, 8bit, 1stop
 *   no parity, Hardwarehandshake (RTS-CTS) (which are the drivers
 *   default values)
 *
 *******************************************************************************
 *******************************************************************************
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "xorgVersion.h"


#include "misc.h"
#include "xf86.h"
#include "xf86_OSproc.h"
#include "xf86Xinput.h"
#include "exevents.h"

#include "xf86Module.h"

/**
 * models to be treated specially.
 */
#define MODEL_UNKNOWN  -1
#define MODEL_SUNIT_D  1

typedef struct {
    int         type;
    const char  *name;
} Model;

static Model SupportedModels[] =
{
    {MODEL_SUNIT_D, "Sunit dSeries"}, /* sunit dSeries models don't reply to queries */
    {MODEL_UNKNOWN, NULL}
};
/*
 ***************************************************************************
 *
 * Default constants.
 *
 ***************************************************************************
 */
#define ELO_MAX_TRIALS	3		/* Number of timeouts waiting for a	*/
					/* pending reply.			*/
#define ELO_MAX_WAIT		100000	/* Max wait time for a reply (microsec)	*/
#define ELO_UNTOUCH_DELAY	5	/* 100 ms				*/
#define ELO_REPORT_DELAY	1	/* 40 ms or 25 motion reports/s		*/
#define ELO_LINK_SPEED		B9600	/* 9600 Bauds				*/
#define ELO_PORT		"/dev/ttyS1"

#define DEFAULT_MAX_X		3000
#define DEFAULT_MIN_X		600
#define DEFAULT_MAX_Y		3000
#define DEFAULT_MIN_Y		600


/*
 ***************************************************************************
 *
 * Protocol constants.
 *
 ***************************************************************************
 */
#define ELO_PACKET_SIZE		10

#define ELO_SYNC_BYTE		'U'	/* Sync byte. First of a packet.	*/
#define ELO_TOUCH		'T'	/* Report of touchs and motions. Not	*
					 * used by 2310.			*/
#define ELO_OWNER		'O'	/* Report vendor name.			*/
#define ELO_ID			'I'	/* Report of type and features.		*/
#define ELO_MODE		'M'	/* Set current operating mode.		*/
#define ELO_PARAMETER		'P'	/* Set the serial parameters.		*/
#define ELO_REPORT		'B'	/* Set touch reports timings.		*/
#define ELO_ACK			'A'	/* Acknowledge packet			*/

#define ELO_INIT_CHECKSUM	0xAA	/* Initial value of checksum.		*/

#define	ELO_PRESS		0x01	/* Flags in ELO_TOUCH status byte	*/
#define	ELO_STREAM		0x02
#define ELO_RELEASE		0x04

#define ELO_TOUCH_MODE		0x01	/* Flags in ELO_MODE command		*/
#define ELO_STREAM_MODE		0x02
#define ELO_UNTOUCH_MODE	0x04
#define ELO_RANGE_CHECK_MODE	0x40
#define ELO_TRIM_MODE		0x02
#define ELO_CALIB_MODE		0x04
#define ELO_SCALING_MODE	0x08
#define ELO_TRACKING_MODE	0x40

#define ELO_SERIAL_SPEED	0x06	/* Flags for high speed serial (19200)	*/
#define ELO_SERIAL_MASK		0xF8

#define ELO_SERIAL_IO		'0'	/* Indicator byte for PARAMETER command */


/*
 ***************************************************************************
 *
 * Useful macros.
 *
 ***************************************************************************
 */
#define WORD_ASSEMBLY(byte1, byte2)	(((byte2) << 8) | (byte1))
#define SYSCALL(call)			while(((call) == -1) && (errno == EINTR))

/* This one is handy, thanx Fred ! */
#ifdef DBG
#undef DBG
#endif
#ifdef DEBUG
#undef DEBUG
#endif

static int      debug_level = 0;
#define DEBUG 1
#if DEBUG
#define DBG(lvl, f) {if ((lvl) <= debug_level) f;}
#else
#define DBG(lvl, f)
#endif

#undef SYSCALL
#undef read
#undef write
#undef close
#define SYSCALL(call) call
#define read(fd, ptr, num) xf86ReadSerial(fd, ptr, num)
#define write(fd, ptr, num) xf86WriteSerial(fd, ptr, num)
#define close(fd) xf86CloseSerial(fd)


/*
 ***************************************************************************
 *
 * Device private records.
 *
 ***************************************************************************
 */
typedef struct _EloPrivateRec {
  char		*input_dev;		/* The touchscreen input tty			*/
  int		min_x;			/* Minimum x reported by calibration		*/
  int		max_x;			/* Maximum x					*/
  int		min_y;			/* Minimum y reported by calibration		*/
  int		max_y;			/* Maximum y					*/
  int		untouch_delay;		/* Delay before reporting an untouch (in ms)    */
  int		report_delay;		/* Delay between touch report packets		*/
  int		screen_no;		/* Screen associated with the device		*/
  int		screen_width;		/* Width of the associated X screen		*/
  int		screen_height;		/* Height of the screen				*/
  Bool		inited;			/* The controller has already been configured ?	*/
  Bool		is_a_2310;		/* Set if the smartset is a 2310.		*/
  int		checksum;		/* Current checksum of data in assembly buffer	*/
  int		packet_buf_p;		/* Assembly buffer pointer			*/
  int		swap_axes;		/* Swap X an Y axes if != 0 */
  unsigned char	packet_buf[ELO_PACKET_SIZE]; /* Assembly buffer				*/
  int		model;			/* one of MODEL_...				*/
} EloPrivateRec, *EloPrivatePtr;

/*
 ***************************************************************************
 *
 * xf86EloGetPacket --
 *	Read a packet from the port. Try to synchronize with start of
 *	packet and compute checksum.
 *      The packet structure read by this function is as follow:
 *		Byte 0 : ELO_SYNC_BYTE
 *		Byte 1
 *		...
 *		Byte 8 : packet data
 *		Byte 9 : checksum of bytes 0 to 8
 *
 *	This function returns if a valid packet has been assembled in
 *	buffer or if no more data is available.
 *
 *	Returns Success if a packet is successfully assembled including
 *	testing checksum. If a packet checksum is incorrect, it is discarded.
 *	Bytes preceding the ELO_SYNC_BYTE are also discarded.
 *	Returns !Success if out of data while reading. The start of the
 *	partially assembled packet is left in buffer, buffer_p and
 *	checksum reflect the current state of assembly.
 *
 ***************************************************************************
 */
static Bool
xf86EloGetPacket(unsigned char	*buffer,
		 int		*buffer_p,
		 int		*checksum,
		 int		fd)
{
  int	num_bytes;
  Bool	ok;

  DBG(4, ErrorF("Entering xf86EloGetPacket with checksum == %d and buffer_p == %d\n",
		*checksum, *buffer_p));

  /*
   * Try to read enough bytes to fill up the packet buffer.
   */
  DBG(4, ErrorF("buffer_p is %d, Trying to read %d bytes from link\n",
		*buffer_p, ELO_PACKET_SIZE - *buffer_p));
  SYSCALL(num_bytes = read(fd,
			   (char *) (buffer + *buffer_p),
			   ELO_PACKET_SIZE - *buffer_p));

  /*
   * Okay, give up.
   */
  if (num_bytes < 0) {
    ErrorF("System error while reading from Elographics touchscreen.");
    return !Success;
  }
  DBG(4, ErrorF("Read %d bytes\n", num_bytes));

  while (num_bytes) {
    /*
     * Sync with the start of a packet.
     */
    if ((*buffer_p == 0) && (buffer[0] != ELO_SYNC_BYTE)) {
      /*
       * No match, shift data one byte toward the start of the buffer.
       */
      ErrorF("Elographics: Dropping one byte in an attempt to synchronize: '%c' 0x%X\n",
	     buffer[0], buffer[0]);
      memcpy(&buffer[0], &buffer[1], num_bytes-1);
    }
    else {
      /*
       * Compute checksum in assembly buffer.
       */
      if (*buffer_p < ELO_PACKET_SIZE-1) {
	*checksum = *checksum + buffer[*buffer_p];
	*checksum = *checksum % 256;
	DBG(4, ErrorF(" 0x%X-->0x%X ", buffer[*buffer_p], *checksum));
      }
      (*buffer_p)++;
    }
    num_bytes--;
  }

  if (*buffer_p == ELO_PACKET_SIZE) {
    /*
     * Got a packet, validate checksum and reset state.
     */
    ok = (*checksum == buffer[ELO_PACKET_SIZE-1]);
    DBG(3, ErrorF("Expecting checksum %d, got %d\n", *checksum, buffer[ELO_PACKET_SIZE-1]));
    *checksum = ELO_INIT_CHECKSUM;
    *buffer_p = 0;

    if (!ok) {
      ErrorF("Checksum error on Elographics touchscreen link\n");
      return !Success;
    }

    /*
     * Valid packet received report it.
     */
    return Success;
  }
  else {
    return !Success;
  }
}


/*
 ***************************************************************************
 *
 * xf86EloReadInput --
 *	Read all pending report packets from the touchscreen and enqueue
 *	them.
 *	If a packet is not fully received it is deferred until the next
 *	call to the function.
 *	Packets recognized by this function comply with the format:
 *
 *		Byte 1 :  ELO_TOUCH
 *		Byte 2 :  Packet type
 *		  Bit 2 : Pen Up   (Release)
 *		  Bit 1 : Position (Stream)
 *		  Bit 0 : Pen Down (Press)
 *		Byte 3 :  X coordinate (lower bits)
 *		Byte 4 :  X coordinate (upper bits)
 *		Byte 5 :  Y coordinate (lower bits)
 *		Byte 6 :  Y coordinate (upper bits)
 *		Byte 7 :  Z coordinate (lower bits)
 *		Byte 8 :  Z coordinates (upper bits)
 *
 *
 ***************************************************************************
 */
static void
xf86EloReadInput(InputInfoPtr	pInfo)
{
  EloPrivatePtr			priv = (EloPrivatePtr)(pInfo->private);
  int				cur_x, cur_y;
  int				state;

  DBG(4, ErrorF("Entering ReadInput\n"));

  /*
   * Read bytes until there's no data left. We may have more or less than
   * one packet worth of data in the OS buffer.
   */
  do {
      if(xf86EloGetPacket(priv->packet_buf,
		       &priv->packet_buf_p,
		       &priv->checksum,
		       pInfo->fd) != Success)
          continue;

      /*
       * Process only ELO_TOUCHs here.
       */
      if (priv->packet_buf[1] == ELO_TOUCH) {
          /*
           * First stick together the various pieces.
           */
          cur_x = WORD_ASSEMBLY(priv->packet_buf[3], priv->packet_buf[4]);
          cur_y = WORD_ASSEMBLY(priv->packet_buf[5], priv->packet_buf[6]);
          state = priv->packet_buf[2] & 0x07;

          DBG(5, ErrorF("ELO got: x(%d), y(%d), %s\n",
                      cur_x, cur_y,
                      (state == ELO_PRESS) ? "Press" :
			((state == ELO_RELEASE) ? "Release" : "Stream")));

          if (priv->min_y > priv->max_y) {
            /* inverted y axis */
            cur_y = priv->max_y - cur_y + priv->min_y;
          }

          if (priv->min_x > priv->max_x) {
            /* inverted x axis */
            cur_x = priv->max_x - cur_x + priv->min_x;
          }


          /*
           * Send events.
           *
           * We *must* generate a motion before a button change if pointer
           * location has changed as DIX assumes this. This is why we always
           * emit a motion, regardless of the kind of packet processed.
           */
          xf86PostMotionEvent(pInfo->dev, TRUE, 0, 2, cur_x, cur_y);

          /*
           * Emit a button press or release.
           */
          if (state == ELO_PRESS || state == ELO_RELEASE) {
              xf86PostButtonEvent(pInfo->dev, TRUE, 1, state == ELO_PRESS, 0, 2, cur_x, cur_y);
          }

          DBG(3, ErrorF("TouchScreen: x(%d), y(%d), %s\n",
                      cur_x, cur_y,
                      (state == ELO_PRESS) ? "Press" : ((state == ELO_RELEASE) ? "Release" : "Stream")));
      }
  }
  while (xf86WaitForInput(pInfo->fd, 0) > 0);  /* don't wait, just check */
}


/*
 ***************************************************************************
 *
 * xf86EloSendPacket --
 *	Emit an height bytes packet to the controller.
 *	The function expects a valid buffer containing the
 *	command to be sent to the controller. It fills it with the
 *	leading sync character an the trailing checksum byte.
 *
 ***************************************************************************
 */
static Bool
xf86EloSendPacket(unsigned char	*packet,
		  int		fd)
{
  int	i, result;
  int	sum = ELO_INIT_CHECKSUM;

  packet[0] = ELO_SYNC_BYTE;
  for (i = 0; i < ELO_PACKET_SIZE-1; i++) {
    sum += packet[i];
    sum &= 0xFF;
  }
  packet[ELO_PACKET_SIZE-1] = sum;

  DBG(4, ErrorF("Sending packet : 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X 0x%X \n",
		packet[0], packet[1], packet[2], packet[3], packet[4],
		packet[5], packet[6], packet[7], packet[8], packet[9]));
  SYSCALL(result = write(fd, packet, ELO_PACKET_SIZE));
  if (result != ELO_PACKET_SIZE) {
    DBG(5, ErrorF("System error while sending to Elographics touchscreen.\n"));
    return !Success;
  }
  else {
    return Success;
  }
}


/*
 ***************************************************************************
 *
 * xf86EloWaitReply --
 *	It is assumed that the reply will be in the few next bytes
 *	read and will be available very soon after the query post. if
 *	these two asumptions are not met, there are chances that the server
 *	will be stuck for a while.
 *	The reply type need to match parameter 'type'.
 *	The reply is left in reply. The function returns Success if the
 *	reply is valid and !Success otherwise.
 *
 ***************************************************************************
 */

static Bool
xf86EloWaitReply(unsigned char	type,
		 unsigned char	*reply,
		 int		fd)
{
  Bool			ok;
  int			i, result;
  int			reply_p = 0;
  int			sum = ELO_INIT_CHECKSUM;

  DBG(4, ErrorF("Waiting a '%c' reply\n", type));
  i = ELO_MAX_TRIALS;
  do {
    ok = !Success;

    /*
     * Wait half a second for the reply. The fuse counts down each
     * timeout and each wrong packet.
     */
    DBG(4, ErrorF("Waiting %d ms for data from port\n", ELO_MAX_WAIT / 1000));
    result = xf86WaitForInput(fd, ELO_MAX_WAIT);
    if (result > 0) {
      ok = xf86EloGetPacket(reply, &reply_p, &sum, fd);
      /*
       * Do not report an error on a 'P' query as the controller
       * might be a 2310.
       */
      if (ok == Success && reply[1] != type && type != ELO_PARAMETER) {
	DBG(3, ErrorF("Wrong reply received\n"));
	ok = !Success;
      }
    }
    else {
      DBG(3, ErrorF("No answer from link : %d\n", result));
    }

    if (result == 0) {
      i--;
    }
  } while(ok != Success && i);

  return ok;
}


/*
 ***************************************************************************
 *
 * xf86EloWaitAck --
 *	Wait for an acknowledge from the controller. Returns Success if
 *	acknowledge received and reported no errors.
 *
  ***************************************************************************
 */
static Bool
xf86EloWaitAck(int	fd)
{
  unsigned char	packet[ELO_PACKET_SIZE];
  int		i, nb_errors;

  if (xf86EloWaitReply(ELO_ACK, packet, fd) == Success) {
    for (i = 0, nb_errors = 0; i < 4; i++) {
      if (packet[2 + i] != '0') {
	nb_errors++;
      }
    }
    if (nb_errors != 0) {
      DBG(2, ErrorF("Elographics acknowledge packet reports %d errors\n",
		    nb_errors));
    }
    return Success;
    /*    return (nb_errors < 4) ? Success : !Success;*/
  }
  else {
    return !Success;
  }
}


/*
 ***************************************************************************
 *
 * xf86EloSendQuery --
 *	Emit a query to the controller and blocks until the reply and
 *	the acknowledge are read.
 *
 *	The reply is left in reply. The function returns Success if the
 *	reply is valid and !Success otherwise.
 *
 ***************************************************************************
 */
static Bool
xf86EloSendQuery(unsigned char	*request,
		 unsigned char	*reply,
		 int		fd)
{
  Bool			ok;

  if (xf86EloSendPacket(request, fd) == Success) {
    ok = xf86EloWaitReply(toupper(request[1]), reply, fd);
    if (ok == Success) {
      ok = xf86EloWaitAck(fd);
    }
    return ok;
  }
  else {
    return !Success;
  }
}


/*
 ***************************************************************************
 *
 * xf86EloSendControl --
 *	Emit a control command to the controller and wait for acknowledge.
 *
 *	Returns Success if acknowledge received and reported no error.
 *
 ***************************************************************************
 */
static Bool
xf86EloSendControl(unsigned char	*control,
		   int			fd)
{
  if (xf86EloSendPacket(control, fd) == Success) {
    return xf86EloWaitAck(fd);
  }
  else {
    return !Success;
  }
}


/*
 ***************************************************************************
 *
 * xf86EloPrintIdent --
 *	Print type of touchscreen and features on controller board.
 *
 ***************************************************************************
 */
static void
xf86EloPrintIdent(unsigned char	*packet,
		  EloPrivatePtr	priv)
{
  xf86Msg(X_PROBED, "Elographics touchscreen is a ");
  switch(packet[2]) {
  case '0':
    xf86Msg(X_NONE, "AccuTouch");
    break;
  case '1':
    xf86Msg(X_NONE, "DuraTouch");
    break;
  case '2':
    xf86Msg(X_NONE, "Intellitouch");
    break;
  }
  xf86Msg(X_NONE, ", connected through a ");
  switch(packet[3]) {
  case '0':
    xf86Msg(X_NONE, "serial link.\n");
    break;
  case '1':
    xf86Msg(X_NONE, "PC-Bus port.\n");
    break;
  case '2':
    xf86Msg(X_NONE, "Micro Channel port.\n");
    break;
  }
  xf86Msg(X_PROBED, "The controller is a model ");
  if (packet[8] & 1) {
    if (priv->is_a_2310) {
      xf86Msg(X_NONE, "E281-2310");
    }
    else {
      xf86Msg(X_NONE, "E271-2210");
    }
  }
  else {
    xf86Msg(X_NONE, "E271-2200");
  }
  xf86Msg(X_NONE, ", firmware revision %d.%d.\n", packet[6], packet[5]);

  if (packet[4]) {
    xf86Msg(X_PROBED, " Additional features:\n");
    if (packet[4] & 0x10) {
      xf86Msg(X_PROBED, "	External A/D converter\n");
    }
    if (packet[4] & 0x20) {
      xf86Msg(X_PROBED, "	32Ko RAM\n");
    }
    if (packet[4] & 0x40) {
      xf86Msg(X_PROBED, "	RAM onboard\n");
    }
    if (packet[4] & 0x80) {
      xf86Msg(X_PROBED, "	Z axis active\n");
    }
    xf86Msg(X_NONE, "\n");
  }
}


/*
 ***************************************************************************
 *
 * xf86EloPtrControl --
 *
 ***************************************************************************
 */

static void
xf86EloPtrControl(DeviceIntPtr	dev,
		  PtrCtrl	*ctrl)
{
}



/*
 ***************************************************************************
 *
 * xf86EloControl --
 *
 ***************************************************************************
 */
static Bool
xf86EloControl(DeviceIntPtr	dev,
	       int		mode)
{
  InputInfoPtr	pInfo = (InputInfoPtr) dev->public.devicePrivate;
  EloPrivatePtr		priv = (EloPrivatePtr)(pInfo->private);
  unsigned char		map[] = { 0, 1 };
  unsigned char		req[ELO_PACKET_SIZE];
  unsigned char		reply[ELO_PACKET_SIZE];
  Atom btn_label;
  Atom axis_labels[2] = { 0, 0 };
  int x0, x1, y0, y1;

  switch(mode) {

  case DEVICE_INIT:
    {
      DBG(2, ErrorF("Elographics touchscreen init...\n"));

      if (priv->screen_no >= screenInfo.numScreens ||
	  priv->screen_no < 0) {
	priv->screen_no = 0;
      }
      priv->screen_width = screenInfo.screens[priv->screen_no]->width;
      priv->screen_height = screenInfo.screens[priv->screen_no]->height;

      /*
       * Device reports button press for up to 1 button.
       */
      if (InitButtonClassDeviceStruct(dev, 1, &btn_label, map) == FALSE) {
	ErrorF("Unable to allocate Elographics touchscreen ButtonClassDeviceStruct\n");
	return !Success;
      }

      if (InitFocusClassDeviceStruct(dev) == FALSE) {
	ErrorF("Unable to allocate Elographics touchscreen FocusClassDeviceStruct\n");
	return !Success;
      }
      if (InitPtrFeedbackClassDeviceStruct(dev, xf86EloPtrControl) == FALSE) {
	  ErrorF("unable to init ptr feedback\n");
	  return !Success;
      }
      /*
       * Device reports motions on 2 axes in absolute coordinates.
       * Axes min and max values are reported in raw coordinates.
       * Resolution is computed roughly by the difference between
       * max and min values scaled from the approximate size of the
       * screen to fit one meter.
       */
      if (InitValuatorClassDeviceStruct(dev, 2, axis_labels,
					GetMotionHistorySize(), Absolute) == FALSE) {
	ErrorF("Unable to allocate Elographics touchscreen ValuatorClassDeviceStruct\n");
	return !Success;
      }
      else {

	/* Correct the coordinates for possibly inverted axis.
	   Leave priv->variables untouched so we can check for
	   inversion on incoming events.
	 */
	y0 = min(priv->min_y, priv->max_y);
	y1 = max(priv->min_y, priv->max_y);
	x0 = min(priv->min_x, priv->max_x);
	x1 = max(priv->min_x, priv->max_x);

	/* I will map coordinates myself */
	InitValuatorAxisStruct(dev, 0,
			       axis_labels[0],
			       x0, x1,
			       9500,
			       0     /* min_res */,
			       9500  /* max_res */,
			       Absolute);
	InitValuatorAxisStruct(dev, 1,
			       axis_labels[1],
			       y0, y1,
			       10500,
			       0     /* min_res */,
			       10500 /* max_res */,
			       Absolute);
      }

      if (InitFocusClassDeviceStruct(dev) == FALSE) {
	ErrorF("Unable to allocate Elographics touchscreen FocusClassDeviceStruct\n");
      }

      /*
       * Allocate the motion events buffer.
       */
      xf86MotionHistoryAllocate(pInfo);


      DBG(2, ErrorF("Done.\n"));
      return Success;
    }

  case DEVICE_ON:
    DBG(2, ErrorF("Elographics touchscreen on...\n"));

    if (pInfo->fd < 0) {

      DBG(2, ErrorF("Elographics touchscreen opening : %s\n", priv->input_dev));
      pInfo->fd = xf86OpenSerial(pInfo->options);
      if (pInfo->fd < 0) {
	ErrorF("Unable to open Elographics touchscreen device");
	return !Success;
      }

      if (priv->model != MODEL_SUNIT_D)
      {
          /*
           * Try to see if the link is at the specified rate and
           * ask the controller to report various infos.
           */
          memset(req, 0, ELO_PACKET_SIZE);
          req[1] = tolower(ELO_PARAMETER);
          if (xf86EloSendQuery(req, reply, pInfo->fd) != Success) {
              priv->is_a_2310 = 1;
              ErrorF("Not at the specified rate or model 2310, will continue\n");
          }

          /*
           * Ask the controller to report various infos.
           */
          memset(req, 0, ELO_PACKET_SIZE);
          req[1] = tolower(ELO_ID);
          if (xf86EloSendQuery(req, reply, pInfo->fd) == Success) {
              xf86EloPrintIdent(reply, priv);
          }
          else {
              DBG(2, ErrorF("Unable to ask Elographics touchscreen identification... Maybe it's GeneralTouch touchscreen...\n"));
          }

          /*
           * Set the operating mode: Stream, no scaling, no calibration,
           * no range checking, no trim, tracking enabled.
           */
          memset(req, 0, ELO_PACKET_SIZE);
          req[1] = ELO_MODE;
          req[3] = ELO_TOUCH_MODE | ELO_STREAM_MODE | ELO_UNTOUCH_MODE;
          req[4] = ELO_TRACKING_MODE;
          if (xf86EloSendControl(req, pInfo->fd) != Success) {
              DBG(2, ErrorF("Unable to change Elographics touchscreen operating mode... Maybe it's GeneralTouch touchscreen...\n"));
          }

          /*
           * Set the touch reports timings from configuration data.
           */
          memset(req, 0, ELO_PACKET_SIZE);
          req[1] = ELO_REPORT;
          req[2] = priv->untouch_delay;
          req[3] = priv->report_delay;
          if (xf86EloSendControl(req, pInfo->fd) != Success) {
              DBG(2, ErrorF("Unable to change Elographics touchscreen reports timings... Maybe it's GeneralTouch touchscreen...\n"));
          }
      }
      xf86AddEnabledDevice(pInfo);
      dev->public.on = TRUE;
    }

    DBG(2, ErrorF("Done\n"));
    return Success;

    /*
     * Deactivate the device. After this, the device will not emit
     * events until a subsequent DEVICE_ON. Thus, we can momentarily
     * close the port.
     */
  case DEVICE_OFF:
    DBG(2, ErrorF("Elographics touchscreen off...\n"));
    dev->public.on = FALSE;
    if (pInfo->fd >= 0) {
      xf86RemoveEnabledDevice(pInfo);
    }
    SYSCALL(close(pInfo->fd));
    pInfo->fd = -1;
    DBG(2, ErrorF("Done\n"));
    return Success;

    /*
     * Final close before server exit. This is used during server shutdown.
     * Close the port and free all the resources.
     */
  case DEVICE_CLOSE:
    DBG(2, ErrorF("Elographics touchscreen close...\n"));
    dev->public.on = FALSE;
    if (pInfo->fd >= 0) {
	xf86RemoveEnabledDevice(pInfo);
    }
    SYSCALL(close(pInfo->fd));
    pInfo->fd = -1;
    DBG(2, ErrorF("Done\n"));
    return Success;

#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) * 100 + GET_ABI_MINOR(ABI_XINPUT_VERSION) >= 1901
  case DEVICE_ABORT:
    return Success;
#endif

  default:
      ErrorF("unsupported mode=%d\n", mode);
      return BadValue;
  }
}

/*
 ***************************************************************************
 *
 * xf86EloAllocate --
 *
 ***************************************************************************
 */
static int
xf86EloAllocate(InputDriverPtr drv, InputInfoPtr pInfo)
{
  EloPrivatePtr		priv;

  priv = malloc(sizeof(EloPrivateRec));
  if (!priv)
    return BadAlloc;

  priv->input_dev = strdup(ELO_PORT);
  priv->min_x = 0;
  priv->max_x = 3000;
  priv->min_y = 0;
  priv->max_y = 3000;
  priv->untouch_delay = ELO_UNTOUCH_DELAY;
  priv->report_delay = ELO_REPORT_DELAY;
  priv->screen_no = 0;
  priv->screen_width = -1;
  priv->screen_height = -1;
  priv->inited = 0;
  priv->is_a_2310 = 0;
  priv->checksum = ELO_INIT_CHECKSUM;
  priv->packet_buf_p = 0;
  priv->swap_axes = 0;

  pInfo->device_control = xf86EloControl;
  pInfo->read_input   = xf86EloReadInput;
  pInfo->control_proc = NULL;
  pInfo->switch_mode  = NULL;
  pInfo->private      = priv;
  pInfo->type_name    = XI_TOUCHSCREEN;

  return Success;
}


static void
xf86EloUninit(InputDriverPtr	drv,
	      InputInfoPtr	pInfo,
	      int flags)
{
  EloPrivatePtr		priv = (EloPrivatePtr) pInfo->private;

  free(priv->input_dev);
  free(priv);
  pInfo->private = NULL;
  xf86DeleteInput(pInfo, 0);
}

static
#if GET_ABI_MAJOR(ABI_XINPUT_VERSION) >= 18
const
#endif
char *default_options[] = {
  "BaudRate", "9600",
  "StopBits", "1",
  "DataBits", "8",
  "Parity", "None",
  "FlowControl", "None",
  NULL
};

static int
xf86EloInit(InputDriverPtr	drv,
	    InputInfoPtr	pInfo,
	    int			flags)
{
  EloPrivatePtr		priv=NULL;
  const char		*str;
  int			portrait = 0;
  int			height, width;
  char			*opt_model;
  Model*		model;
  int			rc;


  rc = xf86EloAllocate(drv, pInfo);
  if (rc != Success)
    return rc;

  priv = pInfo->private;

  str = xf86SetStrOption(pInfo->options, "Device", NULL);
  if (!str) {
    xf86Msg(X_ERROR, "%s: No Device specified in Elographics module config.\n",
	    pInfo->name);
    return BadValue;
  } else {
      pInfo->fd = xf86OpenSerial(pInfo->options);
      if (pInfo->fd < 0) {
	xf86Msg(X_ERROR, "%s: Unable to open Elographics touchscreen device %s", pInfo->name, str);
	return BadValue;
      }
      xf86CloseSerial(pInfo->fd);
      pInfo->fd = -1;
  }

  priv->input_dev = strdup(str);

  opt_model = xf86SetStrOption(pInfo->options, "Model", NULL);
  model = SupportedModels;
  priv->model = MODEL_UNKNOWN;
  while(model->type != MODEL_UNKNOWN && opt_model)
  {
      if (!strcmp(model->name, opt_model))
      {
          priv->model = model->type;
          break;
      }
      model++;
  }

  priv->screen_no = xf86SetIntOption(pInfo->options, "ScreenNo", 0);
  xf86Msg(X_CONFIG, "Elographics associated screen: %d\n", priv->screen_no);
  priv->untouch_delay = xf86SetIntOption(pInfo->options, "UntouchDelay", ELO_UNTOUCH_DELAY);
  xf86Msg(X_CONFIG, "Elographics untouch delay: %d ms\n", priv->untouch_delay*10);
  priv->report_delay = xf86SetIntOption(pInfo->options, "ReportDelay", ELO_REPORT_DELAY);
  xf86Msg(X_CONFIG, "Elographics report delay: %d ms\n", priv->report_delay*10);
  priv->max_x = xf86SetIntOption(pInfo->options, "MaxX", 3000);
  xf86Msg(X_CONFIG, "Elographics maximum x position: %d\n", priv->max_x);
  priv->min_x = xf86SetIntOption(pInfo->options, "MinX", 0);
  xf86Msg(X_CONFIG, "Elographics minimum x position: %d\n", priv->min_x);
  priv->max_y = xf86SetIntOption(pInfo->options, "MaxY", 3000);
  xf86Msg(X_CONFIG, "Elographics maximum y position: %d\n", priv->max_y);
  priv->min_y = xf86SetIntOption(pInfo->options, "MinY", 0);
  xf86Msg(X_CONFIG, "Elographics minimum y position: %d\n", priv->min_y);
  priv->swap_axes = xf86SetBoolOption(pInfo->options, "SwapXY", 0);
  if (priv->swap_axes) {
    xf86Msg(X_CONFIG, "Elographics device will work with X and Y axes swapped\n");
  }
  debug_level = xf86SetIntOption(pInfo->options, "DebugLevel", 0);
  if (debug_level) {
#if DEBUG
    xf86Msg(X_CONFIG, "Elographics debug level sets to %d\n", debug_level);
#else
    xf86Msg(X_INFO, "Elographics debug not available\n");
#endif
  }
  str = xf86SetStrOption(pInfo->options, "PortraitMode", "Landscape");
  if (strcmp(str, "Portrait") == 0) {
    portrait = 1;
  }
  else if (strcmp(str, "PortraitCCW") == 0) {
    portrait = -1;
  }
  else if (strcmp(str, "Landscape") != 0) {
    xf86Msg(X_ERROR, "Elographics portrait mode should be: Portrait, Landscape or PortraitCCW");
    str = "Landscape";
  }
  xf86Msg(X_CONFIG, "Elographics device will work in %s mode\n", str);

  width = priv->max_x - priv->min_x;
  height = priv->max_y - priv->min_y;
  if (width == 0) {
    xf86Msg(X_ERROR, "Elographics: Cannot configure touchscreen with width 0\n");
    return BadValue;
  }
  else if (width < 0) {
    xf86Msg(X_INFO, "Elographics: reverse x mode (minimum x position >= maximum x position)\n");
  }
  if (height == 0) {
    xf86Msg(X_ERROR, "Elographics: Cannot configure touchscreen with height 0\n");
    return BadValue;
  }
  else if (height < 0) {
    xf86Msg(X_INFO, "Elographics: reverse y mode (minimum y position >= maximum y position)\n");
  }

  if (portrait == 1) {
    /*
     * Portrait Clockwise: reverse Y axis and exchange X and Y.
     */
    int tmp;
    tmp = priv->min_y;
    priv->min_y = priv->max_y;
    priv->max_y = tmp;
    priv->swap_axes = (priv->swap_axes==0) ? 1 : 0;
  }
  else if (portrait == -1) {
    /*
     * Portrait Counter Clockwise: reverse X axis and exchange X and Y.
     */
    int tmp;
    tmp = priv->min_x;
    priv->min_x = priv->max_x;
    priv->max_x = tmp;
    priv->swap_axes = (priv->swap_axes==0) ? 1 : 0;
  }

  return Success;
}

_X_EXPORT InputDriverRec ELOGRAPHICS = {
    1,				/* driver version */
    "elographics",		/* driver name */
    NULL,			/* identify */
    xf86EloInit,		/* pre-init */
    xf86EloUninit,		/* un-init */
    NULL,			/* module */
    default_options
};

static pointer
Plug(pointer	module,
     pointer	options,
     int	*errmaj,
     int	*errmin)
{
  xf86AddInputDriver(&ELOGRAPHICS, module, 0);

  return module;
}

static void
Unplug(pointer	p)
{
  DBG(1, ErrorF("EloUnplug\n"));
}

static XF86ModuleVersionInfo version_rec = {
  "elographics",
  MODULEVENDORSTRING,
  MODINFOSTRING1,
  MODINFOSTRING2,
  XORG_VERSION_CURRENT,
  PACKAGE_VERSION_MAJOR, PACKAGE_VERSION_MINOR, PACKAGE_VERSION_PATCHLEVEL,
  ABI_CLASS_XINPUT,
  ABI_XINPUT_VERSION,
  MOD_CLASS_XINPUT,
  { 0, 0, 0, 0 }
};

/*
 * This is the entry point in the module. The name
 * is setup after the pattern <module_name>ModuleData.
 * Do not change it.
 */
_X_EXPORT XF86ModuleData elographicsModuleData = { &version_rec, Plug, Unplug };

