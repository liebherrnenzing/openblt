/************************************************************************************//**
* \file         main.c
* \brief        BootCommander program source file.
* \ingroup      BootCommander
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
#include <stdlib.h>                         /* for standard library                    */
#include <stdio.h>                          /* Standard I/O functions.                 */
#include <string.h>                         /* for string library                      */
#include "openblt.h"                        /* OpenBLT host library.                   */


/****************************************************************************************
* Macro definitions
****************************************************************************************/
/** \brief Program return code indicating that the program executed successfully. */
#define RESULT_OK                           (0)
/** \brief Program return code indicating that an error was detected when processing the
 *         command line parameters/
 */
#define RESULT_ERROR_COMMANDLINE            (1)
/** \brief Program return code indicating that the a generic error was detected. */
#define RESULT_ERROR_GENERIC                (2)


/****************************************************************************************
* Function prototypes
****************************************************************************************/
static void DisplayProgramInfo(void);
static void DisplayProgramUsage(void);
static uint32_t ExtractSessionTypeFromCommandLine(int argc, char const * const argv[]);
static void * ExtractSessionSettingsFromCommandLine(int argc, char const * const argv[],
                                                    uint32_t sessionType);
static uint32_t ExtractTransportTypeFromCommandLine(int argc, char const * const argv[]);
static void * ExtractTransportSettingsFromCommandLine(int argc, 
                                                      char const * const argv[],
                                                      uint32_t transportType);
static char const * const ExtractFirmwareFileFromCommandLine(int argc, 
                                                             char const * const argv[]);


/************************************************************************************//**
** \brief     This is the program entry point.
** \param     argc Number of program arguments.
** \param     argv Array with program arguments.
** \return    Program return code. 0 for success, error code otherwise.
**
****************************************************************************************/
int main(int argc, char const * const argv[])
{
  int result = RESULT_OK;
  uint32_t appSessionType;
  void * appSessionSettings;
  uint32_t appTransportType;
  void * appTransportSettings;
  char const * appFirmwareFile;
  volatile uint32_t waitLoopCnt;

  /* -------------------- Display info ----------------------------------------------- */
  /* Display program info */
  DisplayProgramInfo();

  /* -------------------- Process command line --------------------------------------- */
  /* Extract the session type from the command line. */
  appSessionType = ExtractSessionTypeFromCommandLine(argc, argv);
  /* Extract the session type specific settings from the command line. */
  appSessionSettings = ExtractSessionSettingsFromCommandLine(argc, argv, appSessionType);
  /* Extract the transport type from the command line. */
  appTransportType = ExtractTransportTypeFromCommandLine(argc, argv);
  /* Extract the transport type specific settings from the command line. */
  appTransportSettings = ExtractTransportSettingsFromCommandLine(argc, argv, 
                                                                 appTransportType);
  /* Extract the firmware filename from the command line. */
  appFirmwareFile = ExtractFirmwareFileFromCommandLine(argc, argv);
  /* Check the settings that were detected so far and verify that at least enough
   * command line arguments were actually specified. At a minimum the firmware file
   * needs to be specified.
   */
  if ( (argc < 2) || (appSessionSettings == NULL) || (appTransportSettings == NULL) ||
     (appFirmwareFile == NULL) )
  {
    /* Display program usage. */
    DisplayProgramUsage();
    /* Set error code. */
    result = RESULT_ERROR_COMMANDLINE;
  }
  
  /* -------------------- Display detected parameters -------------------------------- */
  if (result == RESULT_OK)
  {
  /* TODO Display brief overview of detected settings. */
  }
  
  /* -------------------- Firmware loading ------------------------------------------- */
  if (result == RESULT_OK)
  {
    /* Initialize the firmware data module using the S-record parser. */
    BltFirmwareInit(BLT_FIRMWARE_PARSER_SRECORD);
    
    /* TODO Load firmware data from the file and chek the result. */
    
    /* TODO Display brief overview of the firmware data. */
  }
  
  /* -------------------- Session starting ------------------------------------------- */
  if (result == RESULT_OK)
  {
    /* Initialize the session. */
    BltSessionInit(appSessionType,appSessionSettings, 
                  appTransportType, appTransportSettings);
    /* Start the session. */
    if (BltSessionStart() != BLT_RESULT_OK)
    {
      result = RESULT_ERROR_GENERIC;
    }
  }

  /* -------------------- Erase operation -------------------------------------------- */
  if (result == RESULT_OK)
  {
    /* TODO Implement memory erase procedure for all segments. */
  }  
  
  /* -------------------- Program operation ------------------------------------------ */
  if (result == RESULT_OK)
  {
    /* TODO Implement firmware update procedure. */

    /* Do a little dummy delay check LED blink rate on the board to verify that the
     * bootloader got activated.
     */
    for (waitLoopCnt = 0; waitLoopCnt < 500000000; waitLoopCnt++)
    {
      ;
    }
  }

  /* -------------------- Session stopping ------------------------------------------- */
  if (result == RESULT_OK)
  {
    /* Stop the session. */
    BltSessionStop();
  }

  /* -------------------- Cleanup ---------------------------------------------------- */
  /* Terminate the session. */
  BltSessionTerminate();
  /* Terminate the firmware data module. */
  BltFirmwareTerminate();
  /* Free allocated memory */
  free(appTransportSettings);
  free(appSessionSettings);
  /* Give result back. */
  return result;
} /*** end of main ***/


