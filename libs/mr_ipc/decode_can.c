#include <stddef.h>
#include "mr_ipc.h"

void MrIpcDecodeToCan(MrIpcCmdType *Data, MrCs2CanDataType *CanMsg)
{  unsigned long Addr, Length;
   unsigned p1, p2;
   SwitchType Switch;
   DirectionType Direction;
   PositionType Position;
   char Buf[8];

   if (Data != (MrIpcCmdType *)NULL)
   {
      if (MrIpcGetPrivate(Data) == NULL)
         MrIpcDecode(Data);
      MrCs2SetIsCs2(CanMsg, TRUE);
      MrCs2SetResponse(CanMsg, 0);
      MrCs2CalcHash(CanMsg, 0l);
      switch (MrIpcCmdGetCmd(Data))
      {
         case MrIpcCmdNull:
            MrIpcCmdGetNull(Data);
            break;
         case MrIpcCmdRun:
            MrIpcCmdGetRun(Data, &Switch);
            MrCs2SetPrio(CanMsg, MR_CS2_PRIO_4);
            if (Switch == Off)
               MrCs2EncSysStop(CanMsg, MR_CS2_UID_BROADCAST);
            else if (Switch == On)
               MrCs2EncSysGo(CanMsg, MR_CS2_UID_BROADCAST);
            break;
         case MrIpcCmdLocomotiveSpeed:
            MrIpcCmdGetLocomotiveSpeed(Data, &Addr, &p1);
            MrCs2SetPrio(CanMsg, MR_CS2_PRIO_4);
            MrCs2EncSpeed6(CanMsg, Addr, p1);
            break;
         case MrIpcCmdLocomotiveDirection:
            MrIpcCmdGetLocomotiveDir(Data, &Addr, &Direction);
            MrCs2SetPrio(CanMsg, MR_CS2_PRIO_4);
            MrCs2EncDirection5(CanMsg, Addr, (int)Direction);
            break;
         case MrIpcCmdAccSwitch:
            MrIpcCmdGetAccPos(Data, &Addr, &Position);
            MrCs2SetPrio(CanMsg, MR_CS2_PRIO_4);
            MrCs2EncAccSwitch6(CanMsg, Addr, (int)Position, 0);
            break;
         case MrIpcCmdLocomotiveFunction:
            MrIpcCmdGetLocomotiveFkt(Data, &Addr, &p1, &Switch);
            MrCs2SetPrio(CanMsg, MR_CS2_PRIO_4);
            MrCs2EncFunction6(CanMsg, Addr, p1, (int)Switch);
            break;
         case MrIpcCmdRequestMember:
            MrIpcCmdGetRequestMember(Data);
            MrCs2SetPrio(CanMsg, MR_CS2_PRIO_0);
            MrCs2EncPing0(CanMsg);
            break;
         case MrIpcCmdMember:
            MrIpcCmdGetMember(Data, &Addr, &p1, &p2);
            MrCs2SetPrio(CanMsg, MR_CS2_PRIO_0);
            MrCs2EncPing8(CanMsg, Addr, p1, p2);
            break;
         case MrIpcCmdCfgHeader:
            MrIpcCmdGetCfgHeader(Data, &Length, &p1);
            MrCs2SetPrio(CanMsg, MR_CS2_PRIO_0);
            MrCs2EncCfgdatStream6(CanMsg, Length, p1);
            break;
         case MrIpcCmdCfgData:
            MrIpcCmdGetCfgData(Data, Buf);
            MrCs2SetPrio(CanMsg, MR_CS2_PRIO_0);
            MrCs2EncCfgdatStream8(CanMsg, Buf);
            break;
      }
   }
}
