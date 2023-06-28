#include "modbus_callbacks.h"

#include "flash.h"
#include "bootloader.h"

#include "mbframe.h"
#include <string.h>

//static volatile uint8_t firmware[2048];

extern int bootloader_started_programming;
extern int bootloader_needs_reset;

eMBException eMBFlashUpdateCB(UCHAR *pucFrame, USHORT *pusLength)
{
  uint8_t *frame_current;
  uint16_t data_length = *pusLength;
  uint8_t subfunction;
  uint32_t flash_offset;
  uint8_t status;

  static uint32_t fw_size_bytes;

  frame_current = &pucFrame[MB_PDU_FUNC_OFF];
  *pusLength = MB_PDU_FUNC_OFF;

  *frame_current++ = 0x6c;
  (*pusLength)++;

  subfunction = *frame_current++;
  (*pusLength)++;

  switch (subfunction) {
//  case StartProgramming:
//    if (1)
//      *frame_current++ = 0;
//    else
//      *frame_current++ = 1;
//    (*pusLength)++;
//    break;

  case EraseFlash:
    bootloader_started_programming = 1;
//    for (uint32_t i = 0; i < sizeof(firmware); ++i)
//      firmware[i] = 0;
    fw_size_bytes = 0;
    *frame_current++ = flash_erase_app();
    (*pusLength)++;
    break;

  case ProgramFlash:
    data_length = data_length - (1 + 1 + 4); // function, subfunction and offset
    flash_offset = frame_current[0] | (frame_current[1] << 8)
        | (frame_current[2] << 16) | (frame_current[3] << 24);

    status = flash_write_app_data(flash_offset, frame_current + 4, data_length);
    fw_size_bytes += data_length;
    *frame_current++ = status;
    (*pusLength)++;
//    return MB_EX_SLAVE_DEVICE_FAILURE;
    break;

  case GetChecksumFlash:
    flash_lock();
    *frame_current++ = flash_get_app_checksum(fw_size_bytes);
    (*pusLength)++;
    break;

  case ResetDevice:
    bootloader_needs_reset = 1;
    *frame_current++ = 0;
    (*pusLength)++;
    break;

  default:
    return MB_EX_ILLEGAL_FUNCTION;
    break;
  }
//
//  // 3th byte = ожидаемое кол-во данных в ответе
//  data_size_out = *pucFrameCur;
//  if (data_size_out > 250)
//    data_size_out = 250;       // размер всего пакета не может превышать 256байт
//  *pucFrameCur++ = data_size_out;
//  (*usLen)++;
//
//  // 4-7 bytes = адрес
//  pData = (uint8_t*) *pucFrameCur;
//  pData = (uint8_t*) ((uint32_t) pData << 8);
//  pData += *(pucFrameCur + 1);
//  pData = (uint8_t*) ((uint32_t) pData << 8);
//  pData += *(pucFrameCur + 2);
//  pData = (uint8_t*) ((uint32_t) pData << 8);
//  pData += *(pucFrameCur + 3);
//
//  // установка адреса flash в aduc
//  addr = (uint16_t) ((uint32_t) pData & 0x0000FFFE);
//
//  switch (opcode) {
//  case 0:   // общий сброс
//  case 1:   // заполнение массива данных для перепрошивки LPC
//  case 2:   // чтение данных из памяти LPC
//  case 3:   // старт перепрограммирования ПП LPC
//    break;
//
//    // запись данных для перепрошивки ADUC  (запись максимального пакета 256 байт за <=32 мс)
//  case 4:
//
//    pucFrameCur += 4;         // указатель на данные
//
//    // запись кода во временный буфер flash
//    while (data_size_in > 0) {
//      data = (uint16_t) (*pucFrameCur++);
//      data += (uint16_t) (*pucFrameCur++) << 8;
//
//      if (Write_HWordToFLASH(addr, data) != 1) {
//        SET_FLAG_MEM_ACCESS_ERROR();
//        return MB_EX_SLAVE_DEVICE_FAILURE;
//      }
//
//      data_size_in -= 2;
//      addr += 2;
//      if (FwCodeSize < (FLASH_USER_SIZE) / 2)
//        FwCodeSize += 2;
//    }
//    CLR_FLAG_MEM_ACCESS_ERROR();
//    break;
//
//    // чтение данных из памяти ADUC (чтение максимального пакета 256 байт за <=32 мс)
//  case 5:
//
//    data_size_in = 0;
//    while (data_size_in < data_size_out) {
//      ReadHWordFromFLASH(addr, &data);
//
//      *pucFrameCur++ = (unsigned char) data;
//      (*usLen)++;
//      *pucFrameCur++ = (unsigned char) (data >> 8);
//      (*usLen)++;
//
//      data_size_in += 2;
//      addr += 2;
//    }
//    CLR_FLAG_MEM_ACCESS_ERROR();
//    break;
//
//    // старт перепрограммирования ПП ADUC
//  case 6:
//    AducReProg(0, addr, FwCodeSize);
//    break;
//
//  default:  //
//    return MB_EX_ILLEGAL_DATA_VALUE;
//  }
  return MB_EX_NONE;
}
