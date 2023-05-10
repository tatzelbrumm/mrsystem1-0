/**********************************************************************\
* Dateiname: cs2eth.c
* Autor    : Daniel Prein, Michael Bernstein
* Projekt  : client_cs2eth
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: functionality for cs2eth object which handles the
*                   message flow on ethernet in CS2 format
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
#include "cs2eth.h"

/*--- #define-Konstanten und Makros ----------------------------------*/
#define SELECT_TIMEOUT 100
#define MAX_DATAGRAM_SIZE 4096
#define DEVICE_ID 0xFFFF

/*--- Modullokale Variablen ------------------------------------------*/
static BOOL Loop = TRUE;

/*--- Funktionsdefinitionen ------------------------------------------*/

/**********************************************************************\
* Funktionsname: Cs2ethCreate
*
* Kurzbeschreibung: this function creates a cs2eth object and
*                   initializes all members to zero.
*
* Rueckgabewert:
* NULL : creation failed, error
* else : pointer to created cs2eth object
*
\**********************************************************************/
Cs2ethStruct *Cs2ethCreate(void)
{  Cs2ethStruct *Data;

   Data = (Cs2ethStruct *)malloc(sizeof(Cs2ethStruct));
   if (Data != (Cs2ethStruct *)NULL)
   {
      Cs2ethSetVerbose(Data, FALSE);
      Cs2ethSetServerIp(Data, (char *)NULL);
      Cs2ethSetBcIp(Data, (char *)NULL);
      Cs2ethSetServerPort(Data, -1);
      Cs2ethSetClientSock(Data, -1);
   }
   return(Data);
}

/**********************************************************************\
* Funktionsname: Cs2ethDestroy
*
* Kurzbeschreibung: this function destroys a cs2eth object, which was
*                   created by Cs2ethCreate()
*
* Parameter:
* Data : pointer to cs2eth object which should be destroyed
*
\**********************************************************************/
void Cs2ethDestroy(Cs2ethStruct *Data)
{
   if (Cs2ethGetVerbose(Data))
      puts("destroy mrcs2eth");
   if (Cs2ethGetServerIp(Data) != NULL)
      free(Cs2ethGetServerIp(Data));
   if (Cs2ethGetBcIp(Data) != NULL)
      free(Cs2ethGetBcIp(Data));
   free(Data);
}

/**********************************************************************\
* Funktionsname: Cs2ethInit
*
* Kurzbeschreibung: this function sets the values for this object to
*                   the right values for proper work
*
* Parameter:
* Data    : pointer to cs2eth object which contains all needed information
* Verbose : should this module be verbose and print messages
* IpAddr  : ip address of tcp server drehscheibe
* Port    : port of server drehscheibe
* SendBc  : ip address of udp server drehscheibe
* BcAddr  : local IP address as template fpr broadcast address
* Trace   : flag if debug traces should be made
*
\**********************************************************************/
void Cs2ethInit(Cs2ethStruct *Data, BOOL Verbose, char *IpAddr, int Port,
                BOOL SendBc, char *BcAddr
#ifdef TRACE
               , BOOL Trace
#endif
               )
{
   Cs2ethSetVerbose(Data, Verbose);
   if (Cs2ethGetVerbose(Data))
      puts("init mrcs2eth");
#ifdef TRACE
   Cs2ethSetTrace(Data, Trace);
#endif
   Cs2ethSetSendUdpBc(Data, SendBc);
   Cs2ethSetServerIp(Data, IpAddr);
   Cs2ethSetBcIp(Data, BcAddr);
   Cs2ethSetServerPort(Data, Port);
   Cs2ethSetClientSock(Data, -1);
   Cs2ethSetOutsideUdpSock(Data, -1);
   Cs2ethSetOutsideTcpSock(Data, -1);
   Cs2ethSetAppSock(Data, -1);
}

