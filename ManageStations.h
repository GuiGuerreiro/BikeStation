#ifndef MANAGE_STATIONS
#define MANAGE_STATIONS
#define EXISTENTE 1
#define REMOVIDA 0
#define	ADDTOP 0
#define	ADDBOT 1
#define ADDMIDAFT 2
#define ADDMIDBFR 3
#define PARTIDA 0
#define CHEGADA 1


//******************************FUNÇÕES DESTA BIBLIOTECA************************************//
/*
	CreateStationNode -> Cria um nó com a informação lida do ficheiro referente às estações

	AddStationNode -> Adiciona à lista um nó

	DelStationNode -> Apaga um nó da lista

	DelStationList -> Apaga a lista inteira, mantendo a cabeça

	CreateStationHeader -> Cria uma cabeça para uma lista com nós da estrutura StationReg

	CreateStationList -> Lê o ficheiro e cria a lista com todas as estações
																							*/
//******************************************************************************************//
typedef struct StationStruct
{
	int stationID;
	char shortname[10];
	char *stationname;
	char municipality[10];
	double lat;
	double longt;
	int state;
	struct StationStruct *prevnode;
	struct StationStruct *nextnode;
} StationReg;

typedef struct
{
	StationReg *topadd;
	StationReg *botadd;
	long totalnodes;
}StationHeader;


StationHeader *CreateStationHeader();

StationReg *CreateStationNode(
	int stationID,
	char *shortname,
	char *stationname,
	char *municipality,
	double lat,
	double longt,
	int	state
	);

StationReg *AddStationNode(			//Adiciona um nó à lista
	int	addtype,				//Tipo de adição à lista (0=Topo da lista, 1=Fim da lista, 2=Meio da lista)
	StationHeader*,				//Cabeça da lista
	StationReg*,				//Nó a adicionar (com os dados da estação)
	StationReg*					//Endereço do nó de referência para inserção na lista do novo nó	NULL em caso de topo ou cauda
	);


void DelStationNode(
	StationHeader* ,				//Cabeça da lista
	StationReg*					//Nó a apagar
	);

int DelStationList(				//Apaga a lista inteira para libertar memória
	StationHeader*
	);


void CreateStationList(
	char*,
	StationHeader*
	);

void DupStationList(StationHeader *newlist, StationHeader *baselist);



char *mystrdup(const char *s);

char *mystrsep(char **, const char *);

float calcdistance(StationHeader*, int, int);

#endif
