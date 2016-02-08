#ifndef __COMMANDS_H__
#define __COMMANDS_H__

//! Ping command to check that the computer or device is connected
#define SRI_CMD_SEND_PING               0x05
//! Reply command on a request to know if the device/computer is active
#define SRI_CMD_SEND_PONG               0x06

//! Connect command
#define SRI_CMD_CONNECT                 0x02
//! Connect command
#define SRI_CMD_DISCONNECT              0x03
//! Send debug text
#define SRI_CMD_DEBUG_TEXT              0x04
//! Retrieve the firwmare version
#define SRI_CMD_GET_FIRMWARE_REV        0x11
//! Test command, can be used for debug purpose etc
#define SRI_CMD_TEST_COMMAND            0x12
//! Sends data to or from the uC from the settings struct
#define SRI_CMD_SEND_SETTINGS           0x13
//! Save some setting to the EEPROM
#define SRI_CMD_EEPROM_SAVE             0x14
//! Update the UI data in the QT application
#define SRI_CMD_UPDATE_UI               0x15
//! Update status structs
#define SRI_CMD_SEND_STATUS             0x16
//! Display settings have changed
#define SRI_CMD_DISPLAY_RGB_CHANGED     0x17
//! Update the WINKEY functions
#define SRI_CMD_WINKEY                  0x18
//! CW Message command
#define SRI_CMD_CW_MESSAGE              0x19
//! Update status structs
#define SRI_CMD_SEND_STATUS_UPDATE      0x1A
//! AUDIO settings changed
#define SRI_CMD_AUDIO_SETTINGS_CHANGED  0x1B



//! Update the CAT functions (data has been changed and need to re-init the UART parameters)
#define SRI_CMD_CAT_SETTINGS_CHANGED    0x40

//! Winkey characters being sent
#define SRI_SUB_CMD_WINKEY_CHARS_SENT              0x01
//! Winkey Settings changed
#define SRI_CMD_SUB_WINKEY_SETTINGS_CHANGED        0x02

//! CW Message sub command, set text str
#define SRI_SUB_CMD_SET_CW_MESSAGE      0x01
//! CW Message sub command, PLAY CW message
#define SRI_SUB_CMD_PLAY_CW_MESSAGE     0x02
//! CW Message sub command, PLAY CW message
#define SRI_SUB_CMD_STOP_CW_MESSAGE     0x03

#endif
