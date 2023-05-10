/**********************************************************************\
* Dateiname: ms2.c
* Autor    : Michael Bernstein
* Projekt  : client_ms2
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: functionality for ms2 object which handles the
*                   message flow betwween ms2 and drehscheibe
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
#include <linux/can.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <mr_ipc.h>
#include <mr_can.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "ms2.h"

/*--- #define-Konstanten und Makros ----------------------------------*/
#define SELECT_TIMEOUT 100

/*--- Modullokale Variablen ------------------------------------------*/
static BOOL Loop = TRUE;

/*--- Funktionsdefinitionen ------------------------------------------*/

/**********************************************************************\
* Funktionsname: Ms2Create
*
* Kurzbeschreibung: this function creates a ms2 object and initializes
*                   all members to zero.
*
* Rueckgabewert:
* NULL : creation failed, error
* else : pointer to created ms2 object
*
\**********************************************************************/
Ms2Struct *Ms2Create(void)
{  Ms2Struct *Data;

   Data = (Ms2Struct *)malloc(sizeof(Ms2Struct));
   if (Data != (Ms2Struct *)NULL)
   {
      Ms2SetVerbose(Data, FALSE);
      Ms2SetServerIp(Data, (char *)NULL);
      Ms2SetServerPort(Data, -1);
      Ms2SetClientSock(Data, -1);
      Ms2SetCanName(Data, strdup("can0"));
      Ms2SetCanSock(Data, -1);
   }
   return(Data);
}

/**********************************************************************\
* Funktionsname: Ms2Destroy
*
* Kurzbeschreibung: this function destroys a ms2 object, which was
*                   created by Ms2Create()
*
* Parameter:
* Data : pointer to ms2 object which should be destroyed
*
\**********************************************************************/
void Ms2Destroy(Ms2Struct *Data)
{
   if (Ms2GetVerbose(Data))
      puts("destroy mrms2");
   if (Ms2GetServerIp(Data) !=(char *)NULL)
      free(Ms2GetServerIp(Data));
   if (Ms2GetCanName(Data) !=(char *)NULL)
      free(Ms2GetCanName(Data));
   free(Data);
}

/**********************************************************************\
* Funktionsname: Ms2Init
*
* Kurzbeschreibung: this function sets the values for this object to
*                   the right values for proper work
*
* Parameter:
* Data    : pointer to ms2 object which should be destroyed
* Verbose : should this module be verbose and print messages
* IpAddr  : ip address of server drehscheibe
* Port    : port of server drehscheibe
* CanIf   : interface name of can interface
*
\**********************************************************************/
void Ms2Init(Ms2Struct *Data, BOOL Verbose, char *IpAddr, int Port, char *CanIf)
{
   Ms2SetVerbose(Data, Verbose);
   Ms2SetServerIp(Data, IpAddr);
   Ms2SetServerPort(Data, Port);
   Ms2SetClientSock(Data, -1);
   Ms2SetCanSock(Data, -1);
   if (CanIf != (char *)NULL)
   {
      free(Ms2GetCanName(Data));
      Ms2SetCanName(Data, CanIf);
   }
}

