/**********************************************************************\
* Dateiname: ms1.h
* Autor    : Michael Bernstein
* Projekt  : client_ms1
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: api for ms1 object
*
\**********************************************************************/

#ifndef MS1_H
#define MS1_H

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
} Ms1Struct;

/*--- #define-Konstanten und Makros ----------------------------------*/
#define Ms1SetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define Ms1SetServerIp(Data, IpAddr) (Data)->ServerIp=IpAddr
#define Ms1SetServerPort(Data, Port) (Data)->ServerPort=Port
#define Ms1SetClientSock(Data, Sock) (Data)->ClientSock=Sock
#define Ms1SetCanName(Data, Name)    (Data)->CanName=Name
#define Ms1SetCanSock(Data, Sock)    (Data)->CanSock=Sock

#define Ms1GetVerbose(Data)    (Data)->Verbosity
#define Ms1GetServerIp(Data)   (Data)->ServerIp
#define Ms1GetServerPort(Data) (Data)->ServerPort
#define Ms1GetClientSock(Data) (Data)->ClientSock
#define Ms1GetCanName(Data)    (Data)->CanName
#define Ms1GetCanSock(Data)    (Data)->CanSock

/*--- Prototypen globaler Funktionen ---------------------------------*/
Ms1Struct *Ms1Create(void);
void Ms1Destroy(Ms1Struct *Data);
void Ms1Init(Ms1Struct *Data, BOOL Verbose, char *IpAddr, int Port, char *CanIf);
void Ms1Exit(Ms1Struct *Data);
void Ms1Run(Ms1Struct *Data);

#endif /* MS1_H */
