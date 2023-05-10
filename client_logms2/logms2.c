/**********************************************************************\
* Dateiname: logms2.c
* Autor    : Michael Bernstein
* Projekt  : client_logms2
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: functionality for logms2 object which handles logs
*                   the can message from CS2/MS2
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
#include "logms2.h"

/*--- #define-Konstanten und Makros ----------------------------------*/
#define SELECT_TIMEOUT 100

/*--- Modullokale Variablen ------------------------------------------*/
static BOOL Loop = TRUE;

/*--- Funktionsdefinitionen ------------------------------------------*/

/**********************************************************************\
* Funktionsname: Logms2Create
*
* Kurzbeschreibung: this function creates a logms2 object and.
*                   initializes all members to zero.
*
* Rueckgabewert:
* NULL : creation failed, error
* else : pointer to created ms2 object
*
\**********************************************************************/
Logms2Struct *Logms2Create(void)
{  Logms2Struct *Data;

   Data = (Logms2Struct *)malloc(sizeof(Logms2Struct));
   if (Data != (Logms2Struct *)NULL)
   {
      Logms2SetVerbose(Data, FALSE);
      Logms2SetServerIp(Data, (char *)NULL);
      Logms2SetServerPort(Data, -1);
      Logms2SetClientSock(Data, -1);
      Logms2SetCanName(Data, strdup("can0"));
      Logms2SetCanSock(Data, -1);
   }
   return(Data);
}

/**********************************************************************\
* Funktionsname: Logms2Destroy
*
* Kurzbeschreibung: this function destroys a logms2 object, which was
*                   created by Logms2Create()
*
* Parameter:
* Data : pointer to logms2 object which should be destroyed
*
\**********************************************************************/
void Logms2Destroy(Logms2Struct *Data)
{
   if (Logms2GetVerbose(Data))
      puts("destroy mrms2");
   if (Logms2GetServerIp(Data) != NULL)
      free(Logms2GetServerIp(Data));
   free(Logms2GetCanName(Data));
   free(Data);
}

/**********************************************************************\
* Funktionsname: Logms2Init
*
* Kurzbeschreibung: this function sets the values for this object to
*                   the right values for proper work
*
* Parameter:
* Data    : pointer to logms2 object which should be destroyed
* Verbose : should this module be verbose and print messages
* IpAddr  : ip address of server drehscheibe
* Port    : port of server drehscheibe
* CanIf   : interface name of can interface
*
\**********************************************************************/
void Logms2Init(Logms2Struct *Data, BOOL Verbose, char *IpAddr, int Port, char *CanIf)
{
   Logms2SetVerbose(Data, Verbose);
   Logms2SetServerIp(Data, IpAddr);
   Logms2SetServerPort(Data, Port);
   Logms2SetClientSock(Data, -1);
   Logms2SetCanSock(Data, -1);
   if (CanIf != (char *)NULL)
   {
      free(Logms2GetCanName(Data));
      Logms2SetCanName(Data, CanIf);
   }
}

