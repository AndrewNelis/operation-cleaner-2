#include <stdlib.h>
#include "oc2.h"

unsigned int 		lMapnr;
extern unsigned int randseed;

// instead of a dll, we're just calling the external program
// to generate the map for us.
int mainbuild(unsigned int seed)
{
	char command[30] = {0};
	sprintf(command, "./oc2build %d", seed);
	int ret = system(command);
	if (ret != 0) {
		printf("oc2build exited with %d", ret);
		exit(EXIT_FAILURE);
	}

	return 0;
}
