#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "ManageTrips.h"
#include "ManageStations.h"
#define STR_MAX 30


void showMainMenu(char*, char*, char*, int*, int*, TripHeader*, StationHeader*, RouteHeader*);
void showSelectionMenu(TripHeader**, char*, char*, char*, int*, int*);
void showTrips( TripHeader *, char* , char* , int, int);
void showStations( TripHeader*, StationHeader* , char*, char*, int, int);
void showRoutes(RouteHeader*, TripHeader*, StationHeader*);
void showStats(TripHeader *, StationHeader*);
void pause();
void pages(int*);
int getWeekday(char *);
void ScanHour(char*);

#endif // MENU_H_INCLUDED
