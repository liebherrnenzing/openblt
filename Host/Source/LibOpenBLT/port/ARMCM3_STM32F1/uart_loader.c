/************************************************************************************//**
* \file         uart_loader.c
* \brief        Bootloader UART communication interface source file.
* \ingroup      Target_ARMCM3_STM32
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2011  by Feaser    http://www.feaser.com    All rights reserved
*
*----------------------------------------------------------------------------------------
*                            L I C E N S E
*----------------------------------------------------------------------------------------
* This file is part of OpenBLT. OpenBLT is free software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any later
* version.
*
* OpenBLT is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
* without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
* PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with OpenBLT.
* If not, see <http://www.gnu.org/licenses/>.
*
* A special exception to the GPL is included to allow you to distribute a combined work
* that includes OpenBLT without being obliged to provide the source code for any
* proprietary components. The exception text is included at the bottom of the license
* file <license.html>.
*
* \endinternal
****************************************************************************************/

/****************************************************************************************
* Include files
****************************************************************************************/
#include "boot.h"                                /* bootloader generic header          */
#include "uart_loader.h"


#if (BOOT_XCP_UART_LOADER_ENABLE > 0)
/****************************************************************************************
* Type definitions
****************************************************************************************/
/** \brief UART register layout. */
typedef struct
{
  volatile blt_int16u SR;                           /**< status register               */
  blt_int16u          RESERVED0;
  volatile blt_int16u DR;                           /**< data register                 */
  blt_int16u          RESERVED1;
  volatile blt_int16u BRR;                          /**< baudrate register             */
  blt_int16u          RESERVED2;
  volatile blt_int16u CR1;                          /**< control register 1            */
  blt_int16u          RESERVED3;
  volatile blt_int16u CR2;                          /**< control register 2            */
  blt_int16u          RESERVED4;
  volatile blt_int16u CR3;                          /**< control register 3            */
  blt_int16u          RESERVED5;
  volatile blt_int16u GTPR;                         /**< guard time and prescale reg.  */
  blt_int16u          RESERVED6;
} tUartRegs;                                        /**< UART register layout type     */


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief USART enable bit. */
#define UART_BIT_UE    ((blt_int16u)0x2000)
/** \brief Transmitter enable bit. */
#define UART_BIT_TE    ((blt_int16u)0x0008)
/** \brief Receiver enable bit. */
#define UART_BIT_RE    ((blt_int16u)0x0004)
/** \brief Transmit data reg. empty bit. */
#define UART_BIT_TXE   ((blt_int16u)0x0080)
/** \brief Read data reg. not empty bit. */
#define UART_BIT_RXNE  ((blt_int16u)0x0020)


/****************************************************************************************
* Register definitions
****************************************************************************************/
#if (BOOT_XCP_UART_LOADER_CHANNEL_INDEX == 0)
/** \brief Set UART base address to USART1. */
#define UARTx          ((tUartRegs *) (blt_int32u)0x40013800)
#elif (BOOT_XCP_UART_LOADER_CHANNEL_INDEX == 1)
/** \brief Set UART base address to USART2. */
#define UARTx          ((tUartRegs *) (blt_int32u)0x40004400)
#elif (BOOT_XCP_UART_LOADER_CHANNEL_INDEX == 2)
/** \brief Set UART base address to USART3. */
#define UARTx          ((tUartRegs *) (blt_int32u)0x40004800)
#else
/** \brief Set UART base address to USART1 by default. */
#define UARTx          ((tUartRegs *) (blt_int32u)0x40013800)
#endif


/************************************************************************************//**
** \brief     Initializes the UART communication interface.
** \return    none.
**
****************************************************************************************/
void UartLoaderInit(void)
{
  /* the current implementation supports USART1, USART2 and USART3. throw an assertion error in
   * case a different UART channel is configured.
   */
  ASSERT_CT((BOOT_XCP_UART_LOADER_CHANNEL_INDEX == 0) ||
            (BOOT_XCP_UART_LOADER_CHANNEL_INDEX == 1) ||
            (BOOT_XCP_UART_LOADER_CHANNEL_INDEX == 2));
  /* first reset the UART configuration. note that this already configures the UART
   * for 1 stopbit, 8 databits and no parity.
   */
  UARTx->BRR = 0;
  UARTx->CR1 = 0;
  UARTx->CR2 = 0;
  UARTx->CR3 = 0;
  UARTx->GTPR = 0;

  /* configure the baudrate, knowing that PCLKx is configured to be half of
   * BOOT_CPU_SYSTEM_SPEED_KHZ.
   */
  UARTx->BRR = ((BOOT_CPU_SYSTEM_SPEED_KHZ/2)*(blt_int32u)1000)/BOOT_XCP_UART_LOADER_BAUDRATE;
  /* enable the UART including the transmitter and the receiver */
  UARTx->CR1 |= (UART_BIT_UE | UART_BIT_TE | UART_BIT_RE);
} /*** end of UartLoaderInit ***/

/************************************************************************************//**
** \brief     Receives a communication interface byte if one is present.
** \param     data Pointer to byte where the data is to be stored.
** \return    BLT_TRUE if a byte was received, BLT_FALSE otherwise.
**
****************************************************************************************/
blt_bool UartLoaderReceiveByte(blt_int8u *data)
{
  /* check if a new byte was received by means of the RDR-bit */
  if((UARTx->SR & UART_BIT_RXNE) != 0)
  {
    /* store the received byte */
    data[0] = UARTx->DR;
    /* inform caller of the newly received byte */
    return BLT_TRUE;
  }
  /* inform caller that no new data was received */
  return BLT_FALSE;
} /*** end of UartLoaderReceiveByte ***/


/************************************************************************************//**
** \brief     Transmits a communication interface byte.
** \param     data Value of byte that is to be transmitted.
** \return    BLT_TRUE if the byte was transmitted, BLT_FALSE otherwise.
**
****************************************************************************************/
blt_bool UartLoaderTransmitByte(blt_int8u data)
{
  /* check if tx holding register can accept new data */
  if ((UARTx->SR & UART_BIT_TXE) == 0)
  {
    /* UART not ready. should not happen */
    return BLT_FALSE;
  }
  /* write byte to transmit holding register */
  UARTx->DR = data;
  /* wait for tx holding register to be empty */
  while((UARTx->SR & UART_BIT_TXE) == 0)
  {
    /* keep the watchdog happy */
    CopService();
  }
  /* byte transmitted */
  return BLT_TRUE;
} /*** end of UartLoaderTransmitByte ***/
#endif /* BOOT_COM_UART_ENABLE > 0 */


/*********************************** end of uart.c *************************************/
