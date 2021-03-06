/*
* Copyright(C) NXP Semiconductors, 2011
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
 *  \brief USB Host definitions for the LPC microcontrollers.
 *  \copydetails Group_Host_LPC
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in lpcroot/libraries/LPCUSBlib/Drivers/USB/USB.h.
 */

/** \ingroup Group_Host
 *  \defgroup Group_Host_LPC Host Management (LPC)
 *  \brief USB Host definitions for the LPC microcontrollers.
 *
 *  Architecture specific USB Host definitions for the LPC microcontrollers.
 *
 *  @{
 */

#ifndef __USBHOST_LPC_H__
#define __USBHOST_LPC_H__

	/* Includes: */
		#include "../../../../Common/Common.h"
		#include "../StdDescriptors.h"
		#include "../Pipe.h"
		#include "../USBInterrupt.h"

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif

	/* Preprocessor Checks: */
		#if !defined(__INCLUDE_FROM_USB_DRIVER)
			#error Do not include this file directly. Include lpcroot/libraries/LPCUSBlib/Drivers/USB/USB.h instead.
		#endif

	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** Indicates the fixed USB device address which any attached device is enumerated to when in
			 *  host mode. As only one USB device may be attached to the LPC in host mode at any one time
			 *  and that the address used is not important (other than the fact that it is non-zero), a
			 *  fixed value is specified by the library.
			 */
			#define USB_HOST_DEVICEADDRESS                 1

			#if !defined(USB_HOST_TIMEOUT_MS) || defined(__DOXYGEN__)
				/** Constant for the maximum software timeout period of sent USB control transactions to an attached
				 *  device. If a device fails to respond to a sent control request within this period, the
				 *  library will return a timeout error code.
				 *
				 *  This value may be overridden in the user project makefile as the value of the
				 *  \ref USB_HOST_TIMEOUT_MS token, and passed to the compiler using the -D switch.
				 */
				#define USB_HOST_TIMEOUT_MS                1000
			#endif

			#if !defined(HOST_DEVICE_SETTLE_DELAY_MS) || defined(__DOXYGEN__)
				/** Constant for the delay in milliseconds after a device is connected before the library
				 *  will start the enumeration process. Some devices require a delay of up to 5 seconds
				 *  after connection before the enumeration process can start or incorrect operation will
				 *  occur.
				 *
				 *  The default delay value may be overridden in the user project makefile by defining the
				 *  \c HOST_DEVICE_SETTLE_DELAY_MS token to the required delay in milliseconds, and passed to the
				 *  compiler using the -D switch.
				 */
				#define HOST_DEVICE_SETTLE_DELAY_MS        1000
			#endif

			/** Enum for the error codes for the \ref EVENT_USB_Host_HostError() event.
			 *
			 *  \see \ref Group_Events for more information on this event.
			 */
			enum USB_Host_ErrorCodes_t
			{
				HOST_ERROR_VBusVoltageDip       = 0, /**< VBUS voltage dipped to an unacceptable level. This
				                                      *   error may be the result of an attached device drawing
				                                      *   too much current from the VBUS line, or due to the
				                                      *   LPC's power source being unable to supply sufficient
				                                      *   current.
				                                      */
			};

			/** Enum for the error codes for the \ref EVENT_USB_Host_DeviceEnumerationFailed() event.
			 *
			 *  \see \ref Group_Events for more information on this event.
			 */
			enum USB_Host_EnumerationErrorCodes_t
			{
				HOST_ENUMERROR_NoError          = 0, /**< No error occurred. Used internally, this is not a valid
				                                      *   ErrorCode parameter value for the \ref EVENT_USB_Host_DeviceEnumerationFailed()
				                                      *   event.
				                                      */
				HOST_ENUMERROR_WaitStage        = 1, /**< One of the delays between enumeration steps failed
				                                      *   to complete successfully, due to a timeout or other
				                                      *   error.
				                                      */
				HOST_ENUMERROR_NoDeviceDetected = 2, /**< No device was detected, despite the USB data lines
				                                      *   indicating the attachment of a device.
				                                      */
				HOST_ENUMERROR_ControlError     = 3, /**< One of the enumeration control requests failed to
				                                      *   complete successfully.
				                                      */
				HOST_ENUMERROR_PipeConfigError  = 4, /**< The default control pipe (address 0) failed to
				                                      *   configure correctly.
				                                      */
			};

			uint8_t USB_Host_GetActiveHost(void);
			extern uint8_t USB_Host_ControlPipeSize[MAX_USB_CORE];

		/* Inline Functions: */
			#if !defined(NO_SOF_EVENTS)
				/** Enables the host mode Start Of Frame events. When enabled, this causes the
				 *  \ref EVENT_USB_Host_StartOfFrame() event to fire once per millisecond, synchronized to the USB bus,
				 *  at the start of each USB frame when a device is enumerated while in host mode.
				 *
				 *  \note Not available when the \c NO_SOF_EVENTS compile time token is defined.
				 */
				static inline void USB_Host_EnableSOFEvents(void) ATTR_ALWAYS_INLINE;
				static inline void USB_Host_EnableSOFEvents(void)
				{

				}

				/** Disables the host mode Start Of Frame events. When disabled, this stops the firing of the
				 *  \ref EVENT_USB_Host_StartOfFrame() event when enumerated in host mode.
				 *
				 *  \note Not available when the NO_SOF_EVENTS compile time token is defined.
				 */
				static inline void USB_Host_DisableSOFEvents(void) ATTR_ALWAYS_INLINE;
				static inline void USB_Host_DisableSOFEvents(void)
				{

				}
			#endif

			/** Resets the USB bus, including the endpoints in any attached device and pipes on the LPC host.
			 *  USB bus resets leave the default control pipe configured (if already configured).
			 *
			 *  If the USB bus has been suspended prior to issuing a bus reset, the attached device will be
			 *  woken up automatically and the bus resumed after the reset has been correctly issued.
			 */
			static inline void USB_Host_ResetBus(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_ResetBus(void)
			{

			}

			/** Determines if a previously issued bus reset (via the \ref USB_Host_ResetBus() macro) has
			 *  completed.
			 *
			 *  \return Boolean \c true if no bus reset is currently being sent, \c false otherwise.
			 */
			static inline bool USB_Host_IsBusResetComplete(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline bool USB_Host_IsBusResetComplete(void)
			{
				return true;
			}

			/** Resumes USB communications with an attached and enumerated device, by resuming the transmission
			 *  of the 1MS Start Of Frame messages to the device. When resumed, USB communications between the
			 *  host and attached device may occur.
			 */
			static inline void USB_Host_ResumeBus(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_ResumeBus(void)
			{

			}

			/** Suspends the USB bus, preventing any communications from occurring between the host and attached
			 *  device until the bus has been resumed. This stops the transmission of the 1MS Start Of Frame
			 *  messages to the device.
			 */
			static inline void USB_Host_SuspendBus(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_SuspendBus(void)
			{

			}

			/** Determines if the USB bus has been suspended via the use of the \ref USB_Host_SuspendBus() macro,
			 *  false otherwise. While suspended, no USB communications can occur until the bus is resumed,
			 *  except for the Remote Wakeup event from the device if supported.
			 *
			 *  \return Boolean \c true if the bus is currently suspended, \c false otherwise.
			 */
			static inline bool USB_Host_IsBusSuspended(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline bool USB_Host_IsBusSuspended(void)
			{
				return false;
			}

			/** Determines if the attached device is currently enumerated in Full Speed mode (12Mb/s), or
			 *  false if the attached device is enumerated in Low Speed mode (1.5Mb/s).
			 *
			 *  \return Boolean \c true if the attached device is enumerated in Full Speed mode, \c false otherwise.
			 */
			static inline bool USB_Host_IsDeviceFullSpeed(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline bool USB_Host_IsDeviceFullSpeed(void)
			{
				return true; // implement later when needed
			}

			/** Determines if the attached device is currently issuing a Remote Wakeup request, requesting
			 *  that the host resume the USB bus and wake up the device, false otherwise.
			 *
			 *  \return Boolean \c true if the attached device has sent a Remote Wakeup request, \c false otherwise.
			 */
			static inline bool USB_Host_IsRemoteWakeupSent(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline bool USB_Host_IsRemoteWakeupSent(void)
			{
				return false;
			}

			/** Clears the flag indicating that a Remote Wakeup request has been issued by an attached device. */
			static inline void USB_Host_ClearRemoteWakeupSent(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_ClearRemoteWakeupSent(void)
			{

			}

			/** Accepts a Remote Wakeup request from an attached device. This must be issued in response to
			 *  a device's Remote Wakeup request within 2ms for the request to be accepted and the bus to
			 *  be resumed.
			 */
			static inline void USB_Host_ResumeFromWakeupRequest(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_ResumeFromWakeupRequest(void)
			{

			}

			/** Determines if a resume from Remote Wakeup request is currently being sent to an attached
			 *  device.
			 *
			 *  \return Boolean \c true if no resume request is currently being sent, \c false otherwise.
			 */
			static inline bool USB_Host_IsResumeFromWakeupRequestSent(void) ATTR_WARN_UNUSED_RESULT ATTR_ALWAYS_INLINE;
			static inline bool USB_Host_IsResumeFromWakeupRequestSent(void)
			{
				return false;
			}

	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Macros: */
			static inline void USB_Host_HostMode_On(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_HostMode_On(void)
			{

			}

			static inline void USB_Host_HostMode_Off(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_HostMode_Off(void)
			{

			}

			static inline void USB_Host_VBUS_Auto_Enable(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_VBUS_Auto_Enable(void)
			{

			}

			static inline void USB_Host_VBUS_Manual_Enable(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_VBUS_Manual_Enable(void)
			{

			}

			static inline void USB_Host_VBUS_Auto_On(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_VBUS_Auto_On(void)
			{

			}

			static inline void USB_Host_VBUS_Manual_On(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_VBUS_Manual_On(void)
			{

			}

			static inline void USB_Host_VBUS_Auto_Off(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_VBUS_Auto_Off(void)
			{

			}

			static inline void USB_Host_VBUS_Manual_Off(void) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_VBUS_Manual_Off(void)
			{

			}

			static inline void USB_Host_SetDeviceAddress(const uint8_t Address) ATTR_ALWAYS_INLINE;
			static inline void USB_Host_SetDeviceAddress(const uint8_t Address)
			{
				(void)Address;	//To supress warning
			}

		/* Enums: */
			enum USB_Host_WaitMSErrorCodes_t
			{
				HOST_WAITERROR_Successful       = 0,
				HOST_WAITERROR_DeviceDisconnect = 1,
				HOST_WAITERROR_PipeError        = 2,
				HOST_WAITERROR_SetupStalled     = 3,
			};

		/* Function Prototypes: */
			void    USB_Host_ProcessNextHostState(uint8_t corenum);
			uint8_t USB_Host_WaitMS(uint8_t MS);
			/** Returns the current USB frame number, when in host mode. Every millisecond the USB bus is active (i.e. not suspended)
			 *  the frame number is incremented by one.
			 */
			uint16_t USB_Host_GetFrameNumber(void);

			#if defined(__INCLUDE_FROM_HOST_C)
				//static void USB_Host_ResetDevice(void);
			#endif
	#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */

