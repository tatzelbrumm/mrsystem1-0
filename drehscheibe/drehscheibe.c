/**********************************************************************\
* Dateiname: drehscheibe.c
* Autor    : Michael Bernstein
* Projekt  : client_ms1
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: functionality for drehscheibe object which forwards
*                   all message from a client to each other
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
#include <menge.h>
#include <stack.h>
#include <mr_ipc.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "drehscheibe.h"

/*--- #define-Konstanten und Makros ----------------------------------*/
#define SELECT_TIMEOUT 100

/*--- Modullokale Variablen ------------------------------------------*/
static BOOL Loop = TRUE;

/*--- Funktionsdefinitionen ------------------------------------------*/

/**********************************************************************\
* Funktionsname: DrehscheibeCreate
*
* Kurzbeschreibung: this function creates a rehscheibe object and
*                   initializes all members to zero.
*
* Rueckgabewert:
* NULL : creation failed, error
* else : pointer to created drehscheibe object
*
\**********************************************************************/
DrehscheibeStruct *DrehscheibeCreate(void)
{  DrehscheibeStruct *Data;

   Data = (DrehscheibeStruct *)malloc(sizeof(DrehscheibeStruct));
   if (Data != (DrehscheibeStruct *)NULL)
   {
      /* create data container menge which holds all clients */
      DrehscheibeSetClient(Data, MengeCreate());
      if (DrehscheibeGetClient(Data) != (Menge *)NULL)
      {
         /* create data container stack which for temporary datas */
         DrehscheibeSetRemove(Data, StackCreate());
         if (DrehscheibeGetRemove(Data) != (Stack *)NULL)
         {
            DrehscheibeSetVerbose(Data, FALSE);
            DrehscheibeSetServerIp(Data, (char *)NULL);
            DrehscheibeSetServerPort(Data, -1);
            DrehscheibeSetServerSock(Data, -1);
         }
         else
         {
            MengeDestroy(DrehscheibeGetClient(Data));
            free(Data);
            Data = (DrehscheibeStruct *)NULL;
         }
      }
      else
      {
         free(Data);
         Data = (DrehscheibeStruct *)NULL;
      }
   }
   return(Data);
}

/**********************************************************************\
* Funktionsname: DrehscheibeDestroy
*
* Kurzbeschreibung: this function destroys a drehscheibe object, which
*                   was created by DrehscheibeCreate()
*
* Parameter:
* Data : pointer to drehscheibe object which should be destroyed
*
\**********************************************************************/
void DrehscheibeDestroy(DrehscheibeStruct *Data)
{
   if (DrehscheibeGetVerbose(Data))
      puts("destroy drehscheibe");
   free(DrehscheibeGetServerIp(Data));
   StackDestroy(DrehscheibeGetRemove(Data));
   MengeDestroy(DrehscheibeGetClient(Data));
   free(Data);
}

/**********************************************************************\
* Funktionsname: SocketClientsCompare
*
* Kurzbeschreibung: this function is used as a callback for the data
*                   container menge and compares 2 keys
*
* Parameter:
* d1 : first parameter to compare
* d2 : second parameter to compare
*
* Rueckgabewert:
* <0  : d1 < d2
* ==0 : d1 == d2
* >0  : d1 > d2
*
\**********************************************************************/
static int SocketClientsCompare(void *d1, void *d2)
{
   return((long)d1 - (long)d2);
}

/**********************************************************************\
* Funktionsname: StackNullDel
*
* Kurzbeschreibung: this function is uses as a callback for the data
*                   container stack and is called for each element,
*                   which was removed from teh stack. Because the data
*                   container menge also has a pointer to the data, it
*                   is nothing to do
*
* Parameter:
* Data : data part from stack to free
*
\**********************************************************************/
static void StackNullDel(StackDataType Data)
{
}

/**********************************************************************\
* Funktionsname: DrehscheibeInit
*
* Kurzbeschreibung: this function sets the values for this object to
*                   the right values for proper work
*
* Parameter:
* Data    : pointer to drehscheibe object which contains all needed information
* Verbose : should this module be verbose and print messages
* IpAddr  : ip address of server drehscheibe
* Port    : port of server drehscheibe
*
\**********************************************************************/
void DrehscheibeInit(DrehscheibeStruct *Data, BOOL Verbose, char *IpAddr,
                     int Port)
{
   DrehscheibeSetVerbose(Data, Verbose);
   DrehscheibeSetServerIp(Data, IpAddr);
   DrehscheibeSetServerPort(Data, Port);
   DrehscheibeSetServerSock(Data, -1);
   MengeInit(DrehscheibeGetClient(Data), SocketClientsCompare, free);
   StackInit(DrehscheibeGetRemove(Data), StackNullDel);
}

