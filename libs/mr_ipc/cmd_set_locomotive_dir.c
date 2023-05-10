#include <stddef.h>
#include "cJSON/cJSON.h"
#include "mr_ipc.h"

void MrIpcCmdSetLocomotiveDir(MrIpcCmdType *Data, unsigned long Addr, DirectionType Direction)
{  cJSON *root;

   root=cJSON_CreateObject();
   cJSON_AddItemToObject(root, MR_IPC_JSON_OBJECT_CMD, cJSON_CreateString(MR_IPC_JSON_CMD_LOCOMOTIVE_DIR));
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_ADDR, Addr);
   cJSON_AddNumberToObject(root, MR_IPC_JSON_OBJECT_DIRECTION, Direction);
   Data->Rendered = cJSON_Print(root);
   cJSON_Delete(root);
}
