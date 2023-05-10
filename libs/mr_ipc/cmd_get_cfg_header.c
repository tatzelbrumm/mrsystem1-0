#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdGetCfgHeader(MrIpcCmdType *Data, unsigned long *Length, int *Crc)
{
   if ((Data != (MrIpcCmdType *)NULL) && (MrIpcGetPrivate(Data) != NULL))
   {
      *Length = cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_LENGTH)->valueint;
      *Crc = cJSON_GetObjectItem((cJSON *)MrIpcGetPrivate(Data), MR_IPC_JSON_OBJECT_CRC)->valueint;
   }
}
