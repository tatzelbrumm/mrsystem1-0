/**********************************************************************\
* Dateiname: ms2.h
* Autor    : Michael Bernstein
* Projekt  : client_ms2
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: api for ms2 object
*
\**********************************************************************/

#ifndef MS2_H
#define MS2_H

/*--- #includes der Form <...> ---------------------------------------*/
#include <arpa/inet.h>
#include <boolean.h>

/*--- Datentypen (typedef) -------------------------------------------*/
typedef struct {
   BOOL Verbosity;
   char *ServerIp; /* Server addr to listen */
   int ServerPort; /* Server port to listen */
   int ClientSock; /* our socket to server */
   char *CanName;  /* name of can interface */
   int CanSock;    /* socket to outside to send */
} Ms2Struct;

/*--- #define-Konstanten und Makros ----------------------------------*/
#define Ms2SetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define Ms2SetServerIp(Data, IpAddr) (Data)->ServerIp=IpAddr
#define Ms2SetServerPort(Data, Port) (Data)->ServerPort=Port
#define Ms2SetClientSock(Data, Sock) (Data)->ClientSock=Sock
#define Ms2SetCanName(Data, Name)    (Data)->CanName=Name
#define Ms2SetCanSock(Data, Sock)    (Data)->CanSock=Sock

#define Ms2GetVerbose(Data)    (Data)->Verbosity
#define Ms2GetServerIp(Data)   (Data)->ServerIp
#define Ms2GetServerPort(Data) (Data)->ServerPort
#define Ms2GetClientSock(Data) (Data)->ClientSock
#define Ms2GetCanName(Data)    (Data)->CanName
#define Ms2GetCanSock(Data)    (Data)->CanSock

/*--- Prototypen globaler Funktionen ---------------------------------*/
Ms2Struct *Ms2Create(void);
void Ms2Destroy(Ms2Struct *Data);
void Ms2Init(Ms2Struct *Data, BOOL Verbose, char *IpAddr, int Port, char *CanIf);
void Ms2Exit(Ms2Struct *Data);
void Ms2Run(Ms2Struct *Data);

#endif /* MS2_H */
