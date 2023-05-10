#include <string.h>
#include <sys/socket.h>
#include "mr_ipc.h"

int MrIpcSend(int socket, MrIpcCmdType *Data)
{  int BytesSend;

   if ((Data != (MrIpcCmdType *)NULL) &&
       (MrIpcGetRendered(Data) != (char *)NULL))
   {
      BytesSend = send(socket, MrIpcGetRendered(Data),
                       strlen(MrIpcGetRendered(Data)) + 1, 0);
      if (BytesSend == strlen(MrIpcGetRendered(Data)) + 1)
      {
         return MR_IPC_RCV_OK;
      }
      else
      {
         return MR_IPC_RCV_ERROR;
      }
   }
   else
   {
      return MR_IPC_RCV_ERROR;
   }
}
