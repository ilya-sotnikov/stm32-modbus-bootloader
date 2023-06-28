#ifndef MODBUS_CALLBACKS_H
#define MODBUS_CALLBACKS_H

#include "mb.h"

#define MB_FLASH_UPDATE_FUNCION_CODE (0x6c)

enum Subfunctions {
//  StartProgramming,
  EraseFlash,
  ProgramFlash,
  GetChecksumFlash,
  ResetDevice,
};

eMBException eMBFlashUpdateCB(UCHAR *pucFrame, USHORT *pusLength);

#endif /* MODBUS_CALLBACKS_H */