/************************************************************************************//**
** \brief     Outputs information to the user about this program.
**
****************************************************************************************/
static void DisplayProgramInfo(void)
{
  printf("--------------------------------------------------------------------------\n");
  printf("BootCommander version 1.00. Performs firmware updates on a microcontroller\n");
  printf("based system that runs the OpenBLT bootloader.\n\n");
  printf("Copyright (c) 2017 by Feaser  http://www.feaser.com\n");
  printf("-------------------------------------------------------------------------\n");
} /*** end of DisplayProgramInfo ***/


/************************************************************************************//**
** \brief     Outputs information to the user about how to use this program.
**
****************************************************************************************/
static void DisplayProgramUsage(void)
{
  printf("Usage:    BootCommander [options] [firmware file]\n");
  printf("\n");
  printf("Example:  BootCommander -s=xcp -t=xcp_rs232 -d=COM1 -b=19200 firmware.sx\n");
  printf("\n");
  printf("The available options depend on the specified communication session\n");
  printf("protocol and communication transport layer:\n");
  printf("  -s=[name]        Name of the communication session protocol:\n");
  printf("                     xcp (default) -> XCP version 1.0.\n");
  printf("  -t=[name]        Name of the communication transport layer:\n");
  printf("                     xcp_rs232 (default) -> XCP on RS232.\n");
  printf("\n");                   
  printf("XCP version 1.0 settings (xcp):\n");
  printf("  -t1=[timeout]    Command response timeout in milliseconds as a 16-bit\n");
  printf("                   value (Default = 1000 ms).\n");
  printf("  -t3=[timeout]    Start programming timeout in milliseconds as a 16-bit\n");
  printf("                   value (Default = 2000 ms).\n");
  printf("  -t4=[timeout]    Erase memory timeout in milliseconds as a 16-bit\n");
  printf("                   value (Default = 10000 ms).\n");
  printf("  -t5=[timeout]    Program memory and target reset timeout in milli-\n");
  printf("                   seconds as a 16-bit value (Default = 1000 ms).\n");
  printf("  -t7=[timeout]    Busy wait timer timeout in milliseconds as a 16-bit\n"); 
  printf("                   value (Default = 2000 ms).\n");
  printf("  -sk=[file]       Seed/key algorithm library filename (Optional).\n");
  printf("\n");  
  printf("XCP on RS232 settings (xcp_rs232):\n");
  printf("  -d=[name]        Name of the communication device. For example COM1 or\n");
  printf("                   /dev/ttyUSB0 (Mandatory).\n");
  printf("  -b=[value]       The communication speed, a.k.a baudrate in bits per\n");
  printf("                   second, as a 32-bit value (Default = 57600).\n");
  printf("                   Supported values: 9600, 19200, 38400, 57600, 115200.\n");
  printf("\n");                   
  printf("Note that it is not necessary to specify an option if its default value\n");
  printf("is already the desired value.\n");
  printf("-------------------------------------------------------------------------\n");
} /*** end of DisplayProgramUsage ***/


