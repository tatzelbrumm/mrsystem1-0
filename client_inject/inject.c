/**********************************************************************\
* Dateiname: inject.c
* Autor    : Michael Bernstein
* Projekt  : client_ms1
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: functionality for inject object which can inject
*                   message into the system
*
\**********************************************************************/

/*--- #includes der Form <...> ---------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <boolean.h>
#include <mr_ipc.h>
/*--- #includes der Form "..." ---------------------------------------*/
#include "inject.h"

/*--- Funktionsdefinitionen ------------------------------------------*/

/**********************************************************************\
* Funktionsname: InjectCreate
*
* Kurzbeschreibung: this function creates a inject object and
*                   initializes all members to zero.
*
* Rueckgabewert:
* NULL : creation failed, error
* else : pointer to created inject object
*
\**********************************************************************/
InjectStruct *InjectCreate(void)
{  InjectStruct *Data;

   Data = (InjectStruct *)malloc(sizeof(InjectStruct));
   if (Data != (InjectStruct *)NULL)
   {
      InjectSetVerbose(Data, FALSE);
      InjectSetServerIp(Data, (char *)NULL);
      InjectSetServerPort(Data, -1);
      InjectSetClientSock(Data, -1);
   }
   return(Data);
}

/**********************************************************************\
* Funktionsname: InjectDestroy
*
* Kurzbeschreibung: this function destroys a inject object, which was
*                   created by InjectCreate()
*
* Parameter:
* Data : pointer to inject object which should be destroyed
*
\**********************************************************************/
void InjectDestroy(InjectStruct *Data)
{
   if (InjectGetVerbose(Data))
      puts("destroy mrinject");
   if (InjectGetServerIp(Data) != (char *)NULL)
      free(InjectGetServerIp(Data));
   free(Data);
}

/**********************************************************************\
* Funktionsname: InjectInit
*
* Kurzbeschreibung: this function sets the values for this object to
*                   the right values for proper work
*
* Parameter:
* Data    : pointer to inject object which contains all needed information
* Verbose : should this module be verbose and print messages
* IpAddr  : ip address of server drehscheibe
* Port    : port of server drehscheibe
*
\**********************************************************************/
void InjectInit(InjectStruct *Data, BOOL Verbose, char *IpAddr, int Port)
{
   InjectSetVerbose(Data, Verbose);
   InjectSetServerIp(Data, IpAddr);
   InjectSetServerPort(Data, Port);
   InjectSetClientSock(Data, -1);
}

/**********************************************************************\
* Funktionsname: InjectExit
*
* Kurzbeschreibung: this function is the cleanup counterpart for the
*                   init function
*
* Parameter:
* Data    : pointer to inject object which contains all needed information
*
\**********************************************************************/
void InjectExit(InjectStruct *Data)
{
   if (InjectGetVerbose(Data))
      puts("exit mrinject");
   if (InjectGetServerIp(Data) != (char *)NULL)
      free(InjectGetServerIp(Data));
   InjectSetServerIp(Data, (char *)NULL);
}

/**********************************************************************\
* Funktionsname: InjectRun
*
* Kurzbeschreibung: this functions contains the main loop
*
* Parameter:
* Data : pointer to inject object which contains all needed information
*
\**********************************************************************/
void InjectRun(InjectStruct *Data)
{  MrIpcCmdType CmdFrame;

   InjectSetClientSock(Data,
                       MrIpcConnect(InjectGetServerIp(Data),
                                    InjectGetServerPort(Data)));
   if (InjectGetClientSock(Data) >= 0)
   {
      if (InjectGetVerbose(Data))
         puts("ready to send commands");
      MrIpcInit(&CmdFrame);
      MrIpcCmdSetNull(&CmdFrame);
      MrIpcSend(InjectGetClientSock(Data), &CmdFrame);
      MrIpcExit(&CmdFrame);
      MrIpcInit(&CmdFrame);
      MrIpcCmdSetLocomotiveDir(&CmdFrame, 1, Forward);
      MrIpcSend(InjectGetClientSock(Data), &CmdFrame);
      MrIpcExit(&CmdFrame);
      MrIpcInit(&CmdFrame);
      MrIpcCmdSetLocomotiveSpeed(&CmdFrame, 1, 25);
      MrIpcSend(InjectGetClientSock(Data), &CmdFrame);
      MrIpcExit(&CmdFrame);
      /* cleanup */
      if (InjectGetVerbose(Data))
         puts("stop network client");
      MrIpcClose(InjectGetClientSock(Data));
   }
}
