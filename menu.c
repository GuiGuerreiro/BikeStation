#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "menu.h"
#include "ManageStations.h"
#include "ManageTrips.h"


void showMainMenu(char* file, char *btime, char *etime, int *weekday, int *duration, TripHeader *tripshead, StationHeader *stationhead, RouteHeader *routhead)
{
    char str[STR_MAX];
    int i;
    int menu1 = 0;
    system("clear");

    while(1) {
        printf("Escolha uma das seguintes opções:\n");
        printf("1. Seleção de dados\n");
        printf("2. Listagem de viagens\n");
        printf("3. Listagem de estações\n");
        printf("4. Listagem de rotas\n");
        printf("5. Listagem de estatísticas\n");
        printf("6. Terminar\n");

        do {
            fgets(str, STR_MAX, stdin);
            i = sscanf(str, "%d",  &menu1);
        } while ( i != 1);

        if ( menu1 == 1 ){
            system("clear");
            showSelectionMenu(&tripshead, file, btime, etime, weekday, duration);
        } else if (menu1 == 2){
            showTrips(tripshead, btime, etime, *weekday, *duration);
            system("clear");
        } else if (menu1 == 3){
            showStations(tripshead, stationhead, btime, etime, *weekday, *duration);
            system("clear");
        } else if (menu1 == 4){
            showRoutes(routhead, tripshead, stationhead);
        } else if (menu1 == 5){
            showStats(tripshead, stationhead);
        } else if (menu1 == 6){
            system("clear");
            return;
        } else {
            printf("Por favor introduza um numero válido.\n\n");
        }
    }
}

void showSelectionMenu(TripHeader **tripshead, char* file, char *btime, char *etime, int *weekday, int *duration)
{
    char str[STR_MAX];
    int select, c, d;
    int j;
    TripHeader *sel;

    while (1){
        printf("1. Período de tempo\n");
        printf("2. Dia da semana\n");
        printf("3. Duração\n");
        printf("4. Limpar todas as seleções\n");
        printf("5. Voltar ao menu principal\n");

        do {
            fgets(str, STR_MAX, stdin);
            j = sscanf(str, "%d",  &select);
        } while ( j != 1);

        system("clear");

        switch ( select)
        {
        case 1:
            printf("Introduza a hora de inicio (no formato hh:mm:00):\n");
            ScanHour(btime);
            printf("Introduza a hora de fim (no formato hh:mm:00):\n");
            ScanHour(etime);
            system("clear");
            break;
        case 2:

            do {
                printf("Introduza o numero correspondente ao dia da semana pretendido:\n");
                printf("1 -- segunda\n");
                printf("2 -- terca\n");
                printf("3 -- quarta\n");
                printf("4 -- quinta\n");
                printf("5 -- sexta\n");
                printf("6 -- sabado\n");
                printf("7 -- domingo\n");
                fgets(str, STR_MAX, stdin);
                c = sscanf(str, "%d",  weekday);
            } while ( c != 1 );
            system("clear");
            break;
        case 3:
            do {
                printf("Introduza a duração máxima da viagem:\n");
                fgets(str, STR_MAX, stdin);
                d = sscanf(str, "%d",  duration);
            } while ( d != 1);
            system("clear");
            break;
        case 4:
            // repor todos os valores
            strcpy(btime, "");  // copia para btime a string vazia
            strcpy(etime, "");
            *weekday = 0;
            *duration = 0;
            DelTripList(*tripshead);
            free(*tripshead);
            *tripshead=CreateTripHeader();
            CreateTripList(file, *tripshead);
            break;
        case 5:
            sel=SelectTripList(*tripshead, btime, etime, *weekday, *duration);
            DelTripList(*tripshead);
            free(*tripshead);
            *tripshead=sel;
            return ;
        default:
            break;
        }
    }
}



void showTrips( TripHeader *tripshead, char *btime, char *etime, int weekday, int duration)
{
    int pagesize;
    pages(&pagesize);

    PrintTripList(pagesize, tripshead);

}


void showStations( TripHeader *tripshead, StationHeader *stationhead, char *btime, char *etime, int weekday, int duration)
{

    int pagesize;
    pages(&pagesize);


    PrintTripbyStation( pagesize,
                        stationhead,
                        tripshead);

}


void showRoutes(RouteHeader *routhead, TripHeader *tripshead, StationHeader *stationhead)
{
    int pagesize;
    pages(&pagesize);
    PrintRoutes(routhead, tripshead, stationhead);
}

void showStats(TripHeader *tripshead, StationHeader *stationhead)
{
    TripStats(tripshead, stationhead);
}


//funçao para continuar a listagem
void pause(){
    char str[STR_MAX];
    printf("\n--------------------------------------------------------\nPressione uma tecla para continuar\n");
    fgets(str, STR_MAX, stdin); // o tilizador carrega no enter e a listagem continua
    system("clear");
}


//funçao que pergunta ao utilizador quantas páginas quer
void pages(int *pagesize)
{
    int a;
    char str[STR_MAX];
    do {
        printf("Quantas linhas quer por página?\n");
        fgets(str, STR_MAX, stdin);
        a = sscanf(str, "%d",  pagesize);
    } while ( a != 1);
}


// funçao que calcula o dia da semana e devolve o seu inteiro correspondente
int getWeekday(char *bdate)
{
    int k=0, m=0, D=0, C=0, a=0, w=0;
    //f ->
    //k -> dia do mes
    //m -> nºdo mes -- março(1), abril(2), maio(3),..., fevereiro (12)
    //D -> ultimos dois digitos do ano
    //C -> primeiros dois digitos do ano

    if (sscanf(bdate, "%d/%d/%d", &m, &k, &a) == 1)
        return 8;

    if (m>=3)
        m=m-2;
    else
        m=10+m;

    D=a%100;
    C=a/100;

    w = (k + ((13*m-1)/5) + D + (D/4) + (C/4) - 2*C)%7;
    if (w<0)
        w=w+7;

    switch (w) {
        case 0: return 7; break;    //domingo
        case 1: return 1; break;    //segunda
        case 2: return 2; break;    //terca
        case 3: return 3; break;    //quarta
        case 4: return 4; break;    //quinta
        case 5: return 5; break;    //sexta
        case 6: return 6; break;    //sabado
        default: return 8; break;
    }
}

// função que transforma as horas numa string
void ScanHour(char* i)
{
    int a=0;
    int h=0, m=0, s=0;
    char str[STR_MAX];

    do {
        strcpy(i, fgets(str, STR_MAX, stdin));
        a = sscanf(str, "%d:%d:%d\n",  &h, &m, &s);
    } while (a != 3 || h>23 || h<0 || m>=60 || m<0 || s>=60 || s<0);
}