/**********************************************************************\
* Funktionsname: DrehscheibeExit
*
* Kurzbeschreibung: this function is the cleanup counterpart for the
*                   init function
*
* Parameter:
* Data    : pointer to drehscheibe object which contains all needed information
*
\**********************************************************************/
void DrehscheibeExit(DrehscheibeStruct *Data)
{
   if (DrehscheibeGetVerbose(Data))
      puts("exit drehscheibe");
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
* Data : pointer to drehscheibe object which contains all needed information
*
* Rueckgabewert:
* TRUE  : all needed for main loop was successfull started
* FALSE : start failed
*
\**********************************************************************/
static BOOL Start(DrehscheibeStruct *Data)
{  struct sigaction SigStruct;

   DrehscheibeSetServerSock(Data,
                            MrIpcStartServer(DrehscheibeGetServerIp(Data),
                                             DrehscheibeGetServerPort(Data)));
   if (DrehscheibeGetServerSock(Data) >= 0)
   {
      if (DrehscheibeGetVerbose(Data))
         puts("ready for incoming connections");
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
* Data : pointer to drehscheibe object which contains all needed information
*
\**********************************************************************/
static void Stop(DrehscheibeStruct *Data)
{
   if (DrehscheibeGetVerbose(Data))
      puts("stop network server");
   if (DrehscheibeGetServerSock(Data) >= 0)
   {
      MrIpcClose(DrehscheibeGetServerSock(Data));
   }
}

/**********************************************************************\
* Funktionsname: AddNewClient
*
* Kurzbeschreibung: this functions add a new client to the management
*                   data to allow also to inform this client about
*                   system messages from clients
*
* Parameter:
* Data : pointer to drehscheibe object which contains all needed information
*
\**********************************************************************/
static void AddNewClient(DrehscheibeStruct *Data)
{  DrehscheibeClientStruct *SocketEntry;
   int NewClientSock;

   if (DrehscheibeGetVerbose(Data))
      puts("new incoming connection");
   NewClientSock = MrIpcAccept(DrehscheibeGetServerSock(Data));
   SocketEntry = (DrehscheibeClientStruct *)malloc(sizeof(DrehscheibeClientStruct));
   if (SocketEntry != (DrehscheibeClientStruct *)NULL)
   {
      /* we got one, accept */
      DrehclientSetSock(SocketEntry, NewClientSock);
      if (DrehscheibeGetVerbose(Data))
         printf("accept new connection %d\n", DrehclientGetSock(SocketEntry));
      MengeAdd(DrehscheibeGetClient(Data), (MengeDataType)SocketEntry);
   }
   else
   {
      /* can not create client struct, reject */
      if (DrehscheibeGetVerbose(Data))
         puts("reject new connection");
      MrIpcClose(NewClientSock);
   }
}

/**********************************************************************\
* Funktionsname: ProcessSystemData
*
* Kurzbeschreibung: this function evaluates one command frame from the
*                   system (drehscheibe) and do the proper reaction
*
* Parameter:
* Data        : pointer to drehscheibe object which contains all needed information
* CmdFrame    : one command frame from the system
* ClientEntry : struct for client, from which the data is present
*
\**********************************************************************/
static void ProcessSystemData(DrehscheibeStruct *Data, MrIpcCmdType *CmdFrame,
                              DrehscheibeClientStruct *ClientEntry)
{  MengeIterator ClientIter;
   DrehscheibeClientStruct *RcvClientEntry;

   MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
   RcvClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
   while (RcvClientEntry != (DrehscheibeClientStruct *)NULL)
   {
      if (DrehclientGetSock(ClientEntry) != DrehclientGetSock(RcvClientEntry))
      {
         /* send only to other clients */
         if (DrehscheibeGetVerbose(Data))
            printf("send to client with socket %d\n",
                   DrehclientGetSock(RcvClientEntry));
         MrIpcSend(DrehclientGetSock(RcvClientEntry), CmdFrame);
      }
      RcvClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
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
* Data        : pointer to ms1 object which contains all needed information
* ClientEntry : struct for client, from which the data is present
*
* Rueckgabewert:
* TRUE  : client should be removed because socket to client was closed
* FALSE : dont remove client
*
\**********************************************************************/
static BOOL HandleSystemData(DrehscheibeStruct *Data,
                             DrehscheibeClientStruct *ClientEntry)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;
   BOOL Ret;

   if (DrehscheibeGetVerbose(Data))
      printf("data on client socket %d available\n",
             DrehclientGetSock(ClientEntry));
   do {
      MrIpcInit(&CmdFrame);
      RcvReturnValue = MrIpcRecv(DrehclientGetSock(ClientEntry), &CmdFrame);
      if (RcvReturnValue == MR_IPC_RCV_ERROR)
      {
         /* error in recieve from socket */
         if (DrehscheibeGetVerbose(Data))
            puts("Error in recieve from socket!");
         Ret = FALSE;
      }
      else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
      {
         /* socket was closed at remote side */
         if (DrehscheibeGetVerbose(Data))
            puts("client socket was closed");
         MrIpcClose(DrehclientGetSock(ClientEntry));
         Ret = TRUE;
      }
      else
      {
         /* we have received data, process them */
         if (DrehscheibeGetVerbose(Data))
            printf("read new comand frame from socket\n%s\n",
                   MrIpcGetRendered(&CmdFrame));
         ProcessSystemData(Data, &CmdFrame, ClientEntry);
         MrIpcExit(&CmdFrame);
         Ret = FALSE;
      }
   } while (RcvReturnValue > 0);
   return(Ret);
}

/**********************************************************************\
* Funktionsname: DrehsacheibeRun
*
* Kurzbeschreibung: this functions contains the main loop
*
* Parameter:
* Data : pointer to drehscheibe object which contains all needed information
*
\**********************************************************************/
void DrehscheibeRun(DrehscheibeStruct *Data)
{  fd_set ReadFds;
   int RetVal, HighFd;
   struct timeval SelectTimeout;
   DrehscheibeClientStruct *ClientEntry;
   MengeIterator ClientIter;
   time_t Now;

   if (Start(Data))
   {
      while (Loop)
      {
         /* Main loop for receive and send data */

         /* fill fd set for call to select */
         FD_ZERO(&ReadFds);
         HighFd = 0;
         /* 1. server socket */
         if (DrehscheibeGetVerbose(Data))
            printf("add server socket %d\n", DrehscheibeGetServerSock(Data));
         FD_SET(DrehscheibeGetServerSock(Data), &ReadFds);
         if (DrehscheibeGetServerSock(Data) > HighFd)
            HighFd = DrehscheibeGetServerSock(Data);
         /* 2. client sockets */
         MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
         ClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
         while (ClientEntry != (DrehscheibeClientStruct *)NULL)
         {
            if (DrehscheibeGetVerbose(Data))
               printf("add client socket %d\n", DrehclientGetSock(ClientEntry));
            FD_SET(DrehclientGetSock(ClientEntry), &ReadFds);
            if (DrehclientGetSock(ClientEntry) > HighFd)
                  HighFd = DrehclientGetSock(ClientEntry);
            ClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
         }
         /* 3. Timeout */
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (DrehscheibeGetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);

         /* wait for data */
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (DrehscheibeGetVerbose(Data))
            printf("select liefert %d\n", RetVal);

         /* check, who has data */
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            /* timeout, time for periodic tasks */
            Now = time(NULL);
            if (DrehscheibeGetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            /* error, check if some clients disconnect */
            if (DrehscheibeGetVerbose(Data))
               puts("error in main loop");
            /* 1) check clients for closed socket */
            MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
            ClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
            while (ClientEntry != (DrehscheibeClientStruct *)NULL)
            {
               /* check for client socket for data */
               if (HandleSystemData(Data, ClientEntry))
               {
                  StackPush(DrehscheibeGetRemove(Data),
                            (MengeDataType)ClientEntry);
               }
               ClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
            }
            /* remove clients with closed socket */
            while (!StackIsEmpty(DrehscheibeGetRemove(Data)))
            {
               ClientEntry = (DrehscheibeClientStruct *)StackPop(DrehscheibeGetRemove(Data));
               MengeRemove(DrehscheibeGetClient(Data),
                           (MengeDataType)ClientEntry);
            }
         }
         else
         {
            /* data, check at which fd */
            /* 1) check server socket */
            if (FD_ISSET(DrehscheibeGetServerSock(Data), &ReadFds))
            {
               /* new connection requested */
               AddNewClient(Data);
            }
            /* 2) check clients for data */
            MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
            ClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
            while (ClientEntry != (DrehscheibeClientStruct *)NULL)
            {
               /* check for client socket for data */
               if (FD_ISSET(DrehclientGetSock(ClientEntry), &ReadFds))
               {
                  if (HandleSystemData(Data, ClientEntry))
                  {
                     StackPush(DrehscheibeGetRemove(Data),
                               (MengeDataType)ClientEntry);
                  }
               }
               ClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
            }
            /* remove closed clients */
            while (!StackIsEmpty(DrehscheibeGetRemove(Data)))
            {
               ClientEntry = (DrehscheibeClientStruct *)StackPop(DrehscheibeGetRemove(Data));
               MengeRemove(DrehscheibeGetClient(Data),
                           (MengeDataType)ClientEntry);
            }
         }
      }

      /* cleanup */
      if (DrehscheibeGetVerbose(Data))
         puts("close all clients and stop system");
      MengeInitIterator(&ClientIter, DrehscheibeGetClient(Data));
      ClientEntry = (DrehscheibeClientStruct *)MengeFirst(&ClientIter);
      while (ClientEntry != (DrehscheibeClientStruct *)NULL)
      {
         MrIpcClose(DrehclientGetSock(ClientEntry));
         DrehclientSetSock(ClientEntry, -1);
         ClientEntry = (DrehscheibeClientStruct *)MengeNext(&ClientIter);
      }
      Stop(Data);
   }
}
