#include <stdio.h>

short			GenerateMap();
int				LoadNeededFiles();
short			SaveMap(char *s);

short			MapBlock(short mapx, short mapy);
short			PutMap(short mapx,short mapy,short material);

void			AddExtras();
void			LoadMapCode(char * file);
short			ParseMapCode(bool bfirst);
void			DefineTargetArea();
void			BuildPipes();
long			TargetValue();
long			OtherValue();
void			GetCityData();
void			GetCustomers();
void			BeginRandomization();

/*void			Actions();
void			AddDampers();
void			AddDays(long lAddDays);
void			AddDust(short ix,short iy);
void			AddDustParticle(short px, short py, double pdir, short pvel);
void			AddExplosive();
void			AddExtras();
void			AddHelp(char addtxt[30],char hlpcolor);	// 0 white, 1 black, 2 red, 3 green
void			AddMessage(char addtxt[30],char msgcolor);	// 0 white, 1 black, 2 red, 3 green
void			BuildPipes();
unsigned int	CharToInt(char * cText);
bool			CheckCareerCustomers();
bool			CheckDampers();
int				CheckEvents();
short			CheckHiscores();
void			CloseFile();
short			CountAngle(short x1,short y1,short x2,short y2);
void			CountExpenses(char cCode);
short			CountExplosives();
void			CountMapSum();
short			CountNeeded(short icode);
bool			CountOverload();
void			CrashSound(int block);
void			DefineTargetArea();
void			DeleteCurrentSaveFile();
void			DoFlip();
void			DrawAll();
void			DrawBMPButton(char * text, int left_top_x,int left_top_y,int iType);
void			DrawButton(int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y);
void			DrawCircle(short x, short y, double radius, unsigned char r, unsigned char g, unsigned char b);
void			DrawCityMenu();
void			DrawCustomerMenu();
void			DrawDigits(char *text,int x,int y);
void			DrawFillRect(int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y, int r, int g, int b);
void			DrawFinancialMenu();
void			DrawLine(int left,int top,int right,int bottom,short r,short g,short b);
void			DrawMenus();
void			DrawNormalText(char *text,int x,int y, int color);
void			DrawRect(int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y, int r, int g, int b);
void			DrawTextButton(char * text,int left_top_x, int left_top_y, int right_bottom_x, int right_bottom_y);
void			DrawTextRect(char * text,short left,short top,short right,short bottom);
void			DrawTextWBack(char *text,int x,int y,int color,int r,int g,int b);
void			DrawTutorial();
void			EmptyHelp();
void			EmptyMessages();
int				FilterEvents(const SDL_Event *event);
signed short	FindFirstCustomer();
//bool			FindNextCustomer();
short			FindPlace(short istart, short iend);
//bool			FindPrevCustomer();
//void			FindTargetBuilding();
void			FinishInput(char finishtype);
void			GenerateBuildings();
void			GenerateCustomerData(short iCode);
void			GenerateGround();
void			GenerateMap();
void			GenerateRandSeed();
void			GenerateTrees();
void			GetBlockData(char iCode);
void			GetBuildings(char iCode);
void			GetCityData();
void			GetCustomers();
void			GetHiscores();
void			GetMapData();
short			GetRandValue();
void			HandleBlocks();
void			HandleExplosives();
int				InitSDL();
void			LastActionTime();
void			LoadBMP(char *file,unsigned char imagenr);
void			LoadButtonThemes();
bool			LoadGame();
void			LoadGameTexts();
void			LoadLanguages();
short			LoadMap(char *s);
void			LoadMaplist(short iCode);
void			LoadMapCode(char * file);
short			LoadMapTemp(char *s);
int				LoadNeededFiles();
unsigned long	LongRandom();
void			LoadSaveGameHeaders();
void			LoadSettings();
void			LoadTheme(char *file);
void			Mainstructure();
short			MapBlock(short mapx, short mapy);
void			MapCount();
void			MapNrMessage();
void			MoveMap();
void			MoveToNextTutorial(short iDone);
long			MovingCosts(bool bmoving);
void			OKBox(char *sHeader,short iYsize);
int				OpenFile(char *s,char *mode, short iErrors);
long			OtherValue();
short			ParseMapCode(bool bfirst);
void			PlayClick();
void			PlayWav(char *file);
short			PutMap(short mapx,short mapy,short material);
void			Putpixel(int x, int y, int r,int g,int b);
int				QuitSDL();
void			RandomCity();
void			RecordMouse();
void			SaveGame();
void			SaveHiscores();
short			SaveMap(char *s);
void			SaveSettings();
//void			SearchBuilding(char * cBType);
void			SeedGenerator(char *cText);
void			SetAudioFormat();
void			Setmax_y();
void			ShowBMP1(short src_x, short src_y, short src_w, short src_h,short dest_x,short dest_y);
void			ShowBMP2(short src_x, short src_y, short src_w, short src_h,short dest_x,short dest_y);
void			ShowBMP3(short src_x, short src_y, short src_w, short src_h,short dest_x,short dest_y);
void			ShowInitProgress();
void			ShowParseProgress(char * msg);
void			ShowQuitProgress();
short			SpecialOccasions();
long			TargetValue();
void			Update();
void			YesNoBox(char *sHeader,char *sText,char cCode);
void			ZeroDust();
void			ZeroExplosives();
void			ZeroMap(short iCode);
void			ZeroPlayer();

void			zpack(char *filein, char *fileout);
void			zunpack(char *filein, char *fileout);
*/
static const double		pi = 3.1415926535;
static const double		rad=pi/180;

