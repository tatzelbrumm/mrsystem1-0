#include <stddef.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdGetReqestLocinfo(MrIpcCmdType *Data, char *Locname)
{
   if ((Data != (MrIpcCmdType *)NULL) && (MrIpcGetPrivate(Data) != NULL))
   {
      strcpy(Locname, cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_NAME)->valuestring);
   }
}
