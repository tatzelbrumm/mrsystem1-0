#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdSetNull(MrIpcCmdType *Data)
{  cJSON *root;

   root=cJSON_CreateObject();
   cJSON_AddItemToObject(root, MR_IPC_JSON_OBJECT_CMD, cJSON_CreateString(MR_IPC_JSON_CMD_NULL));
   Data->Rendered = cJSON_Print(root);
   cJSON_Delete(root);
}
