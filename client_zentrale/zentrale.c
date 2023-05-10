/**********************************************************************\
* Dateiname: zentrale.c
* Autor    : Michael Bernstein
* Projekt  : client_zentrale
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: functionality for zentrale object which is
*                   reponsible for management tasks
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <mr_ipc.h>
#include <parser.h>
#include <write_cs2.h>
#include <fsm.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "zentrale.h"

/*--- #define-Konstanten und Makros ----------------------------------*/
#define SELECT_TIMEOUT  100
#define TIMER_INTERVALL  10

#define LOK_FILE "lokomotive.cs2"

#define PARAGRAPH_LOK     1
#define PARAGRAPH_NUMLOKS 2

#define NUM_STATES 6
#define STATE_WAIT_FOR_MS2          0
#define STATE_WAIT_LOKNAME_CFG_HDR  1
#define STATE_WAIT_LOKNAME_CFG_DATA 2
#define STATE_WAIT_LOKINFO_CFG_HDR  3
#define STATE_WAIT_LOKINFO_CFG_DATA 4
#define STATE_NORMAL                5
#define NUM_SIGNALS 13

/*--- Modullokale Variablen ------------------------------------------*/
static BOOL Loop = TRUE;

/*--- Funktionsdefinitionen ------------------------------------------*/

/**********************************************************************\
* Funktionsname: WriteLokOfLokomotiveCs2
*
* Kurzbeschreibung: this function writes one lok into lokomotive.cs2
*
* Parameter:
* Data : pointer to zentrale object which contains all needed information
*
\**********************************************************************/
static void WriteLokOfLokomotiveCs2(ZentraleStruct *Data, FILE *LokCs2Stream, LokInfo *Lok)
{  int i;

   if (ZentraleGetVerbose(Data))
      printf("Lok >%s< addr %d\n",
             ZentraleLokGetName(Lok), ZentraleLokGetAdresse(Lok));
   Cs2WriteTitleByName(LokCs2Stream, "lokomotive", 0);
   Cs2WriteULongValueByName(LokCs2Stream, "uid", ZentraleLokGetUid(Lok), 1);
   Cs2WriteStringValueByName(LokCs2Stream, "name", ZentraleLokGetName(Lok), 1);
   Cs2WriteHexValueByName(LokCs2Stream, "adresse", ZentraleLokGetAdresse(Lok), 1);
   Cs2WriteStringValueByName(LokCs2Stream, "typ", ZentraleLokGetTyp(Lok), 1);
   Cs2WriteHexValueByName(LokCs2Stream, "sid", 1, 1);
   Cs2WriteULongValueByName(LokCs2Stream, "mfxuid", ZentraleLokGetMfxUid(Lok), 1);
   Cs2WriteStringValueByName(LokCs2Stream, "icon", ZentraleLokGetName(Lok), 1);
   Cs2WriteIntValueByName(LokCs2Stream, "symbol", ZentraleLokGetSymbol(Lok), 1);
   Cs2WriteIntValueByName(LokCs2Stream, "av", ZentraleLokGetAv(Lok), 1);
   Cs2WriteIntValueByName(LokCs2Stream, "bv", ZentraleLokGetBv(Lok), 1);
   Cs2WriteIntValueByName(LokCs2Stream, "volume", ZentraleLokGetVolume(Lok), 1);
   Cs2WriteIntValueByName(LokCs2Stream, "velocity", ZentraleLokGetVelocity(Lok), 1);
   Cs2WriteIntValueByName(LokCs2Stream, "richtung", ZentraleLokGetRichtung(Lok), 1);
   Cs2WriteIntValueByName(LokCs2Stream, "tachomax", 320, 1);
   Cs2WriteIntValueByName(LokCs2Stream, "vmax", ZentraleLokGetVmax(Lok), 1);
   Cs2WriteIntValueByName(LokCs2Stream, "vmin", ZentraleLokGetVmin(Lok), 1);
   Cs2WriteIntValueByName(LokCs2Stream, "xprotokoll", 0, 1);
   Cs2WriteIntValueByName(LokCs2Stream, "mfxtyp", 0, 1);
   Cs2WriteHexValueByName(LokCs2Stream, "stand", 0, 1);
   Cs2WriteHexValueByName(LokCs2Stream, "fahrt", 0, 1);
   for (i = 0; i < NUM_FUNCTIONS; i++)
   {
      Cs2WriteTitleByName(LokCs2Stream, "funktionen", 1);
      Cs2WriteIntValueByName(LokCs2Stream, "nr", i, 2);
      Cs2WriteIntValueByName(LokCs2Stream, "typ", ZentraleLokGetFktTyp(Lok, i), 2);
      Cs2WriteIntValueByName(LokCs2Stream, "dauer", ZentraleLokGetFktDauer(Lok, i), 2);
      Cs2WriteIntValueByName(LokCs2Stream, "wert", ZentraleLokGetFktWert(Lok, i), 2);
      Cs2WriteHexValueByName(LokCs2Stream, "vorwaerts", 0, 2);
      Cs2WriteHexValueByName(LokCs2Stream, "rueckwaerts", 0, 2);
   }
   Cs2WriteHexLongValueByName(LokCs2Stream, "inTraktion", 0xffffffff, 1);
}

