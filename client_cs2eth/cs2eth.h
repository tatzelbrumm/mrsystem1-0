/**********************************************************************\
* Dateiname: cs2eth.h
* Autor    : Michael Bernstein
* Projekt  : client_cs2eth
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: api for cs2eth object
*
\**********************************************************************/

#ifndef CS2ETH_H
#define CS2ETH_H

/*--- #includes der Form <...> ---------------------------------------*/
#include <arpa/inet.h>
#include <boolean.h>

/*--- Datentypen (typedef) -------------------------------------------*/
typedef struct {
   BOOL Verbosity;
#ifdef TRACE
   BOOL Trace;
#endif
   BOOL SendUdpBc;
   char *ServerIp;                /* Server addr to listen */
   char *BcIp;                    /* broadcast addr for UDP */
   int ServerPort;                /* Server port to listen */
   int ClientSock;                /* our socket to server */
   int OutsideUdpSock;            /* udp socket to outside to listen */
   int OutsideBcSock;             /* udp broadcast socket to outside */
   int OutsideTcpSock;            /* tcp socket to outside to listen */
   struct sockaddr_in ClientAddr; /* addr of client from outside */
   int AppSock;                   /* tcp socket to app */
   struct sockaddr_in AppAddr;    /* addr of app from outside */
} Cs2ethStruct;

/*--- #define-Konstanten und Makros ----------------------------------*/
#define Cs2ethSetVerbose(Data, Verbose)     (Data)->Verbosity=Verbose
#ifdef TRACE
#define Cs2ethSetTrace(Data, DoTrace)       (Data)->Trace=DoTrace
#endif
#define Cs2ethSetSendUdpBc(Data, SendBc)    (Data)->SendUdpBc=SendBc
#define Cs2ethSetServerIp(Data, IpAddr)     (Data)->ServerIp=IpAddr
#define Cs2ethSetBcIp(Data, IpAddr)         (Data)->BcIp=IpAddr
#define Cs2ethSetServerPort(Data, Port)     (Data)->ServerPort=Port
#define Cs2ethSetClientSock(Data, Sock)     (Data)->ClientSock=Sock
#define Cs2ethSetOutsideUdpSock(Data, Sock) (Data)->OutsideUdpSock=Sock
#define Cs2ethSetOutsideBcSock(Data, Sock)  (Data)->OutsideBcSock=Sock
#define Cs2ethSetOutsideTcpSock(Data, Sock) (Data)->OutsideTcpSock=Sock
#define Cs2ethSetAppSock(Data, Sock)        (Data)->AppSock=Sock

#define Cs2ethGetVerbose(Data)       (Data)->Verbosity
#ifdef TRACE
#define Cs2ethGetTrace(Data)         (Data)->Trace
#endif
#define Cs2ethGetSendUdpBc(Data)      (Data)->SendUdpBc
#define Cs2ethGetServerIp(Data)       (Data)->ServerIp
#define Cs2ethGetBcIp(Data)           (Data)->BcIp
#define Cs2ethGetServerPort(Data)     (Data)->ServerPort
#define Cs2ethGetClientSock(Data)     (Data)->ClientSock
#define Cs2ethGetOutsideUdpSock(Data) (Data)->OutsideUdpSock
#define Cs2ethGetOutsideBcSock(Data)  (Data)->OutsideBcSock
#define Cs2ethGetOutsideTcpSock(Data) (Data)->OutsideTcpSock
#define Cs2ethGetClientAddr(Data)     (Data)->ClientAddr
#define Cs2ethGetAppSock(Data)        (Data)->AppSock
#define Cs2ethGetAppAddr(Data)        (Data)->AppAddr

/*--- Prototypen globaler Funktionen ---------------------------------*/
Cs2ethStruct *Cs2ethCreate(void);
void Cs2ethDestroy(Cs2ethStruct *Data);
void Cs2ethInit(Cs2ethStruct *Data, BOOL Verbose, char *IpAddr, int Port,
                BOOL SendBc, char *BcAddr
#ifdef TRACE
                , BOOL Trace
#endif
               );
void Cs2ethExit(Cs2ethStruct *Data);
void Cs2ethRun(Cs2ethStruct *Data);

#endif /* CS2ETH_H */
