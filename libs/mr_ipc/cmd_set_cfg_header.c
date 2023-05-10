#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdSetCfgHeader(MrIpcCmdType *Data, unsigned long Length, int Crc)
{  cJSON *root;

   root=cJSON_CreateObject();
   cJSON_AddItemToObject(root, MR_IPC_JSON_OBJECT_CMD, cJSON_CreateString(MR_IPC_JSON_CMD_CFG_HEADER));
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_LENGTH, Length);
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_CRC, Crc);
   Data->Rendered = cJSON_Print(root);
   cJSON_Delete(root);
}