/**********************************************************************\
* Funktionsname: WriteLokomotiveCs2
*
* Kurzbeschreibung: this function writes lokomotive.cs2
*
* Parameter:
* Data : pointer to zentrale object which contains all needed information
*
\**********************************************************************/
static void WriteLokomotiveCs2(ZentraleStruct *Data)
{  int i;
   FILE *LokCs2Stream;
   char *LokFile;

   if (ZentraleGetLocPath(Data) != (char *)NULL)
   {
      LokFile = (char *)malloc(strlen(ZentraleGetLocPath(Data)) + 
                               strlen(LOK_FILE) + 2);
      if (LokFile != (char *)NULL)
      {
         strcpy(LokFile, ZentraleGetLocPath(Data));
         if (LokFile[strlen(LokFile) - 1] != '/')
            strcat(LokFile, "/");
         strcat(LokFile, LOK_FILE);
         if (ZentraleGetVerbose(Data))
            printf("%d Loks\n", ZentraleGetNumLoks(Data));
         LokCs2Stream = Cs2OpenByName(LokFile);
         if (LokCs2Stream != NULL)
         {
            Cs2WriteParagraphByType(LokCs2Stream, CS2_PARAGRAPH_TYPE_LOKOMOTIVE);
            Cs2WriteTitleByName(LokCs2Stream, "version", 0);
            Cs2WriteIntValueByName(LokCs2Stream, "major", 0, 1);
            Cs2WriteIntValueByName(LokCs2Stream, "minor", 1, 1);
            Cs2WriteTitleByName(LokCs2Stream, "session", 0);
            Cs2WriteIntValueByName(LokCs2Stream, "id", 1, 1);
            for (i = 0; i < ZentraleGetNumLoks(Data); i++)
            {
               WriteLokOfLokomotiveCs2(Data, LokCs2Stream, ZentraleGetLoksNr(Data, i));
            }
            Cs2Close(LokCs2Stream);
         }
         free(LokFile);
      }
      else
      {
         if (ZentraleGetVerbose(Data))
            puts("can not allocate memor for lokfile name");
      }
   }
}

/**********************************************************************\
* Funktionsname: HandleMember
*
* Kurzbeschreibung: this function evaluates the member system message
*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleMember(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Uid;
   unsigned Version, Type;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetMember(CmdFrame, &Uid, &Version, &Type);
   if (ZentraleGetVerbose(Data))
      printf("CAN member %lu, version %d, type %d\n", Uid, Version, Type);
   if (Type >= 0x30)
   {
      /* MS2, query loknamen */
      ZentraleSetNumLoks(Data, 1);
      ZentraleSetLoks(Data, malloc(ZentraleGetNumLoks(Data) * sizeof(LokInfo)));
      ZentraleSetActualIndex(Data, 0);
      MrIpcInit(&Cmd);
      MrIpcCmdSetReqestLocname(&Cmd, ZentraleGetActualIndex(Data), ZentraleGetActualIndex(Data) + 1);
      MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
      MrIpcExit(&Cmd);
      return(STATE_WAIT_LOKNAME_CFG_HDR);
   }
   else
      return(STATE_NO_CHANGE);
}

/**********************************************************************\
* Funktionsname: Ping
*
* Kurzbeschreibung: this function send a ping
*
* Parameter:
* Data : pointer to zentrale object which contains all needed information
*
\**********************************************************************/
static void QuerryMembers(ZentraleStruct *Data)
{  MrIpcCmdType Cmd;

   MrIpcInit(&Cmd);
   MrIpcCmdSetRequestMember(&Cmd);
   MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
   MrIpcExit(&Cmd);
}

