/**
 ** Public domain
 **/
/* $OpenBSD: privsep.c,v 1.1 2006/11/28 20:29:31 matthieu Exp $ */
#include <sys/types.h>
#include <fcntl.h>

int 
priv_init(uid_t uid, gid_t gid)
{
	return 0;
}

int
priv_open_device(char *path)
{
	return open(path, O_RDWR);
}
