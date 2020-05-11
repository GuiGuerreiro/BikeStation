#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ManageTrips.h"
#include "ManageStations.h"
#include "menu.h"


TripReg *CreateTripNode(			//Cria nó para cada viagem
	long tripID,						//ID da viagem
	long tripdursec,					//Duração da viagem em segundos
	char *startdatetime, 			//Data e hora de inicio da viagem
	int begstationID,				//ID da estação incial
	char *enddatetime, 				//Data e hora do fim da viagem
	int endstationID, 				//ID da estação final
	char *bikeID, 					//ID da bicicleta
	int	usertype,					//Tipo de usuário (0=Casual, 1=Registado)
	int userbirthyear, 				//Ano de nascimento do user (0 se usuario casual)
	int usergender 					//Género do user (0=masculino, 1= feminino)
	)
{
	TripReg* tripnode = (TripReg*)malloc(sizeof(TripReg));
    if(tripnode == NULL)
    {
        printf("Error allocating memory\n");
        exit(0);
    }
    tripnode->tripID = tripID;
    tripnode->tripdursec = tripdursec;
    tripnode->startdatetime=mystrdup(startdatetime);
    tripnode->begstationID = begstationID;
    tripnode->enddatetime=mystrdup(enddatetime);
    tripnode->endstationID = endstationID;
    tripnode->bikeID=mystrdup(bikeID);
    tripnode->usertype = usertype;
    tripnode->userbirthyear = userbirthyear;
    tripnode->usergender = usergender;
    tripnode->prevnode = NULL;
    tripnode->nextnode = NULL;
    return tripnode;
}

TripHeader *CreateTripHeader()
{
	TripHeader *tripheader= (TripHeader*)malloc(sizeof(TripHeader));

	tripheader->topadd=NULL;
	tripheader->botadd=NULL;
	tripheader->totalnodes=0;
	return tripheader;
}


TripReg *AddTripNode (
	int addtype,			//Tipo de adição à lista (0=Topo da lista, 1=Fim da lista, 2=Meio da lista)
	TripHeader *head,		//Cabeça da lista
	TripReg *node, 			//Nó a adicionar à lista
	TripReg *refnode			//Nó prévio à posição em que se vai inserir o novo nó. =NULL se for top ou bot
	)
{
	if(head==NULL)
		{
			printf("Trying to create TripList with no head");
			exit(EXIT_FAILURE);
		}
	if(head->totalnodes==0)
	{							//Se for o primeiro elemento da lista head->botadd == head->topadd
		head->topadd = node;
		head->botadd = head->topadd;
		node->prevnode = NULL;
		node->nextnode = NULL;
	}
	else
	{
		switch(addtype)
		{
			case ADDTOP:
				node->nextnode = head->topadd;
				head->topadd = node;
				node->prevnode = NULL;
				break;

				case ADDBOT:
					node->prevnode = head->botadd;
					head->botadd->nextnode = node;
					head->botadd = node;
					node->nextnode = NULL;
					break;

				case ADDMIDAFT:
					if(refnode==NULL)
					{
						printf("Error: Reference node address is NULL\n");
						return(NULL);
					}
					if (refnode->nextnode == NULL)	//BOTADD style
					{
						node->prevnode = head->botadd;
						head->botadd->nextnode = node;
						head->botadd = node;
						node->nextnode = NULL;
					}
					else
					{
						node->prevnode = refnode;
						refnode->nextnode->prevnode=node;
						node->nextnode = refnode->nextnode;
						refnode->nextnode = node;
					}
					break;

				case ADDMIDBFR:
					if(refnode==NULL)
					{
						printf("Error: Reference node address is NULL\n");
						return(NULL);
					}
					if (refnode->prevnode == NULL)	//TOPADD style
					{
						node->nextnode = head->topadd;
						head->topadd = node;
						node->prevnode = NULL;
					}
					else
					{
						node->prevnode = refnode->prevnode;
						refnode->prevnode=node;
						node->nextnode = refnode;
						node->prevnode->nextnode = node;
					}
					break;
		}
	}
	head->totalnodes++;
    return node;
}



