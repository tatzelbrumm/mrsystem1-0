/**********************************************************************\
* Dateiname: log.c
* Autor    : Michael Bernstein
* Projekt  : client_log
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: functionality for log object which logs the
*                   message flow from drehscheibe
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
/*--- #includes der Form "..." ---------------------------------------*/
#include "log.h"

/*--- #define-Konstanten und Makros ----------------------------------*/
#define SELECT_TIMEOUT 100

/*--- Modullokale Variablen ------------------------------------------*/
static BOOL Loop = TRUE;

/*--- Funktionsdefinitionen ------------------------------------------*/

/**********************************************************************\
* Funktionsname: LogCreate
*
* Kurzbeschreibung: this function creates a log object and initializes
*                   all members to zero.
*
* Rueckgabewert:
* NULL : creation failed, error
* else : pointer to created log object
*
\**********************************************************************/
LogStruct *LogCreate(void)
{  LogStruct *Data;

   Data = (LogStruct *)malloc(sizeof(LogStruct));
   if (Data != (LogStruct *)NULL)
   {
      LogSetVerbose(Data, FALSE);
      LogSetServerIp(Data, (char *)NULL);
      LogSetServerPort(Data, -1);
      LogSetClientSock(Data, -1);
   }
   return(Data);
}

/**********************************************************************\
* Funktionsname: LogDestroy
*
* Kurzbeschreibung: this function destroys a log object, which was
*                   created by LogCreate()
*
* Parameter:
* Data : pointer to log object which should be destroyed
*
\**********************************************************************/
void LogDestroy(LogStruct *Data)
{
   if (LogGetVerbose(Data))
      puts("destroy log");
   if (LogGetServerIp(Data) != (char *)NULL)
      free(LogGetServerIp(Data));
   free(Data);
}

/**********************************************************************\
* Funktionsname: LogInit
*
* Kurzbeschreibung: this function sets the values for this object to
*                   the right values for proper work
*
* Parameter:
* Data    : pointer to log object which contains all needed information
* Verbose : should this module be verbose and print messages
* IpAddr  : ip address of server drehscheibe
* Port    : port of server drehscheibe
*
\**********************************************************************/
void LogInit(LogStruct *Data, BOOL Verbose, char *IpAddr, int Port)
{
   LogSetVerbose(Data, Verbose);
   LogSetServerIp(Data, IpAddr);
   LogSetServerPort(Data, Port);
   LogSetClientSock(Data, -1);
}

/**********************************************************************\
* Funktionsname: LogExit
*
* Kurzbeschreibung: this function is the cleanup counterpart for the
*                   init function
*
* Parameter:
* Data    : pointer to log object which contains all needed information
*
\**********************************************************************/
void LogExit(LogStruct *Data)
{
   if (LogGetVerbose(Data))
      puts("exit log");
   if (LogGetServerIp(Data) != (char *)NULL)
      free(LogGetServerIp(Data));
   LogSetServerIp(Data, (char *)NULL);
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
* Data : pointer to log object which contains all needed information
*
* Rueckgabewert:
* TRUE  : all needed for main loop was successfull started
* FALSE : start failed
*
\**********************************************************************/
static BOOL Start(LogStruct *Data)
{  struct sigaction SigStruct;

   LogSetClientSock(Data,
                    MrIpcConnect(LogGetServerIp(Data),
                                 LogGetServerPort(Data)));
   if (LogGetClientSock(Data) >= 0)
   {
      if (LogGetVerbose(Data))
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
* Data : pointer to log object which contains all needed information
*
\**********************************************************************/
static void Stop(LogStruct *Data)
{
   if (LogGetVerbose(Data))
      puts("stop network client");
   if (LogGetClientSock(Data) >= 0)
   {
      MrIpcClose(LogGetClientSock(Data));
   }
}

/**********************************************************************\
* Funktionsname: ProcessSystemData
*
* Kurzbeschreibung: this function evaluates one command frame from the
*                   system (drehscheibe) and do the proper reaction
*
* Parameter:
* Data : pointer to log object which contains all needed information
* CmdFrame : one command frame from the system
*
\**********************************************************************/
static void ProcessSystemData(LogStruct *Data, MrIpcCmdType *CmdFrame)
{  unsigned long Addr;
   unsigned int Param;
   DirectionType Direction;
   time_t Now;

   Now = time(NULL);
   printf("%s> command frame\n", asctime(localtime(&Now)));
   switch (MrIpcCmdGetCmd(CmdFrame))
   {
      case MrIpcCmdNull:
         puts("   cmd null");
         break;
      case MrIpcCmdLocomotiveDirection:
         MrIpcCmdGetLocomotiveDir(CmdFrame, &Addr, &Direction);
         printf("   cmd locomotive addr %ld dir %d\n",
                Addr, Direction);
         break;
      case MrIpcCmdLocomotiveSpeed:
         MrIpcCmdGetLocomotiveSpeed(CmdFrame, &Addr, &Param);
         printf("   cmd locomotive addr %ld speed %d\n",
                Addr, Param);
         break;
   }
}

/**********************************************************************\
* Funktionsname: HandleSystemData
*
* Kurzbeschreibung: this function reads all available command frames
*                   from the system and calls the process function for
*                   each of them
*
* Parameter:
* Data : pointer to log object which contains all needed information
*
\**********************************************************************/
static void HandleSystemData(LogStruct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (LogGetVerbose(Data))
      puts("new data available");
   /* read all system messages present on system socket */
   do {
      MrIpcInit(&CmdFrame);
      RcvReturnValue = MrIpcRecv(LogGetClientSock(Data), &CmdFrame);
      if (RcvReturnValue == MR_IPC_RCV_ERROR)
      {
         /* error in recieve from socket */
         if (LogGetVerbose(Data))
            puts("Error in recieve from socket!");
      }
      else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
      {
         /* socket was closed at remote side */
         if (LogGetVerbose(Data))
            puts("client socket was closed\nmaybe server has stoped");
         Loop = FALSE;
      }
      else
      {
         /* we have received data, process them */
         if (LogGetVerbose(Data))
            puts("read new comand frame from socket");
         ProcessSystemData(Data, &CmdFrame);
         MrIpcExit(&CmdFrame);
      }
   } while (RcvReturnValue > 0);
}

/**********************************************************************\
* Funktionsname: LogRun
*
* Kurzbeschreibung: this functions contains the main loop
*
* Parameter:
* Data : pointer to log object which contains all needed information
*
\**********************************************************************/
void LogRun(LogStruct *Data)
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
         if (LogGetVerbose(Data))
            printf("add client socket %d\n", LogGetClientSock(Data));
         FD_SET(LogGetClientSock(Data), &ReadFds);
         if (LogGetClientSock(Data) > HighFd)
            HighFd = LogGetClientSock(Data);
         /* 2. Timeout */
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (LogGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);

         /* wait for data */
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (LogGetVerbose(Data))
            printf("select liefert %d\n", RetVal);

         /* check, who has data */
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            /* timeout, time for periodic tasks */
            Now = time(NULL);
            if (LogGetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            /* error, finish loop */
            if (LogGetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            /* data, check at which fd */
            if (FD_ISSET(LogGetClientSock(Data), &ReadFds))
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