/**********************************************************************\
* Funktionsname: HandleWaitMs2Timer
*
* Kurzbeschreibung: this function pings for a MS2*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleWaitMs2Timer(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   QuerryMembers(Data);
   return(STATE_NO_CHANGE);
}

/**********************************************************************\
* Funktionsname: HandleLoknameCfgHeader
*
* Kurzbeschreibung: this function evaluates the header of lokname cfg stream
*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleLoknameCfgHeader(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Length;
   int Crc;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgHeader(CmdFrame, &Length, &Crc);
   if (ZentraleGetVerbose(Data))
      printf("LOKNAME CFG data length %lu, crc %d\n", Length, Crc);
   ZentraleSetCfgLength(Data, Length);
   ZentraleSetCfgHaveRead(Data, 0);
   ZentraleSetCfgBuffer(Data, malloc(ZentraleGetCfgLength(Data)));
   if (ZentraleGetCfgBuffer(Data) == NULL)
      return(STATE_NORMAL);
   else
      return(STATE_WAIT_LOKNAME_CFG_DATA);
}

/**********************************************************************\
* Funktionsname: HandleLoknameCfgData
*
* Kurzbeschreibung: this function receives the data of lokname cfg stream
*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleLoknameCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   char Buf [8];
   ParserStruct *LokParser;
   int LineInfo, NumLoks, Paragraph;
   MrIpcCmdType Cmd;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   memcpy(ZentraleGetCfgBuffer(Data) + ZentraleGetCfgHaveRead(Data), Buf, 8);
   ZentraleSetCfgHaveRead(Data, ZentraleGetCfgHaveRead(Data) + 8);
   if (ZentraleGetCfgHaveRead(Data) >= ZentraleGetCfgLength(Data))
   {
      if (ZentraleGetVerbose(Data))
         printf("avaluate lokname cfg\n%s", ZentraleGetCfgBuffer(Data));
      ZentraleGetCfgBuffer(Data)[ZentraleGetCfgLength(Data)] = '\0';
      LokParser = ParserCreate();
      ParserInit(LokParser, PARSER_TYPE_LOKNAMEN, ZentraleGetCfgBuffer(Data));
      ParserSetVerbose(LokParser, FALSE);
      do {
         LineInfo = ParserParse(LokParser);
         switch (LineInfo)
         {
            case PARSER_ERROR:
               if (ZentraleGetVerbose(Data))
                  puts("ERROR in lok cfg");
               break;
            case PARSER_EOF:
               if (ZentraleGetVerbose(Data))
                  puts("end of lok cfg");
               break;
            case PARSER_PARAGRAPH:
               if (ZentraleGetVerbose(Data))
                  printf("new paragraph %s in lok cfg\n", ParserGetName(LokParser));
               switch (ParserGetSubType(LokParser))
               {
                  case PARSER_PARAGRAPH_LOK:
                     if (ZentraleGetVerbose(Data))
                        puts("lok paragraph in lok cfg");
                     Paragraph = PARAGRAPH_LOK;
                     break;
                  case PARSER_PARAGRAPH_NUMLOKS:
                     if (ZentraleGetVerbose(Data))
                        puts("numloks paragraph in lok cfg");
                     Paragraph = PARAGRAPH_NUMLOKS;
                     break;
               }
               break;
            case PARSER_VALUE:
               if (ZentraleGetVerbose(Data))
                  printf("new value %s=%s in lok cfg\n",
                         ParserGetName(LokParser), ParserGetValue(LokParser));
               switch (ParserGetSubType(LokParser))
               {
                  case PARSER_VALUE_NAME:
                     if (ZentraleGetVerbose(Data))
                        printf("lok name %d in lok cfg\n", ZentraleGetActualIndex(Data));
                     if (Paragraph == PARAGRAPH_LOK &&
                         ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
                     {
                        ZentraleLokSetName(ZentraleGetLoksNr(Data, ZentraleGetActualIndex(Data)),
                                           ParserGetValue(LokParser));
                        ZentraleSetActualIndex(Data, ZentraleGetActualIndex(Data) + 1);
                     }
                     break;
                  case PARSER_VALUE_WERT:
                     if (ZentraleGetVerbose(Data))
                        puts("number of loks in lok cfg");
                     if (Paragraph == PARAGRAPH_NUMLOKS)
                     {
                        NumLoks = atoi(ParserGetValue(LokParser));
                        if (ZentraleGetVerbose(Data))
                           printf("number of loks in lok cfg is %d\n", NumLoks);
                        if (ZentraleGetNumLoks(Data) < NumLoks)
                        {
                           ZentraleSetNumLoks(Data, NumLoks);
                           if (ZentraleGetVerbose(Data))
                              printf("new number of loks in lok cfg is %d\n", ZentraleGetNumLoks(Data));
                           ZentraleSetLoks(Data,
                                           realloc(ZentraleGetLoks(Data),
                                                   ZentraleGetNumLoks(Data) * sizeof(LokInfo)));
                        }
                     }
                     break;
               }
               break;
         }
      } while (LineInfo != PARSER_EOF);
      ParserDestroy(LokParser);
      free(ZentraleGetCfgBuffer(Data));
      ZentraleSetCfgLength(Data, 0);
      ZentraleSetCfgHaveRead(Data, 0);
      ZentraleSetCfgBuffer(Data, NULL);
      if (ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
      {
         if (ZentraleGetActualIndex(Data) == 2)
            ZentraleSetActualIndex(Data, 1);
         if (ZentraleGetVerbose(Data))
            printf("request lokname %d\n", ZentraleGetActualIndex(Data));
         MrIpcInit(&Cmd);
         MrIpcCmdSetReqestLocname(&Cmd, ZentraleGetActualIndex(Data), ZentraleGetActualIndex(Data) + 1);
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         MrIpcExit(&Cmd);
         return(STATE_WAIT_LOKNAME_CFG_HDR);
      }
      else
      {
         ZentraleSetActualIndex(Data, 0);
         if (ZentraleGetVerbose(Data))
            printf("request lokinfo %d >%s<\n",
                   ZentraleGetActualIndex(Data),
                   ZentraleLokGetName(ZentraleGetLoksNr(Data,
                                      ZentraleGetActualIndex(Data))));
         MrIpcInit(&Cmd);
         MrIpcCmdSetReqestLocinfo(&Cmd,
                                  ZentraleLokGetName(ZentraleGetLoksNr(Data,
                                                     ZentraleGetActualIndex(Data))));
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         MrIpcExit(&Cmd);
         return(STATE_WAIT_LOKINFO_CFG_HDR);
      }
   }
   else
      return(STATE_WAIT_LOKNAME_CFG_DATA);
}

/**********************************************************************\
* Funktionsname: HandleLokinfoCfgHeader
*
* Kurzbeschreibung: this function evaluates the header of lokinfo cfg stream
*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleLokinfoCfgHeader(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Length;
   int Crc;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgHeader(CmdFrame, &Length, &Crc);
   if (ZentraleGetVerbose(Data))
      printf("LOKINFO CFG data length %lu, crc %d\n", Length, Crc);
   ZentraleSetCfgLength(Data, Length);
   ZentraleSetCfgHaveRead(Data, 0);
   ZentraleSetCfgBuffer(Data, malloc(ZentraleGetCfgLength(Data)));
   if (ZentraleGetCfgBuffer(Data) == NULL)
      return(STATE_NORMAL);
   else
      return(STATE_WAIT_LOKINFO_CFG_DATA);
}

/**********************************************************************\
* Funktionsname: HandleLokinfoCfgData
*
* Kurzbeschreibung: this function receives the data of lokinfo cfg stream
*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleLokinfoCfgData(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   char Buf[17];
   ParserStruct *LokParser;
   int LineInfo, FktIndex;
   MrIpcCmdType Cmd;
   LokInfo *ActualLok;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetCfgData(CmdFrame, Buf);
   memcpy(ZentraleGetCfgBuffer(Data) + ZentraleGetCfgHaveRead(Data), Buf, 8);
   ZentraleSetCfgHaveRead(Data, ZentraleGetCfgHaveRead(Data) + 8);
   if (ZentraleGetVerbose(Data))
      printf("LOKINFO CFG data, aktuell %lu\n", ZentraleGetCfgHaveRead(Data));
   if (ZentraleGetCfgHaveRead(Data) >= ZentraleGetCfgLength(Data))
   {
      ActualLok = ZentraleGetLoksNr(Data, ZentraleGetActualIndex(Data));
      memset(ActualLok, 0, sizeof(LokInfo));
      FktIndex = -1;
      ZentraleGetCfgBuffer(Data)[ZentraleGetCfgLength(Data)] = '\0';
      LokParser = ParserCreate();
      ParserInit(LokParser, PARSER_TYPE_LOKINFO, ZentraleGetCfgBuffer(Data));
      ParserSetVerbose(LokParser, FALSE);
      do {
         LineInfo = ParserParse(LokParser);
         switch (LineInfo)
         {
            case PARSER_ERROR:
               if (ZentraleGetVerbose(Data))
                  puts("ERROR in lok info cfg");
               break;
            case PARSER_EOF:
               if (ZentraleGetVerbose(Data))
                  puts("end of lok info cfg");
               break;
            case PARSER_PARAGRAPH:
               if (ZentraleGetVerbose(Data))
                  printf("new paragraph %s in lok info cfg\n",
                         ParserGetName(LokParser));
               switch (ParserGetSubType(LokParser))
               {
                  case PARSER_PARAGRAPH_LOKOMOTIVE:
                     if (ZentraleGetVerbose(Data))
                        puts("lokomotive paragraph in lok cfg");
                     break;
               }
               break;
            case PARSER_VALUE:
               if (ZentraleGetVerbose(Data))
                  printf("new value %s=%s in lok cfg\n",
                         ParserGetName(LokParser), ParserGetValue(LokParser));
               switch (ParserGetSubType(LokParser))
               {
                  case PARSER_VALUE_LOK:
                     if (ZentraleGetVerbose(Data))
                        puts("neuer lok Eintrag");
                     break;
                  case PARSER_VALUE_UID:
                     if (ZentraleGetVerbose(Data))
                        puts("lok uid");
                     ZentraleLokSetUid(ActualLok,
                                       strtoul(ParserGetValue(LokParser),
                                               NULL, 0));
                     break;
                  case PARSER_VALUE_NAME:
                     if (ZentraleGetVerbose(Data))
                        puts("lok name");
                     ZentraleLokSetName(ActualLok, ParserGetValue(LokParser));
                     break;
                  case PARSER_VALUE_ADRESSE:
                     if (ZentraleGetVerbose(Data))
                        puts("lok adresse");
                     ZentraleLokSetAdresse(ActualLok,
                                           strtoul(ParserGetValue(LokParser),
                                                   NULL, 0));
                     break;
                  case PARSER_VALUE_TYP:
                     if (ParserGetLevel(LokParser) == 1)
                     {
                        if (ZentraleGetVerbose(Data))
                           puts("lok typ");
                        ZentraleLokSetTyp(ActualLok, ParserGetValue(LokParser));
                     }
                     else if (ParserGetLevel(LokParser) == 2)
                     {
                        if (ZentraleGetVerbose(Data))
                           puts("lok function typ");
                        ZentraleLokSetFktTyp(ActualLok, FktIndex,
                                             strtoul(ParserGetValue(LokParser),
                                                     NULL, 0));
                     }
                     break;
                  case PARSER_VALUE_MFXUID:
                     if (ZentraleGetVerbose(Data))
                        puts("lok mfxuid");
                     ZentraleLokSetMfxUid(ActualLok,
                                          strtoul(ParserGetValue(LokParser),
                                                  NULL, 0));
                     break;
                  case PARSER_VALUE_SYMBOL:
                     if (ZentraleGetVerbose(Data))
                        puts("lok symbol");
                     ZentraleLokSetSymbol(ActualLok,
                                          strtoul(ParserGetValue(LokParser),
                                                  NULL, 0));
                     break;
                  case PARSER_VALUE_AV:
                     if (ZentraleGetVerbose(Data))
                        puts("lok av");
                     ZentraleLokSetAv(ActualLok,
                                      strtoul(ParserGetValue(LokParser),
                                              NULL, 0));
                     break;
                  case PARSER_VALUE_BV:
                     if (ZentraleGetVerbose(Data))
                        puts("lok bv");
                     ZentraleLokSetBv(ActualLok,
                                      strtoul(ParserGetValue(LokParser),
                                              NULL, 0));
                     break;
                  case PARSER_VALUE_VOLUME:
                     if (ZentraleGetVerbose(Data))
                        puts("lok volume");
                     ZentraleLokSetVolume(ActualLok,
                                          strtoul(ParserGetValue(LokParser),
                                                  NULL, 0));
                     break;
                  case PARSER_VALUE_VELOCITY:
                     if (ZentraleGetVerbose(Data))
                        puts("lok velocity");
                     ZentraleLokSetVelocity(ActualLok,
                                            strtoul(ParserGetValue(LokParser),
                                                    NULL, 0));
                     break;
                  case PARSER_VALUE_RICHTUNG:
                     if (ZentraleGetVerbose(Data))
                        puts("lok richtung");
                     ZentraleLokSetRichtung(ActualLok,
                                            strtoul(ParserGetValue(LokParser),
                                                    NULL, 0));
                     break;
                  case PARSER_VALUE_VMAX:
                     if (ZentraleGetVerbose(Data))
                        puts("lok vmax");
                     ZentraleLokSetVmax(ActualLok,
                                        strtoul(ParserGetValue(LokParser),
                                                NULL, 0));
                     break;
                  case PARSER_VALUE_VMIN:
                     if (ZentraleGetVerbose(Data))
                        puts("lok vmin");
                     ZentraleLokSetVmin(ActualLok,
                                        strtoul(ParserGetValue(LokParser),
                                                NULL, 0));
                     break;
                  case PARSER_VALUE_FKT:
                     if (ZentraleGetVerbose(Data))
                        puts("lok fkt");
                     FktIndex++;
                     break;
                  case PARSER_VALUE_DAUER:
                     if (ZentraleGetVerbose(Data))
                        puts("lok function dauer");
                     ZentraleLokSetFktDauer(ActualLok, FktIndex,
                                            strtoul(ParserGetValue(LokParser),
                                                    NULL, 0));
                     break;
                  case PARSER_VALUE_WERT:
                     if (ZentraleGetVerbose(Data))
                        puts("lok function wert");
                     ZentraleLokSetFktWert(ActualLok, FktIndex,
                                           strtoul(ParserGetValue(LokParser),
                                                   NULL, 0));
                     break;
               }
               break;
         }
      } while (LineInfo != PARSER_EOF);
      ParserDestroy(LokParser);
      free(ZentraleGetCfgBuffer(Data));
      ZentraleSetCfgLength(Data, 0);
      ZentraleSetCfgHaveRead(Data, 0);
      ZentraleSetCfgBuffer(Data, NULL);
      ZentraleSetActualIndex(Data, ZentraleGetActualIndex(Data) + 1);
      if (ZentraleGetActualIndex(Data) < ZentraleGetNumLoks(Data))
      {
         if (ZentraleGetVerbose(Data))
            printf("request lokinfo %d >%s<\n",
                   ZentraleGetActualIndex(Data),
                   ZentraleLokGetName(ZentraleGetLoksNr(Data,
                                      ZentraleGetActualIndex(Data))));
         MrIpcInit(&Cmd);
         MrIpcCmdSetReqestLocinfo(&Cmd,
                                  ZentraleLokGetName(ZentraleGetLoksNr(Data,
                                                     ZentraleGetActualIndex(Data))));
         MrIpcSend(ZentraleGetClientSock(Data), &Cmd);
         MrIpcExit(&Cmd);
         return(STATE_WAIT_LOKINFO_CFG_HDR);
      }
      else
      {
         WriteLokomotiveCs2(Data);
         return(STATE_NORMAL);
      }
   }
   else
      return(STATE_WAIT_LOKINFO_CFG_DATA);
}

/**********************************************************************\
* Funktionsname: FindLokEntry
*
* Kurzbeschreibung: find lok info for given addr
*
* Parameter:
* Data : pointer to zentrale object which contains all needed information
* Addr : addr of lokinfo
*
\**********************************************************************/
static LokInfo *FindLokEntry(ZentraleStruct *Data, unsigned long Addr)
{  int i;

   i = 0;
   while ((i < ZentraleGetNumLoks(Data)) &&
          (ZentraleLokGetUid(ZentraleGetLoksNr(Data, i)) != Addr))
      i++;
   if (i < ZentraleGetNumLoks(Data))
      return(ZentraleGetLoksNr(Data, i));
   else
      return((LokInfo *)NULL);
}