void DelTripNode(
	TripHeader *header,
	TripReg *node
	)
{
	if(node==NULL)
		printf("Invalid node address\n");
	else
	{
		if(header->totalnodes == 1)
		{
			header->botadd=NULL;
			header->topadd=NULL;
			free(node->startdatetime);
			free(node->enddatetime);
			free(node->bikeID);
			free(node);
			header->totalnodes--;
		}
		else
		{
			if(node->nextnode == NULL) 			//REMOVING TAIL
			{
				header->botadd=node->prevnode;
				node->prevnode->nextnode=NULL;
				free(node->startdatetime);
				free(node->enddatetime);
				free(node->bikeID);
				free(node);
				header->totalnodes--;
			}
			else if(node->prevnode == NULL)		//REMOVING FIRST ELEMENT
			{
				header->topadd=node->nextnode;
				node->nextnode->prevnode=NULL;
				free(node->startdatetime);
				free(node->enddatetime);
				free(node->bikeID);
				free(node);
				header->totalnodes--;
			}
			else
			{
				node->prevnode->nextnode=node->nextnode;
				node->nextnode->prevnode=node->prevnode;
				free(node->startdatetime);
				free(node->enddatetime);
				free(node->bikeID);
				free(node);
				header->totalnodes--;
			}
		}
	}
}

int DelTripList(
	TripHeader *header
	)
{
	TripReg *temp = NULL;
	TripReg *temp1 = NULL;
	temp = header->topadd;

	if(header->topadd==NULL)
		return 1;
	else
	{
		while(temp->nextnode!=NULL)
		{
			temp1 = temp->nextnode;
			free(temp->startdatetime);
			free(temp->enddatetime);
			free(temp->bikeID);
			free(temp);
			temp = temp1;
		}
		free(temp->startdatetime);
		free(temp->enddatetime);
		free(temp->bikeID);
		free(temp);
		header->topadd=NULL;
		header->botadd=NULL;
		header->totalnodes=0;
	}
	return 0;
}

void CreateTripList(
	char* file,
	TripHeader* header
	)
{
	FILE *fp = NULL;
	int begstationID=0, endstationID=0, usertype=0, userbirthyear=0, usergender=0;
	long tripdursec=0, tripID=0;
	TripReg *node=NULL;
	char *str=NULL, *strpt=NULL, *startdatetime=NULL, *enddatetime=NULL, *bikeID=NULL;
	char line[500], temp[10], temp1[10];
	fp=fopen(file, "r");
	if(fp==NULL)
	{
		printf("Invalid trip file, exiting");
		exit(EXIT_FAILURE);
	}
	while(fgets(line, 500, fp))
	{
		strpt=str=mystrdup(line);
		tripID=atoi(mystrsep(&str,","));
		tripdursec=atoi(mystrsep(&str,","));
		startdatetime=mystrdup(mystrsep(&str,","));
		begstationID=atoi(mystrsep(&str,","));
		enddatetime=mystrdup(mystrsep(&str,","));
		endstationID=atoi(mystrsep(&str,","));
		bikeID=mystrdup(mystrsep(&str,","));
		strcpy(temp, mystrsep(&str,","));
		strcpy(temp1, "Casual");
		if(strcmp(temp, temp1)==0)
			usertype=CASUAL;
		else
			usertype=REGISTERED;
		userbirthyear=atoi(mystrsep(&str,","));
		if(usertype==0)
			usergender=UNDIFINED;
		else
		{
			strcpy(temp, mystrsep(&str,","));
			strcpy(temp1, "Male");
			 if(strcmp(temp, temp1)>0)
				usergender=MALE;

			else
				usergender=FEMALE;
		}
		node=CreateTripNode(tripID, tripdursec, startdatetime, begstationID, enddatetime, endstationID, bikeID, usertype, userbirthyear, usergender);
		AddTripNode(ADDBOT, header, node, NULL);
		free(startdatetime);
		free(enddatetime);
		free(bikeID);
		free(strpt);
	}
	fclose(fp);
}


//-----------------------------------------------------------------------------
//                          ROTAS
//------------------------------------------------------------------------------
RouteHeader *CreateRouteHeader() //DONE
{
    RouteHeader *head=(RouteHeader*)malloc(sizeof(RouteHeader));
    head->topadd=NULL;
    return head;
}

RouteReg *CreateRouteNode(		//DONE
    StationReg *station1,		//Estação de partida
    StationReg *station2		//estação de chegada
    )
{
	RouteReg *node=(RouteReg*) malloc(sizeof(RouteReg));
	node->startstation=station1;
	node->endstation=station2;
	node->tripcount=1;
	node->nextnode=NULL;
   return node;
}

