/**********************************************************************\
* Dateiname: log.h
* Autor    : Michael Bernstein
* Projekt  : client_log
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: api for log object
*
\**********************************************************************/

#ifndef LOG_H
#define LOG_H

/*--- #includes der Form <...> ---------------------------------------*/
#include <boolean.h>

/*--- Datentypen (typedef) -------------------------------------------*/
typedef struct {
   BOOL Verbosity;
   char *ServerIp;  /* Server addr to listen */
   int ServerPort;  /* Server port to listen */
   int ClientSock;  /* our socket to server */
} LogStruct;

/*--- #define-Konstanten und Makros ----------------------------------*/
#define LogSetVerbose(Data, Verbose) (Data)->Verbosity=Verbose
#define LogSetServerIp(Data, IpAddr) (Data)->ServerIp=IpAddr
#define LogSetServerPort(Data, Port) (Data)->ServerPort=Port
#define LogSetClientSock(Data, Sock) (Data)->ClientSock=Sock

#define LogGetVerbose(Data)    (Data)->Verbosity
#define LogGetServerIp(Data)   (Data)->ServerIp
#define LogGetServerPort(Data) (Data)->ServerPort
#define LogGetClientSock(Data) (Data)->ClientSock

/*--- Prototypen globaler Funktionen ---------------------------------*/
LogStruct *LogCreate(void);
void LogDestroy(LogStruct *Data);
void LogInit(LogStruct *Data, BOOL Verbose, char *IpAddr, int Port);
void LogExit(LogStruct *Data);
void LogRun(LogStruct *Data);

#endif /* LOG_H */