/**********************************************************************\
* Funktionsname: HandleLokSpeed
*
* Kurzbeschreibung: this function evaluates the lok speed
*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleLokSpeed(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Addr;
   unsigned Speed;
   LokInfo *LokData;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetLocomotiveSpeed(CmdFrame, &Addr, &Speed);
   if (ZentraleGetVerbose(Data))
      printf("LOK Speed addr %lu, speed %d\n", Addr, Speed);
   LokData = FindLokEntry(Data, Addr);
   if (LokData != (LokInfo *)NULL)
      ZentraleLokSetVelocity(LokData, Speed);
   return(STATE_NO_CHANGE);
}

/**********************************************************************\
* Funktionsname: HandleLokDirection
*
* Kurzbeschreibung: this function evaluates the lok direction
*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleLokDirection(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Addr;
   DirectionType Direction;
   LokInfo *LokData;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetLocomotiveDir(CmdFrame, &Addr, &Direction);
   if (ZentraleGetVerbose(Data))
      printf("LOK Direction addr %lu, speed %d\n", Addr, (int)Direction);
   LokData = FindLokEntry(Data, Addr);
   if (LokData != (LokInfo *)NULL)
      ZentraleLokSetRichtung(LokData, (int)Direction);
   return(STATE_NO_CHANGE);
}

/**********************************************************************\
* Funktionsname: HandleLokFunction
*
* Kurzbeschreibung: this function evaluates the lok function
*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleLokFunction(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;
   unsigned long Addr;
   unsigned Function;
   SwitchType Switch;
   LokInfo *LokData;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   MrIpcCmdGetLocomotiveFkt(CmdFrame, &Addr, &Function, &Switch);
   if (ZentraleGetVerbose(Data))
      printf("LOK Function addr %lu, function %d, value %d\n",
             Addr, Function, (int)Switch);
   LokData = FindLokEntry(Data, Addr);
   if (LokData != (LokInfo *)NULL)
      ZentraleLokSetFktWert(LokData, Function, (int)Switch);
   return(STATE_NO_CHANGE);
}

/**********************************************************************\
* Funktionsname: HandleOther
*
* Kurzbeschreibung: this function receives all other datas
*
* Parameter:
* Priv : pointer to zentrale object which contains all needed information
* SignalData : one command frame from the system
*
\**********************************************************************/
static int HandleOther(void *Priv, void *SignalData)
{  ZentraleStruct *Data;
   MrIpcCmdType *CmdFrame;

   Data = (ZentraleStruct *)Priv;
   CmdFrame = (MrIpcCmdType *)SignalData;
   return(STATE_NO_CHANGE);
}

