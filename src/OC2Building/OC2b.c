#include <stdio.h>
#include <stdlib.h>
#include "OC2b.h"

unsigned int lMapnr;
extern unsigned int randseed;

// OPERATION CLEANER 2 BUILDING DLL
// JAN NYMAN

// VERSION 1.2
// 1.2.2007

int mainbuild(unsigned int argc)
{
	lMapnr = argc;
	LoadNeededFiles();

	if (lMapnr >= 0)
	{
		randseed = lMapnr;
		BeginRandomization();
		GenerateMap();
		SaveMap("oc2tmp");
	}

	return 0;
}

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		char *end;
		int seed = strtol(argv[1], &end, 10);
		mainbuild(seed);
		return 0;
	}
	else
	{
		printf("Usage: %s <seed>\n", argv[0]);
		return 1;
	}
	return 0;
}
