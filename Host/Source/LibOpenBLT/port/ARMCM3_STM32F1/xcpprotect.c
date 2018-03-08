/************************************************************************************//**
* \file         port/linux/xcpprotect.c
* \brief        XCP Protection module source file.
* \ingroup      XcpLoader
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
#include <assert.h>                         /* for assertions                          */
#include <stdint.h>                         /* for standard integer types              */
#include <stddef.h>                         /* for NULL declaration                    */
#include <stdbool.h>                        /* for boolean type                        */
#include "xcpprotect.h"                     /* XCP protection module                   */


/****************************************************************************************
* Type definitions
****************************************************************************************/

/****************************************************************************************
* Local data declarations
****************************************************************************************/

/************************************************************************************//**
** \brief     Initializes the XCP protection module.
** \param     seedKeyFile Filename of the seed and key shared library that contains the
**            following functions:
**            - XCP_ComputeKeyFromSeed()
**            - XCP_GetAvailablePrivileges()
**
****************************************************************************************/
void XcpProtectInit(char const * seedKeyFile)
{
} /*** end of XcpProtectInit ***/


/************************************************************************************//**
** \brief     Terminates the XCP protection module.
**
****************************************************************************************/
void XcpProtectTerminate(void)
{
} /*** end of XcpProtectTerminate ***/


/************************************************************************************//**
** \brief     Computes the key for the requested resource.
** \param     resource resource for which the unlock key is requested
** \param     seedLen  length of the seed
** \param     seedPtr  pointer to the seed data
** \param     keyLenPtr pointer where to store the key length
** \param     keyPtr pointer where to store the key data
** \return    True if successful, false otherwise.
**
****************************************************************************************/
bool XCPProtectComputeKeyFromSeed(uint8_t resource, uint8_t seedLen,
                                  uint8_t const * seedPtr, uint8_t * keyLenPtr,
                                  uint8_t * keyPtr)
{
  return true;
} /*** end of XCPProtectComputeKeyFromSeed ***/


/************************************************************************************//**
** \brief     Obtains a bitmask of the resources for which an key algorithm is available.
** \param     resourcePtr pointer where to store the supported resources for the key
**                        computation.
** \return    XCP_RESULT_OK on success, otherwise XCP_RESULT_ERROR.
**
****************************************************************************************/
bool XcpProtectGetPrivileges(uint8_t * resourcePtr)
{
  return true;
} /*** end of XcpProtectGetPrivileges ***/


/*********************************** end of xcpprotect.c *******************************/