static StateFktType StateWaitForMs2[NUM_SIGNALS] = {
   /* STATE_WAIT_FOR_MS2 */
   HandleOther,  /* timer */
   HandleOther,  /* MrIpcCmdNull */
   HandleOther,  /* MrIpcCmdRun */
   HandleOther,  /* MrIpcCmdLocomotiveSpeed */
   HandleOther,  /* MrIpcCmdLocomotiveDirection */
   HandleOther,  /* MrIpcCmdLocomotiveFunction */
   HandleOther,  /* MrIpcCmdAccSwitch */
   HandleOther,  /* MrIpcCmdRequestMember */
   HandleMember, /* MrIpcCmdMember */
   HandleOther,  /* MrIpcCmdRequestLocName */
   HandleOther,  /* MrIpcCmdRequestLocInfo */
   HandleOther,  /* MrIpcCmdCfgHeader */
   HandleOther   /* MrIpcCmdCfgData */
};
static StateFktType StateWaitLoknameCfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKNAME_CFG_HDR */
   HandleOther,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcCmdLocomotiveSpeed */
   HandleOther,            /* MrIpcCmdLocomotiveDirection */
   HandleOther,            /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandleOther,            /* MrIpcCmdRequestMember */
   HandleOther,            /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleLoknameCfgHeader, /* MrIpcCmdCfgHeader */
   HandleOther             /* MrIpcCmdCfgData */
};
static StateFktType StateWaitLoknameCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKNAME_CFG_DATA */
   HandleOther,         /* timer */
   HandleOther,         /* MrIpcCmdNull */
   HandleOther,         /* MrIpcCmdRun */
   HandleOther,         /* MrIpcCmdLocomotiveSpeed */
   HandleOther,         /* MrIpcCmdLocomotiveDirection */
   HandleOther,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,         /* MrIpcCmdAccSwitch */
   HandleOther,         /* MrIpcCmdRequestMember */
   HandleOther,         /* MrIpcCmdMember */
   HandleOther,         /* MrIpcCmdRequestLocName */
   HandleOther,         /* MrIpcCmdRequestLocInfo */
   HandleOther,         /* MrIpcCmdCfgHeader */
   HandleLoknameCfgData /* MrIpcCmdCfgData */
};
static StateFktType StateWaitLokinfoCfgHdr[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKINFO_CFG_HDR */
   HandleOther,            /* timer */
   HandleOther,            /* MrIpcCmdNull */
   HandleOther,            /* MrIpcCmdRun */
   HandleOther,            /* MrIpcCmdLocomotiveSpeed */
   HandleOther,            /* MrIpcCmdLocomotiveDirection */
   HandleOther,            /* MrIpcCmdLocomotiveFunction */
   HandleOther,            /* MrIpcCmdAccSwitch */
   HandleOther,            /* MrIpcCmdRequestMember */
   HandleOther,            /* MrIpcCmdMember */
   HandleOther,            /* MrIpcCmdRequestLocName */
   HandleOther,            /* MrIpcCmdRequestLocInfo */
   HandleLokinfoCfgHeader, /* MrIpcCmdCfgHeader */
   HandleOther             /* MrIpcCmdCfgData */
};
static StateFktType StateWaitLokinfoCfgData[NUM_SIGNALS] = {
   /* STATE_WAIT_LOKINFO_CFG_DATA */
   HandleOther,         /* timer */
   HandleOther,         /* MrIpcCmdNull */
   HandleOther,         /* MrIpcCmdRun */
   HandleOther,         /* MrIpcCmdLocomotiveSpeed */
   HandleOther,         /* MrIpcCmdLocomotiveDirection */
   HandleOther,         /* MrIpcCmdLocomotiveFunction */
   HandleOther,         /* MrIpcCmdAccSwitch */
   HandleOther,         /* MrIpcCmdRequestMember */
   HandleOther,         /* MrIpcCmdMember */
   HandleOther,         /* MrIpcCmdRequestLocName */
   HandleOther,         /* MrIpcCmdRequestLocInfo */
   HandleOther,         /* MrIpcCmdCfgHeader */
   HandleLokinfoCfgData /* MrIpcCmdCfgData */
};
static StateFktType StateNormal[NUM_SIGNALS] = {
   /* STATE_NORMAL */
   HandleOther,        /* timer */
   HandleOther,        /* MrIpcCmdNull */
   HandleOther,        /* MrIpcCmdRun */
   HandleLokSpeed,     /* MrIpcCmdLocomotiveSpeed */
   HandleLokDirection, /* MrIpcCmdLocomotiveDirection */
   HandleLokFunction,  /* MrIpcCmdLocomotiveFunction */
   HandleOther,        /* MrIpcCmdAccSwitch */
   HandleOther,        /* MrIpcCmdRequestMember */
   HandleOther,        /* MrIpcCmdMember */
   HandleOther,        /* MrIpcCmdRequestLocName */
   HandleOther,        /* MrIpcCmdRequestLocInfo */
   HandleOther,        /* MrIpcCmdCfgHeader */
   HandleOther         /* MrIpcCmdCfgData */
};
static SignalFunctionsType StateMachineFunctions[NUM_STATES] = {
   StateWaitForMs2,
   StateWaitLoknameCfgHdr,
   StateWaitLoknameCfgData,
   StateWaitLokinfoCfgHdr,
   StateWaitLokinfoCfgData,
   StateNormal
};