/**********************************************************************\
* Funktionsname: Ms2Exit
*
* Kurzbeschreibung: this function is the cleanup counterpart for the
*                   init function
*
* Parameter:
* Data    : pointer to ms2 object which should be destroyed
*
\**********************************************************************/
void Ms2Exit(Ms2Struct *Data)
{
   if (Ms2GetVerbose(Data))
      puts("exit mrms2");
   if (Ms2GetServerIp(Data) !=(char *)NULL)
      free(Ms2GetServerIp(Data));
   Ms2SetServerIp(Data, (char *)NULL);
   if (Ms2GetCanName(Data) !=(char *)NULL)
      free(Ms2GetCanName(Data));
   Ms2SetCanName(Data, (char *)NULL);
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
* Data : pointer to ms2 object which contains all needed information
*
* Rueckgabewert:
* TRUE  : all needed for main loop was successfull started
* FALSE : start failed
*
\**********************************************************************/
static BOOL Start(Ms2Struct *Data)
{  struct sigaction SigStruct;

   if (Ms2GetVerbose(Data))
      puts("start mrm2: open socket to system");
   Ms2SetClientSock(Data,
                    MrIpcConnect(Ms2GetServerIp(Data),
                                 Ms2GetServerPort(Data)));
   if (Ms2GetClientSock(Data) >= 0)
   {
      if (Ms2GetVerbose(Data))
         puts("start mrm2: open can socket");
      Ms2SetCanSock(Data, MrMs2Connect(Ms2GetCanName(Data)));
      if (Ms2GetCanSock(Data) >= 0)
      {
         if (Ms2GetVerbose(Data))
            puts("ready for incoming comands from system");
         SigStruct.sa_handler = SigHandler;
         sigemptyset(&SigStruct.sa_mask);
         SigStruct.sa_flags = 0;
         sigaction(SIGINT, &SigStruct, NULL);
         sigaction(SIGQUIT, &SigStruct, NULL);
         sigaction(SIGTERM, &SigStruct, NULL);
         SendMagicStart60113Frame(Ms2GetCanSock(Data), Ms2GetVerbose(Data));
         return(TRUE);
      }
      else
      {
         MrIpcClose(Ms2GetClientSock(Data));
         return(FALSE);
      }
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
* Data : pointer to ms object which contains all needed information
*
\**********************************************************************/
static void Stop(Ms2Struct *Data)
{
   if (Ms2GetVerbose(Data))
      puts("stop network client");
   MrIpcClose(Ms2GetClientSock(Data));
   MrMs2Close(Ms2GetCanSock(Data));
}

/**********************************************************************\
* Funktionsname: Ping
*
* Kurzbeschreibung: this function send a ping
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
static void Ping(Ms2Struct *Data)
{  MrCs2CanDataType CanMsg;
   struct can_frame CanFrame;

   MrMs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST); /* CS2 did not set hash */
   MrCs2SetResponse(&CanMsg, 1);
   MrMs2EncPing8(&CanMsg, 0l, 0x0100, 0xe1);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
}

/**********************************************************************\
* Funktionsname: QuerryLokdata
*
* Kurzbeschreibung: this function querry the lok database
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
static void QuerryLokdata(Ms2Struct *Data)
{  MrCs2CanDataType CanMsg;
   struct can_frame CanFrame;

   /* request lokdb */
   MrMs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST);
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   MrCs2EncConfigQuery(&CanMsg, MR_MS2_CFG_LOCDB);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
   /* Folgenachricht (leer) */
   MrMs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST);
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   MrCs2EncConfigQuery0(&CanMsg);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
}

/**********************************************************************\
* Funktionsname: QuerryLoknamen
*
* Kurzbeschreibung: this function querry the lok names
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
static void QuerryLoknamen(Ms2Struct *Data, int Start, int End)
{  MrCs2CanDataType CanMsg;
   struct can_frame CanFrame;
   char MsgBuf[9];

   if (Ms2GetVerbose(Data))
      printf("querry loknamen %d - %d\n", Start, End);
   /* request lokdb */
   MrMs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST);
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   MrCs2EncConfigQuery(&CanMsg, MR_MS2_CFG_LOCNAMES);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
   /* Folgenachricht (start und ende) */
   MrMs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST);
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   sprintf(MsgBuf, "%d %d", Start, End);
   MrCs2EncConfigQuery(&CanMsg, MsgBuf);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
}

/**********************************************************************\
* Funktionsname: QuerryLokinfo
*
* Kurzbeschreibung: this function querry the info of one loc
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
static void QuerryLokinfo(Ms2Struct *Data, char *Locname)
{  MrCs2CanDataType CanMsg;
   struct can_frame CanFrame;
   char MsgBuf[9];

   if (Ms2GetVerbose(Data))
      printf("querry lokinfo >%s<\n", Locname);
   /* request lokdb */
   MrMs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST);
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   MrCs2EncConfigQuery(&CanMsg, MR_MS2_CFG_LOCINFO);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
   /* Folgenachricht 1 (Dateiname, erste 8 Byte) */
   MrMs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST);
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   strncpy(MsgBuf, Locname, 8);
   MsgBuf[8] = '\0';
   MrCs2EncConfigQuery(&CanMsg, MsgBuf);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
   /* Folgenachricht 2 (Dateiname, zweite 8 Byte) */
   MrMs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST);
   MrCs2SetResponse(&CanMsg, 0);
   MrCs2SetPrio(&CanMsg, MR_CS2_PRIO_0);
   if (strlen(Locname) > 8)
   {
      strncpy(MsgBuf, Locname + 8, 8);
      MsgBuf[8] = '\0';
   }
   else
   {
      MsgBuf[0] = '\0';
   }
   MrCs2EncConfigQuery(&CanMsg, MsgBuf);
   MrMs2Encode(&CanMsg, &CanFrame);
   MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
}