void DupTripList(TripHeader *newlist, TripHeader *baselist)
{
    TripReg *ttemp=NULL;
    ttemp=baselist->topadd;
        while(ttemp!=NULL)
        {
            AddTripNode(ADDBOT, newlist,
            CreateTripNode(ttemp->tripID,
    ttemp->tripdursec,
    ttemp->startdatetime,
    ttemp->begstationID,
    ttemp->enddatetime,
    ttemp->endstationID,
    ttemp->bikeID,
    ttemp->usertype,
    ttemp->userbirthyear,
    ttemp->usergender), NULL);
            ttemp=ttemp->nextnode;
        }
}

void AddRouteNode(RouteHeader *head, RouteReg *node)	//DONE
{
    if(head->topadd==NULL)      //Primeiro elemento da lista
    {
        head->topadd=node;
        node->nextnode=NULL;
    }
    else        //Todos os outros. Adiciona ao topo
    {
        node->nextnode=head->topadd;
        head->topadd=node;
    }
}

void DelRouteList(RouteHeader *head)
{
    RouteReg *temp=NULL, *temp1=NULL;

    temp=head->topadd;

    while(temp!=NULL)
    {
        temp1=temp->nextnode;
        free(temp);
        temp=temp1;
    }
    head->topadd=NULL;
}

void DupRouteList(RouteHeader *dest, RouteHeader *source)
{
	RouteReg *node=NULL, *aux=NULL;

	aux=source->topadd;
	while(aux!=NULL)
	{
		node=(RouteReg*)malloc(sizeof(RouteReg));
		node->startstation=aux->startstation;
		node->endstation=aux->endstation;
		node->tripcount=aux->tripcount;
		AddRouteNode(dest, node);
		aux=aux->nextnode;
	}

}

void DelRouteNode(RouteHeader *head, RouteReg *node)
{
	RouteReg *aux=NULL;
	if(head->topadd==node) //apaga o primeiro elemento
	{
		head->topadd=node->nextnode;
	}
	else
	{
		aux=head->topadd;
		while(aux->nextnode!=node)
		{
			aux=aux->nextnode;
		}
		aux->nextnode=node->nextnode;
	}
	free(node);
}

void CreateRouteList(RouteHeader *routehead, TripHeader *triphead, StationHeader *stationhead)		//Função principal. Cria a lista das rotas a partir da lista de viagens selecionada
{
	TripReg *tripnode=triphead->topadd;
	StationReg *stationnode=NULL, *begstation=NULL, *endstation=NULL;
	RouteReg *routenode=NULL;
	int foundroute=0;

	while(tripnode!=NULL)
	{
		if(routehead->topadd==NULL)	//Este if é para o primeiro elemento da lista das rotas. Vai encontrar os pares estação de inicio->estação e coloca na lista
		{
			stationnode=stationhead->topadd;
			while(stationnode!=NULL) //Percorre os elementos da lista estações
			{
				if(stationnode->stationID==tripnode->begstationID)	//Quando o id da estação corresponde ao id da estação de partida
				{
					begstation=stationnode;
				}
				if(stationnode->stationID==tripnode->endstationID)	//Quando o id da estação corresponde ao id da estação de chegada
				{
					endstation=stationnode;
				}
				stationnode=stationnode->nextnode;
			}
			if(begstation!=NULL && endstation!=NULL) //Verificação para ver se a bicicleta não parte ou chega a uma estação invalida(removida ou inexistente)
			{
				AddRouteNode(routehead, CreateRouteNode(begstation, endstation));	//Cria o nó com a informação encontrada acima
			}
		}
		else //Para os casos em que já há nós na lista, tem que se verificar se a rota já existe ou não
		{
			begstation=NULL;
			endstation=NULL;
			stationnode=stationhead->topadd;
			while(stationnode!=NULL)	//Percorre os elementos da lista estações
			{
				if(stationnode->stationID==tripnode->begstationID)			//Encontra os pares viagem/estação
				{
					begstation=stationnode;
				}
				if(stationnode->stationID==tripnode->endstationID)
				{
					endstation=stationnode;
				}
				stationnode=stationnode->nextnode;
			}
			routenode=routehead->topadd;
			foundroute=0; //inicializa a flag
			while(foundroute==0 && routenode!=NULL && begstation!=NULL && endstation!=NULL)	//Percorre os nós da lista das rotas
			{
				if((routenode->startstation->stationID==tripnode->begstationID) && (routenode->endstation->stationID==tripnode->endstationID))
				{
					foundroute=1; //Se já houver algum par estação de partida->estação de chegada igual ao encontrado acima. Incrementa o nr de viagens,
					routenode->tripcount++; //liga a flag, pq encontrou o par, e salta da iteração da lista
				}
				routenode=routenode->nextnode;
			}
			if(foundroute==0 && begstation!=NULL && endstation!=NULL)	//Só cria o nó com a informação acima se não for encontrado par
			{
				AddRouteNode(routehead, CreateRouteNode(begstation, endstation));	//Cria o nó com a informação encontrada acima
			}
		}
		tripnode=tripnode->nextnode;
	}
}


