/**********************************************************************\
* Dateiname: ms1.c
* Autor    : Daniel Prein, Michael Bernstein
* Projekt  : client_ms1
*
* Copyright (C) Daniel Prein, Gerhard Bertelsmann, Michael Bernstein
*
* Kurzbeschreibung: functionality for ms1 object which handles the
*                   message flow betwween ms1 and drehscheibe
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
#include "ms1.h"
#include "maerklin.h"

/*--- #define-Konstanten und Makros ----------------------------------*/
#define SELECT_TIMEOUT 100
#define MAXDG   4096	/* maximum datagram size */
#define MAXUDP  16	/* maximum datagram size */

/*--- Modullokale Variablen ------------------------------------------*/
static BOOL Loop = TRUE;

/*--- Funktionsdefinitionen ------------------------------------------*/

/**********************************************************************\
* Funktionsname: Ms1Create
*
* Kurzbeschreibung: this function creates a ms1 object and initializes
*                   all members to zero.
*
* Rueckgabewert:
* NULL : creation failed, error
* else : pointer to created ms1 object
*
\**********************************************************************/
Ms1Struct *Ms1Create(void)
{  Ms1Struct *Data;

   Data = (Ms1Struct *)malloc(sizeof(Ms1Struct));
   if (Data != (Ms1Struct *)NULL)
   {
      Ms1SetVerbose(Data, FALSE);
      Ms1SetServerIp(Data, (char *)NULL);
      Ms1SetServerPort(Data, -1);
      Ms1SetClientSock(Data, -1);
      Ms1SetCanName(Data, strdup("can0"));
      Ms1SetCanSock(Data, -1);
   }
   return(Data);
}

/**********************************************************************\
* Funktionsname: Ms1Destroy
*
* Kurzbeschreibung: this function destroys a ms1 object, which was
*                   created by Ms1Create()
*
* Parameter:
* Data : pointer to ms1 object which should be destroyed
*
\**********************************************************************/
void Ms1Destroy(Ms1Struct *Data)
{
   if (Ms1GetVerbose(Data))
      puts("destroy ms1");
   if (Ms1GetServerIp(Data) != (char *)NULL)
      free(Ms1GetServerIp(Data));
   if (Ms1GetCanName(Data) != (char *)NULL)
      free(Ms1GetCanName(Data));
   free(Data);
}

/**********************************************************************\
* Funktionsname: Ms1Init
*
* Kurzbeschreibung: this function sets the values for this object to
*                   the right values for proper work
*
* Parameter:
* Data    : pointer to ms1 object which contains all needed information
* Verbose : should this module be verbose and print messages
* IpAddr  : ip address of server drehscheibe
* Port    : port of server drehscheibe
* CanIf   : interface name of can interface
*
\**********************************************************************/
void Ms1Init(Ms1Struct *Data, BOOL Verbose, char *IpAddr, int Port, char *CanIf)
{
   Ms1SetVerbose(Data, Verbose);
   Ms1SetServerIp(Data, IpAddr);
   Ms1SetServerPort(Data, Port);
   Ms1SetClientSock(Data, -1);
   Ms1SetCanSock(Data, -1);
   if (CanIf != (char *)NULL)
   {
      Ms1SetCanName(Data, CanIf);
   }
}