/**********************************************************************\
* Funktionsname: ZentraleCreate
*
* Kurzbeschreibung: this function creates a zentrale object and
*                   initializes all members to zero.
*
* Rueckgabewert:
* NULL : creation failed, error
* else : pointer to created zentrale object
*
\**********************************************************************/
ZentraleStruct *ZentraleCreate(void)
{  ZentraleStruct *Data;

   Data = (ZentraleStruct *)malloc(sizeof(ZentraleStruct));
   if (Data != (ZentraleStruct *)NULL)
   {
      ZentraleSetVerbose(Data, FALSE);
      ZentraleSetServerIp(Data, (char *)NULL);
      ZentraleSetServerPort(Data, -1);
      ZentraleSetClientSock(Data, -1);
      ZentraleSetNumLoks(Data, 0);
      ZentraleSetLoks(Data, NULL);
      ZentraleSetStateMachine(Data, FsmCreate());
      ZentraleSetLocPath(Data, (char *)NULL);
   }
   return(Data);
}

/**********************************************************************\
* Funktionsname: ZentraleDestroy
*
* Kurzbeschreibung: this function destroys a zentrale object, which was
*                   created by ZentraleCreate()
*
* Parameter:
* Data : pointer to zentrale object which should be destroyed
*
\**********************************************************************/
void ZentraleDestroy(ZentraleStruct *Data)
{
   if (ZentraleGetVerbose(Data))
      puts("destroy zentrale");
   if (ZentraleGetServerIp(Data) != (char *)NULL)
      free(ZentraleGetServerIp(Data));
   if (ZentraleGetLoks(Data) != NULL)
      free(ZentraleGetLoks(Data));
   if (ZentraleGetStateMachine(Data) != NULL)
      FsmDestroy(ZentraleGetStateMachine(Data));
   if (ZentraleGetLocPath(Data) != NULL)
      free(ZentraleGetLocPath(Data));
   free(Data);
}

