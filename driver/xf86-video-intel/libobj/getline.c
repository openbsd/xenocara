#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

extern int getline(char **line, size_t *len, FILE *file);

int getline(char **line, size_t *len, FILE *file)
{
	char *ptr, *end;
	int c;

	if (*line == NULL) {
		errno = EINVAL;
		if (*len == 0)
			*line = malloc(4096);
		if (*line == NULL)
			return -1;

		*len = 4096;
	}

	ptr = *line;
	end = *line + *len;

	while ((c = fgetc(file)) != EOF) {
		if (ptr + 1 >= end) {
			char *newline;
			int offset;

			newline = realloc(*line, *len + 4096);
			if (newline == NULL)
				return -1;

			offset = ptr - *line;

			*line = newline;
			*len += 4096;

			ptr = *line + offset;
			end = *line + *len;
		}

		*ptr++ = c;
		if (c == '\n') {
			*ptr = '\0';
			return ptr - *line;
		}
	}
	*ptr = '\0';
	return -1;
}
