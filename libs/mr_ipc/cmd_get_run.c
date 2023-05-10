#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdGetRun(MrIpcCmdType *Data, SwitchType *Switch)
{
   if ((Data != (MrIpcCmdType *)NULL) && (MrIpcGetPrivate(Data) != NULL))
   {
      *Switch = cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_SWITCH)->valueint;
   }
}