/**********************************************************************\
* Funktionsname: ZentraleInit
*
* Kurzbeschreibung: this function sets the values for this object to
*                   the right values for proper work
*
* Parameter:
* Data    : pointer to zentrale object which contains all needed information
* Verbose : should this module be verbose and print messages
* IpAddr  : ip address of server drehscheibe
* Port    : port of server drehscheibe
*
\**********************************************************************/
void ZentraleInit(ZentraleStruct *Data, BOOL Verbose, char *IpAddr, int Port,
                  char *LocPath)
{
   ZentraleSetVerbose(Data, Verbose);
   ZentraleSetServerIp(Data, IpAddr);
   ZentraleSetServerPort(Data, Port);
   ZentraleSetClientSock(Data, -1);
   ZentraleSetCfgLength(Data, 0);
   ZentraleSetCfgHaveRead(Data, 0);
   ZentraleSetCfgBuffer(Data, NULL);
   FsmInit(ZentraleGetStateMachine(Data), (void *)Data, STATE_WAIT_FOR_MS2,
           NUM_SIGNALS, NUM_STATES, StateMachineFunctions);
   ZentraleSetLocPath(Data, LocPath);
}

/**********************************************************************\
* Funktionsname: ZentraleExit
*
* Kurzbeschreibung: this function is the cleanup counterpart for the
*                   init function
*
* Parameter:
* Data    : pointer to zentrale object which contains all needed information
*
\**********************************************************************/
void ZentraleExit(ZentraleStruct *Data)
{
   if (ZentraleGetVerbose(Data))
      puts("exit zentrale");
   if (ZentraleGetServerIp(Data) != (char *)NULL)
      free(ZentraleGetServerIp(Data));
   ZentraleSetServerIp(Data, (char *)NULL);
   if (ZentraleGetLoks(Data) != NULL)
      free(ZentraleGetLoks(Data));
   ZentraleSetLoks(Data, NULL);
   if (ZentraleGetLocPath(Data) != NULL)
      free(ZentraleGetLocPath(Data));
   ZentraleSetLocPath(Data, (char *)NULL);
}

/**********************************************************************\
* Funktionsname: SigHandler
*
* Kurzbeschreibung: this functions is called for a SIGINT (CTRL C) or
*                   SIGTERM and stops the main loop.
*
* Parameter:
* sig : signal which should processed
*
\**********************************************************************/
static void SigHandler(int sig)
{
   Loop = FALSE;
}

