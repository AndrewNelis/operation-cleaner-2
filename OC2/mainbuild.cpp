#include "oc2.h"

unsigned int 		lMapnr;
extern unsigned int randseed;

int mainbuild(unsigned int argc)
{
	lMapnr=argc;
	LoadNeededFiles();
	if(lMapnr>=0)
	{
		randseed=lMapnr;
		BeginRandomization();
		GenerateMap(1);
		SaveMap("oc2tmp");
	}

return 0;
}
