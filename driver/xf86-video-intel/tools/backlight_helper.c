#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>

#define DBG 0

#if defined(__GNUC__) && (__GNUC__ > 3)
__attribute__((format(printf, 1, 2), noreturn))
#endif
static void die(const char *format, ...)
{
	if (DBG) {
		va_list args;

		va_start(args, format);
		vfprintf(stderr, format, args);
		va_end(args);
	}

	exit(1);
}

int main(int argc, char *argv[])
{
	struct stat st;
	char buf[1024];
	int len, fd;

	if (argc != 2)
		die("Usage: xf86-video-intel-backlight-helper <iface>\n");

	if (strchr(argv[1], '/') != NULL)
		die("Invalid interface '%s': contains '/'\n", argv[1]);

	if (snprintf(buf, sizeof(buf),
		     "/sys/class/backlight/%s/brightness",
		     argv[1]) >= sizeof(buf))
		die("Invalid interface '%s': name too long\n", argv[1]);

	fd = open(buf, O_RDWR);
	if (fd < 0 || fstat(fd, &st) || major(st.st_dev))
		die("Invalid interface '%s': unknown backlight file\n", argv[1]);

	while (fgets(buf, sizeof(buf), stdin)) {
		len = strlen(buf);
		if (write(fd, buf, len) != len)
			die("Failed to update backlight interface '%s': errno=%d\n", argv[1], errno);
	}

	return 0;
}
