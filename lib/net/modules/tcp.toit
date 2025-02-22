// Copyright (C) 2018 Toitware ApS. All rights reserved.
// Use of this source code is governed by an MIT-style license that can be
// found in the lib/LICENSE file.

import monitor show ResourceState_
import net
import net.tcp as net
import reader show Reader

import .dns
import .mtu

TOIT_TCP_READ_  ::= 1 << 0
TOIT_TCP_WRITE_ ::= 1 << 1
TOIT_TCP_CLOSE_ ::= 1 << 2
TOIT_TCP_ERROR_ ::= 1 << 3

TOIT_TCP_OPTION_PORT_          ::= 1
TOIT_TCP_OPTION_PEER_PORT_     ::= 2
TOIT_TCP_OPTION_ADDRESS_       ::= 3
TOIT_TCP_OPTION_PEER_ADDRESS_  ::= 4
TOIT_TCP_OPTION_KEEP_ALIVE_    ::= 5
TOIT_TCP_OPTION_NO_DELAY_      ::= 6
TOIT_TCP_OPTION_WINDOW_SIZE_   ::= 7
TOIT_TCP_OPTION_SEND_BUFFER_   ::= 8

// Underlying TCP socket, used to implement the TcpSocket and TcpServerSocket
// classes. It provides basic support for managing the underlying resource
// state and for closing.
class TcpSocket_:
  state_ := null

  local_address -> net.SocketAddress:
    return net.SocketAddress
      net.IpAddress.parse
        get_option_ TOIT_TCP_OPTION_ADDRESS_
      get_option_ TOIT_TCP_OPTION_PORT_

  close:
    state := state_
    if state == null: return
    state_ = null
    tcp_close_ state.group state.resource
    state.dispose
    // Remove the finalizer installed in [listen].
    remove_finalizer this

  mtu -> int: return TOIT_MTU_TCP

  open_ id:
    // TODO(kasper): Is this useful or should we just throw if we are already connected?
    if state_: close
    group := tcp_resource_group_
    state_ = ResourceState_ group id
    add_finalizer this::
      // TODO(kasper): We'd like to issue a "WARNING: socket was not closed" message here,
      // but the message cannot be printed on stdout because that interferes with the
      // LSP protocol.
      tcp_close_ group id

  ensure_state_ bits --error_bits=TOIT_TCP_ERROR_ [--failure]:
    state := ensure_state_
    state_bits := state.wait_for_state (bits | error_bits)
    if state_bits == 0:
      return failure.call "NOT_CONNECTED"
    if (state_bits & error_bits) == 0:
      return state
    error := tcp_error_ state.resource
    close
    return failure.call error

  ensure_state_:
    if state_: return state_
    throw "NOT_CONNECTED"

  get_option_ option:
    state := ensure_state_
    return tcp_get_option_ state.group state.resource option

  set_option_ option value:
    state := ensure_state_
    return tcp_set_option_ state.group state.resource option value


class TcpServerSocket extends TcpSocket_ implements net.ServerSocket:
  backlog_ := 0

  constructor: return TcpServerSocket 10
  constructor .backlog_:

  listen address port:
    open_ (tcp_listen_ tcp_resource_group_ address port backlog_)

  accept:
    return accept: throw it

  accept [failure]:
    state := ensure_state_ TOIT_TCP_READ_ --failure=failure
    id := tcp_accept_ state.group state.resource
    if not id:
      state_.clear_state TOIT_TCP_READ_
      return null
    // Create a new client socket and return it.
    socket := TcpSocket
    socket.open_ id
    return socket


class TcpSocket extends TcpSocket_ implements net.Socket Reader:
  window_size_ := 0

  constructor: return TcpSocket 0
  constructor .window_size_:

  peer_address -> net.SocketAddress:
    return net.SocketAddress
      net.IpAddress.parse
        get_option_ TOIT_TCP_OPTION_PEER_ADDRESS_
      get_option_ TOIT_TCP_OPTION_PEER_PORT_

  keep_alive: return get_option_ TOIT_TCP_OPTION_KEEP_ALIVE_
  no_delay: return get_option_ TOIT_TCP_OPTION_NO_DELAY_
  window_size: return get_option_ TOIT_TCP_OPTION_WINDOW_SIZE_

  set_keep_alive value: return set_option_ TOIT_TCP_OPTION_KEEP_ALIVE_ value
  set_no_delay value: return set_option_ TOIT_TCP_OPTION_NO_DELAY_ value

  // TODO(kasper): Make window size a named parameter to [connect]?
  connect hostname port:
    return connect hostname port: throw it

  connect hostname port [failure]:
    address := dns_lookup hostname
    open_ (tcp_connect_ tcp_resource_group_ address.raw port window_size_)
    ensure_state_ TOIT_TCP_WRITE_ --error_bits=(TOIT_TCP_ERROR_ | TOIT_TCP_CLOSE_) --failure=failure

  read:
    state := ensure_state_ TOIT_TCP_READ_ --failure=: throw it
    result := tcp_read_ state.group state.resource
    if result != -1: return result
    // TODO(anders): We could consider always clearing this after all reads.
    state.clear_state TOIT_TCP_READ_
    return ByteArray 0

  write data from = 0 to = data.size:
    state := ensure_state_ TOIT_TCP_WRITE_ --error_bits=(TOIT_TCP_ERROR_ | TOIT_TCP_CLOSE_) --failure=: throw it
    wrote := tcp_write_ state.group state.resource data from to
    if wrote == -1:
      state.clear_state TOIT_TCP_WRITE_
      return 0
    return wrote

  close_write -> none:
    state := state_
    if state == null: return
    tcp_close_write_ state.group state.resource


// Lazily-initialized resource group reference.
tcp_resource_group_ ::= tcp_init_


// Top level TCP primitives.
tcp_init_:
  #primitive.tcp.init

tcp_close_ socket_resource_group descriptor:
  #primitive.tcp.close

tcp_close_write_ socket_resource_group descriptor:
  #primitive.tcp.close_write

tcp_connect_ socket_resource_group address port window_size:
  #primitive.tcp.connect

tcp_accept_ socket_resource_group descriptor:
  #primitive.tcp.accept

tcp_listen_ socket_resource_group address port backlog:
  #primitive.tcp.listen

tcp_write_ socket_resource_group descriptor data from to:
  #primitive.tcp.write

tcp_read_ socket_resource_group descriptor:
  #primitive.tcp.read

tcp_error_ descriptor:
  #primitive.tcp.error

tcp_get_option_ socket_resource_group id option:
  #primitive.tcp.get_option

tcp_set_option_ socket_resource_group id option value:
  #primitive.tcp.set_option
