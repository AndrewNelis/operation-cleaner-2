#include "SDL/SDL.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "oc2.h"

// OPERATION CLEANER 2
// JAN NYMAN

// MUST BE COMPILED & BUILT IN VC++6 IN DEBUG MODE, OTHERWISE RANDOMIZE DOESN'T WORK PROPERLY

void FreeImages()
{
	SDL_FreeSurface(image);
	SDL_FreeSurface(explosionimage);
	SDL_FreeSurface(buttonimage);
}

int main(int argc, char *argv[])
{
	LoadSettings();
	InitSDL();
	LoadNeededFiles();
	SetAudioFormat();
	ShowInitProgress();

	Mainstructure();

	FreeImages();
	QuitSDL();
	exit(0);

return 0;
}