void SortTripByStation(StationReg *selstation, TripHeader *triphead)
{
	TripReg *tripnode=NULL;

	tripnode=triphead->topadd;
	while(tripnode!=NULL)
	{
		if(tripnode->begstationID!=selstation->stationID && tripnode->endstationID!=selstation->stationID)
		{
			DelTripNode(triphead, tripnode);
		}
		tripnode=tripnode->nextnode;
	}
}


void PrintRoutes(RouteHeader *routehead, TripHeader *triphead, StationHeader *stationhead)
{

    RouteReg* current;

    int a=0;
    int stationID=0;
    char str[STR_MAX];
    int i=0;
    int pagesize=0;

    do {
        printf("Escolha uma estação através do seu identificador\n");
        fgets(str, STR_MAX, stdin);
        a = sscanf(str, "%d",  &stationID);
    } while ( a != 1);


    system("clear");


	if (routehead != NULL) {
        current=routehead->topadd;
    }
    else {
        return;
    }

	while (current != NULL ) {

        if (current->startstation->stationID == stationID){
            printf("Viagens a partir da estação escolhida: \n");
            printf("ID estação de partida: %d\n", current->startstation->stationID);
            printf("ID estação de chegada: %d\n", current->endstation->stationID);
            printf("Número de viagens: %d\n\n", current->tripcount);
            if(++i >=pagesize) {
            pause();
            i=0;
        }
        }

         if (current->endstation->stationID == stationID){
            printf("Viagens a chegar à estação escolhida: \n");
            printf("ID estação de partida: %d\n", current->startstation->stationID);
            printf("ID estação de chegada: %d\n", current->endstation->stationID);
            printf("Número de viagens: %d\n\n", current->tripcount);
            if(++i >=pagesize) {
            pause();
            i=0;
        }
        }



        current = current->nextnode;


	}
    pause();


}

//-------------------------------------------------------------------
//              VIAGENS
//--------------------------------------------------------------------


// função pra imprimir todas as viagens
void PrintTripList(int pagesize, TripHeader* header)
{

	TripReg* current;
	int i=0;

    system("clear");


	if (header != NULL) {
        current=header->topadd;
    }
    else {
        return;
    }

	while (current != NULL ) {

        printf("ID da viagem: %ld\n", current->tripID);
        printf("Duração: %ld\n", current->tripdursec);
        printf("Data e hora de inicio: %s\n", current->startdatetime);
        printf("ID estaçao inicial: %d\n", current->begstationID);
        printf("Data e hora de fim: %s\n", current->enddatetime);
        printf("ID estação final: %d\n", current->endstationID);
        printf("ID biciclet: %s\n", current->bikeID);
        printf("Tipo de Usuário (Registado-1, Casual-0: %d\n", current->usertype);
        // caso seja usuário imprime os parametros:
        if (current->usertype == 1){
            printf("Ano de Nascimento: %d\n", current->userbirthyear);
            printf("Género (M-1, F-2): %d\n\n", current->usergender);
        } else {
            printf("\n");
        }

        current = current->nextnode;
        if(++i >=pagesize) {
            pause();
            i=0;
        }
	}
    pause();

}