/**********************************************************************\
* Funktionsname: ProcessSystemData
*
* Kurzbeschreibung: this function evaluates one command frame from the
*                   system (drehscheibe) and do the proper reaction
*
* Parameter:
* Data : pointer to ms2 object which contains all needed information
* CmdFrame : one command frame from the system
*
\**********************************************************************/
static void ProcessSystemData(Ms2Struct *Data, MrIpcCmdType *CmdFrame)
{  MrMs2CanDataType CanMsg;
   struct can_frame CanFrame;
   int i, StartIdx, EndIdx;
   char LokName[17];

   MrIpcDecode(CmdFrame);
   switch (MrIpcCmdGetCmd(CmdFrame))
   {
      case MrIpcCmdNull:
      case MrIpcCmdMember:
         break;
      case MrIpcCmdRequestLocName:
         MrIpcCmdGetReqestLocname(CmdFrame, &StartIdx, &EndIdx);
         QuerryLoknamen(Data, StartIdx, EndIdx);
         break;
      case MrIpcCmdRequestLocInfo:
         MrIpcCmdGetReqestLocinfo(CmdFrame, LokName);
         QuerryLokinfo(Data, LokName);
         break;
      default:
         MrIpcDecodeToCan(CmdFrame, &CanMsg);
         MrMs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST);
         MrMs2Encode(&CanMsg, &CanFrame);
         if (Ms2GetVerbose(Data))
         {
            printf("send can data 0x%lx %d\n    ",
                   MrCs2GetId(&CanMsg), MrCs2GetDlc(&CanMsg));
            for (i = 0; i < 8; i++)
               printf("0x%02x ", CanMsg.Data[i]);
            printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
                   MrCs2GetHash(&CanMsg), MrCs2GetResponse(&CanMsg),
                   MrCs2GetCommand(&CanMsg), MrCs2GetPrio(&CanMsg));
         }
         MrMs2Send(Ms2GetCanSock(Data), &CanFrame);
         break;
   }
   MrIpcExit(CmdFrame);
}

/**********************************************************************\
* Funktionsname: HandleSystemData
*
* Kurzbeschreibung: this function reads all available command frames
*                   from the system and calls the process function for
*                   each of them
*
* Parameter:
* Data : pointer to ms2 object which contains all needed information
*
\**********************************************************************/
static void HandleSystemData(Ms2Struct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (Ms2GetVerbose(Data))
      puts("new data available");
   MrIpcInit(&CmdFrame);
   RcvReturnValue = MrIpcRecv(Ms2GetClientSock(Data), &CmdFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      /* error in recieve from socket */
      if (Ms2GetVerbose(Data))
         puts("Error in recieve from socket!");
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      /* socket was closed at remote side */
      if (Ms2GetVerbose(Data))
         puts("client socket was closed\nmaybe server has stoped");
      Loop = FALSE;
   }
   else
   {
      /* we have received data, process them */
      if (Ms2GetVerbose(Data))
         printf("read new comand frame from socket\n%s\n",
                MrIpcGetRendered(&CmdFrame));
      ProcessSystemData(Data, &CmdFrame);
   }
}

