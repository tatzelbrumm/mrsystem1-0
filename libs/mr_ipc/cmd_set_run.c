#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdSetRun(MrIpcCmdType *Data, SwitchType Switch)
{  cJSON *root;

   root=cJSON_CreateObject();
   cJSON_AddItemToObject(root, MR_IPC_JSON_OBJECT_CMD, cJSON_CreateString(MR_IPC_JSON_CMD_RUN));
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_SWITCH, Switch);
   Data->Rendered = cJSON_Print(root);
   cJSON_Delete(root);
}