//função que escolhe quais viagens devem ser impressas
TripHeader* SelectTripList( TripHeader *fullListheader, char* btime, char *etime, int weekday, int duration)
{
    char *starttime="", *endtime="";
    char startdatetime[STR_MAX];
    char enddatetime[STR_MAX];

    TripReg *cursor;



	if (fullListheader != NULL)
        cursor=fullListheader->topadd;
    else
        exit(EXIT_FAILURE);


    //cria uma nova cabeça
    TripHeader *selection = CreateTripHeader();


    while (cursor != NULL) {

        strcpy(startdatetime, cursor->startdatetime);
        strtok(startdatetime, " ");
        starttime=strtok(NULL, " ");

        strcpy(enddatetime, cursor->enddatetime);
        strtok(enddatetime, " ");
        endtime=strtok(NULL, " ");

        if((btime[0]== 0  || (strcmp(btime, starttime) <= 0 && strcmp(etime, starttime) >= 0)) &&
            //compara as strings e se o etime for menor que o startime dá 1...
           (etime[0]== 0  || (strcmp(etime, endtime) >= 0 && strcmp(btime, endtime) <= 0)) &&
           (weekday==0|| getWeekday(cursor->startdatetime) == weekday) &&
           // compara a string com o dia da semana correspondente a data, com o dia da semana escolhido pelo utilizador
           (duration==0 || cursor->tripdursec <= duration)) {

            //adiciona os nos correspondentes
            AddTripNode(ADDBOT, selection,
                        CreateTripNode(cursor->tripID, cursor->tripdursec, cursor->startdatetime, cursor->begstationID, cursor->enddatetime, cursor->endstationID, cursor->bikeID, cursor->usertype, cursor->userbirthyear, cursor->usergender ),
                        NULL);
        }
        cursor = cursor->nextnode;

    }

    return selection;
}




//--------------------------------------------------------------------
//                  ESTAÇÔES
//-----------------------------------------------------------------------

//função para calcular as estatisticas pedidas na listagem de estações
void tripsByStations(int tripEvent,
                      TripHeader* header,
                      int stationId ,
                      int* maxHourly,
                      float* avgHourly,
                      int* minHourly,
                      int* count) {


    int
     curHour=-1,
     curDay=-1,
     curMonth=-1,
     curYear=-1,
     TotalHours=-1,       // nr de horas diferentes em que ocorreram saidas
     Total=0,            // total de saídas
     curTotal=0,
     h,m,s,y,mon,d;

    TripReg *cursor;

    *maxHourly=0;
    *minHourly=999999;
    *count=0;



	if (header != NULL)
        cursor=header->topadd;
    else
        return;

    while (cursor != NULL) {
        //separa a string em inteiros
        if (tripEvent == TRIP_START) {
            sscanf(cursor->startdatetime, "%d/%d/%d %d:%d:%d", &mon, &d, &y, &h, &m, &s);
        } else {
            sscanf(cursor->enddatetime, "%d/%d/%d %d:%d:%d", &mon, &d, &y, &h, &m, &s);
        }
        //compara esses dados com os dados currentes
        if (!(curHour==h && curDay==d && curMonth==mon && curYear==y) || cursor->nextnode==NULL) {
            Total += curTotal;
            // calculo do ḿáximo de bicicletas que sairam da estação naquela hora
            if (curTotal>*maxHourly){
                *maxHourly=curTotal;
            }
            if (curTotal>0 && curTotal<*minHourly){
                *minHourly=curTotal;
            }
            //se tiverem ocorrido saidas naquela hora então o TotalHours soma 1
            TotalHours+=(curTotal>0);
            curTotal=0;
            //atualiza os dados
            curHour=h;
            curDay=d;
            curMonth=mon;
            curYear=y;
        }

        if (curHour==h && curDay==d && curMonth==mon && curYear==y) {
            if (cursor->begstationID == stationId) {
               curTotal++;
            }
        }

        cursor=cursor->nextnode;
    }
    *avgHourly = Total/(float)TotalHours;
    *count=Total;


}


