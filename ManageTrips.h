#ifndef MANAGE_TRIP
#define	MANAGE_TRIP
#include "ManageStations.h"
#define	ADDTOP 0
#define	ADDBOT 1
#define ADDMIDAFT 2
#define ADDMIDBFR 3
#define CASUAL 0
#define REGISTERED 1
#define UNDIFINED 0
#define MALE 1
#define FEMALE 2
#define CURRENT_YEAR 2017

#define SORT_BY_STARTDATETIME 100
#define SORT_BY_ENDDATETIME 101

#define TRIP_START 1
#define TRIP_END 2
//******************************FUNÇÕES DESTA BIBLIOTECA************************************//
/*
	CreateTripNode -> Cria um nó com a informação lida do ficheiro referente à viagens

	AddTripNode -> Adiciona à lista um nó

	DelTripNode -> Apaga um nó da lista

	DelTripList -> Apaga a lista inteira, mantendo a cabeça

	CreateTripHeader -> Cria uma cabeça para uma lista com nós da estrutura TripReg

	CreateTripList -> Lê o ficheiro e cria a lista com todas as viagens
																							*/
//******************************************************************************************//



typedef struct TripStruct
{
	long tripID;											//ID da viagem
	long tripdursec;									//Duração da viagem em segundos
	char *startdatetime; 							//Data e hora de inicio da viagem
	int begstationID; 									//ID da estação incial
	char *enddatetime; 								//Data e hora do fim da viagem
	int endstationID; 									//ID da estação final
	char *bikeID; 									//ID da bicicleta
	int	usertype;										//Tipo de usuário (0=Casual, 1=Registado)
	int userbirthyear; 									//Ano de nascimento do user (0 se usuario casual)
	int usergender; 									//Género do user (0=se casual, 1=masculino, 2= feminino)
	struct TripStruct *nextnode;						//Endereço do próximo nó da lista
	struct TripStruct *prevnode;						//Endereço do previous nó da lista
} TripReg;

typedef	struct
{
	TripReg *topadd;							//Endereço do primeiro nó da lista
	TripReg *botadd;							//Endereço do último nó da lista
	long totalnodes;							//Numero total de nós na lista
} TripHeader;


typedef struct RouteStruct
{
	StationReg* startstation;
	StationReg* endstation;
	int tripcount;
	struct RouteStruct *nextnode;
}RouteReg;

typedef struct
{
	RouteReg *topadd;
} RouteHeader;

TripHeader *CreateTripHeader();

TripReg *CreateTripNode(					//Cria um novo nó para cada viagem
	long tripID,								//ID da viagem
	long tripdursec,						//Duração da viagem em segundos
	char *startdatetime, 					//Data e hora de inicio da viagem
	int begstationID,						//ID da estação incial
	char *enddatetime, 						//Data e hora do fim da viagem
	int endstationID, 						//ID da estação final
	char *bikeID, 							//ID da bicicleta
	int	usertype,							//Tipo de usuário (0=Casual, 1=Registado)
	int userbirthyear, 						//Ano de nascimento do user (0 se usuario casual)
	int usergender 							//Género do user (0=masculino, 1= feminino)
	);

TripReg	*AddTripNode(			//Adiciona um nó à lista
	int	addtype,				//Tipo de adição à lista (0=Topo da lista, 1=Fim da lista, 2=Meio da lista)
	TripHeader*,				//Cabeça da lista
	TripReg*,					//Nó a adicionar (com os dados de uma viagem)
	TripReg*					//Endereço do nó de referência para inserção na lista do novo nó
	);

void DelTripNode(
	TripHeader* ,				//Cabeça da lista
	TripReg*					//Nó a apagar
	);

int DelTripList(				//Apaga a lista inteira para libertar memória
	TripHeader*
	);

void CreateTripList(
	char*,
	TripHeader*
	);

RouteHeader *CreateRouteHeader();


RouteReg *CreateRouteNode(		//DONE
    StationReg *station1,		//Estação de partida
    StationReg *station2		//estação de chegada
    );

void DupTripList(TripHeader *newlist, TripHeader *baselist);

void DupRouteList(RouteHeader*, RouteHeader*);

void SortTripByStation(StationReg *selstation, TripHeader *triphead);

void AddRouteNode(RouteHeader *head, RouteReg *node);	//DONE

void DelRouteList(RouteHeader *head);

void DelRouteNode(RouteHeader *head, RouteReg *node);

//Função principal. Cria a lista das rotas a partir da lista de viagens selecionada
void CreateRouteList(RouteHeader *routehead, TripHeader *triphead, StationHeader *stationhead);

void PrintRoutes(RouteHeader*, TripHeader*, StationHeader*);

void PrintTripbyStation(
    int pagesize,
    StationHeader*,
    TripHeader*);


void PrintTripList(
	int ,
	TripHeader*
	);


TripHeader* SelectTripList(
    TripHeader*,
    char*,
    char*,
    int,
    int );

void tripsByStations(
     int ,
     TripHeader* ,
      int  ,
      int* ,
      float* ,
      int* ,
      int* );

void sortTrips(TripHeader*,int);

void swapTrips(TripReg*, TripReg*);


void TripStats(TripHeader*, StationHeader*);



#endif