/**********************************************************************\
* Funktionsname: ProcessCanData
*
* Kurzbeschreibung: this function evaluates one can frame from the ms2
*                   and do the proper reaction
*
* Parameter:
* Data : pointer to ms2 object which contains all needed information
* frame : one can frame from the ms1
*
\**********************************************************************/
static void ProcessCanData(Ms2Struct *Data, MrMs2CanDataType *CanMsg)
{  MrIpcCmdType Cmd;
   int i;

   if (MrCs2GetIsCs2(CanMsg))
   {
      if (Ms2GetVerbose(Data))
      {
         printf("get can data 0x%lx %d\n    ",
                MrCs2GetId(CanMsg), MrCs2GetDlc(CanMsg));
         for (i = 0; i < 8; i++)
            printf("0x%02x ", CanMsg->Data[i]);
         printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
                MrCs2GetHash(CanMsg), MrCs2GetResponse(CanMsg),
                MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg));
      }
      if (MrCs2GetCommand(CanMsg) == MR_CS2_CMD_PING)
      {
         if (Ms2GetVerbose(Data))
            puts("handle ping");
         if (MrCs2GetDlc(CanMsg) == 0)
         {
            if (Ms2GetVerbose(Data))
               puts("answer ping");
            /* answer ping */
            Ping(Data);
         }
         else if (MrCs2GetDlc(CanMsg) == 8)
         {
            if (Ms2GetVerbose(Data))
               puts("send ping to drehscheibe");
            MrIpcInit(&Cmd);
            MrIpcEncodeFromCan(&Cmd, CanMsg);
            MrIpcSend(Ms2GetClientSock(Data), &Cmd);
            MrIpcExit(&Cmd);
         }
      }
      else
      {
         if (Ms2GetVerbose(Data))
            puts("send to drehscheibe");
         MrIpcInit(&Cmd);
         MrIpcEncodeFromCan(&Cmd, CanMsg);
         MrIpcSend(Ms2GetClientSock(Data), &Cmd);
         MrIpcExit(&Cmd);
      }
   }
}

/**********************************************************************\
* Funktionsname: HandleCanData
*
* Kurzbeschreibung: this function reads all available can frames from
*                   the ms2 and calls the process function for each of
*                   them
*
* Parameter:
* Data : pointer to ms2 object which contains all needed information
*
\**********************************************************************/
static void HandleCanData(Ms2Struct *Data)
{  struct can_frame frame;
   MrMs2CanDataType CanMsg;

   /* read all CAN frames present on can socket */
   if (read(Ms2GetCanSock(Data), &frame, sizeof(struct can_frame)) ==
       sizeof(struct can_frame))
   {
      /* process the received can frame */
      MrMs2Decode(&CanMsg, &frame);
      ProcessCanData(Data, &CanMsg);
   }
}

/**********************************************************************\
* Funktionsname: Ms2Run
*
* Kurzbeschreibung: this functions contains the main loop
*
* Parameter:
* Data : pointer to ms2 object which contains all needed information
*
\**********************************************************************/
void Ms2Run(Ms2Struct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      while (Loop)
      {
         /* Main loop for receive and send data */

         /* fill fd set for call to select */
         FD_ZERO(&ReadFds);
         HighFd = 0;
         /* 1. client socket */
         if (Ms2GetVerbose(Data))
            printf("add client socket %d\n", Ms2GetClientSock(Data));
         FD_SET(Ms2GetClientSock(Data), &ReadFds);
         if (Ms2GetClientSock(Data) > HighFd)
            HighFd = Ms2GetClientSock(Data);
         /* 2. CAN socket */
         if (Ms2GetVerbose(Data))
            printf("add can socket %d\n", Ms2GetCanSock(Data));
         FD_SET(Ms2GetCanSock(Data), &ReadFds);
         if (Ms2GetCanSock(Data) > HighFd)
            HighFd = Ms2GetCanSock(Data);
         /* 3. Timeout */
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Ms2GetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);

         /* wait for data */
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (Ms2GetVerbose(Data))
            printf("select liefert %d\n", RetVal);

         /* check, who has data */
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            /* timeout, time for periodic tasks */
            Now = time(NULL);
            if (Ms2GetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
            Ping(Data);
         }
         else if (RetVal < 0)
         {
            /* error, finish loop */
            if (Ms2GetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            /* data, check at which fd */
            if (FD_ISSET(Ms2GetClientSock(Data), &ReadFds))
            {
               /* new cmd frame */
               if (Ms2GetVerbose(Data))
                  puts("new data on cmd socket to drehscheibe");
               HandleSystemData(Data);
            }
            if (FD_ISSET(Ms2GetCanSock(Data), &ReadFds))
            {
               /* new remote frame */
               if (Ms2GetVerbose(Data))
                  puts("new data on can socket");
               HandleCanData(Data);
            }
         }
      }

      /* cleanup */
      Stop(Data);
   }
}
