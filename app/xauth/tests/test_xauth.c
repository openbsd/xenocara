#include <unistd.h>

int main(int argc, char *argv[])
{
    execlp("cmdtest", "cmdtest", ".", NULL);
}
