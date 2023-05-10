#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdSetReqestLocinfo(MrIpcCmdType *Data, char *Locname)
{  cJSON *root;

   root=cJSON_CreateObject();
   cJSON_AddItemToObject(root, MR_IPC_JSON_OBJECT_CMD, cJSON_CreateString(MR_IPC_JSON_CMD_REQUEST_LOC_INFO));
   cJSON_AddStringToObject(root, MR_IPC_JSON_OBJECT_NAME, Locname);
   Data->Rendered = cJSON_Print(root);
   cJSON_Delete(root);
}
