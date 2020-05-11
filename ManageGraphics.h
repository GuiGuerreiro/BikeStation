#ifndef MANAGE_GRAPHICS
#define MANAGE_GRAPHICS
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include "ManageStations.h"
#include "ManageTrips.h"

#define WINDOW_POSX 200       // initial position of the window: x
#define WINDOW_POSY 200       // initial position of the window: y
#define PINWIDTH 17
#define PINHEIGHT 32
#define MOUSEOFFSETX 8
#define MOUSEOFFSETY 24
#define STARTSCREEN 0
#define SELSCREEN 1
#define MAP 2
#define ALLSTATIONS 0
#define ACTIVESTATIONS 1
#define REMOVEDSTATIONS 2
#define PICKSTATION 3

void InitSDL();
SDL_Renderer* CreateRenderer(int , int , SDL_Window *);
void InitEverything(int , int , TTF_Font **, SDL_Surface**, SDL_Window ** , SDL_Renderer ** );
void InitFont();
SDL_Window* CreateWindow(int , int );




void RenderMap(SDL_Surface *_img[], SDL_Renderer* _renderer, int*);
void RenderStart(SDL_Surface *_img[], SDL_Renderer* _renderer, int*);
void RenderSelScreen(SDL_Surface *_img[], SDL_Renderer* _renderer, int*);
void RenderStationOnMap(int userchoice, StationHeader *stationhead,  SDL_Surface *imgs[], SDL_Renderer *renderer);
void RenderCheck(int x, int y, SDL_Surface *_imgs[], SDL_Renderer *_renderer);

void UndoStationSel(SDL_Surface *_imgs[], SDL_Renderer *_renderer);

void UndoDaySel(SDL_Surface *_imgs[], SDL_Renderer *_renderer);



int RenderLogo(int, int, SDL_Surface *, SDL_Renderer *);


SDL_Texture *CreatePinText(SDL_Surface *, SDL_Renderer*);

SDL_Rect RenderPin(int , int , SDL_Texture *, SDL_Renderer*);

void DestroyPinText(SDL_Texture *);


int LatToPy(double);

int LgtToPx(double);


int RenderText(int, int, const char *, TTF_Font *, SDL_Color *, SDL_Renderer *);

StationReg *CheckMousePos(int, int, StationHeader*);

void DrawRoute(SDL_Renderer* _renderer,RouteHeader* routehead);







#endif
