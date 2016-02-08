/*! \file display.h
 *  \brief Display interface
 *  \author Mikael Larsmark, SM2WMV
 *  \date 2015-12-14
 *  \code #include "display.h" \endcode
 */
//    Copyright (C) 2015  Mikael Larsmark, SM2WMV
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef _DISPLAY_
#define _DISPLAY_

#ifdef __LPC17XX__
  #include "lpc_types.h"
#endif

#ifdef __LPC17XX__

void display_welcome_screen(void);
void display_update(void);
void display_update_winkey_tx_char(uint8_t character);

void display_ptt_active(void);
void display_ptt_deactive(void);

#endif

#endif
