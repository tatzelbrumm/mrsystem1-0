/**********************************************************************\
* Dateiname: drehscheibe.h
* Autor    : Michael Bernstein
* Projekt  : drehscheibe
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: api for drehscheibe object
*
\**********************************************************************/

#ifndef DREHSCHEIBE_H
#define DREHSCHEIBE_H

/*--- #includes der Form <...> ---------------------------------------*/
#include <boolean.h>
#include <menge.h>
#include <stack.h>

/*--- Datentypen (typedef) -------------------------------------------*/
typedef struct {
   int ClientSock;
} DrehscheibeClientStruct;

/*--- #define-Konstanten und Makros ----------------------------------*/
#define DrehclientSetSock(Data, Sock) (Data)->ClientSock=Sock

#define DrehclientGetSock(Data) (Data)->ClientSock

/*--- Datentypen (typedef) -------------------------------------------*/
typedef struct {
   BOOL Verbosity;
   char *ServerIp;        /* Server addr to listen */
   int ServerPort;        /* Server port to listen */
   int ServerSock;        /* Socket descriptor for server */
   Menge *SocketClients;  /* all clients connecting to sockets */
   Stack *RemoveClients;  /* clients to remove from SocketClients */
} DrehscheibeStruct;

/*--- #define-Konstanten und Makros ----------------------------------*/
#define DrehscheibeSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define DrehscheibeSetServerIp(Data, IpAddr) (Data)->ServerIp=IpAddr
#define DrehscheibeSetServerPort(Data, Port) (Data)->ServerPort=Port
#define DrehscheibeSetServerSock(Data, Sock) (Data)->ServerSock=Sock
#define DrehscheibeSetClient(Data, Client)   (Data)->SocketClients=Client
#define DrehscheibeSetRemove(Data, Remove)   (Data)->RemoveClients=Remove

#define DrehscheibeGetVerbose(Data)    (Data)->Verbosity
#define DrehscheibeGetServerIp(Data)   (Data)->ServerIp
#define DrehscheibeGetServerPort(Data) (Data)->ServerPort
#define DrehscheibeGetServerSock(Data) (Data)->ServerSock
#define DrehscheibeGetClient(Data)     (Data)->SocketClients
#define DrehscheibeGetRemove(Data)     (Data)->RemoveClients

/*--- Prototypen globaler Funktionen ---------------------------------*/
DrehscheibeStruct *DrehscheibeCreate(void);
void DrehscheibeDestroy(DrehscheibeStruct *Data);
void DrehscheibeInit(DrehscheibeStruct *Data, BOOL Verbose, char *IpAddr,
                     int Port);
void DrehscheibeExit(DrehscheibeStruct *Data);
void DrehscheibeRun(DrehscheibeStruct *Data);

#endif /* DREHSCHEIBE_H */
