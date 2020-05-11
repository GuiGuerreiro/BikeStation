#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ManageTrips.h"
#include "ManageStations.h"
#include "ManageGraphics.h"
#include "menu.h"


int main(int argc, char *argv[])
{
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	TTF_Font *serif = NULL;
	SDL_Event event;
	SDL_Surface *imgs[9];
	StationHeader *stationhead=NULL, *sortedsthead=NULL;
	StationReg *selstation=NULL;
	TripHeader *triphead=NULL, *sortedtriphead=NULL;
	RouteHeader *routehead=NULL, *duproutehead=NULL;
	RouteReg *routenode=NULL;
	int currentscreen=0;  //0->Startscreen, 1->SelScreen, 2->MAP
	int userstation=0; //0->Todas as estações, 1->Estações Ativas, 2->estações removidas, 3->Escolher estação no mapa
	int routesdrawn=0;
	char concat[100];
	int inbikes=0, outbikes=0;


	int quit=0;
	int mousex=0, mousey=0;
	int width=1317, height=582;

	int  weekday=0, duration=0;
    char btime[STR_MAX]="";
    char etime[STR_MAX]="";

	if(argc!=4)
	{
		printf("Wrong number of starting arguments\n");
		exit(EXIT_FAILURE);
	}

	else if(strcmp(argv[1],"-t")!=0 && strcmp(argv[1], "-g")!=0)
	{
		printf("Invalid first argument\nMust be either '-g' for graphic mode or '-t' for text mode\n");
		exit(EXIT_FAILURE);
	}
	if(strcmp(argv[1],"-t")==0) //modo texto
	{
		stationhead=CreateStationHeader();
		triphead=CreateTripHeader();
		CreateTripList(argv[2], triphead);
    	CreateStationList(argv[3], stationhead);
    	routehead=CreateRouteHeader();
        CreateRouteList(routehead, triphead, stationhead);

		showMainMenu(argv[2], btime, etime, &weekday, &duration, triphead, stationhead, routehead);

		DelStationList(stationhead);
		//DelTripList(triphead);
		free(stationhead);
		free(triphead);
	}


	if(strcmp(argv[1],"-g")==0)		//Modo gráfico
	{
		//Initialize images, fonts, window etc
		InitEverything(width, height, &serif, imgs, &window, &renderer);
        RenderStart(imgs, renderer, &currentscreen);
        stationhead=CreateStationHeader();
        triphead=CreateTripHeader();
        routehead=CreateRouteHeader();
        duproutehead=CreateRouteHeader();
        sortedsthead=CreateStationHeader();
        sortedtriphead=CreateTripHeader();
        weekday=-1;
        userstation=-1;

		CreateStationList(argv[3], stationhead);
		CreateTripList(argv[2], triphead);

		while( quit == 0 )
  		{
        	// while there's events to handle
        	while( SDL_PollEvent( &event ) )
        	{
        	    if( event.type == SDL_QUIT )
        	    {
        	        quit=1;
	       		}
	        	else if ( event.type == SDL_MOUSEBUTTONDOWN )
	        	{
	        		switch (event.button.button){
	        		case SDL_BUTTON_LEFT:
	        			SDL_GetMouseState(&mousex, &mousey);

	        			switch(currentscreen)
	        			{
	        			case STARTSCREEN: //ecrã de inicio
	        				if(mousex>=592 && mousex<=758 && mousey>=487 && mousey<=542)
	        					RenderSelScreen(imgs, renderer, &currentscreen);
	        			break;

	        			case SELSCREEN:
	        				if(mousex>=191 && mousey>=174 && mousex<=391 && mousey<=198)
	        				{
	        					UndoStationSel(imgs, renderer);
	        					userstation=ALLSTATIONS;
	        					RenderCheck(191, 174, imgs, renderer);
	        				}
	        				else if(mousex>=191 && mousey>=216 && mousex<=368 && mousey<=242)
	        				{
	        					UndoStationSel(imgs, renderer);
	        					userstation=ACTIVESTATIONS;
	        					RenderCheck(191, 216, imgs, renderer);
	        				}
	        				else if(mousex>=191 && mousey>=257 && mousex<=411 && mousey<=282)
	        				{
	        					UndoStationSel(imgs, renderer);
	        					userstation=REMOVEDSTATIONS;
	        					RenderCheck(191, 257, imgs, renderer);
	        				}
	        				else if(mousex>=191 && mousey>=291 && mousex<=481 && mousey<=315)
	        				{
	        					UndoStationSel(imgs, renderer);
	        					userstation=PICKSTATION;
	        					RenderCheck(191, 291, imgs, renderer);
	        				}
	        				if(mousex>=761 && mousex<=923)
	        				{
	        					if(mousey>=169 && mousey<=169+24)
	        					{
	        						UndoDaySel(imgs, renderer);
	        						weekday=1;
	        						RenderCheck(896, 169, imgs, renderer);
	        					}
	        					else if(mousey>=200 && mousey<=224)
	        					{
	        						UndoDaySel(imgs, renderer);
	        						weekday=2;
	        						RenderCheck(896, 200, imgs, renderer);
	        					}
	        					else if(mousey>=236 && mousey<=236+24)
	        					{
	        						UndoDaySel(imgs, renderer);
	        						weekday=3;
	        						RenderCheck(896, 236, imgs, renderer);
	        					}
	        					else if(mousey>=271 && mousey<=271+24)
	        					{
	        						UndoDaySel(imgs, renderer);
	        						weekday=4;
	        						RenderCheck(899, 271, imgs, renderer);
	        					}
	        					else if(mousey>=306 && mousey<=330)
	        					{
	        						UndoDaySel(imgs, renderer);
	        						weekday=5;
	        						RenderCheck(899, 306, imgs, renderer);
	        					}
	        					else if(mousey>=340 && mousey<=364)
	        					{
	        						UndoDaySel(imgs, renderer);
	        						weekday=6;
	        						RenderCheck(899, 340, imgs, renderer);
	        					}
	        					else if(mousey>=370 && mousey<=370+24)
	        					{
	        						UndoDaySel(imgs, renderer);
	        						weekday=7;
	        						RenderCheck(899, 370, imgs, renderer);
	        					}
	        				}
	        				if(mousey>=481 && mousey<=582)
	        				{
	        					if(mousex>=0 && mousex<=109) //Volta para o ecra inicial
	        					{
	        						RenderStart(imgs, renderer, &currentscreen);
	        						weekday=-1;
	        						userstation=-1;
	        					}
	        					else if(mousex>=1209 && mousex<=1317)	//Avança no ecra
	        					{
	        						if(weekday>=0 && weekday<=7 && userstation>=0 && userstation<=3)
	        						{
	        							DelStationList(sortedsthead); //Apaga a lista das estações selecionadas. Liberta a cabeça para uso
	        							DupStationList(sortedsthead, stationhead); //Copia a lista original para a sortedsthead
	        							DupTripList(sortedtriphead, triphead);		//Copia a lista original para a sortedtriphead
	        							RenderMap(imgs, renderer, &currentscreen); //Faz render do mapa
	        							RenderStationOnMap(userstation, sortedsthead, imgs, renderer); //Faz render, no mapa, das estações e apaga da lista as estações que não correspondem.
	        							btime[0]=0;
										etime[0]=0;
										if(userstation!=3) //Se não for a opção de escolher a estação
										{

											DelRouteList(routehead);	//Apaga a lista das rotas a partir das duas listas acima. Liberta a cabeça para uso futuro
											DelRouteList(duproutehead);
											CreateRouteList(routehead, SelectTripList(sortedtriphead, btime, etime, weekday, 0), sortedsthead);//Cria a lista de rotas, a partir do ficheiro da estações e viagens
	        								DupRouteList(duproutehead, routehead); //Guarda a lista das rotas intacta, que a draw route estraçalha toda
		       								DrawRoute(renderer, routehead); //Desenha todas as rotas encontradas
	  	     								DelTripList(sortedtriphead); //Apaga a lista das viagens ordenadas por dia da semana. Liberta a cabeça para uso futuro
	        								weekday=-1;	//Reseta as flags
	        								userstation=-1;
	        								routesdrawn=1;
										}
	        						}
	        						else //Não foram escolhidos todos os parametros
	        							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info", "Por favor selecione mais parâmetros", window);
	        					}
	        				}
	        			break;

	        			case MAP: //Qd entra aqui já está o mapa desenhado

	        			if(mousex>=0 && mousey>=0 && mousex<=65 && mousey<=56) //Volta para o ecrã anterior
	        			{
	        				RenderSelScreen(imgs, renderer, &currentscreen);
	        				DelRouteList(duproutehead);
	        				weekday=-1;
	        				userstation=-1;
	        				routesdrawn=0;
	        			}
	        			else if(userstation==3 && routesdrawn==0)
						{
							selstation=CheckMousePos(mousex, mousey, stationhead);
							if(selstation!=NULL)
							{
								DelRouteList(routehead);
								DelRouteList(duproutehead);
								SortTripByStation(selstation, sortedtriphead); //Apaga da lista auxiliar de viagens os nós com viagens que não incluam a estação selecionada
	        					CreateRouteList(routehead, SelectTripList(sortedtriphead, btime, etime, weekday, 0), sortedsthead); //Cria a lista das rotas a partir das viagens selecionadas
	       						DupRouteList(duproutehead, routehead);
	       						DrawRoute(renderer, routehead); //Desenhas as linhas das rotas
	        					if(routehead->topadd==NULL) //Não foi adicionado nenhum nó à lista das rotas, logo não há rotas para a estação selecionada
	        					{
	        						routesdrawn=0;
	        						SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Info", "Não existem rotas para esta estação", window);
	        						DupTripList(sortedtriphead, triphead);
        						}
	        					else
	        					{
	        						routesdrawn=1;
	        						weekday=-1;	//Reseta as flags
	       							userstation=-1;
	        					}
	        				}
						}
						if(routesdrawn==1)
						{
							selstation=NULL;
							selstation=CheckMousePos(mousex, mousey, stationhead);
							if(selstation!=NULL)
							{
								outbikes=0;
								inbikes=0;
								routenode=duproutehead->topadd;

								while(routenode!=NULL)
								{
									if(selstation->stationID==routenode->startstation->stationID)
										outbikes=outbikes+routenode->tripcount;
									else if(selstation->stationID==routenode->endstation->stationID)
										inbikes=inbikes+routenode->tripcount;
									routenode=routenode->nextnode;
								}
							sprintf(concat, "%s //BikeFlow in:%d out:%d",selstation->stationname, inbikes, outbikes);
							SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Dados da Estação", concat, window);
							}
						}

	        			}
	        		}
				}
			}
		}

		DelStationList(stationhead);
		free(stationhead);
		DelStationList(sortedsthead);
		free(sortedsthead);
		DelTripList(triphead);
		free(triphead);
		DelTripList(sortedtriphead);
		free(sortedtriphead);
		DelRouteList(routehead);
		free(routehead);
		DelRouteList(duproutehead);
		free(duproutehead);
		TTF_CloseFont(serif);
		SDL_FreeSurface(imgs[0]);
    	SDL_FreeSurface(imgs[1]);
    	SDL_FreeSurface(imgs[2]);
    	SDL_FreeSurface(imgs[3]);
    	SDL_FreeSurface(imgs[4]);
    	SDL_FreeSurface(imgs[5]);
    	SDL_FreeSurface(imgs[6]);
    	SDL_FreeSurface(imgs[7]);
    	SDL_FreeSurface(imgs[8]);
    	//SDL_FreeSurface(imgs[9]);
    	SDL_DestroyRenderer(renderer);
    	SDL_DestroyWindow(window);
    	SDL_Quit();
    	return EXIT_SUCCESS;
	}
}



