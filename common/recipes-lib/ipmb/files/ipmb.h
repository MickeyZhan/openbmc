/*
 *
 * Copyright 2015-present Facebook. All Rights Reserved.
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __IPMB_H__
#define __IPMB_H__

#ifdef __cplusplus
extern "C" {
#endif

#define SOCK_PATH_IPMB "/tmp/ipmb_socket"

#define BMC_SLAVE_ADDR 0x10
#define BRIDGE_SLAVE_ADDR 0x20
#define ZERO_CKSUM_CONST 0x100

// rqSA, rsSA, rqSeq, hdrCksum, dataCksum
#define IPMB_HDR_SIZE 5

// rqSA, NetFn, hdrCksum
#define IPMB_DATA_OFFSET 3

// Slot#0 is on I2C Bus1
#define IPMB_BUS_SLOT0 1

#define TIMEOUT_IPMI 4
#define MAX_IPMB_RES_LEN 255

typedef struct _ipmb_req_t {
  uint8_t res_slave_addr;
  uint8_t netfn_lun;
  uint8_t hdr_cksum;
  uint8_t req_slave_addr;
  uint8_t seq_lun;
  uint8_t cmd;
  uint8_t data[];
} ipmb_req_t;

typedef struct _ipmb_res_t {
  uint8_t req_slave_addr;
  uint8_t netfn_lun;
  uint8_t hdr_cksum;
  uint8_t res_slave_addr;
  uint8_t seq_lun;
  uint8_t cmd;
  uint8_t cc;
  uint8_t data[];
} ipmb_res_t;

void lib_ipmb_handle(unsigned char bus_id,
                  unsigned char *request, unsigned char req_len,
                  unsigned char *response, unsigned char *res_len);

#ifdef __cplusplus
} // extern "C"
#endif

#endif /* __IPMB_H__ */
