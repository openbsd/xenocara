/*
 * Copyright (c) 2013 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * Authors:
 *    Chris Wilson <chris@chris-wilson.co.uk>
 *
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>

#include "sna.h"

#define ACPI_SOCKET  "/var/run/acpid.socket"

int sna_acpi_open(void)
{
	struct sockaddr_un addr;
	int fd, ret;

	DBG(("%s\n", __FUNCTION__));

	fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (fd < 0)
		return -1;

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, ACPI_SOCKET);

	ret = connect(fd, (struct sockaddr *)&addr, sizeof(addr));
	if (ret < 0) {
		close(fd);
		return -1;
	}

	DBG(("%s: opened socket to APCI daemon, fd=%d\n", __FUNCTION__, fd));

	return fd;
}

void _sna_acpi_wakeup(struct sna *sna)
{
	char *eol;
	int n;

	n = read(sna->acpi.fd,
		 sna->acpi.event + sna->acpi.offset,
		 sna->acpi.remain);
	DBG(("%s: read %d bytes from acpid\n", __FUNCTION__, n));
	if (n <= 0) {
		/* We will get '0' if we run out of space whilst reading
		 * one event - that should never happen, so treat it as
		 * an error and give up.
		 */
		if (n < 0)
			n = errno;
		switch (n) {
		case EAGAIN:
		case EINTR:
			return;
		}

		DBG(("%s: error [%d], detaching from acpid\n", __FUNCTION__, n));

		/* XXX reattach later? */
		RemoveNotifyFd(sna->acpi.fd);
		sna_acpi_fini(sna);
		return;
	}

	sna->acpi.event[sna->acpi.offset + n] = '\0';
	sna->acpi.offset += n;
	sna->acpi.remain -= n;

	DBG(("%s: event string [%d]: '%s'\n", __FUNCTION__, sna->acpi.offset, sna->acpi.event));

	do {
		eol = strchr(sna->acpi.event, '\n');
		if (eol == NULL)
			return;

		if (strncmp(sna->acpi.event, "ac_adapter", 10) == 0) {
			char *space = sna->acpi.event;
			int state = -1;

			/* ac_adapter ACAD 00000080 00000001 */

			space = strchr(space, ' ');
			if (space)
				space = strchr(space + 1, ' ');
			if (space)
				space = strchr(space + 1, ' ');
			if (space)
				state = atoi(space + 1);

			DBG(("%s: ac_adapter event new state=%d\n", __FUNCTION__, state));
			if (state)
				sna->flags &= ~SNA_POWERSAVE;
			else
				sna->flags |= SNA_POWERSAVE;
		}

		n = (sna->acpi.event + sna->acpi.offset) - ++eol;
		memmove(sna->acpi.event, eol, n+1);
		sna->acpi.offset = n;
		sna->acpi.remain = sizeof(sna->acpi.event) - 1 - n;
	} while (n);
}

#if HAVE_NOTIFY_FD
static void sna_acpi_notify(int fd, int read, void *data)
{
	_sna_acpi_wakeup(data);
}
#endif

static int read_power_state(const char *path)
{
	DIR *dir;
	struct dirent *de;
	int i = -1;

	DBG(("%s: searching '%s'\n", __FUNCTION__, path));

	dir = opendir(path);
	if (dir == NULL)
		return -1;

	while ((de = readdir(dir))) {
		char buf[1024];
		int fd;

		if (*de->d_name == '.')
			continue;

		DBG(("%s: checking '%s'\n", __FUNCTION__, de->d_name));

		snprintf(buf, sizeof(buf), "%s/%s/type", path, de->d_name);
		fd = open(buf, 0);
		if (fd < 0)
			continue;

		i = read(fd, buf, sizeof(buf));
		buf[i > 0 ? i - 1: 0] = '\0';
		close(fd);

		DBG(("%s: %s is of type '%s'\n", __FUNCTION__, de->d_name, buf));

		if (strcmp(buf, "Mains"))
			continue;

		snprintf(buf, sizeof(buf), "%s/%s/online", path, de->d_name);
		fd = open(buf, 0);
		if (fd < 0)
			continue;

		i = read(fd, buf, sizeof(buf));
		buf[i > 0 ? i - 1: 0] = '\0';
		if (i > 0)
			i = atoi(buf);
		DBG(("%s: %s is online? '%s'\n", __FUNCTION__, de->d_name, buf));
		close(fd);

		break;
	}
	closedir(dir);

	return i;
}

void sna_acpi_init(struct sna *sna)
{
	if (sna->acpi.fd < 0)
		return;

	if (sna->flags & SNA_PERFORMANCE)
		return;

	DBG(("%s: attaching to acpid\n", __FUNCTION__));

	SetNotifyFd(sna->acpi.fd, sna_acpi_notify, X_NOTIFY_READ, sna);
	sna->acpi.remain = sizeof(sna->acpi.event) - 1;
	sna->acpi.offset = 0;

	/* Read initial states */
	if (read_power_state("/sys/class/power_supply") == 0) {
		DBG(("%s: AC adapter is currently offline\n", __FUNCTION__));
		sna->flags |= SNA_POWERSAVE;
	}
}

void sna_acpi_fini(struct sna *sna)
{
	if (sna->acpi.fd < 0)
		return;

	close(sna->acpi.fd);
	sna->acpi.fd = -1;

	sna->flags &= ~SNA_POWERSAVE;
}
