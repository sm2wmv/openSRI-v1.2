/*
* Copyright(C) NXP Semiconductors, 2012
* All rights reserved.
*
* Software that is described herein is for illustrative purposes only
* which provides customers with programming information regarding the
* LPC products.  This software is supplied "AS IS" without any warranties of
* any kind, and NXP Semiconductors and its licensor disclaim any and 
* all warranties, express or implied, including all implied warranties of 
* merchantability, fitness for a particular purpose and non-infringement of 
* intellectual property rights.  NXP Semiconductors assumes no responsibility
* or liability for the use of the software, conveys no license or rights under any
* patent, copyright, mask work right, or any other intellectual property rights in 
* or to any products. NXP Semiconductors reserves the right to make changes
* in the software without notification. NXP Semiconductors also makes no 
* representation or warranty that such application will be suitable for the
* specified use without further testing or modification.
* 
* Permission to use, copy, modify, and distribute this software and its 
* documentation is hereby granted, under NXP Semiconductors' and its 
* licensor's relevant copyrights in the software, without fee, provided that it 
* is used in conjunction with NXP Semiconductors microcontrollers.  This 
* copyright, permission, and disclaimer notice must appear in all copies of 
* this code.
*/

/** \file
 *
 *  Main source file for the KeyboardHost demo. This file contains the main tasks of
 *  the demo and is responsible for the initial application hardware configuration.
 */

#include "misc.h"
#include "keyboard_host.h"

/** LPCUSBlib HID Class driver interface configuration and state information. This structure is
 *  passed to all HID Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_HID_Host_t Keyboard_HID_Interface = {
		.Config =
			{
				.DataINPipeNumber       = 1,
				.DataINPipeDoubleBank   = false,

				.DataOUTPipeNumber      = 2,
				.DataOUTPipeDoubleBank  = false,

				.HIDInterfaceProtocol   = HID_CSCP_KeyboardBootProtocol,
			},
	};

void keyboard_host_process(void) {
  KeyboardHost_Task();

  HID_Host_USBTask(&Keyboard_HID_Interface);
  USB_USBTask();
}

/** Task to manage an enumerated USB keyboard once connected, to display key state
 *  data as it is received.
 */
void KeyboardHost_Task(void)
{
	if (USB_HostState[Keyboard_HID_Interface.Config.PortNumber] != HOST_STATE_Configured)
	  return;

	if (HID_Host_IsReportReceived(&Keyboard_HID_Interface))
	{
		USB_KeyboardReport_Data_t KeyboardReport;
		HID_Host_ReceiveReport(&Keyboard_HID_Interface, &KeyboardReport);

		uint8_t KeyCode = KeyboardReport.KeyCode[0];

		if (KeyCode)
		{
			char PressedKey = 0;

			/* Retrieve pressed key character if alphanumeric */
			if ((KeyCode >= HID_KEYBOARD_SC_A) && (KeyCode <= HID_KEYBOARD_SC_Z))
			{
				PressedKey = (KeyCode - HID_KEYBOARD_SC_A) + 'A';
			}
			else if ((KeyCode >= HID_KEYBOARD_SC_1_AND_EXCLAMATION) &
			         (KeyCode  < HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS))
			{
				PressedKey = (KeyCode - HID_KEYBOARD_SC_1_AND_EXCLAMATION) + '1';
			}
			else if (KeyCode == HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS)
			{
				PressedKey = '0';
			}
			else if (KeyCode == HID_KEYBOARD_SC_SPACE)
			{
				PressedKey = ' ';
			}
			else if (KeyCode == HID_KEYBOARD_SC_ENTER)
			{
				PressedKey = '\n';
			}
			
			if (PressedKey)
			  PRINTF("USB: PRESSED: %c\n",PressedKey);
		}
	}
}

/** Event handler for the USB_DeviceAttached event. This indicates that a device has been attached to the host, and
 *  starts the library USB task to begin the enumeration and USB management process.
 */
void EVENT_USB_Host_DeviceAttached(const uint8_t corenum)
{
  PRINTF("USB: Device Attached on port %d\r\n",corenum);
}

/** Event handler for the USB_DeviceUnattached event. This indicates that a device has been removed from the host, and
 *  stops the library USB task management process.
 */
void EVENT_USB_Host_DeviceUnattached(const uint8_t corenum)
{
  PRINTF("USB: Device Unattached on port %d\r\n",corenum);
}

/** Event handler for the USB_DeviceEnumerationComplete event. This indicates that a device has been successfully
 *  enumerated by the host and is now ready to be used by the application.
 */
void EVENT_USB_Host_DeviceEnumerationComplete(const uint8_t corenum)
{
	uint16_t ConfigDescriptorSize;
	uint8_t  ConfigDescriptorData[512];

	if (USB_Host_GetDeviceConfigDescriptor(corenum, 1, &ConfigDescriptorSize, ConfigDescriptorData,
	                                       sizeof(ConfigDescriptorData)) != HOST_GETCONFIG_Successful)
	{
		PRINTF("USB: Error Retrieving Configuration Descriptor.\r\n");
		return;
	}

	Keyboard_HID_Interface.Config.PortNumber = corenum;

	if (HID_Host_ConfigurePipes(&Keyboard_HID_Interface,
	                            ConfigDescriptorSize, ConfigDescriptorData) != HID_ENUMERROR_NoError)
	{
		PRINTF("USB: Attached Device Not a Valid Keyboard.\r\n");
		return;
	}

	if (USB_Host_SetDeviceConfiguration(Keyboard_HID_Interface.Config.PortNumber,1) != HOST_SENDCONTROL_Successful)
	{
		PRINTF("USB: Error Setting Device Configuration.\r\n");
		return;
	}

	if (HID_Host_SetBootProtocol(&Keyboard_HID_Interface) != 0)
	{
		PRINTF("USB: Could not Set Boot Protocol Mode.\r\n");
		USB_Host_SetDeviceConfiguration(Keyboard_HID_Interface.Config.PortNumber,0);
		return;
	}

	PRINTF("USB: Keyboard Enumerated.\r\n");
}

/** Event handler for the USB_HostError event. This indicates that a hardware error occurred while in host mode. */
void EVENT_USB_Host_HostError(const uint8_t corenum, const uint8_t ErrorCode)
{
	USB_Disable();

	PRINTF("USB: HOST ERROR: %i, %i, %i\n", corenum, ErrorCode);
	for(;;);
}

/** Event handler for the USB_DeviceEnumerationFailed event. This indicates that a problem occurred while
 *  enumerating an attached USB device.
 */
void EVENT_USB_Host_DeviceEnumerationFailed(const uint8_t corenum,
											const uint8_t ErrorCode,
                                            const uint8_t SubErrorCode)
{
  PRINTF("USB: DEV NUM ERROR: %i, %i, %i\n", corenum, ErrorCode, SubErrorCode);

}

/** Dummy callback function for HID Parser
 */
bool CALLBACK_HIDParser_FilterHIDReportItem(HID_ReportItem_t* const CurrentItem)
{
  //To get rid of warning
  if (CurrentItem == CurrentItem)
    return true;

  return true;
}