void PrintTripbyStation(int pagesize,
                        StationHeader *stationheader,
                        TripHeader *tripheader)
{
    int maxHourlyStarts=0;
    float avgHourlyStarts=0;
    int minHourlyStarts=0;
    int countStarts=0;
    int maxHourlyEnds=0;
    float avgHourlyEnds=0;
    int minHourlyEnds=0;
    int countEnds=0;
    TripHeader * tripheaderStart;
    TripHeader * tripheaderEnd;

    int i=0;
    StationReg* aux=NULL;

    system("clear");


    if (stationheader != NULL)
        aux=stationheader->topadd;
    else {
        return;
    }

    tripheaderStart=SelectTripList(tripheader,"","",0,0);
    sortTrips(tripheaderStart, SORT_BY_STARTDATETIME);

    tripheaderEnd=SelectTripList(tripheader,"","",0,0);
    sortTrips(tripheaderEnd, SORT_BY_ENDDATETIME);

    while (aux != NULL ) {

        tripsByStations(TRIP_START, tripheaderStart, aux->stationID,
                        &maxHourlyStarts,
                        &avgHourlyStarts,
                        &minHourlyStarts,
                        &countStarts);

        tripsByStations(TRIP_END, tripheaderEnd, aux->stationID,
                        &maxHourlyEnds,
                        &avgHourlyEnds,
                        &minHourlyEnds,
                        &countEnds);

        printf("ID estação: %d\n", aux->stationID);
        printf("Nome da estação: %s\n", aux->stationname);
        printf("latitude da estação: %f\n", aux->lat );
        printf("longitude estação: %f\n", aux->longt);
        if(countStarts==0){
            printf("Não há registo de saídas nesta estação\n");
        } else {
            printf("Máximo de saidas: %d\n", maxHourlyStarts);
            printf("Média de saidas: %.2f\n", avgHourlyStarts);
            printf("Minimo de saidas: %d\n", minHourlyStarts);
        }
        if(countEnds==0){
            printf("Não há registo de chegadas nesta estação\n");
        } else {
            printf("Máximo de chegadas: %d\n", maxHourlyEnds);
            printf("Média de chegadas: %.2f\n", avgHourlyEnds);
            printf("Minimo de chegadas: %d\n\n", minHourlyEnds);
        }
        aux = aux->nextnode;
        if(++i >=pagesize) {
            pause();
            i=0;
        }
    }

    DelTripList(tripheaderStart);
    DelTripList(tripheaderEnd);
    free(tripheaderStart);
    free(tripheaderEnd);

    pause();
}




//função para ordenar as viagens
void sortTrips(TripHeader* header ,int sortBy) {

    int swapped=0,y=0,mon=0,d=0,h=0,m=0,s=0;
    TripReg *current1;
    char curDate[STR_MAX];
    char nextDate[STR_MAX];

    if (header != NULL)
        current1=header->topadd;
    else
        return;

    do
    {
        swapped = 0;
        if (header != NULL)
            current1=header->topadd;
        else
            return;

        while (current1->nextnode != NULL)
        {
            if  (sortBy == SORT_BY_STARTDATETIME){
                sscanf(current1->startdatetime, "%d/%d/%d %d:%d:%d", &mon, &d, &y, &h, &m, &s);
                sprintf(curDate,"%4d/%2d/%2d %2d", y,mon,d,h );
                sscanf(current1->nextnode->startdatetime, "%d/%d/%d %d:%d:%d", &mon, &d, &y, &h, &m, &s);
                sprintf(nextDate,"%4d/%2d/%2d %2d", y,mon,d,h );
            }
            else if  (sortBy == SORT_BY_ENDDATETIME){
                sscanf(current1->enddatetime, "%d/%d/%d %d:%d:%d", &mon, &d, &y, &h, &m, &s);
                sprintf(curDate,"%4d/%2d/%2d %2d", y,mon,d,h );
                sscanf(current1->nextnode->enddatetime, "%d/%d/%d %d:%d:%d", &mon, &d, &y, &h, &m, &s);
                sprintf(nextDate,"%4d/%2d/%2d %2d", y,mon,d,h );
            }
            else return;


            if (strcmp(curDate, nextDate)>0) {
                swapTrips(current1, current1->nextnode);
                swapped = 1;
            }
            current1 = current1->nextnode;
        }
    }
    while (swapped);
}



//função para trocar os dados de dois noṍs, a e b
void swapTrips(TripReg *a, TripReg *b)
{
    TripReg temp;

    temp.tripID = a->tripID ;
    temp.tripdursec = a->tripdursec;
    temp.startdatetime = a->startdatetime;
    temp.begstationID = a->begstationID;
    temp.enddatetime = a->enddatetime;
    temp.endstationID = a->endstationID ;
    temp.bikeID = a->bikeID;
    temp.usertype = a->usertype;
    temp.userbirthyear = a->userbirthyear;
    temp.usergender = a->usergender;

    a->tripID = b->tripID ;
    a->tripdursec = b->tripdursec;
    a->startdatetime = b->startdatetime;
    a->begstationID = b->begstationID;
    a->enddatetime = b->enddatetime;
    a->endstationID = b->endstationID ;
    a->bikeID = b->bikeID;
    a->usertype = b->usertype;
    a->userbirthyear = b->userbirthyear;
    a->usergender = b->usergender;

    b->tripID = temp.tripID ;
    b->tripdursec = temp.tripdursec;
    b->startdatetime = temp.startdatetime;
    b->begstationID = temp.begstationID;
    b->enddatetime = temp.enddatetime;
    b->endstationID = temp.endstationID ;
    b->bikeID = temp.bikeID;
    b->usertype = temp.usertype;
    b->userbirthyear = temp.userbirthyear;
    b->usergender = temp.usergender;

}

