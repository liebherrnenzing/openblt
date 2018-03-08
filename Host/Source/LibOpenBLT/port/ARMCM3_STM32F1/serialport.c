/************************************************************************************//**
* \file         port\linux\serialport.c
* \brief        Serial port source file.
* \ingroup      SerialBoot
* \internal
*----------------------------------------------------------------------------------------
*                          C O P Y R I G H T
*----------------------------------------------------------------------------------------
*   Copyright (c) 2017  by Feaser    http://www.feaser.com    All rights reserved
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
* You have received a copy of the GNU General Public License along with OpenBLT. It
* should be located in ".\Doc\license.html". If not, contact Feaser to obtain a copy.
*
* \endinternal
****************************************************************************************/

/****************************************************************************************
* Include files
****************************************************************************************/
#include "boot.h"
#include <stdint.h>                         /* for standard integer types              */
#include <stddef.h>                         /* for NULL declaration                    */
#include <stdbool.h>                        /* for boolean type                        */
#include "serialport.h"                     /* serial port module                      */
#include "uart_loader.h"
#include "util.h"

#include "assert.h"


/****************************************************************************************
* Macro definitions
****************************************************************************************/

/****************************************************************************************
* Local data declarations
****************************************************************************************/

/****************************************************************************************
* Local constant declarations
****************************************************************************************/

/************************************************************************************//**
** \brief     Initializes the serial port module.
**
****************************************************************************************/
void SerialPortInit(void)
{
	/* not needed */
} /*** end of SerialPortInit ***/


/************************************************************************************//**
** \brief     Terminates the serial port module.
**
****************************************************************************************/
void SerialPortTerminate(void)
{
	/* not needed */
} /*** end of SerialPortTerminate ***/


/************************************************************************************//**
** \brief     Opens the connection with the serial port configured as 8,N,1 and no flow
**            control.
** \param     portname The name of the serial port to open, i.e. /dev/ttyUSB0.
** \param     baudrate The desired communication speed.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
bool SerialPortOpen(char const * portname, tSerialPortBaudrate baudrate)
{
  UartLoaderInit();
  /* success */
  return true;
} /*** end of SerialPortOpen ***/


/************************************************************************************//**
** \brief     Closes the connection with the serial port.
** \return    None.
**
****************************************************************************************/
void SerialPortClose(void)
{
  /* not needed */
} /*** end of SerialPortClose ***/


/************************************************************************************//**
** \brief     Writes data to the serial port.
** \param     data Pointer to byte array with data to write.
** \param     length Number of bytes to write.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
bool SerialPortWrite(uint8_t const * data, uint32_t length)
{
  size_t bytesWritten;
  uint32_t i;

  /* check parameters */
  //assert(data != NULL);
  //assert(length > 0);
  bytesWritten = 0;

  for (i = 0; i < length; i++)
  {
	  UartLoaderTransmitByte(data[i]);
	  bytesWritten++;
  }

  /* check and return the result */
  return (bytesWritten == length);
} /*** end of SerialPortWrite ***/


/************************************************************************************//**
** \brief     Reads data from the serial port in a blocking manner.
** \param     data Pointer to byte array to store read data.
** \param     length Number of bytes to read.
** \return    True if successful, false otherwise.
**
****************************************************************************************/
bool SerialPortRead(uint8_t *data, uint32_t length)
{
  size_t bytesRead;
  uint32_t dwToRead;
  uint32_t timeoutTime;

  /* check parameters */
 // assert(data != NULL);
 // assert(length > 0);

  bytesRead = 0;
  dwToRead = length;

#define UART_RX_TIMEOUT_MIN_MS   (5)

  /* set timeout */
  timeoutTime = UtilTimeGetSystemTimeMs() + UART_RX_TIMEOUT_MIN_MS;

  while(dwToRead > 0)
  {
	if(UartLoaderReceiveByte(data))
    {
      /* update the bytes that were already read */
      data++;
      dwToRead--;
      bytesRead++;

      /* update timeout */
      timeoutTime = UtilTimeGetSystemTimeMs() + UART_RX_TIMEOUT_MIN_MS;
    }

    /* check for timeout if not yet done */
    if ( UtilTimeGetSystemTimeMs() >= timeoutTime )
    {
      /* timeout occurred */
      return false;
    }
  }

  /* check and return the result */
  return (bytesRead == length);
} /*** end of SerialPortRead ***/


/*********************************** end of serialport.c *******************************/