/**********************************************************************\
* Funktionsname: Cs2ethExit
*
* Kurzbeschreibung: this function is the cleanup counterpart for the
*                   init function
*
* Parameter:
* Data    : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
void Cs2ethExit(Cs2ethStruct *Data)
{
   if (Cs2ethGetVerbose(Data))
      puts("exit mrcs2eth");
   if (Cs2ethGetServerIp(Data) != NULL)
      free(Cs2ethGetServerIp(Data));
   Cs2ethSetServerIp(Data, (char *)NULL);
   if (Cs2ethGetBcIp(Data) != NULL)
      free(Cs2ethGetBcIp(Data));
   Cs2ethSetBcIp(Data, (char *)NULL);
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
* Data : pointer to cs2eth object which contains all needed information
*
* Rueckgabewert:
* TRUE  : all needed for main loop was successfull started
* FALSE : start failed
*
\**********************************************************************/
static BOOL Start(Cs2ethStruct *Data)
{  struct sigaction SigStruct;

   if (Cs2ethGetVerbose(Data))
      puts("start mrcs2eth: connect to system");
   Cs2ethSetClientSock(Data,
                       MrIpcConnect(Cs2ethGetServerIp(Data),
                                    Cs2ethGetServerPort(Data)));
   if (Cs2ethGetClientSock(Data) >= 0)
   {
      if (Cs2ethGetVerbose(Data))
         puts("start mrcs2eth; stat udp server");
      Cs2ethSetOutsideUdpSock(Data, MrEthCs2StartServer());
      if (Cs2ethGetOutsideUdpSock(Data) >= 0)
      {
         if (Cs2ethGetVerbose(Data))
            puts("start mrcs2eth: start tcp server");
         Cs2ethSetOutsideTcpSock(Data, MrEthCs2StartAppServer());
         if (Cs2ethGetOutsideTcpSock(Data) >= 0)
         {
            if (Cs2ethGetSendUdpBc(Data))
            {
               if (Cs2ethGetVerbose(Data))
                  puts("start mrcs2eth: start bc server");
               Cs2ethSetOutsideBcSock(Data, MrEthCs2StartBcServer());
               if (Cs2ethGetOutsideBcSock(Data) >= 0)
               {
                  MrEthCs2MkBcAddr(&Cs2ethGetClientAddr(Data), Cs2ethGetBcIp(Data));
               }
               else
               {
                  MrEthCs2Close(Cs2ethGetOutsideTcpSock(Data));
                  MrEthCs2Close(Cs2ethGetOutsideUdpSock(Data));
                  MrIpcClose(Cs2ethGetClientSock(Data));
                  return(FALSE);
               }
            }
            if (Cs2ethGetVerbose(Data))
               puts("ready for incoming comands from system");
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
            MrEthCs2Close(Cs2ethGetOutsideUdpSock(Data));
            MrIpcClose(Cs2ethGetClientSock(Data));
            return(FALSE);
         }
      }
      else
      {
         MrIpcClose(Cs2ethGetClientSock(Data));
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
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
static void Stop(Cs2ethStruct *Data)
{
   if (Cs2ethGetVerbose(Data))
      puts("stop mrcs2eth");
   MrIpcClose(Cs2ethGetClientSock(Data));
   MrEthCs2Close(Cs2ethGetOutsideUdpSock(Data));
   if (Cs2ethGetOutsideBcSock(Data) >= 0)
      MrEthCs2Close(Cs2ethGetOutsideBcSock(Data));
   MrEthCs2Close(Cs2ethGetOutsideTcpSock(Data));
   if (Cs2ethGetAppSock(Data) >= 0)
      MrEthCs2Close(Cs2ethGetAppSock(Data));
}

/**********************************************************************\
* Funktionsname: ProcessSystemData
*
* Kurzbeschreibung: this function evaluates one command frame from the
*                   system (drehscheibe) and do the proper reaction
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
* CmdFrame : one command frame from the system
*
\**********************************************************************/
static void ProcessSystemData(Cs2ethStruct *Data, MrIpcCmdType *CmdFrame)
{  char UdpFrame[MR_CS2_UDP_LENGTH];
   MrCs2CanDataType CanMsg;

   MrIpcDecode(CmdFrame);
   if (MrIpcCmdGetCmd(CmdFrame) != MrIpcCmdNull)
   {
      MrIpcDecodeToCan(CmdFrame, &CanMsg);
      MrCs2CalcHash(&CanMsg, MR_CS2_UID_BROADCAST);
      MrCs2SetResponse(&CanMsg, 1);
#ifdef TRACE
      if (Cs2ethGetVerbose(Data))
         puts("send to UDP client");
      if (Cs2ethGetVerbose(Data) && Cs2ethGetTrace(Data))
         MrCs2Trace(&CanMsg);
#endif
      MrEthCs2Encode(UdpFrame, &CanMsg);
      MrEthCs2Send(Cs2ethGetOutsideUdpSock(Data),
                   (struct sockaddr_in *)&(Cs2ethGetClientAddr(Data)),
                   UdpFrame);
      MrEthCs2Send(Cs2ethGetAppSock(Data),
                   (struct sockaddr_in *)&(Cs2ethGetAppAddr(Data)),
                   UdpFrame);
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
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
static void HandleSystemData(Cs2ethStruct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (Cs2ethGetVerbose(Data))
      puts("new data available");
   /* read all system messages present on system socket */
   MrIpcInit(&CmdFrame);
   RcvReturnValue = MrIpcRecv(Cs2ethGetClientSock(Data), &CmdFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      /* error in recieve from socket */
      if (Cs2ethGetVerbose(Data))
         puts("Error in recieve from socket!");
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      /* socket was closed at remote side */
      if (Cs2ethGetVerbose(Data))
         puts("client socket was closed\nmaybe server has stoped");
      Loop = FALSE;
   }
   else
   {
      /* we have received data, process them */
      if (Cs2ethGetVerbose(Data))
         printf("read new comand frame from socket\n%s\n",
                MrIpcGetRendered(&CmdFrame));
      ProcessSystemData(Data, &CmdFrame);
   }
}

/**********************************************************************\
* Funktionsname: ProcessOutsideData
*
* Kurzbeschreibung: this function evaluates one ethernet udp frame from
*                   outside and do the proper reaction
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
* frame : one can frame from the ethernet
*
\**********************************************************************/
static void ProcessOutsideData(Cs2ethStruct *Data, MrCs2CanDataType *CanMsg)
{  MrIpcCmdType Cmd;
   unsigned char UdpFrame[MAX_DATAGRAM_SIZE];
   unsigned long Uid;
   int SoftwareVersion, DeviceId, i;

   switch (MrMs2GetCommand(CanMsg))
   {
      case MR_MS2_CMD_PING:
         /* we answer to ping */
         if (Cs2ethGetVerbose(Data))
         {
            printf("can data 0x%lx %d\n    ",
                   MrCs2GetId(CanMsg), MrCs2GetDlc(CanMsg));
            for (i = 0; i < 8; i++)
               printf("0x%02x ", CanMsg->Data[i]);
            printf("\n    hash 0x%x resp 0x%x cmd 0x%x prio 0x%x\n",
                   MrCs2GetHash(CanMsg), MrCs2GetResponse(CanMsg),
                   MrCs2GetCommand(CanMsg), MrCs2GetPrio(CanMsg));
         }
         switch (MrCs2GetNumParamBytes(CanMsg))
         {
            case 0:
               MrCs2DecPing0(CanMsg);
               if (Cs2ethGetVerbose(Data))
                  puts("Softwarestand Anfrage/Teilnehmer Ping");
               break;
            case 8:
               MrCs2DecPing8(CanMsg, &Uid, &SoftwareVersion, &DeviceId);
               if (Cs2ethGetVerbose(Data))
                  printf("Softwarestand Anfrage/Teilnehmer Ping (Uid = 0x%lx, SwVersion = 0x%x, DeviceId = 0x%x)\n",
                         Uid, SoftwareVersion, DeviceId);
               break;
            default:
               if (Cs2ethGetVerbose(Data))
                  puts("Softwarestand Anfrage/Teilnehmer Ping (unknown parameter)");
               break;
         }
         MrCs2SetHash(CanMsg, MR_CS2_UID_BROADCAST); /* CS2 did not set hash */
         MrCs2SetResponse(CanMsg, 0);
         MrCs2EncPing0(CanMsg);
         MrEthCs2Encode(UdpFrame, CanMsg);
         if (Cs2ethGetSendUdpBc(Data))
         {
            MrEthCs2Send(Cs2ethGetOutsideBcSock(Data),
                         &Cs2ethGetClientAddr(Data),
                         UdpFrame);
         }
         else
         {
            MrEthCs2Send(Cs2ethGetOutsideUdpSock(Data),
                         &Cs2ethGetClientAddr(Data),
                         UdpFrame);
         }
         break;
      default:
         /* send to drehscheibe */
#ifdef TRACE
         if (Cs2ethGetVerbose(Data) && Cs2ethGetTrace(Data))
            MrCs2Trace(CanMsg);
#endif
         MrIpcInit(&Cmd);
         MrIpcEncodeFromCan(&Cmd, CanMsg);
         MrIpcSend(Cs2ethGetClientSock(Data), &Cmd);
         MrIpcExit(&Cmd);
         break;
   }
}

/**********************************************************************\
* Funktionsname: HandleOutsideData
*
* Kurzbeschreibung: this function reads all available etherner udp
*                   frames and calls the process function for each of
*                   them
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
static void HandleOutsideData(Cs2ethStruct *Data)
{  unsigned char UdpFrame[MAX_DATAGRAM_SIZE];
   MrCs2CanDataType CanMsg;
   BOOL HaveRecv;
   struct sockaddr_in caddr;

   if (Cs2ethGetSendUdpBc(Data))
   {
      HaveRecv = MrEthCs2Recv(Cs2ethGetOutsideUdpSock(Data), &caddr, UdpFrame);
   }
   else
   {
      HaveRecv = MrEthCs2Recv(Cs2ethGetOutsideUdpSock(Data),
                              &Cs2ethGetClientAddr(Data),
                              UdpFrame);
   }
   if (HaveRecv)
   {
      if (Cs2ethGetVerbose(Data))
         puts("read new udp frame");
      MrEthCs2Decode(&CanMsg, UdpFrame);
      ProcessOutsideData(Data, &CanMsg);
   } 
}

/**********************************************************************\
* Funktionsname: HandleOutsideTcpConnection
*
* Kurzbeschreibung: this function accepts a incoming tcp connection
*                   from a remote app
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
static void HandleOutsideTcpConnection(Cs2ethStruct *Data)
{
   if (Cs2ethGetVerbose(Data))
      puts("accept tcp connection");
   Cs2ethSetAppSock(Data, MrEthCs2Accept(Cs2ethGetOutsideTcpSock(Data)));
}

/**********************************************************************\
* Funktionsname: HandleAppData
*
* Kurzbeschreibung: this function reads all available etherner tcp
*                   frames and calls the process function for each of
*                   them
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
static void HandleAppData(Cs2ethStruct *Data)
{  unsigned char UdpFrame[MAX_DATAGRAM_SIZE];
   MrCs2CanDataType CanMsg;
   int RcvReturnValue;

   RcvReturnValue = MrEthCs2Recv(Cs2ethGetAppSock(Data),
                                 &Cs2ethGetAppAddr(Data),
                                 UdpFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      /* error in recieve from socket */
      if (Cs2ethGetVerbose(Data))
         puts("Error in recieve from app socket!");
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      /* socket was closed at remote side */
      if (Cs2ethGetVerbose(Data))
         puts("app socket was closed");
      Cs2ethSetAppSock(Data, -1);
   }
   else
   {
      if (Cs2ethGetVerbose(Data))
         puts("read new app frame");
      MrEthCs2Decode(&CanMsg, UdpFrame);
      ProcessOutsideData(Data, &CanMsg);
   } 
}

/**********************************************************************\
* Funktionsname: Cs2ethRun
*
* Kurzbeschreibung: this functions contains the main loop
*
* Parameter:
* Data : pointer to cs2eth object which contains all needed information
*
\**********************************************************************/
void Cs2ethRun(Cs2ethStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   time_t Now;

   if (Start(Data))
   {
      if (Cs2ethGetVerbose(Data))
         puts("run mrcs2eth");
      while (Loop)
      {
         /* Main loop for receive and send data */

         /* fill fd set for call to select */
         FD_ZERO(&ReadFds);
         HighFd = 0;
         /* 1. client socket to system */
         if (Cs2ethGetVerbose(Data))
            printf("add client socket %d\n", Cs2ethGetClientSock(Data));
         FD_SET(Cs2ethGetClientSock(Data), &ReadFds);
         if (Cs2ethGetClientSock(Data) > HighFd)
            HighFd = Cs2ethGetClientSock(Data);
         /* 2. Outside UDP socket */
         if (Cs2ethGetVerbose(Data))
            printf("add outside udp socket %d\n", Cs2ethGetOutsideUdpSock(Data));
         FD_SET(Cs2ethGetOutsideUdpSock(Data), &ReadFds);
         if (Cs2ethGetOutsideUdpSock(Data) > HighFd)
            HighFd = Cs2ethGetOutsideUdpSock(Data);
         /* 3. Outside TCP socket */
         if (Cs2ethGetVerbose(Data))
            printf("add outside tcp socket %d\n", Cs2ethGetOutsideTcpSock(Data));
         FD_SET(Cs2ethGetOutsideTcpSock(Data), &ReadFds);
         if (Cs2ethGetOutsideTcpSock(Data) > HighFd)
            HighFd = Cs2ethGetOutsideTcpSock(Data);
         /* 4. Outside UDP socket */
         if (Cs2ethGetAppSock(Data) >= 0)
         {
            if (Cs2ethGetVerbose(Data))
               printf("add app socket %d\n", Cs2ethGetAppSock(Data));
            FD_SET(Cs2ethGetAppSock(Data), &ReadFds);
            if (Cs2ethGetAppSock(Data) > HighFd)
               HighFd = Cs2ethGetAppSock(Data);
         }
         /* 5. Timeout */
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Cs2ethGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);

         /* wait for data */
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (Cs2ethGetVerbose(Data))
            printf("select liefert %d\n", RetVal);

         /* check, who has data */
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            /* timeout, time for periodic tasks */
            Now = time(NULL);
            if (Cs2ethGetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            /* error, finish loop */
            if (Cs2ethGetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            /* data, check at which fd */
            if (FD_ISSET(Cs2ethGetClientSock(Data), &ReadFds))
            {
               /* new cmd frame */
               if (Cs2ethGetVerbose(Data))
                  puts("data on cmd socket to drehscheibe");
               HandleSystemData(Data);
            }
            if (FD_ISSET(Cs2ethGetOutsideUdpSock(Data), &ReadFds))
            {
               /* new remote frame */
               if (Cs2ethGetVerbose(Data))
                  puts("data on udp socket to remote");
               HandleOutsideData(Data);
            }
            if (FD_ISSET(Cs2ethGetOutsideTcpSock(Data), &ReadFds))
            {
               /* new remote concection frame */
               if (Cs2ethGetVerbose(Data))
                  puts("data on tcp socket server to remote");
               HandleOutsideTcpConnection(Data);
            }
            if ((Cs2ethGetAppSock(Data) >= 0) &&
                (FD_ISSET(Cs2ethGetAppSock(Data), &ReadFds)))
            {
               /* new app frame */
               if (Cs2ethGetVerbose(Data))
                  puts("data on app socket to remote");
               HandleAppData(Data);
            }
         }
      }

      /* cleanup */
      Stop(Data);
   }
}
