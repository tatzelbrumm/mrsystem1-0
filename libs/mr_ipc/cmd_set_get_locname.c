#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdSetReqestLocname(MrIpcCmdType *Data, unsigned StartIdx, unsigned EndIdx)
{  cJSON *root;

   root=cJSON_CreateObject();
   cJSON_AddItemToObject(root, MR_IPC_JSON_OBJECT_CMD, cJSON_CreateString(MR_IPC_JSON_CMD_REQUEST_LOC_NAME));
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_START_IDX, StartIdx);
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_END_IDX, EndIdx);
   Data->Rendered = cJSON_Print(root);
   cJSON_Delete(root);
}
