#ifndef MR_IPC_H
#define MR_IPC_H

#include <unistd.h>
#include <sys/socket.h>
#include <mr_can.h>

/* return values for function MrIpcSend() */
#define MR_IPC_RCV_ERROR  -1
#define MR_IPC_RCV_CLOSED 0
#define MR_IPC_RCV_OK     1

/* object names for JSON command */
#define MR_IPC_JSON_OBJECT_CMD "cmd"
#define MR_IPC_JSON_OBJECT_ADDR      "addr"
#define MR_IPC_JSON_OBJECT_DIRECTION "direction"
#define MR_IPC_JSON_OBJECT_SPEED     "speed"
#define MR_IPC_JSON_OBJECT_POSITION  "position"
#define MR_IPC_JSON_OBJECT_FUNCTION  "function"
#define MR_IPC_JSON_OBJECT_SWITCH    "switch"
#define MR_IPC_JSON_OBJECT_VERSION   "version"
#define MR_IPC_JSON_OBJECT_TYPE      "type"
#define MR_IPC_JSON_OBJECT_START_IDX "start_idx"
#define MR_IPC_JSON_OBJECT_END_IDX   "end_idx"
#define MR_IPC_JSON_OBJECT_LENGTH    "length"
#define MR_IPC_JSON_OBJECT_NAME      "name"
#define MR_IPC_JSON_OBJECT_CRC       "crc"
#define MR_IPC_JSON_OBJECT_CFG_DATA  "cfg_data"

/* values for JSON object "cmd" */
#define MR_IPC_JSON_CMD_NULL             "null"
#define MR_IPC_JSON_CMD_RUN              "run"
#define MR_IPC_JSON_CMD_LOCOMOTIVE_DIR   "locomotive_direction"
#define MR_IPC_JSON_CMD_LOCOMOTIVE_SPEED "locomotive_speed"
#define MR_IPC_JSON_CMD_LOCOMOTIVE_FKT   "locomotive_function"
#define MR_IPC_JSON_CMD_ACC_SWITCH       "acc_switch"
#define MR_IPC_JSON_CMD_REQUEST_MEMBER   "get_member"
#define MR_IPC_JSON_CMD_MEMBER           "member"
#define MR_IPC_JSON_CMD_REQUEST_LOC_NAME "get_loc_name"
#define MR_IPC_JSON_CMD_REQUEST_LOC_INFO "get_loc_info"
#define MR_IPC_JSON_CMD_CFG_HEADER       "cfg_header"
#define MR_IPC_JSON_CMD_CFG_DATA         "cfg_data"

typedef struct {
   char *Rendered;
   void *Private;
} MrIpcCmdType;

#define MrIpcSetRendered(Data,p) (Data)->Rendered=p
#define MrIpcSetPrivate(Data,p)  (Data)->Private=p

#define MrIpcGetRendered(Data) (Data)->Rendered
#define MrIpcGetPrivate(Data)  (Data)->Private

typedef enum { Off, On } SwitchType;
typedef enum { Forward, Backward } DirectionType;
typedef enum { Left, Right } PositionType;

/* return values for function MrIpcCmdGetCmd() */
typedef enum {
   MrIpcCmdNull,                /* empty command */
   MrIpcCmdRun,                 /* start/stop command */
   MrIpcCmdLocomotiveSpeed,     /* command to set speed of a locomotive */
   MrIpcCmdLocomotiveDirection, /* command to set direction of a locomotive */
   MrIpcCmdLocomotiveFunction,  /* command to set function of a locomotive */
   MrIpcCmdAccSwitch,           /* command to switch accesoires */
   MrIpcCmdRequestMember,       /* command to request CAN member */
   MrIpcCmdMember,              /* command to announce member */
   MrIpcCmdRequestLocName,      /* command to request loc name from MS2 */
   MrIpcCmdRequestLocInfo,      /* command to request loc info from MS2 */
   MrIpcCmdCfgHeader,           /* command with header of cfg data */
   MrIpcCmdCfgData              /* command with data of cfg data */
} MrIpcCommandValue;

#define MrIpcClose(socket) close(socket)

MrIpcCmdType *MrIpcCreate(void);
void MrIpcDestroy(MrIpcCmdType *Data);
void MrIpcInit(MrIpcCmdType *Data);
void MrIpcExit(MrIpcCmdType *Data);
int MrIpcConnect(char *IpAddr, int Port);
int MrIpcStartServer(char *IpAddr, int Port);
int MrIpcAccept(int ServerSock);
int MrIpcSend(int socket, MrIpcCmdType *Data);
int MrIpcRecv(int socket, MrIpcCmdType *Data);

void MrIpcEncodeFromCan(MrIpcCmdType *Data, MrCs2CanDataType *CanMsg);
void MrIpcDecodeToCan(MrIpcCmdType *Data, MrCs2CanDataType *CanMsg);

void MrIpcCmdSetNull(MrIpcCmdType *Data);
void MrIpcCmdSetRun(MrIpcCmdType *Data, SwitchType Switch);
void MrIpcCmdSetLocomotiveSpeed(MrIpcCmdType *Data, unsigned long Addr, unsigned Speed);
void MrIpcCmdSetLocomotiveDir(MrIpcCmdType *Data, unsigned long Addr, DirectionType Direction);
void MrIpcCmdSetLocomotiveFkt(MrIpcCmdType *Data, unsigned long Addr, unsigned Function, SwitchType Switch);
void MrIpcCmdSetAccPos(MrIpcCmdType *Data, unsigned long Addr, PositionType Position);
void MrIpcCmdSetRequest(MrIpcCmdType *Data);
void MrIpcCmdSetRequestMember(MrIpcCmdType *Data);
void MrIpcCmdSetMember(MrIpcCmdType *Data, unsigned long Addr, unsigned Version, unsigned Type);
void MrIpcCmdSetReqestLocname(MrIpcCmdType *Data, unsigned StartIdx, unsigned EndIdx);
void MrIpcCmdSetReqestLocinfo(MrIpcCmdType *Data, char *Locname);
void MrIpcCmdSetCfgHeader(MrIpcCmdType *Data, unsigned long Length, int Crc);
void MrIpcCmdSetCfgData(MrIpcCmdType *Data, char *Buf);

MrIpcCommandValue MrIpcCmdGetCmd(MrIpcCmdType *Data);
#define MrIpcCmdGetNull(Data)
void MrIpcCmdGetRun(MrIpcCmdType *Data, SwitchType *Switch);
void MrIpcCmdGetLocomotiveDir(MrIpcCmdType *Data, unsigned long *Addr, DirectionType *Direction);
void MrIpcCmdGetLocomotiveSpeed(MrIpcCmdType *Data, unsigned long *Addr, unsigned *Speed);
void MrIpcCmdGetLocomotiveFkt(MrIpcCmdType *Data, unsigned long *Addr, unsigned *Function, SwitchType *Switch);
void MrIpcCmdGetAccPos(MrIpcCmdType *Data, unsigned long *Addr, PositionType *Position);
#define MrIpcCmdGetRequestMember(Data)
void MrIpcCmdGetMember(MrIpcCmdType *Data, unsigned long *Addr, unsigned *Version, unsigned *Type);
void MrIpcCmdGetReqestLocname(MrIpcCmdType *Data, unsigned *StartIdx, unsigned *EndIdx);
void MrIpcCmdGetReqestLocinfo(MrIpcCmdType *Data, char *Locinfo);
void MrIpcCmdGetCfgHeader(MrIpcCmdType *Data, unsigned long *Length, int *Crc);
void MrIpcCmdGetCfgData(MrIpcCmdType *Data, char *Buf);

#endif