/**********************************************************************\
* Funktionsname: Start
*
* Kurzbeschreibung: this function starts all needed for the main loop,
*                   e.g. opens network connections, ...
*
* Parameter:
* Data : pointer to zentrale object which contains all needed information
*
* Rueckgabewert:
* TRUE  : all needed for main loop was successfull started
* FALSE : start failed
*
\**********************************************************************/
static BOOL Start(ZentraleStruct *Data)
{  struct sigaction SigStruct;

   ZentraleSetClientSock(Data,
                         MrIpcConnect(ZentraleGetServerIp(Data),
                                      ZentraleGetServerPort(Data)));
   if (ZentraleGetClientSock(Data) >= 0)
   {
      if (ZentraleGetVerbose(Data))
         puts("ready for incoming comands");
      SigStruct.sa_handler = SigHandler;
      sigemptyset(&SigStruct.sa_mask);
      SigStruct.sa_flags = 0;
      sigaction(SIGINT, &SigStruct, NULL);
      sigaction(SIGQUIT, &SigStruct, NULL);
      sigaction(SIGTERM, &SigStruct, NULL);
      return(TRUE);
   }
   else
   {
      return(FALSE);
   }
}

/**********************************************************************\
* Funktionsname: Stop
*
* Kurzbeschreibung: this function stops all what was startet for the
*                   main loop, e.g. closes network connections, ...
*
* Parameter:
* Data : pointer to zentrale object which contains all needed information
*
\**********************************************************************/
static void Stop(ZentraleStruct *Data)
{
   if (ZentraleGetVerbose(Data))
      puts("stop network client");
   if (ZentraleGetClientSock(Data) >= 0)
   {
      MrIpcClose(ZentraleGetClientSock(Data));
   }
}

/**********************************************************************\
* Funktionsname: ProcessSystemData
*
* Kurzbeschreibung: this function evaluates one command frame from the
*                   system (drehscheibe) and do the proper reaction
*
* Parameter:
* Data : pointer to zentrale object which contains all needed information
* CmdFrame : one command frame from the system
*
\**********************************************************************/
static void ProcessSystemData(ZentraleStruct *Data, MrIpcCmdType *CmdFrame)
{  int NewState;

   MrIpcDecode(CmdFrame);
   FsmDo(ZentraleGetStateMachine(Data), MrIpcCmdGetCmd(CmdFrame) + 1, (void *)CmdFrame);
}

/**********************************************************************\
* Funktionsname: HandleSystemData
*
* Kurzbeschreibung: this function reads all available command frames
*                   from the system and calls the process function for
*                   each of them
*
* Parameter:
* Data : pointer to ms1 object which contains all needed information
*
\**********************************************************************/
static void HandleSystemData(ZentraleStruct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (ZentraleGetVerbose(Data))
      puts("new data available");
   do {
      MrIpcInit(&CmdFrame);
      RcvReturnValue = MrIpcRecv(ZentraleGetClientSock(Data), &CmdFrame);
      if (RcvReturnValue == MR_IPC_RCV_ERROR)
      {
         /* error in recieve from socket */
         if (ZentraleGetVerbose(Data))
            puts("Error in recieve from socket!");
      }
      else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
      {
         /* socket was closed at remote side */
         if (ZentraleGetVerbose(Data))
            puts("client socket was closed\nmaybe server has stoped");
         Loop = FALSE;
      }
      else
      {
         /* we have received data, process them */
         if (ZentraleGetVerbose(Data))
            printf("read new comand frame from socket\n%s\n",
                   MrIpcGetRendered(&CmdFrame));
         ProcessSystemData(Data, &CmdFrame);
         MrIpcExit(&CmdFrame);
      }
   } while (RcvReturnValue > 0);
}

/**********************************************************************\
* Funktionsname: ZentraleRun
*
* Kurzbeschreibung: this functions contains the main loop
*
* Parameter:
* Data : pointer to zentrale object which contains all needed information
*
\**********************************************************************/
void ZentraleRun(ZentraleStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   time_t LastTime, Now;

   if (Start(Data))
   {
      QuerryMembers(Data);
      LastTime = time(NULL);
      while (Loop)
      {
         /* Main loop for receive and send data */

         /* fill fd set for call to select */
         FD_ZERO(&ReadFds);
         HighFd = 0;
         /* 1. client socket */
         if (ZentraleGetVerbose(Data))
            printf("add client socket %d\n", ZentraleGetClientSock(Data));
         FD_SET(ZentraleGetClientSock(Data), &ReadFds);
         if (ZentraleGetClientSock(Data) > HighFd)
            HighFd = ZentraleGetClientSock(Data);
         /* 2. Timeout */
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (ZentraleGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);

         /* wait for data */
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (ZentraleGetVerbose(Data))
            printf("select liefert %d\n", RetVal);

         /* check, who has data */
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            /* timeout, time for periodic tasks */
            Now = time(NULL);
            if (ZentraleGetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
            if ((Now - LastTime) > TIMER_INTERVALL)
            {
               FsmDo(ZentraleGetStateMachine(Data), 0, (void *)NULL);
               LastTime = Now;
            }
         }
         else if (RetVal < 0)
         {
            /* error, finish loop */
            if (ZentraleGetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            Now = time(NULL);
            if ((Now - LastTime) > TIMER_INTERVALL)
            {
               FsmDo(ZentraleGetStateMachine(Data), 0, (void *)NULL);
               LastTime = Now;
            }
            /* data, check at which fd */
            if (FD_ISSET(ZentraleGetClientSock(Data), &ReadFds))
            {
               /* new cmd frame */
               HandleSystemData(Data);
            }
         }
      }

      /* cleanup */
      Stop(Data);
   }
}
