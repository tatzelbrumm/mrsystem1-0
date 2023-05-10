#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdGetLocomotiveFkt(MrIpcCmdType *Data, unsigned long *Addr, unsigned *Function, SwitchType *Switch)
{
   if ((Data != (MrIpcCmdType *)NULL) && (MrIpcGetPrivate(Data) != NULL))
   {
      *Addr = cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_ADDR)->valueint;
      *Function = cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_FUNCTION)->valueint;
      *Switch = cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_SWITCH)->valueint;
   }
}
