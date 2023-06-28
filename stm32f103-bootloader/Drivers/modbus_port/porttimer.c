/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"
#include "porttimer.h"
#include "stm32f1xx_ll_tim.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"

BOOL xMBPortTimersInit(USHORT usTim1Timerout50us)
{
  (void) usTim1Timerout50us;
  LL_TIM_SetUpdateSource(TIM4, LL_TIM_UPDATESOURCE_COUNTER);
  return TRUE;
}

inline void vMBPortTimersEnable()
{
  LL_TIM_GenerateEvent_UPDATE(TIM4);
  LL_TIM_EnableIT_UPDATE(TIM4);
  LL_TIM_EnableCounter(TIM4);
}

inline void vMBPortTimersDisable()
{
  LL_TIM_DisableCounter(TIM4);
  LL_TIM_DisableIT_UPDATE(TIM4);
}

/* Create an ISR which is called whenever the timer has expired. This function
 * must then call pxMBPortCBTimerExpired( ) to notify the protocol stack that
 * the timer has expired.
 */
void TIM4_IRQ_Callback(void)
{
  pxMBPortCBTimerExpired();
}
