#ifndef _EEPROM_M24_H_
#define _EEPROM_M24_H_

//! The address of the external EEPROM
#define EEPROM_M24_ADDR	0x50

/*! \brief Writes a byte to the external EEPROM
 *  \param eeprom_address The address where we wish to write in the EEPROM
 *  \param value The byte we wish to write
 *  \return 0 if the write failed, 1 if it succeeded */
uint8_t eeprom_m24_write_byte(uint16_t eeprom_address, uint8_t value);

/*! \brief Reads a byte from the external EEPROM
 *  \param eeprom_address The address where we wish to read in the EEPROM
 *  \return The returned value read from the EEPROM */
uint8_t eeprom_m24_read_byte(uint16_t eeprom_address);

/*! \brief Writes a block up to 64 bytes to the external EEPROM
 *  \param start_address The address where we wish to start writing to the EEPROM
 *  \param length The data length
 *  \param data The data that we wish to write into the EEPROM
 *  \return 0 if the write failed, 1 if it succeeded */
uint8_t eeprom_m24_write_block64(uint16_t start_address, uint8_t length, uint8_t *data);

/*! \brief Reads a block up to 64 bytes from the external EEPROM
 *  \param start_address The address where we wish to start reading from in the EEPROM
 *  \param length The length of the data
 *  \param data A pointer to where we wish to store the returned data from the EEPROM
 *  \return 0 if the read failed, 1 if it succeeded */
uint8_t eeprom_m24_read_block64(uint16_t start_address, uint8_t length, uint8_t *data);

/*! \brief Writes a block to the external EEPROM. Since there is a delay in the write we use
 *  \brief the event timer and repeat the write if the size is > 64 bytes with an interval
 *  \brief of 10 ms.
 *  \param start_address The address where we wish to start writing to the EEPROM
 *  \param length The data length
 *  \param data The data that we wish to write into the EEPROM
 *  \return 0 if the write failed, 1 if it succeeded */
uint8_t eeprom_m24_write_block(uint16_t start_address, uint16_t length, uint8_t *data);

/*! \brief Reads a block from the external EEPROM. Each block read of 64 bytes is delayed by 100us
 *  \param start_address The address where we wish to start reading from in the EEPROM
 *  \param length The length of the data
 *  \param data A pointer to where we wish to store the returned data from the EEPROM
 *  \return 0 if the read failed, 1 if it succeeded */
uint8_t eeprom_m24_read_block(uint16_t start_address, uint16_t length, uint8_t *data);

/*! \brief Check if there currently is a write in progress
 *  \return 0 if no write is in progress, 1 if there is a write going on */
uint8_t eeprom_m24_get_write_in_progress(void);

#endif
