#define _LARGEFILE64_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/errno.h>
#include "os.h"
#include "geode.h"

static int
_msr_open(void)
{
    static int msrfd = 0;

    if (msrfd == 0) {
	msrfd = open("/dev/cpu/0/msr", O_RDWR);
	if (msrfd == -1)
	    ErrorF("Unable to open /dev/cpu/0/msr: %d\n", errno);
    }

    return msrfd;
}

int
GeodeReadMSR(unsigned long addr, unsigned long *lo, unsigned long *hi)
{
    unsigned int data[2];
    int fd = _msr_open();
    int ret;

    if (fd == -1)
	return -1;

    ret = lseek64(fd, (off64_t) addr, SEEK_SET);

    if (ret == -1)
	return -1;

    ret = read(fd, (void *)data, sizeof(data));

    if (ret != 8)
	return -1;

    *hi = data[1];
    *lo = data[0];

    return 0;
}

int
GeodeWriteMSR(unsigned long addr, unsigned long lo, unsigned long hi)
{
    unsigned int data[2];
    int fd = _msr_open();

    if (fd == -1)
	return -1;

    if (lseek64(fd, (off64_t) addr, SEEK_SET) == -1)
	return -1;

    data[0] = lo;
    data[1] = hi;

    if (write(fd, (void *)data, 8) != 8)
	return -1;

    return 0;
}
