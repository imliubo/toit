// Copyright (C) 2021 Toitware ApS. All rights reserved.
// Use of this source code is governed by an MIT-style license that can be
// found in the lib/LICENSE file.

import bytes
import log
import uart
import at

import .ublox_cellular
import .cellular_base
import .cellular

/**
Driver for Sara-R4, GSM communicating over NB-IoT & M1.
*/
class SaraR4 extends UBloxCellular:
  static CONFIG_ ::= {
    // Disable UART power saving.
    "+UPSV": [0],
    // Disable UE power saving.
    "+CPSMS": [0],
    // Disables the TCP socket Graceful Dormant Close feature. With this enabled,
    // the module waits for ack (or timeout) from peer, before closing socket
    // resources.
    "+USOCLCFG": [0],
    // The following fails when using mno=100:
    //
    //    Disable eDRX.
    //    "+CEDRXS": [0],
  }

  pwr_on/Pin?
  reset_n/Pin?

  constructor uart/uart.Port --logger=log.default --.pwr_on=null --.reset_n=null:
    super
      uart
      --logger=logger
      --config=CONFIG_
      --cat_m1
      --cat_nb1
      --preferred_baud_rate=460800
      --async_socket_connect
      --async_socket_close
      --use_psm=false

  on_connected_ session/at.Session:

  on_reset session/at.Session:
    session.send CFUN.reset

  power_on -> none:
    if pwr_on:
      pwr_on.on
      sleep --ms=150
      pwr_on.off
      // The chip needs the pin to be off for 250ms so it doesn't turn off again.
      sleep --ms=250

  power_off -> none:
    if pwr_on:
      pwr_on.on
      sleep --ms=1500
      pwr_on.off

  reset -> none:
    if reset_n:
      reset_n.on
      sleep --ms=10_000
      reset_n.off

  recover_modem:
    reset