/**********************************************************************\
* Funktionsname: Ms1Exit
*
* Kurzbeschreibung: this function is the cleanup counterpart for the
*                   init function
*
* Parameter:
* Data    : pointer to ms1 object which contains all needed information
*
\**********************************************************************/
void Ms1Exit(Ms1Struct *Data)
{
   if (Ms1GetVerbose(Data))
      puts("exit ms1");
   if (Ms1GetServerIp(Data) != (char *)NULL)
      free(Ms1GetServerIp(Data));
   Ms1SetServerIp(Data, (char *)NULL);
   if (Ms1GetCanName(Data) != (char *)NULL)
      free(Ms1GetCanName(Data));
   Ms1SetCanName(Data, (char *)NULL);
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
* Data : pointer to ms1 object which contains all needed information
*
* Rueckgabewert:
* TRUE  : all needed for main loop was successfull started
* FALSE : start failed
*
\**********************************************************************/
static BOOL Start(Ms1Struct *Data)
{  struct sigaction SigStruct;
   struct sockaddr_in baddr;
   const int on = 1;

   /* open socket ot system drehscheibe */
   Ms1SetClientSock(Data,
                    MrIpcConnect(Ms1GetServerIp(Data),
                                 Ms1GetServerPort(Data)));
   if (Ms1GetClientSock(Data) >= 0)
   {
      /* open socket to can bus */
      Ms1SetCanSock(Data, MrMs1Connect(Ms1GetCanName(Data)));
      if (Ms1GetCanSock(Data) >= 0)
      {
         if (Ms1GetVerbose(Data))
            puts("ready for incoming comands from system");
         /* set signal handler */
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
         MrIpcClose(Ms1GetClientSock(Data));
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
* Data : pointer to ms1 object which contains all needed information
*
\**********************************************************************/
static void Stop(Ms1Struct *Data)
{
   if (Ms1GetVerbose(Data))
      puts("stop network client");
   /* close all sockets */
   MrIpcClose(Ms1GetClientSock(Data));
   MrMs1Close(Ms1GetCanSock(Data));
}

/**********************************************************************\
* Funktionsname: ProcessSystemData
*
* Kurzbeschreibung: this function evaluates one command frame from the
*                   system (drehscheibe) and do the proper reaction
*
* Parameter:
* Data : pointer to ms1 object which contains all needed information
* CmdFrame : one command frame from the system
*
\**********************************************************************/
static void ProcessSystemData(Ms1Struct *Data, MrIpcCmdType *CmdFrame)
{  struct can_frame CanFrame;

/* TODO: encode can frame from CmdFrame */
   MrMs1Send(Ms1GetCanSock(Data), &CanFrame);
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
static void HandleSystemData(Ms1Struct *Data)
{  MrIpcCmdType CmdFrame;
   int RcvReturnValue;

   if (Ms1GetVerbose(Data))
      puts("new data available");
   /* read all system messages present on system socket */
   do {
      MrIpcInit(&CmdFrame); /* init our receive struct */
      RcvReturnValue = MrIpcRecv(Ms1GetClientSock(Data), &CmdFrame);
      if (RcvReturnValue == MR_IPC_RCV_ERROR)
      {
         /* error in receive from socket */
         if (Ms1GetVerbose(Data))
            puts("Error in recieve from socket!");
      }
      else if (RcvReturnValue == MR_IPC_RCV_CLOSED)
      {
         /* socket was closed at remote side */
         if (Ms1GetVerbose(Data))
            puts("client socket was closed\nmaybe server has stoped");
         Loop = FALSE;
      }
      else
      {
         /* process the received system frame */
         if (Ms1GetVerbose(Data))
            puts("read new comand frame from socket");
         ProcessSystemData(Data, &CmdFrame);
         MrIpcExit(&CmdFrame); /* cleanup our struct for next data */
      }
   } while (RcvReturnValue > 0);
}

static int get_wert_for_index (int index) {

    switch (index) {
        case 1:
            return(0x00); //GO
            break;

        case 2:
            return(0x00); //kein Kurzschluss
            break;

        case 3:
            return(0x00);
            break;

        case 4:
            return(0x01); //Normalbetrieb
            break;

        default:
            break;
    }

}

static void split_canid(struct ms1_handle *handle, uint32_t id) {
    bzero(handle,sizeof(struct ms1_handle));
    handle->prio=(id >> 26) & 0x03;
    if (handle->prio == 3) {
        handle->node = id & 0x7FU;
        id = id >> 7;
        handle->direction = id & 0x01U;
        id = id >> 1;
        handle->MID = id & 0x7FU;
        id = id >> 7;
        handle->stage = id & 0x07U;
        id = id >> 3;
        handle->UID = id & 0xFFU;
    } else {
        handle->node = id & 0x7FU;
        id = id >> 7;
        handle->command = id & 0x7FU;
        id = id >> 3;
        handle->object_handle= id & 0xFFFF;
    }
}

/**********************************************************************\
* Funktionsname: ProcessCanData
*
* Kurzbeschreibung: this function evaluates one can frame from the ms1
*                   and do the proper reaction
*
* Parameter:
* Data : pointer to ms1 object which contains all needed information
* frame : one can frame from the ms1
*
\**********************************************************************/
static void ProcessCanData(Ms1Struct *Data, struct can_frame *frame)
{  MrIpcCmdType CmdFrame;
   struct can_frame out_msg;
   int canid, uid, nbytes, i, udpsend, sc;
   unsigned char udpframe[MAXDG];
   int ms_uid[4];
   int statustyp[64];
   int lastmfxnr = 4;
   struct ms1_handle my_ms1_handle;

   MrIpcInit(&CmdFrame); /* init the struct to the system */
      canid = 0;
/* TODO: set Cmd and send to drehscheibe instead of call to write */
			if (frame->can_id & CAN_EFF_FLAG) { // only EFF frames are valid
			
				
			// prepare UDP frame 
			bzero(udpframe,13);
			frame->can_id &= CAN_EFF_MASK;
			udpframe[0] = (frame->can_id >> 24) & 0x000000FF;
			udpframe[1] = (frame->can_id >> 16) & 0x000000FF;
			udpframe[2] = (frame->can_id >>  8) & 0x000000FF;
			udpframe[3] = frame->can_id & 0x000000FF;
			udpframe[4] = frame->can_dlc;
			memcpy(&udpframe[5],&frame->data,frame->can_dlc);
				
			//CAN Frame ID zerlegen
            split_canid(&my_ms1_handle, frame->can_id);


            // Slave Anmeldung Mobile Station erkannt
			if (my_ms1_handle.prio == 0x07 /*0b111*/) {
				
				udpsend = 0; //dieser Frame soll nicht per UDP weitergeleitet werden
				
				switch(my_ms1_handle.stage) {
				
					case 0x00 : //Stufe 0
						
						printf("found MS1 slave device\n");
						printf("<- 0x%X Stufe 0\n",frame->can_id);
				
						ms_uid[3] = (frame->can_id & 0x3FC0000) >> 18;
						//printf("MS UID[3]: 0x%X\n",ms_uid[3]);
				
						// sende Quittung Stufe 0
						out_msg.can_id = frame->can_id ^ 0x80;  //MID=0
						
						printf("-> 0x%X Stufe 0 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x01; //Master UID[3]
						out_msg.data[1] = 0x01; //Master UID[2]
						out_msg.data[2] = 0x01; //Master UID[1]
						out_msg.data[3] = 0x01; //Master UID[0]
						out_msg.data[4] = 0x00;
						out_msg.data[5] = 0x01;
						out_msg.data[6] = 0x00;
						out_msg.data[7] = 0x00; //MS-Master
					
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}
						
						break;
					
					case 0x04 /*0b00100*/ : //Stufe 1
						
						printf("<- 0x%X Stufe 1\n",frame->can_id);
						
						ms_uid[2] = (frame->can_id & 0x3FC0000) >> 18;
						//printf("MS UID[2]: 0x%X\n",ms_uid[2]);
						
						// sende Quittung Stufe 1
						out_msg.can_id = frame->can_id ^ 0x80; //MID=0
						
						printf("-> 0x%X Stufe 1 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x00; 
						out_msg.data[1] = 0x00;
						out_msg.data[2] = 0x00;
						out_msg.data[3] = 0x97;
						out_msg.data[4] = 0x01; //MS-Master
						out_msg.data[5] = 0x00; //MS-Master
						out_msg.data[6] = 0xc0; //MS-Master
						out_msg.data[7] = 0x10; //MS-Master
					
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}
						
						break;
						
					case 0x08 /*0b01000*/ : //Stufe 2
						
						printf("<- 0x%X Stufe 2\n",frame->can_id);
						
						ms_uid[1] = (frame->can_id & 0x3FC0000) >> 18;
						//printf("MS UID[1]: 0x%X\n",ms_uid[1]);
						
						// sende Quittung Stufe 2
						out_msg.can_id = frame->can_id ^ 0x80; //MID=0
						
						printf("-> 0x%X Stufe 2 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x01; 
						out_msg.data[1] = 0x00;
						out_msg.data[2] = 0x00;
						out_msg.data[3] = 0x69;
						out_msg.data[4] = 0x00;
						out_msg.data[5] = 0x00;
						out_msg.data[6] = 0x40;
						out_msg.data[7] = 0x10; 
						
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}						
						
						break;
				
						
					case 0x0c /*0b01100*/ : // Stufe 3
						
						printf("<- 0x%X Stufe 3\n",frame->can_id);
						
						ms_uid[0] = (frame->can_id & 0x3FC0000) >> 18;
						//printf("MS UID[0]: 0x%X\n",ms_uid[0]);
						
						// sende Quittung Stufe 3
						out_msg.can_id = frame->can_id ^ 0x80; //MID=0
						
						printf("-> 0x%X Stufe 3 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x00; 
						out_msg.data[1] = 0x00;
						out_msg.data[2] = 0x40;
						out_msg.data[3] = 0x10;
						out_msg.data[4] = 0x01;
						out_msg.data[5] = 0x03;
						out_msg.data[6] = 0x00;
						out_msg.data[7] = 0x84; //MS-Master
						
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");							
						}
												
						break;	
						
					case 0x10 /*0b10000*/ : //Stufe 4
						printf("<- 0x%X Stufe 4\n",frame->can_id);
						break;
						
					case 0x14 /*0b10100*/ : //Stufe 5
						printf("<- 0x%X Stufe 5\n",frame->can_id);
						break;
						
					case 0x18 /*0b11000*/ : //Stufe 6
						printf("<- 0x%X Stufe 6\n",frame->can_id);
						break;
						
					case 0x1c /*0b11100*/ : //Stufe 7
						
						printf("<- 0x%X Stufe 7\n",frame->can_id);
						
						// sende Quittung Stufe 7
						out_msg.can_id = frame->can_id ^ 0x80; //MID=0
						
						printf("-> 0x%X Stufe 7 Quittung\n",out_msg.can_id);
						
						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = ms_uid[3];
						out_msg.data[1] = ms_uid[2];
						out_msg.data[2] = ms_uid[1];
						out_msg.data[3] = ms_uid[0];
						out_msg.data[4] = 0x00;
						out_msg.data[5] = 0x01;
						out_msg.data[6] = 0x01; //Knotennr. bei CS eigentlich min 3 
						out_msg.data[7] = 0x00; //ACHTUNG!!! 0x01 = Update!!! NIE BENUTZEN!!!
						
						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}
						
						break;
						
				} // Ende Switch

			}

			// Aufruestung MS
			if (my_ms1_handle.prio == 0x06 /*0b110*/) {
				
				udpsend = 0; //dieser Frame soll nicht per UDP weitergeleitet werden
				
				// welcher Befehl wurde gesendet
                switch (my_ms1_handle.MID) {

                    case 0x01 /*0b001*/ : 

                        switch (frame->data[1]) {

                            case 0x03 : // Get Lok Typ

                                printf("<- 0x%X Get Lok Type\n",frame->can_id);

                                out_msg.can_id = frame->can_id - 1;

                                printf("-> 0x%X Send Lok Type\n",out_msg.can_id);

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;
                                out_msg.can_dlc = 6;
                                out_msg.data[0] = frame->data[0];
                                out_msg.data[1] = frame->data[1];
                                out_msg.data[2] = frame->data[2];
                                out_msg.data[3] = frame->data[3];
                                out_msg.data[4] = 0x00;
                                out_msg.data[5] = 0x03; //Dampflok

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                break;

                            case 0x01 : // Schienenformat

                                printf("<- 0x%X Request Schienenformat\n",frame->can_id);

                                out_msg.can_id = frame->can_id - 1;

                                printf("-> 0x%X Send Schienenformat\n",out_msg.can_id);

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;
                                out_msg.can_dlc = 8;
                                out_msg.data[0] = frame->data[0];
                                out_msg.data[1] = frame->data[1];
                                out_msg.data[2] = frame->data[2];
                                out_msg.data[3] = frame->data[3];
                                out_msg.data[4] = 0x00;
                                out_msg.data[6] = 0x00;

                                if (frame->data[2] == 0x00) {
                                    out_msg.data[5] = 0x03;
                                    out_msg.data[7] = 0x03;
                                }

                                if (frame->data[2] == 0x01) {
                                    out_msg.data[5] = 0x4E;
                                    out_msg.data[7] = 0x00;
                                }

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                break;

                        }



                        break;


					case 0x04 /*0b100*/ : // Anfrage bzgl. Slave Description

						printf("<- 0x%X Anfrage Slave Description\n",frame->can_id);

						out_msg.can_id = frame->can_id - 1;

						printf("-> 0x%X Anfrage Slave Description Quittung\n",out_msg.can_id);

						out_msg.can_id &= CAN_EFF_MASK;
						out_msg.can_id |= CAN_EFF_FLAG;
						out_msg.can_dlc = 8;
						out_msg.data[0] = 0x40;;
						out_msg.data[1] = frame->data[1];
						out_msg.data[2] = 0x00;
						out_msg.data[3] = 0x00;
						out_msg.data[4] = 0x00; //Obj H
						out_msg.data[5] = 0x01;	//Obj L
						out_msg.data[6] = 0x00;
						out_msg.data[7] = 0x01;

						if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
							perror("CAN write error");
						}

						break;

                     case 0x03 /*0b011*/ :

                        // System Status-Handle oder Loksat?

                        switch (frame->data[0]) {

                            case 0x03 : // Anfrage bzgl. System-Status-Handle

                                printf("<- 0x%X Anfrage System-Status-Handle\n",frame->can_id);

                                out_msg.can_id = frame->can_id - 1;

                                printf("-> 0x%X Anfrage System-Status-Handle Quittung\n",out_msg.can_id);

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;
                                out_msg.can_dlc = 8;
                                out_msg.data[0] = 0x03;
                                out_msg.data[1] = 0x00;
                                out_msg.data[2] = 0x00;
                                out_msg.data[3] = 0x00;
                                out_msg.data[4] = 0x00; //Obj H
                                out_msg.data[5] = 0x02;	//Obj L
                                out_msg.data[6] = 0x00;
                                out_msg.data[7] = 0x01;

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                break;

                            case 0x80 : // Lokstat

                                //dirty hack! Es wird testweise eine einzige Lok übertragen

                                if (frame->data[2] == 0x00) { //erste Anfrage, noch keine Lok. Eine Lok wird gesendet

                                    printf("<- 0x%X Lokstat Austausch (1)\n",frame->can_id);

                                    out_msg.can_id = frame->can_id - 1;

                                    printf("-> 0x%X Lokstat Austausch (1)\n",out_msg.can_id);

                                    out_msg.can_id &= CAN_EFF_MASK;
                                    out_msg.can_id |= CAN_EFF_FLAG;
                                    out_msg.can_dlc = 8;
                                    out_msg.data[0] = frame->data[0];
                                    out_msg.data[1] = frame->data[1];
                                    out_msg.data[2] = 0x00;
                                    out_msg.data[3] = 0x00;
                                    out_msg.data[4] = 0x02;
                                    out_msg.data[5] = 0x02;
                                    out_msg.data[6] = 0x00;
                                    out_msg.data[7] = 0x01;
                                }

                                if (frame->data[2] == 0x02) { //bereits eine Lok vorhanden, wir senden keine neue

                                    printf("<- 0x%X Lokstat Austausch (2)\n",frame->can_id);

                                    out_msg.can_id = frame->can_id - 1;

                                    printf("-> 0x%X Lokstat Austausch (2)\n",out_msg.can_id);

                                    out_msg.can_id &= CAN_EFF_MASK;
                                    out_msg.can_id |= CAN_EFF_FLAG;
                                    out_msg.can_dlc = 4;
                                    out_msg.data[0] = frame->data[0];
                                    out_msg.data[1] = frame->data[1];
                                    out_msg.data[2] = frame->data[2];
                                    out_msg.data[3] = frame->data[3];
                                }

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                break;
                            }

                        break;



                    case 0x02 /*0b010*/ : // Anfrage bzgl. System-Status-Austausch

                        printf("<- 0x%X Anfrage System-Status-Austausch\n",frame->can_id);


                        if (frame->data[1] == 0x01) { //Anfrage nach dem Statustyp

                            if (frame->data[0] <= 0x04) {

                                //Verknüpfung Index und Statustyp an Slave senden
                                out_msg.can_id = frame->can_id - 1;

                                printf("-> 0x%X Sende Statustyp\n",out_msg.can_id);

                                out_msg.can_dlc = 3;
                                out_msg.data[0] = frame->data[0]; //INDEX
                                out_msg.data[1] = 0x01;
                                out_msg.data[2] = frame->data[0]; //eigentlich STATUSTYP

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                statustyp[frame->data[0]] = out_msg.data[2];  //Verknüpfung Index und Statustyp speichern

                                if (frame->data[0] == 0x03) { // 0x03 ubekannter STATUSTYP, es wird direkt der WERT gesendet
                                    goto Wertsenden;
                                }
                            }


                            if (frame->data[0] == 0x05) {

                                //Verknüpfung Index und Statustyp an Slave senden
                                out_msg.can_id = frame->can_id - 1;

                                printf("-> 0x%X Sende Statustyp\n",out_msg.can_id);

                                out_msg.can_dlc = 2;
                                out_msg.data[0] = frame->data[0]; //INDEX
                                out_msg.data[1] = 0x01;

                                out_msg.can_id &= CAN_EFF_MASK;
                                out_msg.can_id |= CAN_EFF_FLAG;

                                if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                    perror("CAN write error");
                                }

                                statustyp[frame->data[0]] = out_msg.data[2];  //Verknüpfung Index und Statustyp speichern

                            }

                        }


                        if (frame->data[1] == 0x00) { //Anfrage nach dem Wert

                            Wertsenden:

                            printf("-> 0x%X Sende Statustyp\n",out_msg.can_id);

                            //Wert für Index an Slave senden
                            out_msg.can_id = frame->can_id - 1;

                            out_msg.can_dlc = 3;
                            out_msg.data[0] = frame->data[0]; //Index
                            out_msg.data[1] = 0x00;
                            out_msg.data[2] = get_wert_for_index(frame->data[0]);

                            out_msg.can_id &= CAN_EFF_MASK;
                            out_msg.can_id |= CAN_EFF_FLAG;

                            if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                                perror("CAN write error");
                            }

                        }

                        break;


				} // Ende Switch

			}

			// Zyklische Überwachung
			if (my_ms1_handle.prio == 0x03 /*0b011*/) {

                if (frame->can_dlc == 4) {

                    printf("<- 0x%X Ping 1\n",frame->can_id);

                    out_msg.can_id = frame->can_id - 1;
                    //out_msg.can_id = 0xC000380;  //Master Knoten ist 00

                    printf("-> 0x%X Pong 1\n",out_msg.can_id);

                    out_msg.can_id &= CAN_EFF_MASK;
                    out_msg.can_id |= CAN_EFF_FLAG;
                    out_msg.can_dlc = 8;
                    out_msg.data[0] = 0x00;
                    out_msg.data[1] = 0x00;
                    out_msg.data[2] = 0x00;
                    out_msg.data[3] = frame->data[3];
                    out_msg.data[4] = 0x01; //master uid 3
                    out_msg.data[5] = 0x01; //master uid 2
                    out_msg.data[6] = 0x01; //master uid 1
                    out_msg.data[7] = 0x01; //master uid 0



                    if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                        perror("CAN write error");
                    }

                    out_msg.can_id = frame->can_id - 1;
                    //out_msg.can_id = 0xC000380;

                    printf("-> 0x%X Pong 2\n",out_msg.can_id);

                    out_msg.can_id &= CAN_EFF_MASK;
                    out_msg.can_id |= CAN_EFF_FLAG;
                    out_msg.can_dlc = 8;
                    out_msg.data[0] = 0x00;
                    out_msg.data[1] = 0x00;
                    out_msg.data[2] = 0x00;
                    out_msg.data[3] = 0x01;  //frame->data[3];
                    out_msg.data[4] = ms_uid[3];
                    out_msg.data[5] = ms_uid[2];
                    out_msg.data[6] = ms_uid[1];
                    out_msg.data[7] = ms_uid[0];

                    if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
                        perror("CAN write error");
                    }

                }

                if (frame->can_dlc == 8) {

                    printf("<- 0x%X Ping 2\n",frame->can_id);

                }

			}

			// Das Programm reagiert auf die Erkennung eines mfx-Dekoders und vergibt eine SID (Befehl 3.2: MFX Bind)
			if (((frame->can_id & 0xFFFF0000) == 0x00030000UL) && ( frame->can_dlc== 5)) {

				udpsend = 1;

/* this should be done in client_zentrale */
				printf("CAN>UDP MFX 0x%06X\n",frame->can_id);
				lastmfxnr = lastmfxnr + 1;

				out_msg.can_id = 0x040301UL;
				out_msg.can_id &= CAN_EFF_MASK;
				out_msg.can_id |= CAN_EFF_FLAG;
				out_msg.can_dlc = 6;
				out_msg.data[0] = frame->data[0];
				out_msg.data[1] = frame->data[1];
				out_msg.data[2] = frame->data[2];
				out_msg.data[3] = frame->data[3];
				out_msg.data[4] = (lastmfxnr >> 8) & 0x00FF;
				out_msg.data[5] = lastmfxnr & 0x00FF;

				if ((nbytes = write(sc, &out_msg, sizeof(out_msg))) != sizeof(out_msg)) {
					perror("CAN write error MFX Nr __");
				}

				uid = frame->data[3] | (frame->data[2] << 8) | (frame->data[1] << 16) | (frame->data[0] << 24);

				udpframe[0] = (out_msg.can_id >> 24) & 0x000000FF;
				udpframe[1] = (out_msg.can_id >> 16) & 0x000000FF;
				udpframe[2] = (out_msg.can_id >> 8) & 0x000000FF;
				udpframe[3] =  out_msg.can_id & 0x000000FF;
				udpframe[4] =  out_msg.can_dlc;

				for (i = 0; i < 8; ++i)
				{
					if (i < out_msg.can_dlc)
						udpframe[5+i] = out_msg.data[i];
					else
						udpframe[5+i] = 0;
				}

				printf("\nNeuer mfx-Dekoder (UID 0x%X) angemeldet, Nummer: %d\n> ", uid, lastmfxnr);

				//muss jetzt noch in Lokliste eingetragen werden!!!
				FILE *datei;
				datei = fopen ("lokliste.txt", "a");

				if (datei != NULL) {
					fprintf (datei, "%d;%X\n>", lastmfxnr, uid);
					fclose (datei);					
				}

			}

			// send UDP frame
			if (udpsend == 1) { // nicht jeder CAN Frame muss über UDP raus!
/* should be done in client_cs2eth */
#if 0
				s=sendto(sb, udpframe, 13, 0, (struct sockaddr *)&baddr, sizeof(baddr));
				if (s != 13)
					perror("UDP write __");
#endif	
				if (Ms1GetVerbose(Data) ) {
					printf("->CAN>UDP CANID 0x%06X R",frame->can_id);
					printf(" [%d]", udpframe[4]);
					for (i=5; i<5+frame->can_dlc;i++) {
						printf(" %02x", udpframe[i]);
					}
					printf("\n");
				}
			}
		}
      MrIpcSend(Ms1GetClientSock(Data), &CmdFrame);
      MrIpcExit(&CmdFrame);
}

