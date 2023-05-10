/**********************************************************************\
* Dateiname: logms2.h
* Autor    : Michael Bernstein
* Projekt  : client_logms2
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: api for logms2 object
*
\**********************************************************************/

#ifndef LOGMS2_H
#define LOGMS2_H

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
} Logms2Struct;

/*--- #define-Konstanten und Makros ----------------------------------*/
#define Logms2SetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define Logms2SetServerIp(Data, IpAddr) (Data)->ServerIp=IpAddr
#define Logms2SetServerPort(Data, Port) (Data)->ServerPort=Port
#define Logms2SetClientSock(Data, Sock) (Data)->ClientSock=Sock
#define Logms2SetCanName(Data, Name)    (Data)->CanName=Name
#define Logms2SetCanSock(Data, Sock)    (Data)->CanSock=Sock

#define Logms2GetVerbose(Data)    (Data)->Verbosity
#define Logms2GetServerIp(Data)   (Data)->ServerIp
#define Logms2GetServerPort(Data) (Data)->ServerPort
#define Logms2GetClientSock(Data) (Data)->ClientSock
#define Logms2GetCanName(Data)    (Data)->CanName
#define Logms2GetCanSock(Data)    (Data)->CanSock

/*--- Prototypen globaler Funktionen ---------------------------------*/
Logms2Struct *Logms2Create(void);
void Logms2Destroy(Logms2Struct *Data);
void Logms2Init(Logms2Struct *Data, BOOL Verbose, char *IpAddr, int Port, char *CanIf);
void Logms2Exit(Logms2Struct *Data);
void Logms2Run(Logms2Struct *Data);

#endif /* LOGMS2_H */
