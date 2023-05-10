/**********************************************************************\
* Dateiname: zentrale.h
* Autor    : Michael Bernstein
* Projekt  : client_zentrale
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: api for zentrale object
*
\**********************************************************************/

#ifndef ZENTRALE_H
#define ZENTRALE_H

/*--- #includes der Form <...> ---------------------------------------*/
#include <boolean.h>
#include <fsm.h>

#define MM2_PRG_TEXT "mm2_prg"

#define MM2_PRG_TYP 0x01

#define NUM_FUNCTIONS 16


/*--- Datentypen (typedef) -------------------------------------------*/
typedef struct {
   int Typ;
   int Dauer;
   int Wert;
} LokFktTyp;

#define ZentraleLokFktSetTyp(Data, Val)   (Data)->Typ=Val
#define ZentraleLokFktSetDauer(Data, Val) (Data)->Dauer=Val
#define ZentraleLokFktSetWert(Data, Val)  (Data)->Wert=Val

#define ZentraleLokFktGetTyp(Data, Val)   (Data)->Typ
#define ZentraleLokFktGetDauer(Data, Val) (Data)->Dauer
#define ZentraleLokFktGetWert(Data, Val)  (Data)->Wert

typedef struct {
   unsigned long Uid;
   char Name[17];
   int Adresse;
   char Typ[20];
   unsigned long MfxUid;
   int Symbol;
   int Av;
   int Bv;
   int Volume;
   int Velocity;
   int Richtung;
   int Vmax;
   int Vmin;
   LokFktTyp Fkt[NUM_FUNCTIONS];
} LokInfo;

#define ZentraleLokSetUid(Data, Val)      (Data)->Uid=Val
#define ZentraleLokSetName(Data, Val)     strcpy((Data)->Name,Val)
#define ZentraleLokSetAdresse(Data, Val)  (Data)->Adresse=Val
#define ZentraleLokSetTyp(Data, Val)      strcpy((Data)->Typ,Val)
#define ZentraleLokSetMfxUid(Data, Val)   (Data)->MfxUid=Val
#define ZentraleLokSetSymbol(Data, Val)   (Data)->Symbol=Val
#define ZentraleLokSetAv(Data, Val)       (Data)->Av=Val
#define ZentraleLokSetBv(Data, Val)       (Data)->Bv=Val
#define ZentraleLokSetVolume(Data, Val)   (Data)->Volume=Val
#define ZentraleLokSetVelocity(Data, Val) (Data)->Velocity=Val
#define ZentraleLokSetRichtung(Data, Val) (Data)->Richtung=Val
#define ZentraleLokSetVmax(Data, Val)     (Data)->Vmax=Val
#define ZentraleLokSetVmin(Data, Val)     (Data)->Vmin=Val

#define ZentraleLokSetFktTyp(Data, Idx, Val)   (Data)->Fkt[Idx].Typ=Val
#define ZentraleLokSetFktDauer(Data, Idx, Val) (Data)->Fkt[Idx].Dauer=Val
#define ZentraleLokSetFktWert(Data, Idx, Val)  (Data)->Fkt[Idx].Wert=Val

#define ZentraleLokGetUid(Data)      (Data)->Uid
#define ZentraleLokGetName(Data)     (Data)->Name
#define ZentraleLokGetAdresse(Data)  (Data)->Adresse
#define ZentraleLokGetTyp(Data)      (Data)->Typ
#define ZentraleLokGetMfxUid(Data)   (Data)->MfxUid
#define ZentraleLokGetSymbol(Data)   (Data)->Symbol
#define ZentraleLokGetAv(Data)       (Data)->Av
#define ZentraleLokGetBv(Data)       (Data)->Bv
#define ZentraleLokGetVolume(Data)   (Data)->Volume
#define ZentraleLokGetVelocity(Data) (Data)->Velocity
#define ZentraleLokGetRichtung(Data) (Data)->Richtung
#define ZentraleLokGetVmax(Data)     (Data)->Vmax
#define ZentraleLokGetVmin(Data)     (Data)->Vmin

#define ZentraleLokGetFktTyp(Data, Idx)   (Data)->Fkt[Idx].Typ
#define ZentraleLokGetFktDauer(Data, Idx) (Data)->Fkt[Idx].Dauer
#define ZentraleLokGetFktWert(Data, Idx)  (Data)->Fkt[Idx].Wert

typedef struct {
   BOOL Verbosity;
   char *ServerIp;  /* Server addr to listen */
   int ServerPort;  /* Server port to listen */
   int ClientSock;  /* our socket to server */
   FsmStruct *StateMachine;
   char *LocPath;   /* path where to write lokomotive.cs2 */
   /* data for receive cfg config data */
   BOOL HaveDb;
   unsigned long CfgLength;
   unsigned long CfgHaveRead;
   char *CfgBuffer;
   int ActualIndex;
   /* lok data */
   int NumLoks;
   LokInfo *Loks;
} ZentraleStruct;

/*--- #define-Konstanten und Makros ----------------------------------*/
#define ZentraleSetVerbose(Data, Verbose)  (Data)->Verbosity=Verbose
#define ZentraleSetServerIp(Data, IpAddr)  (Data)->ServerIp=IpAddr
#define ZentraleSetServerPort(Data, Port)  (Data)->ServerPort=Port
#define ZentraleSetClientSock(Data, Sock)  (Data)->ClientSock=Sock
#define ZentraleSetStateMachine(Data, Fsm) (Data)->StateMachine=Fsm
#define ZentraleSetLocPath(Data, Path)     (Data)->LocPath=Path
#define ZentraleSetCfgLength(Data, Len)    (Data)->CfgLength=Len
#define ZentraleSetCfgHaveRead(Data, Len)  (Data)->CfgHaveRead=Len
#define ZentraleSetCfgBuffer(Data, Buf)    (Data)->CfgBuffer=Buf
#define ZentraleSetActualIndex(Data, i)    (Data)->ActualIndex=i
#define ZentraleSetNumLoks(Data, Num)      (Data)->NumLoks=Num
#define ZentraleSetLoks(Data, Buf)         (Data)->Loks=Buf

#define ZentraleGetVerbose(Data)        (Data)->Verbosity
#define ZentraleGetServerIp(Data)       (Data)->ServerIp
#define ZentraleGetServerPort(Data)     (Data)->ServerPort
#define ZentraleGetClientSock(Data)     (Data)->ClientSock
#define ZentraleGetStateMachine(Data)   (Data)->StateMachine
#define ZentraleGetLocPath(Data)        (Data)->LocPath
#define ZentraleGetCfgLength(Data)      (Data)->CfgLength
#define ZentraleGetCfgHaveRead(Data)    (Data)->CfgHaveRead
#define ZentraleGetCfgBuffer(Data)      (Data)->CfgBuffer
#define ZentraleGetLoks(Data)           (Data)->Loks
#define ZentraleGetActualIndex(Data)    (Data)->ActualIndex
#define ZentraleGetNumLoks(Data)        (Data)->NumLoks
#define ZentraleGetLoksNr(Data, i)      &((Data)->Loks[i])

/*--- Prototypen globaler Funktionen ---------------------------------*/
ZentraleStruct *ZentraleCreate(void);
void ZentraleDestroy(ZentraleStruct *Data);
void ZentraleInit(ZentraleStruct *Data, BOOL Verbose, char *IpAddr, int Port,
                  char *LocPath);
void ZentraleExit(ZentraleStruct *Data);
void ZentraleRun(ZentraleStruct *Data);

#endif /* ZENTRALE_H */
