#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "ManageStations.h"
#include "menu.h"
#include "ManageGraphics.h"

StationHeader *CreateStationHeader()
{
	StationHeader *stationheader= (StationHeader*)malloc(sizeof(StationHeader));

	stationheader->topadd=NULL;
	stationheader->botadd=NULL;
	stationheader->totalnodes=0;
	return stationheader;
}

StationReg *CreateStationNode(
	int stationID,
	char *shortname,
	char *stationname,
	char *municipality,
	double lat,
	double longt,
	int	state
	)
{
	//char *str;
	StationReg* stationnode = (StationReg*)malloc(sizeof(StationReg));
    if(stationnode == NULL)
    {
        printf("Error allocating memory\n");
        exit(0);
    }
    stationnode->stationID=stationID;
    strcpy(stationnode->shortname, shortname);
    stationnode->stationname=mystrdup(stationname);  //mystrdup já aloca a memória dinamicamente para o tamanho da stationname
    strcpy(stationnode->municipality, municipality);
    stationnode->lat=lat;
    stationnode->longt=longt;
    stationnode->state=state;
    stationnode->prevnode = NULL;
    stationnode->nextnode = NULL;

    return stationnode;
}


StationReg *AddStationNode (
    int addtype,            //Tipo de adição à lista (0=Topo da lista, 1=Fim da lista, 2=Meio da lista)
    StationHeader *head,       //Cabeça da lista
    StationReg *node,          //Nó a adicionar à lista
    StationReg *refnode            //Nó prévio à posição em que se vai inserir o novo nó. =NULL se for top ou bot
    )
{
    if(head==NULL)
        {
            printf("Trying to create StationList with no head");
            exit(EXIT_FAILURE);
        }
    if(head->totalnodes==0)
    {                           //Se for o primeiro elemento da lista head->botadd == head->topadd
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
                    if (refnode->nextnode == NULL)  //BOTADD style
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
                    if (refnode->prevnode == NULL)  //TOPADD style
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

void DelStationNode(
    StationHeader *header,
    StationReg *node
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
            free(node->stationname);
            free(node);
            header->totalnodes--;
        }
        else
        {
            if(node->nextnode == NULL)          //REMOVING TAIL
            {
                header->botadd=node->prevnode;
                node->prevnode->nextnode=NULL;
                free(node->stationname);
                free(node);
                header->totalnodes--;
            }
            else if(node->prevnode == NULL)     //REMOVING FIRST ELEMENT
            {
                header->topadd=node->nextnode;
                node->nextnode->prevnode=NULL;
                free(node->stationname);
                free(node);
                header->totalnodes--;
            }
            else
            {
                node->prevnode->nextnode=node->nextnode;
                node->nextnode->prevnode=node->prevnode;
                free(node);
                header->totalnodes--;
            }
        }
    }
}

void DupStationList(StationHeader *newlist, StationHeader *baselist)
{
    StationReg *sttemp=NULL;
    sttemp=baselist->topadd;
        while(sttemp!=NULL)
        {
            AddStationNode(ADDBOT, newlist,
            CreateStationNode(sttemp->stationID, sttemp->shortname, sttemp->stationname, sttemp->municipality, sttemp->lat, sttemp->longt, sttemp->state), NULL);
            sttemp=sttemp->nextnode;
        }
}


int DelStationList(
    StationHeader *header
    )
{
    StationReg *temp = NULL;
    StationReg *temp1 = NULL;
    temp = header->topadd;

    if(header->topadd==NULL)
        return 1;
    else
    {
        while(temp->nextnode!=NULL)
        {
            temp1 = temp->nextnode;
            free(temp->stationname);
            free(temp);
            temp = temp1;
        }
        free(temp->stationname);
        free(temp);
        header->topadd=NULL;
        header->botadd=NULL;
        header->totalnodes=0;
    }
    return 0;
}


void CreateStationList(
    char* file,
    StationHeader* header
    )
{
    FILE *fp = NULL;
    int stationID=0,state=0;
    double lat=0.0, longt=0.0;
    char shortname[25], municipality[20];
    StationReg *node=NULL;
    char *str=NULL, *strpt=NULL, *stationname=NULL;
    char line[500], temp[10], temp1[10];
    fp=fopen(file, "r");
    if(fp==NULL)
    {
        printf("Failed to open file\n");
	    exit(EXIT_FAILURE);
    }

    while(fgets(line, 500, fp))
    {
        strpt=str=mystrdup(line);
        stationID=atoi(mystrsep(&str,","));
        strcpy(shortname, mystrsep(&str,","));
        stationname=mystrdup(mystrsep(&str,","));
        strcpy(municipality, mystrsep(&str,","));
        lat=atof(mystrsep(&str,","));
        longt=atof(mystrsep(&str,","));
        strcpy(temp, "Removed");
        strcpy(temp1, mystrsep(&str,","));
        if(strcmp(temp, temp1)<0)
            state=REMOVIDA;
        else
            state=EXISTENTE;
        node=CreateStationNode(stationID, shortname, stationname, municipality, lat, longt, state);
        AddStationNode(ADDBOT, header, node, NULL);
        free(strpt);
        free(stationname);
    }
    fclose(fp);
}


char *mystrdup(const char *s)
{
    char *p= malloc(strlen(s)+1);
    if(p)
        strcpy(p,s);
    return p;
}



char *mystrsep(char **stringp, const char *delim)
{
    char *string;
    const char* spanp;
    int c, sc;
    char *tok;

    if((string=*stringp)==NULL)
        return NULL;

    for(tok=string;;)
    {
        c=*string++;
        spanp=delim;

        do
        {
            if((sc=*spanp++)==c)
            {
                if(c==0)
                    string=NULL;
                else
                    string[-1]=0;
                *stringp=string;
                return tok;
            }
        } while(sc!=0);
    }
}


// funçao pra calcular a distancia entre duas estações
float calcdistance(StationHeader *header, int station1, int station2 )
{
    StationReg* current;
    int lat1, lgt1, lat2, lgt2;
    int cateto1, cateto2;
    int distance=0;
    float distanceinkm=0;

    if (header != NULL)
        current=header->topadd;

    while(current != NULL){


        if (current->stationID==station1){     //calcular a latitude em pixeis para a estação escolhida
            lat1=LatToPy(current->lat);
            lgt1=LgtToPx(current->longt);
        }
        if (current->stationID==station2){
            lat2=LatToPy(current->lat);
            lgt2=LgtToPx(current->longt);
        }

        current=current->nextnode;
    }

    cateto1=lat1-lat2;
    cateto2=lgt1-lgt2;
    distance=sqrt((cateto1*cateto1) + (cateto2*cateto2)); // teorema de pitágoras

    // fator de conversão pixel para km
    // distañcia stationID=4 - stationID=5 = 1.76km
    // 1km = 92.04px

    distanceinkm = distance/92.04;

    return distanceinkm;

}