static const short		BUILDINGS=100;
static const short		MAXBLOCKS=150;

  /*
static					SDL_Surface *screen;
static					SDL_Surface *image;
static					SDL_Surface *explosionimage;
static					SDL_Surface *buttonimage;
*/

static							FILE *stream;

static const char		MAXSAVEFILES=100;

static const int		MAXMAPFILES=10000;

static const int		MAPSIZE=250;

static const char		MAXEXPLOSIVETYPES=6;
//static char			exp_name[MAXEXPLOSIVETYPES][20]={"Dynamite 20 %","Dynamite 40 %","Dynamite 60 %","TNT","C-4","RDX"};
//static short			exp_velocity[MAXEXPLOSIVETYPES]={2500,3000,4200,5000,6500,7000};//{2500,3200,5500,6940,8370,8700};
static short			exp_velocity[MAXEXPLOSIVETYPES]={2500,2700,3000,4000,4500,5000};//{2500,3200,5500,6940,8370,8700};


//static short			exp_force[MAXEXPLOSIVETYPES]={200,280,360,500,800,1000};//{2500,3200,5500,6940,8370,8700};
static short			exp_force[MAXEXPLOSIVETYPES]={250,270,300,400,450,500};//{2500,3200,5500,6940,8370,8700};

static short			exp_cost[MAXEXPLOSIVETYPES]={5,8,12,20,30,50};	//{5,8,12,20,30,50};

static const short		MAXEXPLOSIONPARTICLES=30000;

static const char		MAXACCESSORIES=4;
//static char			acc_name[MAXEXPLOSIVETYPES][20]={"Blast caps","Detonators","Fuse wire","Dampers"};
static short			acc_cost[MAXEXPLOSIVETYPES]={5,50,2,60};

static const char		MAXCREW=4;
//static char			crw_name[MAXCREW][20]={"Workers","Bulldozers","Lorries","Guards"};
static short			crw_cost[MAXEXPLOSIVETYPES]={100,200,150,80};

static const char		MAXADVDECISIONS=4;
//static char			adv_decisions[MAXADVDECISIONS][25]={"Pay taxes","Accountant","Insurances","Sabotage competitors"};

const short				MAXCHARGES=50;

const short				MAXDUST=200;
const short				MAXDUSTPARTICLES=20000;

const short				MAXCAREERMAPS=200;
const short				MAXCITIES=20;
const short				MAXCUSTOMERS=200;

//const short				MAXGAMETXT=600;

const short				MAXMAPCODE=10000;

static short			rep_level[12]={-300,-200,-100,-10,0,10,50,100,300,500,700,1000};		// reputation levels

const short				MAXHISCORE=10;

const short				MAXTUTORIALS=10;

const short				MAXMESSAGES=100;
