/*
 * power-util
 *
 * Copyright 2015-present Facebook. All Rights Reserved.
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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <syslog.h>
#include <stdint.h>
#include <string.h>
#include <getopt.h>
#include <stdbool.h>
#include <fcntl.h>
#include <openbmc/pal.h>

#define POWER_ON_STR        "on"
#define POWER_OFF_STR       "off"

const char *pwr_option_list = "status, graceful-shutdown, off, on, cycle, 12V-off,"
  "12V-on, 12V-cycle, sled-cycle";

enum {
  PWR_STATUS = 1,
  PWR_GRACEFUL_SHUTDOWN,
  PWR_OFF,
  PWR_ON,
  PWR_CYCLE,
  PWR_12V_OFF,
  PWR_12V_ON,
  PWR_12V_CYCLE,
  PWR_SLED_CYCLE
};

static int
set_last_pwr_state(uint8_t fru, char * state) {

  int ret;
  char key[MAX_KEY_LEN] = {0};

  sprintf(key, "pwr_server%d_last_state", (int) fru);

  ret = pal_set_key_value(key, state);
  if (ret < 0) {
    syslog(LOG_ALERT, "set_last_pwr_state: pal_set_key_value failed for "
        "fru %u", fru);
  }
  return ret;
}

static void
print_usage() {
  printf("Usage: power-util [ %s ] [ %s ]\n", pal_server_list, pwr_option_list);
}

static int
get_power_opt(char *option, uint8_t *opt) {

  if (!strcmp(option, "status")) {
    *opt = PWR_STATUS;
  } else if (!strcmp(option, "graceful-shutdown")) {
    *opt = PWR_GRACEFUL_SHUTDOWN;
  } else if (!strcmp(option, "off")) {
    *opt = PWR_OFF;
  } else if (!strcmp(option, "on")) {
    *opt = PWR_ON;
  } else if (!strcmp(option, "cycle")) {
    *opt = PWR_CYCLE;
  } else if (!strcmp(option, "12V-off")) {
    *opt = PWR_12V_OFF;
  } else if (!strcmp(option, "12V-on")) {
    *opt = PWR_12V_ON;
  } else if (!strcmp(option, "12V-cycle")) {
    *opt = PWR_12V_CYCLE;
  } else if (!strcmp(option, "sled-cycle")) {
    *opt = PWR_SLED_CYCLE;
  } else {
    return -1;
  }

  return 0;
}

static int
power_util(uint8_t fru, uint8_t opt) {

  int ret;
  uint8_t status;

  switch(opt) {
    case PWR_STATUS:
      ret = pal_get_server_power(fru, &status);
      if (ret < 0) {
        syslog(LOG_ALERT, "power_util: pal_get_server_power failed for fru %u\n", fru);
        return ret;
      }
      printf("Power status for fru %u : %s\n", fru, status?"ON":"OFF");
      break;

    case PWR_GRACEFUL_SHUTDOWN:

      printf("Shutting down fru %u gracefully...\n", fru);

      ret = pal_set_server_power(fru, SERVER_GRACEFUL_SHUTDOWN);
      if (ret < 0) {
        syslog(LOG_ALERT, "power_util: pal_set_server_power failed for"
          " fru %u", fru);
        return ret;
      } else if (ret == 1) {
        printf("fru %u is already powered OFF...\n", fru);
        return 0;
      }

      ret = set_last_pwr_state(fru, POWER_OFF_STR);
      if (ret < 0) {
        return ret;
      }

      ret = pal_set_led(fru, LED_STATE_OFF);
      if (ret < 0) {
        syslog(LOG_ALERT, "power_util: pal_set_led failed for fru %u", fru);
        return ret;
      }
      break;

    case PWR_OFF:

      printf("Powering fru %u to OFF state...\n", fru);

      ret = pal_set_server_power(fru, SERVER_POWER_OFF);
      if (ret < 0) {
        syslog(LOG_ALERT, "power_util: pal_set_server_power failed for"
          " fru %u", fru);
        return ret;
      } else if (ret == 1) {
        printf("fru %u is already powered OFF...\n", fru);
        return 0;
      }

      ret = set_last_pwr_state(fru, POWER_OFF_STR);
      if (ret < 0) {
        return ret;
      }

      ret = pal_set_led(fru, LED_STATE_OFF);
      if (ret < 0) {
        syslog(LOG_ALERT, "power_util: pal_set_led failed for fru %u", fru);
        return ret;
      }
      break;

    case PWR_ON:

      printf("Powering fru %u to ON state...\n", fru);

      ret = pal_set_server_power(fru, SERVER_POWER_ON);
      if (ret < 0) {
        syslog(LOG_ALERT, "power_util: pal_set_server_power failed for"
          " fru %u", fru);
        return ret;
      } else if (ret == 1) {
        printf("fru %u is already powered ON...\n", fru);
        return 0;
      }

      ret = set_last_pwr_state(fru, POWER_ON_STR);
      if (ret < 0) {
        return ret;
      }

      ret = pal_set_led(fru, LED_STATE_ON);
      if (ret < 0) {
        syslog(LOG_ALERT, "power_util: pal_set_led failed for fru %u", fru);
        return ret;
      }
      break;

    case PWR_CYCLE:

      printf("Power cycling fru %u...\n", fru);

      ret = pal_set_server_power(fru, SERVER_POWER_CYCLE);
      if (ret < 0) {
        syslog(LOG_ALERT, "power_util: pal_set_server_power failed for"
          " fru %u", fru);
        return ret;
      }

      ret = set_last_pwr_state(fru, POWER_ON_STR);
      if (ret < 0) {
        return ret;
      }

      ret = pal_set_led(fru, LED_STATE_ON);
      if (ret < 0) {
        syslog(LOG_ALERT, "power_util: pal_set_led failed for fru %u", fru);
        return ret;
      }
      break;

    case PWR_12V_OFF:
      ret = 0; // TODO: Need to add the API to support this power state setting
      break;

    case PWR_12V_ON:
      ret = 0; // TODO: Need to add the API to support this power state setting
      break;

    case PWR_12V_CYCLE:
      ret = 0; // TODO: Need to add the API to support this power state setting
      break;

    case PWR_SLED_CYCLE:
      ret = 0; // TODO: Need to add the API to support this power state setting
      break;

    default:
      syslog(LOG_ALERT, "power_util: wrong option");

  }

  return ret;
}

int
main(int argc, char **argv) {

  int ret;

  uint8_t fru, status, opt;

  if (argc < 3) {
    print_usage();
    exit (-1);
  }

  ret = get_power_opt(argv[2], &opt);
  if (ret < 0) {
    printf("Wrong option: %s\n", argv[2]);
    print_usage();
    exit(-1);
  }

  ret = pal_get_fru_id(argv[1], &fru);
  if (ret < 0) {
    printf("Wrong fru: %s\n", argv[1]);
    print_usage();
    exit(-1);
  }

  ret = pal_is_server_prsnt(fru, &status);
  if (status == 0) {
    printf("%s is empty!\n", argv[1]);
    print_usage();
    exit(-1);
  }

  ret = power_util(fru, opt);
  if (ret < 0) {
    print_usage();
    return ret;
  }
}
