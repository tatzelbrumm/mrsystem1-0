#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdGetReqestLocname(MrIpcCmdType *Data, unsigned *StartIdx, unsigned *EndIdx)
{
   if ((Data != (MrIpcCmdType *)NULL) && (MrIpcGetPrivate(Data) != NULL))
   {
      *StartIdx = cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_START_IDX)->valueint;
      *EndIdx = cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_END_IDX)->valueint;
   }
}