//-------------------------------------------------------------------
//              ESTATISTICAS
//-------------------------------------------------------------------


//funçao para calcular as estatisticas
void TripStats(TripHeader *header, StationHeader *stationheader)
{

    TripReg *current=NULL;

    float malestats=0;
    float femalestats=0;
    float agestats=0;
    float durationstats=0;
    float femvelocity=0;
    float malevelocity=0;
    int tripsbyuser=0;
    int totaltrips=0;
    int male=0;
    int age=0;
    int agearray[75]={0};
    int durationarray[24]={0};
    int i=0, j=0, l=0;
    float duration=0;
    float distance=0;
    int femaledist[75]={0};
    int maledist[75]={0};
    int femaletime[75]={0};
    int maletime[75]={0};
    int a=0, sel=0;
    char str[STR_MAX];

    if (header != NULL)
        current=header->topadd;
    else
        return;

    while (current != NULL) {
        totaltrips++;
        if (current->usertype == REGISTERED){
            tripsbyuser++;
            if (current->usergender == MALE){
                male++;
            }

            age=CURRENT_YEAR-current->userbirthyear;
            i=age/2;        // idades de dois em dois, array com metade do tamanho
            agearray[i]++;
        }

        if (current->tripdursec <= 21600){ //6 horas de duração maxima
            duration=current->tripdursec / 60;
            j=duration/15;
            durationarray[j]++;
        }

        if(current->usergender==2)
        {
            femaletime[i] +=  current->tripdursec;
            distance=calcdistance(stationheader, current->begstationID, current->endstationID);
            femaledist[i] += distance;
        }
        if(current->usergender==1)
        {
            maletime[i] +=  current->tripdursec;
            distance=calcdistance(stationheader, current->begstationID, current->endstationID);
            maledist[i] += distance;
        }

        current = current->nextnode;
    }
    do{
    system("clear");
    printf("Escolha uma das seguintes opções:\n");
    printf("    1. Estatísticas por género\n");
    printf("    2. Estatísticas por idade\n");
    printf("    3. Estatísticas da duração\n");
    printf("    4. Estatísticas da velocidade\n");
    printf("    5. Voltar ao menu principal\n");

    do {
        fgets(str, STR_MAX, stdin);
        a = sscanf(str, "%d",  &sel);
    } while ( a != 1);
    system("clear");
    switch(sel)
    {
        case 1:
            malestats = male / (float)tripsbyuser * 100;
            femalestats = (tripsbyuser - male) / (float)tripsbyuser * 100;

            printf("Percentagem de viagens efetuadas por utilizadores do género masculino: %.2f%%\n", malestats);
            printf("Percentagem de viagens efetuadas por utilizadores do género feminino: %.2f%%\n\n", femalestats);
            pause();
            break;
        case 2:
            printf("         |  Percentagem por idades |\n\n\n");
            for (i=0; i<75; i++)
            {
                agestats = agearray[i] / (float)tripsbyuser * 100 ;
                if(agearray[i]!=0){
                    printf(" %3d-%3d |         %.4f%%         |\n", i*2, (i*2+1), agestats );
                }
            }
            pause();
            break;
        case 3:
            printf("         |  Percentagem por duração |\n");
            for (j=0; j<24; j++)
            {
                durationstats = durationarray[j] / (float)totaltrips * 100 ;
                printf(" %3d-%3d |         %.4f%%          |\n", j*15, (j*15+15), durationstats );
            }
            pause();
            break;
        case 4:
            printf("         |  F     |  M       |\n");
            for (l=0; l<75; l++)
            {
                femvelocity = femaledist[l] / (float)(femaletime[l]/3600);
                malevelocity = femaledist[l] / (float)(femaletime[l]/3600);

                printf(" %3d-%3d |  %.2f  |   %.2f   |\n", l*2, (l*2+1), femvelocity, malevelocity);
            }
            pause();
            break;
        case 5:
            system("clear");
            return;
        default:
            printf("Por favor introduza um numero válido\n");
            pause();
            break;
        }
    }while(1);
}
