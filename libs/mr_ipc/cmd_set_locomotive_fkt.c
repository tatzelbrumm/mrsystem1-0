#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdSetLocomotiveFkt(MrIpcCmdType *Data, unsigned long Addr, unsigned Function, SwitchType Switch)
{  cJSON *root;

   root=cJSON_CreateObject();
   cJSON_AddItemToObject(root, MR_IPC_JSON_OBJECT_CMD, cJSON_CreateString(MR_IPC_JSON_CMD_LOCOMOTIVE_FKT));
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_ADDR, Addr);
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_FUNCTION, Function);
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_SWITCH, Switch);
   Data->Rendered = cJSON_Print(root);
   cJSON_Delete(root);
}
