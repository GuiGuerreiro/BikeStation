#include "ManageGraphics.h"
#include "ManageStations.h"
#include "ManageTrips.h"

/**
 * InitSDL: Initializes the SDL2 graphic library
 */
void InitSDL()
{
    // init SDL library
    if ( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        printf(" Failed to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
}
/**
*Creates renderer for all the rendering
*/

SDL_Renderer* CreateRenderer(int width, int height, SDL_Window *_window)
{
    SDL_Renderer *renderer;
    // init renderer
    renderer = SDL_CreateRenderer( _window, -1, 0 );
    if ( renderer == NULL )
    {
        printf("Failed to create renderer : %s", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // set size of renderer to the same as window
    SDL_RenderSetLogicalSize( renderer, width, height );

    return renderer;
}

/**
*Inits everything
*/
void InitEverything(int width, int height, TTF_Font **_font, SDL_Surface** _img, SDL_Window** _window, SDL_Renderer** _renderer)
{
    InitSDL();
    InitFont();
    *_window = CreateWindow(width, height);
    *_renderer = CreateRenderer(width, height, *_window);

    // load the table texture
    _img[0] = IMG_Load(".//graphics/mapa.png");
    if (_img[0] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
     // load IST logo
    _img[1] = IMG_Load(".//graphics/ist_logo.png");
    if (_img[1] == NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
     //Load station icon
    _img[2] = IMG_Load(".//graphics/pin.png");
    if(_img[2]==NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    _img[3] = IMG_Load(".//graphics/pinr.png");
    if(_img[3]==NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    _img[4]=IMG_Load(".//graphics/start.png");
    if(_img[4]==NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    _img[5]=IMG_Load(".//graphics/sel.png");
    if(_img[5]==NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    _img[6]=IMG_Load(".//graphics/check.png");
    if(_img[6]==NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    _img[7]=IMG_Load(".//graphics/stat.png");
    if(_img[7]==NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    _img[8]=IMG_Load(".//graphics/trip.png");
    if(_img[8]==NULL)
    {
        printf("Unable to load image: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    // this opens (loads) a font file and sets a size
    *_font = TTF_OpenFont(".//graphics/FreeSerif.ttf", 16);
    if(!*_font)
    {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
*Inits textfont
*/
void InitFont()
{
    // Init font library
    if(TTF_Init()==-1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
}

/**
*Creates app window
*/

SDL_Window* CreateWindow(int width, int height)
{
    SDL_Window *window;
    // init window
    window = SDL_CreateWindow( "BikeStation", WINDOW_POSX, WINDOW_POSY, width, height, 0 );
    // check for error !
    if ( window == NULL )
    {
        printf("Failed to create window : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
    return window;
}


/**
*Renders any text
*/
int RenderText(int x, int y, const char *text, TTF_Font *_font, SDL_Color *_color, SDL_Renderer* _renderer)
{
    SDL_Surface *text_surface;
    SDL_Texture *text_texture;
    SDL_Rect solidRect;

    solidRect.x = x;
    solidRect.y = y;

    // create a surface from the string text with a predefined font
    text_surface = TTF_RenderText_Blended(_font,text,*_color);
    if(!text_surface)
    {
        printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }
    // create texture

    text_texture = SDL_CreateTextureFromSurface(_renderer, text_surface);
    // obtain size

    SDL_QueryTexture( text_texture, NULL, NULL, &solidRect.w, &solidRect.h );
    // render it !

    SDL_RenderCopy(_renderer, text_texture, NULL, &solidRect);
    // clear memory

    SDL_DestroyTexture(text_texture);
    SDL_FreeSurface(text_surface);
    return solidRect.h;
}



void RenderMap(SDL_Surface *_img[], SDL_Renderer* _renderer, int *screen)
{
    SDL_Texture *map_texture;

    // clear the window
    SDL_RenderClear( _renderer );

    // draws the table texture
    map_texture = SDL_CreateTextureFromSurface(_renderer, _img[0]);
    SDL_RenderCopy(_renderer, map_texture, NULL, NULL);
    SDL_RenderPresent(_renderer);

    //render the IST Logo
    RenderLogo(1190, 450, _img[1], _renderer);
    // destroy everything
    SDL_DestroyTexture(map_texture);
    // store some variables for later use
    *screen=MAP;
}

void RenderStart(SDL_Surface *_img[], SDL_Renderer* _renderer, int *screen)
{
    SDL_Texture *start_text;

    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 0); //Limpa o ecra a branco
    SDL_RenderClear(_renderer);

    start_text = SDL_CreateTextureFromSurface(_renderer, _img[4]);
    SDL_RenderCopy(_renderer, start_text, NULL, NULL);

    SDL_RenderPresent(_renderer);

    SDL_DestroyTexture(start_text);
    *screen=STARTSCREEN;
}

void RenderSelScreen(SDL_Surface *_img[], SDL_Renderer* _renderer, int *screen)
{
    SDL_Texture *sel_texture;

    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 0); //Limpa o ecra a branco
    SDL_RenderClear(_renderer);
    sel_texture = SDL_CreateTextureFromSurface(_renderer, _img[5]);
    SDL_RenderCopy(_renderer, sel_texture, NULL, NULL);

    SDL_RenderPresent(_renderer);

    SDL_DestroyTexture(sel_texture);
    *screen=SELSCREEN;

}
int RenderLogo(int x, int y, SDL_Surface *_logoIST, SDL_Renderer* _renderer)
{
    SDL_Texture *text_IST;
    SDL_Rect boardPos;

    // space occupied by the logo
    boardPos.x = x;
    boardPos.y = y;
    boardPos.w = _logoIST->w;
    boardPos.h = _logoIST->h;

    // render it
    text_IST = SDL_CreateTextureFromSurface(_renderer, _logoIST);
    SDL_RenderCopy(_renderer, text_IST, NULL, &boardPos);

    // destroy associated texture !
    SDL_DestroyTexture(text_IST);
    return _logoIST->h;
}
void UndoStationSel(SDL_Surface *_imgs[], SDL_Renderer *_renderer)
{
    SDL_Texture *stat_text;
    SDL_Rect statPos;

    statPos.x=168;
    statPos.y = 112;
    statPos.w = _imgs[7]->w;
    statPos.h = _imgs[7]->h;

    stat_text = SDL_CreateTextureFromSurface(_renderer, _imgs[7]);
    SDL_RenderCopy(_renderer, stat_text, NULL, &statPos);
    SDL_RenderPresent(_renderer);
    SDL_DestroyTexture(stat_text);
}
void UndoDaySel(SDL_Surface *_imgs[], SDL_Renderer *_renderer)
{
    SDL_Texture *day_text;
    SDL_Rect dayPos;

    dayPos.x=751;
    dayPos.y = 116;
    dayPos.w = _imgs[8]->w;
    dayPos.h = _imgs[8]->h;

    day_text = SDL_CreateTextureFromSurface(_renderer, _imgs[8]);
    SDL_RenderCopy(_renderer, day_text, NULL, &dayPos);
    SDL_RenderPresent(_renderer);
    SDL_DestroyTexture(day_text);
}
void RenderCheck(int x, int y, SDL_Surface *_imgs[], SDL_Renderer *_renderer)
{
    SDL_Texture *check_text;
    SDL_Rect checkPos;

    checkPos.x=x;
    checkPos.y = y;
    checkPos.w = _imgs[6]->w;
    checkPos.h = _imgs[6]->h;

    check_text = SDL_CreateTextureFromSurface(_renderer, _imgs[6]);
    SDL_RenderCopy(_renderer, check_text, NULL, &checkPos);
    SDL_RenderPresent(_renderer);
    SDL_DestroyTexture(check_text);

}

SDL_Texture *CreatePinText(SDL_Surface *_pin, SDL_Renderer* _renderer)
{
    SDL_Texture *pin_text;


    //texture it
    pin_text = SDL_CreateTextureFromSurface(_renderer, _pin);

    return pin_text;
}



SDL_Rect RenderPin(int x, int y, SDL_Texture *pin_text, SDL_Renderer* _renderer)
{
    SDL_Rect pinPos;

    //space occupied by the pin
    pinPos.x=x;
    pinPos.y=y;
    pinPos.w= PINWIDTH;
    pinPos.h= PINHEIGHT;


    SDL_RenderCopy(_renderer, pin_text, NULL, &pinPos);
    SDL_RenderPresent(_renderer);
    return pinPos;
}

void DestroyPinText(SDL_Texture *pin_text)
{
    SDL_DestroyTexture(pin_text);
    return;
}

int LatToPy(double lat)
{
    int py=0;
    //cálculo interno da latitude. Dado ser um mapa linear (baixa escala)
    py= (lat-42.356961)*(439-196)/(42.332970-42.356961)+196;
    return py;
}

int LgtToPx(double lgt)
{
    int px=0;
    //Cálculo interno da longitude
    px=(lgt+71.092508)*(886-554)/(-71.049107+71.092508)+554;
    return px;
}

StationReg *CheckMousePos(int x, int y, StationHeader *stationhead)
{
    StationReg *node=NULL;
    int py=0, px=0;

    node=stationhead->topadd;

    while(node!=NULL)
    {
        py=LatToPy(node->lat);
        px=LgtToPx(node->longt);

        if(x<=px+PINWIDTH && x>=px && y>=py && y<=py+PINWIDTH)
        {
            return node;
        }
      node=node->nextnode;
    }
    return NULL;
}

void DrawRoute(SDL_Renderer* _renderer, RouteHeader* routehead)
{
    RouteReg *routenode=NULL, *temp=NULL;
    int r=0, g=0, b=0;
    int pxi=0, pyi=0, pxf=0, pyf=0;
    int totaltrips=0;
    float linethick=0.0;
    int linethickpy=0;
    int foundroute=0;
    int samestation=0;

    routenode=routehead->topadd;
    while(routenode!=NULL)
    {
        samestation=0;
        if(routenode->startstation==routenode->endstation)
        {
            samestation=1;
        }

        temp=routehead->topadd;

        foundroute=0;
        while(temp!=NULL && foundroute==0 && samestation==0) //Vai procurar a rota par. Ou seja, a rota contrária
        {
           if(temp->startstation==routenode->endstation && temp->endstation==routenode->startstation && routenode->startstation!=routenode->endstation) //Condição para a rota contrária
            {
                r=255; g=0; b=0; //Cor vermelha
                SDL_SetRenderDrawColor(_renderer, r, g, b, 0);
                foundroute=1;
                pxi=LgtToPx(temp->startstation->longt)+9;   // Acerta o inicio da linha com o centro da bola vermelha
                pyi=LatToPy(temp->startstation->lat)+8;     //
                pxf=LgtToPx(temp->endstation->longt);
                pyf=LatToPy(temp->endstation->lat)+PINHEIGHT; //Acerta o fim da linha com o fim do pin
                totaltrips=temp->tripcount;
                linethick= (2*totaltrips+55)/30;
                while(linethick>=1)
                {
                    linethickpy=linethick+0.5;
                    SDL_RenderDrawLine(_renderer, pxi, pyi-linethickpy, pxf, pyf-linethickpy); //Desenha linha multiplas consecutivas
                    linethick--;                                                               //em qtd proporcional ao nr viagens
                }
                DelRouteNode(routehead, temp);
            }
            else
            {
                temp=temp->nextnode;
            }

        }

        if(samestation==1)
        {
            samestation=0;
             r=255; g=255; b=0; //Cor amarela
            SDL_SetRenderDrawColor(_renderer, r, g, b, 0);
            pxi=LgtToPx(routenode->startstation->longt)+9;
            pyi=LatToPy(routenode->startstation->lat)+8;
            pxf=LgtToPx(routenode->endstation->longt);
            pyf=LatToPy(routenode->endstation->lat)+PINHEIGHT;
            totaltrips=routenode->tripcount;

            linethick= (2*totaltrips+55)/30;

        }
        else /*Desenha reta entre as duas estações*/
        {
            r=0; g=0; b=255; //Cor azul
            SDL_SetRenderDrawColor(_renderer, r, g, b, 0);
            pxi=LgtToPx(routenode->startstation->longt)+9;
            pyi=LatToPy(routenode->startstation->lat)+8;
            pxf=LgtToPx(routenode->endstation->longt);
            pyf=LatToPy(routenode->endstation->lat)+PINHEIGHT;
            totaltrips=routenode->tripcount;
            linethick= (2*totaltrips+55)/30;

            while(linethick>=1)
            {
                linethickpy=linethick+0.5;
                SDL_RenderDrawLine(_renderer, pxi, pyi-linethickpy, pxf, pyf-linethickpy);
                linethick--;
            }
            SDL_RenderPresent(_renderer);
        }
//printf("routenode atual %d->%d\n", routenode->startstation->stationID, routenode->endstation->stationID);
        routenode=routenode->nextnode;
    }
}

void RenderStationOnMap(int userchoice, StationHeader *stationhead, SDL_Surface *imgs[], SDL_Renderer *renderer)
{
    StationReg *stnode=NULL, *sttemp=NULL;
    SDL_Texture *pin, *pinr;

    pin=CreatePinText(imgs[2], renderer);
    pinr=CreatePinText(imgs[3], renderer);
    switch(userchoice)
    {
         case ALLSTATIONS:
         stnode=stationhead->topadd;
         while(stnode!=NULL)
         {
             if(stnode->state==REMOVIDA)
             {
                 RenderPin(LgtToPx(stnode->longt), LatToPy(stnode->lat), pinr, renderer);
                 stnode=stnode->nextnode;
             }
             else
             {
                 RenderPin(LgtToPx(stnode->longt), LatToPy(stnode->lat), pin, renderer);
                 stnode=stnode->nextnode;
             }
    }
    break;
    case ACTIVESTATIONS:
    stnode=stationhead->topadd;
    while(stnode!=NULL)
    {
        sttemp=stnode->nextnode;
        if(stnode->state==REMOVIDA)
        {
            DelStationNode(stationhead, stnode);
            stnode=sttemp;
        }
        else
        {
            RenderPin(LgtToPx(stnode->longt), LatToPy(stnode->lat), pin, renderer); //imgs[3] para pin estação removida
            stnode=stnode->nextnode;
        }
    }
    break;
    case REMOVEDSTATIONS:
    stnode=stationhead->topadd;
    while(stnode!=NULL)
    {
        sttemp=stnode->nextnode;
        if(stnode->state==REMOVIDA)
        {
            RenderPin(LgtToPx(stnode->longt), LatToPy(stnode->lat), pinr, renderer);
            stnode=stnode->nextnode;
        }
        else
        {
            DelStationNode(stationhead, stnode);
            stnode=sttemp;
        }
    }
    break;
    case PICKSTATION:
    stnode=stationhead->topadd;
    while(stnode!=NULL)
    {
        sttemp=stnode->nextnode;
        if(stnode->state==REMOVIDA)
        {
            RenderPin(LgtToPx(stnode->longt), LatToPy(stnode->lat), pinr, renderer);
            stnode=stnode->nextnode;
        }
        else
        {
            RenderPin(LgtToPx(stnode->longt), LatToPy(stnode->lat), pin, renderer);
            stnode=stnode->nextnode;
        }
    }


    break;
    }
}
