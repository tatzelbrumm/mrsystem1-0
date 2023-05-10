#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdSetMember(MrIpcCmdType *Data, unsigned long Addr, unsigned Version, unsigned Type)
{  cJSON *root;

   root=cJSON_CreateObject();
   cJSON_AddItemToObject(root, MR_IPC_JSON_OBJECT_CMD, cJSON_CreateString(MR_IPC_JSON_CMD_MEMBER));
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_ADDR, Addr);
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_VERSION, Version);
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_TYPE, Type);
   Data->Rendered = cJSON_Print(root);
   cJSON_Delete(root);
}
