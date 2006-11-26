
#include "Manager.h"

#include "Client.h"
#include "Border.h"

int main(int argc, char **argv)
{
    int i;
    
    if (argc > 1) {
	for (i = strlen(argv[0])-1; i > 0 && argv[0][i] != '/'; --i);
	fprintf(stderr, "usage: %s\n", argv[0] + (i > 0) + i);
	exit(2);
    }
    
    WindowManager manager;
    return 0;
}

