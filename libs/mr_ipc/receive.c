#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include "mr_ipc.h"

#define MAX_BUF_LEN 512

int MrIpcRecv(int socket, MrIpcCmdType *Data)
{  int RecvBytes, Pos;
   char LocalBuffer[MAX_BUF_LEN];

   /* read bytes until detect string end */
   Pos = 0;
   do {
      RecvBytes = recv(socket, &(LocalBuffer[Pos]), 1, 0);
      if (RecvBytes < 0)
      {
         /* Error in read, maybe no data left */
         return MR_IPC_RCV_ERROR;
      }
      else if (RecvBytes == 0)
      {
         /* socket was closed at remote side */
         return MR_IPC_RCV_CLOSED;
      }
      else
      {
         /* we have received data, next pos in receive buffer */
         Pos++;
      }
   } while ((Pos < MAX_BUF_LEN) && (LocalBuffer[Pos - 1] != '\0'));
   /* we are here if we have more characters than our buffer can
      recieve which indicates an error. Otherwise we have one
      complete message which is a null terminated string */
   if (LocalBuffer[Pos - 1] == '\0')
   {
      /* we have one complete message */
      Data->Rendered = strdup(LocalBuffer);
      return MR_IPC_RCV_OK;
   }
   else
   {
      /* Error: read to much bytes */
      return MR_IPC_RCV_ERROR;
   }
}