/************************************************************************************//**
** \brief     Parses the command line to extract the session type. This is the one
**            specified via the -s=[name] parameter.
** \param     argc Number of program arguments.
** \param     argv Array with program parameter strings.
** \return    The session type value as used in LibOpenBLT.
**
****************************************************************************************/
static uint32_t ExtractSessionTypeFromCommandLine(int argc, char const * const argv[])
{
  uint32_t result;
  uint8_t paramIdx;
  uint8_t mapIdx; 
  /* Mapping of the supported session types to the session type values. */
  const struct
  {
    char * name;
    uint32_t value;
  } sessionMap[] =
  {
    { .name = "xcp",  .value = BLT_SESSION_XCP_V10 }
  };
  
  /* Set the default session in case nothing was specified on the command line. */
  result = sessionMap[0].value;

  /* Check parameters. */
  assert(argv != NULL);

  /* Only continue if parameters are valid. */
  if (argv != NULL) /*lint !e774 */
  {
    /* Loop through all the command line parameters, just skip the 1st one because this
     * is the name of the program, which we are not interested in.
     */
    for (paramIdx = 1; paramIdx < argc; paramIdx++)
    {
      /* Check of the if -s is present in this argument. Note that the argument must have
       * at least 4 characters to be a valid -s=[name] parameter.
       */
      if ( (strstr(argv[paramIdx], "-s=") != NULL) && (strlen(argv[paramIdx]) > 3) )
      {
        /* Loop through the map to find a matching session name. */
        for (mapIdx = 0; mapIdx < sizeof(sessionMap)/sizeof(sessionMap[0]); mapIdx++)
        {
          /* Check for matching name. */
          if (strcmp(sessionMap[mapIdx].name, &argv[paramIdx][3]) == 0)
          {
            /* Match found, now store the associated session type value. */
            result = sessionMap[mapIdx].value;
            /* No need to continue searching. */
            break;
          }
        }
      }
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of ExtractSessionTypeFromCommandLine ***/


/************************************************************************************//**
** \brief     Parses the command line to extract the session settings based on the
**            specified session type. Note that this function allocates the memory
**            necessary to store the settings. It is the caller's responsibility to free
**            this memory after it is done with it.
** \param     argc Number of program arguments.
** \param     argv Array with program parameter strings.
** \param     sessionType The session type for which to extract the settings.
** \return    Pointer to the session settings structure as used in LibOpenBLT.
**
****************************************************************************************/
static void * ExtractSessionSettingsFromCommandLine(int argc, char const * const argv[],
                                                    uint32_t sessionType)
{
  void * result = NULL;
  uint8_t paramIdx;
  
  /* Check parameters. */
  assert(argv != NULL);

  /* Only continue if parameters are valid. */
  if (argv != NULL) /*lint !e774 */
  {
    /* Filter on the session type. */
    switch (sessionType) 
    {
      /* -------------------------- XCP v1.0 ----------------------------------------- */
      case BLT_SESSION_XCP_V10:
        /* The following session specific command line parameters are supported:
         *   -t1=[timeout]  -> Command response timeout in milliseconds.
         *   -t3=[timeout]  -> Start programming timeout in milliseconds.
         *   -t4=[timeout]  -> Erase memory timeout in milliseconds.
         *   -t5=[timeout]  -> Program memory and reset timeout in milliseconds.
         *   -t7=[timeout]  -> Busy wait timer timeout in milliseconds.
         *   -sk=[file]     -> Seed/key algorithm library filename.  
         */
        /* Allocate memory for storing the settings and check the result. */
        result = malloc(sizeof(tBltSessionSettingsXcpV10));
        assert(result != NULL);
        if (result != NULL) /*lint !e774 */
        {
          /* Create typed pointer for easy reading. */
          tBltSessionSettingsXcpV10 * xcpSettings = (tBltSessionSettingsXcpV10 *)result;
          /* Set default values. */
          xcpSettings->timeoutT1 = 1000;
          xcpSettings->timeoutT3 = 2000;
          xcpSettings->timeoutT4 = 10000;
          xcpSettings->timeoutT5 = 1000;
          xcpSettings->timeoutT7 = 2000;
          xcpSettings->seedKeyFile = NULL;
          /* Loop through all the command line parameters, just skip the 1st one because 
           * this  is the name of the program, which we are not interested in.
           */
          for (paramIdx = 1; paramIdx < argc; paramIdx++)
          {
            /* Is this the -t1=[timeout] parameter? */
            if ( (strstr(argv[paramIdx], "-t1=") != NULL) && 
                 (strlen(argv[paramIdx]) > 4) )
            {
              /* Extract the timeout value. */
              sscanf(&argv[paramIdx][4], "%hu", &(xcpSettings->timeoutT1));
              /* Continue with next loop iteration. */
              continue;
            }
            /* Is this the -t3=[timeout] parameter? */
            if ( (strstr(argv[paramIdx], "-t3=") != NULL) && 
                 (strlen(argv[paramIdx]) > 4) )
            {
              /* Extract the timeout value. */
              sscanf(&argv[paramIdx][4], "%hu", &(xcpSettings->timeoutT3));
              /* Continue with next loop iteration. */
              continue;
            }
            /* Is this the -t4=[timeout] parameter? */
            if ( (strstr(argv[paramIdx], "-t4=") != NULL) && 
                 (strlen(argv[paramIdx]) > 4) )
            {
              /* Extract the timeout value. */
              sscanf(&argv[paramIdx][4], "%hu", &(xcpSettings->timeoutT4));
              /* Continue with next loop iteration. */
              continue;
            }
            /* Is this the -t5=[timeout] parameter? */
            if ( (strstr(argv[paramIdx], "-t5=") != NULL) && 
                 (strlen(argv[paramIdx]) > 4) )
            {
              /* Extract the timeout value. */
              sscanf(&argv[paramIdx][4], "%hu", &(xcpSettings->timeoutT5));
              /* Continue with next loop iteration. */
              continue;
            }
            /* Is this the -t7=[timeout] parameter? */
            if ( (strstr(argv[paramIdx], "-t7=") != NULL) && 
                 (strlen(argv[paramIdx]) > 4) )
            {
              /* Extract the timeout value. */
              sscanf(&argv[paramIdx][4], "%hu", &(xcpSettings->timeoutT7));
              /* Continue with next loop iteration. */
              continue;
            }
            /* Is this the -sk=[file] parameter? */
            if ( (strstr(argv[paramIdx], "-sk=") != NULL) && 
                 (strlen(argv[paramIdx]) > 4) )
            {
              /* Store the pointer to the seek/key filename. */
              xcpSettings->seedKeyFile = &argv[paramIdx][4];
              /* Continue with next loop iteration. */
              continue;
            }
          }
        }
        break;
      /* -------------------------- Unknown ------------------------------------------ */
      default:
        /* Noting to extract. */
        break;
    }
  }  
  /* Give the result back to the caller. */
  return result;
} /*** end of ExtractSessionSettingsFromCommandLine ***/


/************************************************************************************//**
** \brief     Parses the command line to extract the transport type. This is the one
**            specified via the -t=[name] parameter.
** \param     argc Number of program arguments.
** \param     argv Array with program parameter strings.
** \return    The transport type value as used in LibOpenBLT.
**
****************************************************************************************/
static uint32_t ExtractTransportTypeFromCommandLine(int argc, char const * const argv[])
{
  uint32_t result;
  uint8_t paramIdx;
  uint8_t mapIdx; 
  /* Mapping of the supported transport types to the transport type values. */
  const struct
  {
    char * name;
    uint32_t value;
  } transportMap[] =
  {
    { .name = "xcp_rs232", .value = BLT_TRANSPORT_XCP_V10_RS232 }
  };
  
  /* Set the default transport type in case nothing was specified on the command line. */
  result = transportMap[0].value;
  
  /* Check parameters. */
  assert(argv != NULL);

  /* Only continue if parameters are valid. */
  if (argv != NULL) /*lint !e774 */
  {
    /* Loop through all the command line parameters, just skip the 1st one because this
     * is the name of the program, which we are not interested in.
     */
    for (paramIdx = 1; paramIdx < argc; paramIdx++)
    {
      /* Check of the if -t is present in this argument. Note that the argument must have
       * at least 4 characters to be a valid -t=[name] parameter.
       */
      if ( (strstr(argv[paramIdx], "-t=") != NULL) && (strlen(argv[paramIdx]) > 3) )
      {
        /* Loop through the map to find a matching transport name. */
        for (mapIdx = 0; mapIdx < sizeof(transportMap)/sizeof(transportMap[0]); mapIdx++)
        {
          /* Check for matching name. */
          if (strcmp(transportMap[mapIdx].name, &argv[paramIdx][3]) == 0)
          {
            /* Match found, now store the associated transport type value. */
            result = transportMap[mapIdx].value;
            /* No need to continue searching. */
            break;
          }
        }
      }
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of ExtractTransportTypeFromCommandLine ***/


/************************************************************************************//**
** \brief     Parses the command line to extract the transport settings based on the
**            specified session type. Note that this function allocates the memory
**            necessary to store the settings. It is the caller's responsibility to free
**            this memory after it is done with it.
** \param     argc Number of program arguments.
** \param     argv Array with program parameter strings.
** \param     transportType The transport type for which to extract the settings.
** \return    Pointer to the transport settings structure as used in LibOpenBLT.
**
****************************************************************************************/
static void * ExtractTransportSettingsFromCommandLine(int argc, 
                                                      char const * const argv[],
                                                      uint32_t transportType)
{
  void * result = NULL;
  uint8_t paramIdx;
  
  /* Check parameters. */
  assert(argv != NULL);

  /* Only continue if parameters are valid. */
  if (argv != NULL) /*lint !e774 */
  {
    /* Filter on the session type. */
    switch (transportType) 
    {
      /* -------------------------- XCP on RS232 ------------------------------------- */
      case BLT_TRANSPORT_XCP_V10_RS232:
        /* The following transport layer specific command line parameters are supported:
         *   -d=[name]      -> Device name: /dev/ttyUSB0, COM1, etc.
         *   -b=[value]     -> Baudrate in bits per second.
         */
        /* Allocate memory for storing the settings and check the result. */
        result = malloc(sizeof(tBltTransportSettingsXcpV10Rs232));
        assert(result != NULL);
        if (result != NULL) /*lint !e774 */
        {
          /* Create typed pointer for easy reading. */
          tBltTransportSettingsXcpV10Rs232 * rs232Settings = 
            (tBltTransportSettingsXcpV10Rs232 *)result;
          /* Set default values. */
          rs232Settings->portName = NULL;
          rs232Settings->baudrate = 57600;
          /* Loop through all the command line parameters, just skip the 1st one because 
           * this  is the name of the program, which we are not interested in.
           */
          for (paramIdx = 1; paramIdx < argc; paramIdx++)
          {
            /* Is this the -d=[name] parameter? */
            if ( (strstr(argv[paramIdx], "-d=") != NULL) && 
                 (strlen(argv[paramIdx]) > 3) )
            {
              /* Store the pointer to the device name. */
              rs232Settings->portName = &argv[paramIdx][3];
              /* Continue with next loop iteration. */
              continue;
            }
            /* Is this the -b=[value] parameter? */
            if ( (strstr(argv[paramIdx], "-b=") != NULL) && 
                 (strlen(argv[paramIdx]) > 3) )
            {
              /* Extract the baudrate value. */
              sscanf(&argv[paramIdx][3], "%u", &(rs232Settings->baudrate));
              /* Continue with next loop iteration. */
              continue;
            }
          }
        }
        break;
      /* -------------------------- Unknown ------------------------------------------ */
      default:
        /* Noting to extract. */
        break;
    }
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of ExtractTransportSettingsFromCommandLine ***/


/************************************************************************************//**
** \brief     Parses the command line to extract the firmware file. This is the only
**            parameter that is specified without a '-' character at the start. 
** \param     argc Number of program arguments.
** \param     argv Array with program parameter strings.
** \return    Pointer to the character array (string) with the firmware file.
**
****************************************************************************************/
static char const * const ExtractFirmwareFileFromCommandLine(int argc, 
                                                             char const * const argv[])
{
  char const * result = NULL;
  uint8_t paramIdx;
  
    /* Check parameters. */
  assert(argv != NULL);

  /* Only continue if parameters are valid. */
  if (argv != NULL) /*lint !e774 */
  {
    /* Loop through all the command line parameters, just skip the 1st one because this
     * is the name of the program, which we are not interested in.
     */
    for (paramIdx = 1; paramIdx < argc; paramIdx++)
    {
      /* Does this one not start with the '-' character and does it have a plausible 
       * length?
       */
      if ( (argv[paramIdx][0] != '-') && (strlen(argv[paramIdx]) > 2) )
      {
        /* Store the pointer to the firmware file. */
        result = argv[paramIdx];
      }
    }    
  }
  /* Give the result back to the caller. */
  return result;
} /*** end of ExtractFirmwareFileFromCommandLine ***/


/*********************************** end of main.c *************************************/

