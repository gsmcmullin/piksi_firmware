/*
 * Copyright (C) 2011 Fergus Noble <fergusnoble@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef SWIFTNAV_SBP_H
#define SWIFTNAV_SBP_H

#include <libopencm3/cm3/common.h>
#include "peripherals/usart.h"
#include "sbp_messages.h"

/** \addtogroup sbp
 * \{ */

#define SBP_HEADER_1 0xBE
#define SBP_HEADER_2 0xEF

#define SBP_MSG(msg_type, item) sbp_send_msg(msg_type, sizeof(item), (u8*)&(item))

/* Define the type of our callback function
 * for convenience.
 */
typedef void(*msg_callback_t)(u8 msg[]);

/* Define a linked list of message callbacks. */
typedef struct msg_callbacks_node {
  u8 msg_type;
  msg_callback_t cb;
  struct msg_callbacks_node* next;
} msg_callbacks_node_t;

typedef struct {
  enum {
    WAITING_1 = 0,
    WAITING_2,
    GET_TYPE,
    GET_LEN,
    GET_MSG,
    GET_CRC
  } state;
  u8 msg_type;
  u8 msg_len;
  u8 msg_n_read;
  u8 msg_buff[256];
  u8 crc_n_read;
  u8 crc[2];
  usart_rx_dma_state* rx_state;
} sbp_process_messages_state_t;

/** \} */

void sbp_setup(u8 use_settings);
void sbp_disable();
u32 sbp_send_msg(u8 msg_type, u8 len, u8 buff[]);
void sbp_register_callback(u8 msg_type, msg_callback_t cb, msg_callbacks_node_t* node);
msg_callback_t sbp_find_callback(u8 msg_id);
void sbp_process_usart(sbp_process_messages_state_t* s);
void sbp_process_messages();
u16 crc16_ccitt(const u8* buf, u8 len, u16 crc);

#endif