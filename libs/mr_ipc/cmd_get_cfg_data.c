#include <stddef.h>
#include <string.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdGetCfgData(MrIpcCmdType *Data, char *Buf)
{
   if ((Data != (MrIpcCmdType *)NULL) && (MrIpcGetPrivate(Data) != NULL) && (Buf != NULL))
   {
      memcpy(Buf, cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_CFG_DATA)->valuestring, 8);
   }
}
