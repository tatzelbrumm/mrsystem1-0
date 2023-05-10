/**********************************************************************\
* Dateiname: inject.h
* Autor    : Michael Bernstein
* Projekt  : client_inject
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: api for ms1 object
*
\**********************************************************************/

#ifndef INJECT_H
#define INJECT_H

/*--- #includes der Form <...> ---------------------------------------*/
#include <boolean.h>

/*--- Datentypen (typedef) -------------------------------------------*/
typedef struct {
   BOOL Verbosity;
   char *ServerIp;  /* Server addr to listen */
   int ServerPort;  /* Server port to listen */
   int ClientSock;  /* our socket to server */
} InjectStruct;

/*--- #define-Konstanten und Makros ----------------------------------*/
#define InjectSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define InjectSetServerIp(Data, IpAddr) (Data)->ServerIp=IpAddr
#define InjectSetServerPort(Data, Port) (Data)->ServerPort=Port
#define InjectSetClientSock(Data, Sock) (Data)->ClientSock=Sock

#define InjectGetVerbose(Data)    (Data)->Verbosity
#define InjectGetServerIp(Data)   (Data)->ServerIp
#define InjectGetServerPort(Data) (Data)->ServerPort
#define InjectGetClientSock(Data) (Data)->ClientSock

/*--- Prototypen globaler Funktionen ---------------------------------*/
InjectStruct *InjectCreate(void);
void InjectDestroy(InjectStruct *Data);
void InjectInit(InjectStruct *Data, BOOL Verbose, char *IpAddr, int Port);
void InjectExit(InjectStruct *Data);
void InjectRun(InjectStruct *Data);

#endif /* INJECT_H */