/**********************************************************************\
* Funktionsname: Logms2Exit
*
* Kurzbeschreibung: this function is the cleanup counterpart for the
*                   init function
*
* Parameter:
* Data    : pointer to logms2 object which should be destroyed
*
\**********************************************************************/
void Logms2Exit(Logms2Struct *Data)
{
   if (Logms2GetVerbose(Data))
      puts("exit mrms2");
   if (Logms2GetServerIp(Data) != NULL)
      free(Logms2GetServerIp(Data));
   Logms2SetServerIp(Data, (char *)NULL);
   if (Logms2GetCanName(Data) != (char *)NULL)
      free(Logms2GetCanName(Data));
   Logms2SetCanName(Data, (char *)NULL);
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
static BOOL Start(Logms2Struct *Data)
{  struct sigaction SigStruct;

   if ((Logms2GetServerIp(Data) != NULL) && (Logms2GetServerPort(Data) != -1))
      Logms2SetClientSock(Data,
                          MrIpcConnect(Logms2GetServerIp(Data),
                                       Logms2GetServerPort(Data)));
   Logms2SetCanSock(Data, MrMs2Connect(Logms2GetCanName(Data)));
   if (Logms2GetCanSock(Data) >= 0)
   {
      if (Logms2GetVerbose(Data))
         puts("ready for incoming commands from system");
      SigStruct.sa_handler = SigHandler;
      sigemptyset(&SigStruct.sa_mask);
      SigStruct.sa_flags = 0;
      sigaction(SIGINT, &SigStruct, NULL);
      sigaction(SIGQUIT, &SigStruct, NULL);
      sigaction(SIGTERM, &SigStruct, NULL);
      SendMagicStart60113Frame(Logms2GetCanSock(Data), Logms2GetVerbose(Data));
      return(TRUE);
   }
   else
   {
      if (Logms2GetClientSock(Data) != -1)
         MrIpcClose(Logms2GetClientSock(Data));
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
* Data : pointer to logms2 object which contains all needed information
*
\**********************************************************************/
static void Stop(Logms2Struct *Data)
{
   if (Logms2GetVerbose(Data))
      puts("stop mrms2");
   if (Logms2GetClientSock(Data) != -1)
      MrIpcClose(Logms2GetClientSock(Data));
   MrMs2Close(Logms2GetCanSock(Data));
}

/**********************************************************************\
* Funktionsname: ProcessSystemData
*
* Kurzbeschreibung: this function evaluates one command frame from the
*                   system (drehscheibe) and do the proper reaction
*
* Parameter:
* Data : pointer to logms2 object which contains all needed information
* CmdFrame : one command frame from the system
*
\**********************************************************************/
static void ProcessSystemData(Logms2Struct *Data, MrIpcCmdType *CmdFrame)
{
}

/**********************************************************************\
* Funktionsname: HandleSystemData
*
* Kurzbeschreibung: this function reads all available command frames
*                   from the system and calls the process function for
*                   each of them
*
* Parameter:
* Data : pointer to logms2 object which contains all needed information
*
\**********************************************************************/
static void HandleSystemData(Logms2Struct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (Logms2GetVerbose(Data))
      puts("new data available");
   /* read all system messages present on system socket */
   MrIpcInit(&CmdFrame);
   RcvReturnValue = MrIpcRecv(Logms2GetClientSock(Data), &CmdFrame);
   if (RcvReturnValue == MR_IPC_RCV_ERROR)
   {
      /* error in recieve from socket */
      if (Logms2GetVerbose(Data))
         puts("Error in recieve from socket!");
   }
   else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
   {
      /* socket was closed at remote side */
      if (Logms2GetVerbose(Data))
         puts("client socket was closed\nmaybe server has stoped");
      Loop = FALSE;
   }
   else
   {
      /* we have received data, process them */
      if (Logms2GetVerbose(Data))
         printf("read new comand frame from socket\n%s\n",
                MrIpcGetRendered(&CmdFrame));
      ProcessSystemData(Data, &CmdFrame);
      MrIpcExit(&CmdFrame);
   }
}

/**********************************************************************\
* Funktionsname: ProcessCanData
*
* Kurzbeschreibung: this function evaluates one can frame from the ms2
*                   and do the proper reaction
*
* Parameter:
* Data : pointer to logms2 object which contains all needed information
* frame : one can frame from the ms1
*
\**********************************************************************/
static void ProcessCanData(Logms2Struct *Data, struct can_frame *CanFrame)
{  MrMs2CanDataType CanMsg;

   if (Logms2GetVerbose(Data))
   {
      MrMs2Decode(&CanMsg, CanFrame);
      if (MrMs2IsMs2(&CanMsg))
      {
         MrMs2Trace(&CanMsg);
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
* Data : pointer to logms2 object which contains all needed information
*
\**********************************************************************/
static void HandleCanData(Logms2Struct *Data)
{  struct can_frame frame;

   /* read all CAN frames present on can socket */
   if (read(Logms2GetCanSock(Data), &frame, sizeof(struct can_frame)) ==
       sizeof(struct can_frame))
   {
      /* process the received can frame */
      ProcessCanData(Data, &frame);
   }
}

/**********************************************************************\
* Funktionsname: Logms2Run
*
* Kurzbeschreibung: this functions contains the main loop
*
* Parameter:
* Data : pointer to logms2 object which contains all needed information
*
\**********************************************************************/
void Logms2Run(Logms2Struct *Data)
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
         if (Logms2GetClientSock(Data) != -1)
         {
            /* 1. client socket */
            if (Logms2GetVerbose(Data))
               printf("add client socket %d\n", Logms2GetClientSock(Data));
            FD_SET(Logms2GetClientSock(Data), &ReadFds);
            if (Logms2GetClientSock(Data) > HighFd)
               HighFd = Logms2GetClientSock(Data);
         }
         /* 2. CAN socket */
         if (Logms2GetVerbose(Data))
            printf("add can socket %d\n", Logms2GetCanSock(Data));
         FD_SET(Logms2GetCanSock(Data), &ReadFds);
         if (Logms2GetCanSock(Data) > HighFd)
            HighFd = Logms2GetCanSock(Data);
         /* 3. Timeout */
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Logms2GetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);

         /* wait for data */
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (Logms2GetVerbose(Data))
            printf("select liefert %d\n", RetVal);

         /* check, who has data */
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            /* timeout, time for periodic tasks */
            Now = time(NULL);
            if (Logms2GetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            /* error, finish loop */
            if (Logms2GetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            /* data, check at which fd */
            if ((Logms2GetClientSock(Data) != -1) &&
                (FD_ISSET(Logms2GetClientSock(Data), &ReadFds)))
            {
               /* new cmd frame */
               HandleSystemData(Data);
            }
            if (FD_ISSET(Logms2GetCanSock(Data), &ReadFds))
            {
               /* new remote frame */
               HandleCanData(Data);
            }
         }
      }

      /* cleanup */
      Stop(Data);
   }
}
