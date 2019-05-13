/****************************** Module Header ******************************\
* Module Name:  Service.h
* Project:      CppWin7TriggerStartService
* Copyright (c) Microsoft Corporation.
* 
* The file defines the global settings of the Windows service:
* 
*  SERVICE_NAME:          Internal name of the service
*  SERVICE_DISPLAY_NAME:  Displayed name of the service
*  SERVICE_DEPENDENCIES:  List of service dependencies
*  SERVICE_ACCOUNT:       The account under which the service should run
*  SERVICE_PASSWORD:      The password to the service account name
*  SERVICE_TRIGGER_START_ON_USB or SERVICE_TRIGGER_START_ON_IP_ADDRESS:
*                         The service trigger-start configuration
* 
* The file also declares the starting point of running the service: 
* 
*    RunService 
* 
* This source is subject to the Microsoft Public License.
* See http://www.microsoft.com/en-us/openness/licenses.aspx#MPL.
* All other rights reserved.
* 
* THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, 
* EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED 
* WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
\***************************************************************************/

#pragma once


// 
// Settings of the service
// 

// Internal name of the service
#define SERVICE_NAME             L"CppWin7TriggerStartService"

// Displayed name of the service
#define SERVICE_DISPLAY_NAME     L"CppWin7TriggerStartService Sample Service"

// List of service dependencies - "dep1\0dep2\0\0"
#define SERVICE_DEPENDENCIES     L""

// The name of the account under which the service should run
#define SERVICE_ACCOUNT          L"NT AUTHORITY\\LocalService"

// The password to the service account name
#define SERVICE_PASSWORD         NULL

// Choose *one* of the following service start triggers:
//   On USB drive arrival 
//   On IP address arrival
//   On firewall port events

#define SERVICE_TRIGGER_START_ON_USB
//#define SERVICE_TRIGGER_START_ON_IP_ADDRESS
//#define SERVICE_TRIGGER_START_ON_FIREWALL_PORT


//
//  FUNCTION: RunService
//
//  PURPOSE: Run the service
//
//  PARAMETERS:
//    none
//
//  RETURN VALUE:
//    none
//
//  COMMENTS:
//    none
//
void RunService();