/**********************************************************************\
* Funktionsname: HandleCanData
*
* Kurzbeschreibung: this function reads all available can frames from
*                   the ms1 and calls the process function for each of
*                   them
*
* Parameter:
* Data : pointer to ms1 object which contains all needed information
*
\**********************************************************************/
static void HandleCanData(Ms1Struct *Data)
{  struct can_frame frame;

   /* read all CAN frames present on can socket */
   while (read(Ms1GetCanSock(Data), &frame, sizeof(struct can_frame)) ==
          sizeof(struct can_frame))
   {
      /* process the received can frame */
      ProcessCanData(Data, &frame);
   }
}

/**********************************************************************\
* Funktionsname: Ms1Run
*
* Kurzbeschreibung: this functions contains the main loop
*
* Parameter:
* Data : pointer to ms1 object which contains all needed information
*
\**********************************************************************/
void Ms1Run(Ms1Struct *Data)
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
         if (Ms1GetVerbose(Data))
            printf("add client socket %d\n", Ms1GetClientSock(Data));
         FD_SET(Ms1GetClientSock(Data), &ReadFds);
         if (Ms1GetClientSock(Data) > HighFd)
            HighFd = Ms1GetClientSock(Data);
         /* 2. CAN socket */
         if (Ms1GetVerbose(Data))
            printf("add can socket %d\n", Ms1GetCanSock(Data));
         FD_SET(Ms1GetCanSock(Data), &ReadFds);
         if (Ms1GetCanSock(Data) > HighFd)
            HighFd = Ms1GetCanSock(Data);
         /* 3. Timeout */
         SelectTimeout.tv_sec = SELECT_TIMEOUT;
         SelectTimeout.tv_usec = 0;
         if (Ms1GetVerbose(Data))
            printf("wait for %d fd, max %ld s\n", HighFd, SelectTimeout.tv_sec);

         /* wait for data */
         RetVal = select(HighFd + 1, &ReadFds, NULL, NULL, &SelectTimeout);
         if (Ms1GetVerbose(Data))
            printf("select liefert %d\n", RetVal);

         /* check, who has data */
         if (((RetVal == -1) && (errno == EINTR)) || (RetVal == 0))
         {
            /* timeout, time for periodic tasks */
            Now = time(NULL);
            if (Ms1GetVerbose(Data))
               printf("interrupt at %s\n", asctime(localtime(&Now)));
         }
         else if (RetVal < 0)
         {
            /* error, finish loop */
            if (Ms1GetVerbose(Data))
               puts("error in main loop");
            Loop = FALSE;
         }
         else
         {
            /* data, check at which fd */
            if (FD_ISSET(Ms1GetClientSock(Data), &ReadFds))
            {
               /* new cmd frame */
               HandleSystemData(Data);
            }
            if (FD_ISSET(Ms1GetCanSock(Data), &ReadFds))
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
