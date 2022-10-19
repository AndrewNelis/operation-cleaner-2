//#include "SDL.h"   /* All SDL App's need this */
#include <stdio.h>
//#include <stdlib.h>
//#include <time.h>
// #include <windows.h>
#include "OC2b.h"

unsigned int 		lMapnr;
extern unsigned int randseed;

// OPERATION CLEANER 2 BUILDING DLL
// JAN NYMAN

// VERSION 1.2
// 1.2.2007

// extern "C" __declspec(dllexport)
int mainbuild(unsigned int argc)
{
	lMapnr=argc;
/*	if(argc>1)
	{
		printf("argc %d: %s\n",argc,argv[1]);
		lMapnr=atoi(argv[1]);
		printf("Number: %u\n",lMapnr);
	}
*/
//	printf("argc %u %u\n",argc,lMapnr);

	LoadNeededFiles();
//	printf("LoadNeededFiles()\n");

	if(lMapnr>=0)
	{
		randseed=lMapnr;
		BeginRandomization();
//		printf("BeginRandomization()\n");
		GenerateMap();
//		printf("GenerateMap()\n");
		SaveMap("oc2tmp");
//		printf("SaveMap\n");
	}

//	exit(0);

return 0;
}
