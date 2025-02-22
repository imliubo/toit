// Copyright (C) 2018 Toitware ApS.
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; version
// 2.1 only.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// The license can be found in the file `LICENSE` in the top level
// directory of this repository.

#pragma once

#include <mbedtls/aes.h>

#include "top.h"
#include "objects.h"
#include "program.h"

namespace toit {

// ----------------------------------------------------------------------------

#define MODULES(M)                           \
  M(core,    MODULE_CORE)                    \
  M(timer,   MODULE_TIMER)                   \
  M(tcp,     MODULE_TCP)                     \
  M(udp,     MODULE_UDP)                     \
  M(tls,     MODULE_TLS)                     \
  M(dns,     MODULE_DNS)                     \
  M(esp32,   MODULE_ESP32)                   \
  M(i2c,     MODULE_I2C)                     \
  M(i2s,     MODULE_I2S)                     \
  M(spi,     MODULE_SPI)                     \
  M(uart,    MODULE_UART)                    \
  M(crypto,  MODULE_CRYPTO)                  \
  M(encoding,MODULE_ENCODING)                \
  M(font,    MODULE_FONT)                    \
  M(bitmap,  MODULE_BITMAP)                  \
  M(events,  MODULE_EVENTS)                  \
  M(wifi,    MODULE_WIFI)                    \
  M(ethernet,MODULE_ETHERNET)                \
  M(ble,     MODULE_BLE)                     \
  M(dhcp,    MODULE_DHCP)                    \
  M(snapshot,MODULE_SNAPSHOT)                \
  M(serialization, MODULE_SERIALIZATION)     \
  M(image,   MODULE_IMAGE)                   \
  M(blob,    MODULE_BLOB)                    \
  M(gpio,    MODULE_GPIO)                    \
  M(adc,     MODULE_ADC)                     \
  M(pwm,     MODULE_PWM)                     \
  M(programs_registry, MODULE_PROGRAMS_REGISTRY) \
  M(file,    MODULE_FILE)                    \
  M(pipe,    MODULE_PIPE)                    \
  M(zlib,    MODULE_ZLIB)                    \
  M(subprocess, MODULE_SUBPROCESS)           \
  M(math,    MODULE_MATH)                    \
  M(x509,    MODULE_X509)                    \
  M(rpc,     MODULE_RPC)                     \

#define MODULE_CORE(PRIMITIVE)               \
  PRIMITIVE(print_string_on_stdout, 1)       \
  PRIMITIVE(print_string_on_stderr, 1)       \
  PRIMITIVE(time, 0)                         \
  PRIMITIVE(time_info, 2)                    \
  PRIMITIVE(seconds_since_epoch_local, 7)    \
  PRIMITIVE(set_tz, 1)                       \
  PRIMITIVE(platform, 0)                     \
  PRIMITIVE(process_stats, 2)                \
  PRIMITIVE(bytes_allocated_delta, 0)        \
  PRIMITIVE(string_length, 1)                \
  PRIMITIVE(string_at, 2)                    \
  PRIMITIVE(string_raw_at, 2)                \
  PRIMITIVE(array_length, 1)                 \
  PRIMITIVE(array_at, 2)                     \
  PRIMITIVE(array_at_put, 3)                 \
  PRIMITIVE(array_new, 2)                    \
  PRIMITIVE(array_expand, 3)                 \
  PRIMITIVE(array_replace, 5)                \
  PRIMITIVE(list_add, 2)                     \
  PRIMITIVE(smi_unary_minus, 1)              \
  PRIMITIVE(smi_not, 1)                      \
  PRIMITIVE(smi_and, 2)                      \
  PRIMITIVE(smi_or, 2)                       \
  PRIMITIVE(smi_xor, 2)                      \
  PRIMITIVE(string_add, 2)                   \
  PRIMITIVE(string_slice, 3)                 \
  PRIMITIVE(int64_to_string, 2)              \
  PRIMITIVE(printf_style_int64_to_string, 2) \
  PRIMITIVE(smi_to_string_base_10, 1)        \
  PRIMITIVE(compare_to, 2)                   \
  PRIMITIVE(min_special_compare_to, 2)       \
  PRIMITIVE(string_equals, 2)                \
  PRIMITIVE(string_compare, 2)               \
  PRIMITIVE(string_rune_count, 1)            \
  PRIMITIVE(object_equals, 2)                \
  PRIMITIVE(identical, 2)                    \
  PRIMITIVE(random, 0)                       \
  PRIMITIVE(random_seed, 1)                  \
  PRIMITIVE(add_entropy, 1)                  \
  PRIMITIVE(count_leading_zeros, 1)          \
  PRIMITIVE(number_to_float, 1)              \
  PRIMITIVE(put_uint_big_endian, 5)          \
  PRIMITIVE(read_int_big_endian, 4)          \
  PRIMITIVE(read_uint_big_endian, 4)         \
  PRIMITIVE(put_uint_little_endian, 5)       \
  PRIMITIVE(read_int_little_endian, 4)       \
  PRIMITIVE(read_uint_little_endian, 4)      \
  PRIMITIVE(put_float_64_little_endian, 4)   \
  PRIMITIVE(put_float_32_little_endian, 4)   \
  PRIMITIVE(smi_add, 2)                      \
  PRIMITIVE(smi_subtract, 2)                 \
  PRIMITIVE(smi_multiply, 2)                 \
  PRIMITIVE(smi_divide, 2)                   \
  PRIMITIVE(float_unary_minus, 1)            \
  PRIMITIVE(float_add, 2)                    \
  PRIMITIVE(float_subtract, 2)               \
  PRIMITIVE(float_multiply, 2)               \
  PRIMITIVE(float_divide, 2)                 \
  PRIMITIVE(float_mod, 2)                    \
  PRIMITIVE(float_round, 2)                  \
  PRIMITIVE(float_parse, 3)                  \
  PRIMITIVE(float_sign, 1)                   \
  PRIMITIVE(float_is_nan, 1)                 \
  PRIMITIVE(float_is_finite, 1)              \
  PRIMITIVE(smi_less_than, 2)                \
  PRIMITIVE(smi_less_than_or_equal, 2)       \
  PRIMITIVE(smi_greater_than, 2)             \
  PRIMITIVE(smi_greater_than_or_equal, 2)    \
  PRIMITIVE(smi_mod, 2)                      \
  PRIMITIVE(float_less_than, 2)              \
  PRIMITIVE(float_less_than_or_equal, 2)     \
  PRIMITIVE(float_greater_than, 2)           \
  PRIMITIVE(float_greater_than_or_equal, 2)  \
  PRIMITIVE(string_hash_code, 1)             \
  PRIMITIVE(string_slice_hash_code, 1)       \
  PRIMITIVE(hash_simple_json_string, 2)      \
  PRIMITIVE(compare_simple_json_string, 3)   \
  PRIMITIVE(size_of_json_number, 2)          \
  PRIMITIVE(json_skip_whitespace, 2)         \
  PRIMITIVE(smi_equals, 2)                   \
  PRIMITIVE(float_equals, 2)                 \
  PRIMITIVE(smi_shift_right,  2)             \
  PRIMITIVE(smi_unsigned_shift_right,  2)    \
  PRIMITIVE(smi_shift_left, 2)               \
  PRIMITIVE(float_to_string, 2)              \
  PRIMITIVE(float_to_raw, 1)                 \
  PRIMITIVE(raw_to_float, 1)                 \
  PRIMITIVE(float_to_raw32, 1)               \
  PRIMITIVE(raw32_to_float, 1)               \
  PRIMITIVE(object_class_id, 1)              \
  PRIMITIVE(number_to_integer, 1)            \
  PRIMITIVE(float_sqrt, 1)                   \
  PRIMITIVE(args, 0)                         \
  PRIMITIVE(hatch, 2)                        \
  PRIMITIVE(hatch_method, 0)                 \
  PRIMITIVE(hatch_args, 0)                   \
  PRIMITIVE(get_generic_resource_group, 0)   \
  PRIMITIVE(signal_kill, 1)                  \
  PRIMITIVE(current_process_id, 0)           \
  PRIMITIVE(system_send, 2)                  \
  PRIMITIVE(process_send, 3)                 \
  PRIMITIVE(task_peek_message_type, 0)       \
  PRIMITIVE(task_receive_message, 0)         \
  PRIMITIVE(concat_strings, 1)               \
  PRIMITIVE(task_current, 0)                 \
  PRIMITIVE(task_new, 1)                     \
  PRIMITIVE(task_transfer, 2)                \
  PRIMITIVE(task_stack, 1)                   \
  PRIMITIVE(task_reset_stack_limit, 0)       \
  PRIMITIVE(gc_count, 0)                     \
  PRIMITIVE(byte_array_is_raw_bytes, 1)      \
  PRIMITIVE(byte_array_length, 1)            \
  PRIMITIVE(byte_array_at, 2)                \
  PRIMITIVE(byte_array_at_put, 3)            \
  PRIMITIVE(byte_array_new, 1)               \
  PRIMITIVE(byte_array_new_external, 1)      \
  PRIMITIVE(byte_array_replace, 5)           \
  PRIMITIVE(byte_array_is_valid_string_content, 3) \
  PRIMITIVE(byte_array_convert_to_string, 3) \
  PRIMITIVE(blob_index_of, 4)                \
  PRIMITIVE(string_from_rune, 1)             \
  PRIMITIVE(string_write_to_byte_array, 5)   \
  PRIMITIVE(create_off_heap_byte_array, 1)   \
  PRIMITIVE(add_finalizer, 2)                \
  PRIMITIVE(remove_finalizer, 1)             \
  PRIMITIVE(next_finalizer_to_run, 0)        \
  PRIMITIVE(set_finalizer_notifier, 1)       \
  PRIMITIVE(large_integer_unary_minus, 1)    \
  PRIMITIVE(large_integer_not, 1)            \
  PRIMITIVE(large_integer_and, 2)            \
  PRIMITIVE(large_integer_or, 2)             \
  PRIMITIVE(large_integer_xor, 2)            \
  PRIMITIVE(large_integer_shift_right,  2)   \
  PRIMITIVE(large_integer_unsigned_shift_right,  2) \
  PRIMITIVE(large_integer_shift_left, 2)     \
  PRIMITIVE(large_integer_add, 2)            \
  PRIMITIVE(large_integer_subtract, 2)       \
  PRIMITIVE(large_integer_multiply, 2)       \
  PRIMITIVE(large_integer_divide, 2)         \
  PRIMITIVE(large_integer_mod, 2)            \
  PRIMITIVE(large_integer_equals, 2)         \
  PRIMITIVE(large_integer_less_than, 2)             \
  PRIMITIVE(large_integer_less_than_or_equal, 2)    \
  PRIMITIVE(large_integer_greater_than, 2)          \
  PRIMITIVE(large_integer_greater_than_or_equal, 2) \
  PRIMITIVE(vm_sdk_version, 0)               \
  PRIMITIVE(vm_sdk_info, 0)                  \
  PRIMITIVE(vm_sdk_model, 0)                 \
  PRIMITIVE(app_sdk_version, 0)              \
  PRIMITIVE(app_sdk_info, 0)                 \
  PRIMITIVE(encode_object, 1)                \
  PRIMITIVE(encode_error, 2)                 \
  PRIMITIVE(rebuild_hash_index, 2)           \
  PRIMITIVE(profiler_install, 1)             \
  PRIMITIVE(profiler_start, 0)               \
  PRIMITIVE(profiler_stop, 0)                \
  PRIMITIVE(profiler_encode, 2)              \
  PRIMITIVE(profiler_uninstall, 0)           \
  PRIMITIVE(set_max_heap_size, 1)            \
  PRIMITIVE(get_real_time_clock, 0)          \
  PRIMITIVE(set_real_time_clock, 2)          \
  PRIMITIVE(get_system_time, 0)              \
  PRIMITIVE(debug_set_memory_limit, 1)       \
  PRIMITIVE(dump_heap, 1)                    \
  PRIMITIVE(serial_print_heap_report, 0)     \
  PRIMITIVE(get_env, 1)                      \
  PRIMITIVE(varint_encode, 3)                \
  PRIMITIVE(varint_decode, 2)                \
  PRIMITIVE(literal_index, 1)                \
  PRIMITIVE(word_size, 0)                    \

#define MODULE_TIMER(PRIMITIVE)              \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(create, 1)                       \
  PRIMITIVE(arm, 2)                          \
  PRIMITIVE(delete, 2)                       \

#define MODULE_TCP(PRIMITIVE)                \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(close, 2)                        \
  PRIMITIVE(close_write, 2)                  \
  PRIMITIVE(connect, 4)                      \
  PRIMITIVE(accept, 2)                       \
  PRIMITIVE(listen, 4)                       \
  PRIMITIVE(write, 5)                        \
  PRIMITIVE(read, 2)                         \
  PRIMITIVE(error, 1)                        \
  PRIMITIVE(get_option, 3)                   \
  PRIMITIVE(set_option, 4)                   \

#define MODULE_UDP(PRIMITIVE)                \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(bind, 3)                         \
  PRIMITIVE(connect, 4)                      \
  PRIMITIVE(receive, 3)                      \
  PRIMITIVE(send, 7)                         \
  PRIMITIVE(get_option, 3)                   \
  PRIMITIVE(set_option, 4)                   \
  PRIMITIVE(error, 1)                        \
  PRIMITIVE(close, 2)                        \

#define MODULE_TLS(PRIMITIVE)                \
  PRIMITIVE(init, 1)                         \
  PRIMITIVE(init_socket, 2)                  \
  PRIMITIVE(create, 2)                       \
  PRIMITIVE(set_outgoing, 3)                 \
  PRIMITIVE(get_outgoing_fullness, 1)        \
  PRIMITIVE(set_incoming, 3)                 \
  PRIMITIVE(get_incoming_from, 1)            \
  PRIMITIVE(handshake, 1)                    \
  PRIMITIVE(close, 1)                        \
  PRIMITIVE(close_write, 1)                  \
  PRIMITIVE(read, 1)                         \
  PRIMITIVE(write, 4)                        \
  PRIMITIVE(add_root_certificate, 2)         \
  PRIMITIVE(add_certificate, 4)              \
  PRIMITIVE(error, 2)                        \
  PRIMITIVE(get_session, 1)                  \
  PRIMITIVE(set_session, 2)                  \

#define MODULE_DNS(PRIMITIVE)                \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(lookup, 2)                       \
  PRIMITIVE(lookup_result, 2)                \

#define MODULE_WIFI(PRIMITIVE)               \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(close, 1)                        \
  PRIMITIVE(connect, 3)                      \
  PRIMITIVE(setup_ip, 1)                     \
  PRIMITIVE(disconnect, 2)                   \
  PRIMITIVE(disconnect_reason, 1)            \
  PRIMITIVE(get_ip, 1)                       \
  PRIMITIVE(get_rssi, 1)                     \

#define MODULE_ETHERNET(PRIMITIVE)           \
  PRIMITIVE(init_esp32, 5)                   \
  PRIMITIVE(init_spi, 3)                     \
  PRIMITIVE(close, 1)                        \
  PRIMITIVE(connect, 1)                      \
  PRIMITIVE(setup_ip, 1)                     \
  PRIMITIVE(disconnect, 2)                   \
  PRIMITIVE(get_ip, 1)                       \

#define MODULE_BLE(PRIMITIVE)                \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(gap, 1)                          \
  PRIMITIVE(close, 1)                        \
  PRIMITIVE(scan_start, 2)                   \
  PRIMITIVE(scan_next, 1)                    \
  PRIMITIVE(scan_stop, 1)                    \
  PRIMITIVE(advertise_start, 3)              \
  PRIMITIVE(advertise_config, 4)             \
  PRIMITIVE(advertise_stop, 1)               \
  PRIMITIVE(connect, 2)                      \
  PRIMITIVE(get_gatt, 1)                     \
  PRIMITIVE(request_result, 1)               \
  PRIMITIVE(request_data, 1)                 \
  PRIMITIVE(request_service, 2)              \
  PRIMITIVE(request_characteristic, 3)       \
  PRIMITIVE(request_attribute, 2)            \

#define MODULE_DHCP(PRIMITIVE)               \
  PRIMITIVE(wait_for_lwip_dhcp_on_linux, 0)  \

#define MODULE_ESP32(PRIMITIVE)              \
  PRIMITIVE(reset_reason, 0)                 \
  PRIMITIVE(enable_external_wakeup, 2)       \
  PRIMITIVE(wakeup_cause, 0)                 \
  PRIMITIVE(ext1_wakeup_status, 1)           \
  PRIMITIVE(total_deep_sleep_time, 0)        \
  PRIMITIVE(total_run_time, 0)               \
  PRIMITIVE(image_config, 0)                 \
  PRIMITIVE(get_mac_address, 0)              \

#define MODULE_I2C(PRIMITIVE)                \
  PRIMITIVE(init, 3)                         \
  PRIMITIVE(close, 1)                        \
  PRIMITIVE(write, 3)                        \
  PRIMITIVE(read, 3)                         \
  PRIMITIVE(read_reg, 4)                     \

#define MODULE_I2S(PRIMITIVE)                \
  PRIMITIVE(init, 6)                         \
  PRIMITIVE(close, 1)                        \
  PRIMITIVE(write, 2)                        \

#define MODULE_SPI(PRIMITIVE)                \
  PRIMITIVE(init, 3)                         \
  PRIMITIVE(close, 1)                        \
  PRIMITIVE(device, 7)                       \
  PRIMITIVE(device_close, 2)                 \
  PRIMITIVE(transfer, 8)                     \

#define MODULE_UART(PRIMITIVE)               \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(create, 10)                      \
  PRIMITIVE(close, 2)                        \
  PRIMITIVE(set_baud_rate, 2)                \
  PRIMITIVE(write, 6)                        \
  PRIMITIVE(read, 1)                         \

#define MODULE_CRYPTO(PRIMITIVE)             \
  PRIMITIVE(sha1_start, 1)                   \
  PRIMITIVE(sha1_add, 4)                     \
  PRIMITIVE(sha1_get, 1)                     \
  PRIMITIVE(sha256_start, 1)                 \
  PRIMITIVE(sha256_add, 4)                   \
  PRIMITIVE(sha256_get, 1)                   \
  PRIMITIVE(aes_cbc_init, 4)                 \
  PRIMITIVE(aes_cbc_crypt, 5)                \
  PRIMITIVE(aes_cbc_close, 1)                \

#define MODULE_ENCODING(PRIMITIVE)           \
  PRIMITIVE(base64_encode, 2)                \
  PRIMITIVE(base64_decode, 2)                \
  PRIMITIVE(hex_encode, 1)                   \
  PRIMITIVE(hex_decode, 1)                   \

#define MODULE_FONT(PRIMITIVE)               \
  PRIMITIVE(get_font, 2)                     \
  PRIMITIVE(get_text_size, 3)                \
  PRIMITIVE(get_nonbuiltin, 2)               \
  PRIMITIVE(delete_font, 1)                  \
  PRIMITIVE(contains, 2)                     \

#define MODULE_BITMAP(PRIMITIVE)             \
  PRIMITIVE(draw_text, 8)                    \
  PRIMITIVE(byte_draw_text, 8)               \
  PRIMITIVE(draw_bitmap, 10)                 \
  PRIMITIVE(draw_bytemap, 9)                 \
  PRIMITIVE(byte_zap, 2)                     \
  PRIMITIVE(blit, 11)                        \
  PRIMITIVE(rectangle, 7)                    \
  PRIMITIVE(byte_rectangle, 7)               \
  PRIMITIVE(composit, 6)                     \
  PRIMITIVE(bytemap_blur, 4)                 \

#define MODULE_EVENTS(PRIMITIVE)             \
  PRIMITIVE(read_state, 2)                   \
  PRIMITIVE(register_object_notifier, 3)     \

#define MODULE_SNAPSHOT(PRIMITIVE)           \
  PRIMITIVE(launch, 4)                       \

#define MODULE_SERIALIZATION(PRIMITIVE)      \
  PRIMITIVE(serialize, 1)                    \
  PRIMITIVE(deserialize, 1)                  \

#define MODULE_IMAGE(PRIMITIVE)              \
  PRIMITIVE(writer_create, 2)                \
  PRIMITIVE(writer_write, 4)                 \
  PRIMITIVE(writer_commit, 2)                \
  PRIMITIVE(writer_close, 1)                 \

#define MODULE_BLOB(PRIMITIVE)               \
  PRIMITIVE(writer_create, 2)                \
  PRIMITIVE(writer_write, 2)                 \
  PRIMITIVE(writer_commit, 4)                \
  PRIMITIVE(writer_close, 1)                 \
  PRIMITIVE(content, 1)                      \
  PRIMITIVE(prepare_app_content, 2)          \
  PRIMITIVE(app_content, 1)                  \

#define MODULE_GPIO(PRIMITIVE)               \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(use, 2)                          \
  PRIMITIVE(unuse, 2)                        \
  PRIMITIVE(config, 6)                       \
  PRIMITIVE(get, 1)                          \
  PRIMITIVE(set, 2)                          \
  PRIMITIVE(config_interrupt, 2)             \

#define MODULE_ADC(PRIMITIVE)               \
  PRIMITIVE(init, 2)                        \
  PRIMITIVE(get, 2)                         \
  PRIMITIVE(close, 1)                       \

#define MODULE_PWM(PRIMITIVE)                \
  PRIMITIVE(init, 1)                         \
  PRIMITIVE(close, 1)                        \
  PRIMITIVE(start, 3)                        \
  PRIMITIVE(factor, 2)                       \
  PRIMITIVE(set_factor, 3)                   \
  PRIMITIVE(close_channel, 2)                \

#define MODULE_PROGRAMS_REGISTRY(PRIMITIVE)  \
  PRIMITIVE(next_group_id, 0)                \
  PRIMITIVE(spawn, 3)                        \
  PRIMITIVE(is_running, 2)                   \
  PRIMITIVE(kill, 2)                         \

#define MODULE_FILE(PRIMITIVE)               \
  PRIMITIVE(open, 3)                         \
  PRIMITIVE(read, 1)                         \
  PRIMITIVE(write, 4)                        \
  PRIMITIVE(close, 1)                        \
  PRIMITIVE(unlink, 1)                       \
  PRIMITIVE(rmdir, 1)                        \
  PRIMITIVE(rename, 2)                       \
  PRIMITIVE(chdir, 1)                        \
  PRIMITIVE(mkdir, 2)                        \
  PRIMITIVE(opendir, 1)                      \
  PRIMITIVE(readdir, 1)                      \
  PRIMITIVE(closedir, 1)                     \
  PRIMITIVE(stat, 2)                         \
  PRIMITIVE(mkdtemp, 1)                      \
  PRIMITIVE(is_open_file, 1)                 \
  PRIMITIVE(realpath, 1)                     \
  PRIMITIVE(cwd, 0)                          \

#define MODULE_PIPE(PRIMITIVE)               \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(close, 2)                        \
  PRIMITIVE(create_pipe, 2)                  \
  PRIMITIVE(fd_to_pipe, 2)                   \
  PRIMITIVE(write, 4)                        \
  PRIMITIVE(read, 1)                         \
  PRIMITIVE(fork, 9)                         \
  PRIMITIVE(fd, 1)                           \
  PRIMITIVE(is_a_tty, 1)                     \

#define MODULE_ZLIB(PRIMITIVE)               \
  PRIMITIVE(adler32_start, 1)                \
  PRIMITIVE(adler32_add, 5)                  \
  PRIMITIVE(adler32_get, 2)                  \
  PRIMITIVE(rle_start, 1)                    \
  PRIMITIVE(rle_add, 6)                      \
  PRIMITIVE(rle_finish, 3)                   \

#define MODULE_SUBPROCESS(PRIMITIVE)         \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(dont_wait_for, 1)                \
  PRIMITIVE(wait_for, 1)                     \
  PRIMITIVE(kill, 2)                         \
  PRIMITIVE(strsignal, 1)                    \

#define MODULE_MATH(PRIMITIVE)               \
  PRIMITIVE(sin, 1)                          \
  PRIMITIVE(cos, 1)                          \
  PRIMITIVE(tan, 1)                          \
  PRIMITIVE(sinh, 1)                         \
  PRIMITIVE(cosh, 1)                         \
  PRIMITIVE(tanh, 1)                         \
  PRIMITIVE(asin, 1)                         \
  PRIMITIVE(acos, 1)                         \
  PRIMITIVE(atan, 1)                         \
  PRIMITIVE(atan2, 2)                        \
  PRIMITIVE(sqrt, 1)                         \
  PRIMITIVE(pow, 2)                          \
  PRIMITIVE(exp, 1)                          \
  PRIMITIVE(log, 1)                          \

#define MODULE_X509(PRIMITIVE)               \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(parse, 2)                        \
  PRIMITIVE(get_common_name, 1)              \
  PRIMITIVE(close, 1)                        \

#define MODULE_RPC(PRIMITIVE)                \
  PRIMITIVE(init, 0)                         \
  PRIMITIVE(create_channel, 2)               \
  PRIMITIVE(open_channel, 2)                 \
  PRIMITIVE(send_status, 2)                  \
  PRIMITIVE(send, 4)                         \
  PRIMITIVE(has_frame, 1)                    \
  PRIMITIVE(get_stream_id, 1)                \
  PRIMITIVE(get_bits, 1)                     \
  PRIMITIVE(take_bytes, 1)                   \
  PRIMITIVE(skip, 1)                         \
  PRIMITIVE(close, 2)                        \

// ----------------------------------------------------------------------------

#define MODULE_IMPLEMENTATION_PRIMITIVE(name, arity)                \
  static Object* primitive_##name(Process*, Object**);
#define MODULE_IMPLEMENTATION_ENTRY(name, arity)                    \
  { (void*) primitive_##name, arity },
#define MODULE_IMPLEMENTATION(name, entries)                        \
  entries(MODULE_IMPLEMENTATION_PRIMITIVE)                          \
  static const PrimitiveEntry name##_primitive_table[] = {          \
    entries(MODULE_IMPLEMENTATION_ENTRY)                            \
  };                                                                \
  const PrimitiveEntry* name##_primitives = name##_primitive_table;

// ----------------------------------------------------------------------------

#define PRIMITIVE(name) \
  static Object* primitive_##name(Process* process, Object** __args)

// Usage to extract primitive arguments:
//   ARGS(int, fd, String, name)
//
// ARGS takes pairs: first type and then name
// NB: Currently ARGS only takes upto 8 pairs.

#define __ARG__(N, name, type, test)    \
  Object* _raw_##name = __args[-(N)];   \
  if (!_raw_##name->test()) WRONG_TYPE; \
  type* name = type::cast(_raw_##name);

#define _A_T_Array(N, name)         __ARG__(N, name, Array, is_array)
#define _A_T_String(N, name)        __ARG__(N, name, String, is_string)
#define _A_T_ByteArray(N, name)     __ARG__(N, name, ByteArray, is_byte_array)
#define _A_T_Task(N, name)          __ARG__(N, name, Task, is_task)
#define _A_T_Instance(N, name)      __ARG__(N, name, Instance, is_instance)
#define _A_T_HeapObject(N, name)    __ARG__(N, name, HeapObject, is_heap_object)
#define _A_T_LargeInteger(N, name)  __ARG__(N, name, LargeInteger, is_large_integer)
#define _A_T_Object(N, name) Object* name = __args[-(N)];

// Covers the range of int or Smi, whichever is smaller.
#define _A_T_int(N, name)                               \
  Object* _raw_##name = __args[-(N)];                   \
  if (!_raw_##name->is_smi()) {                         \
    if (_raw_##name->is_large_integer()) OUT_OF_RANGE;  \
    else WRONG_TYPE;                                    \
  }                                                     \
  word _word_##name = Smi::cast(_raw_##name)->value();  \
  int name = _word_##name;                              \
  if (name != _word_##name) OUT_OF_RANGE;               \

#define _A_T_uint8(N, name)                                           \
  Object* _raw_##name = __args[-(N)];                                 \
  if (!_raw_##name->is_smi()) {                                       \
    if (_raw_##name->is_large_integer()) OUT_OF_RANGE;                \
    else WRONG_TYPE;                                                  \
  }                                                                   \
  word _value_##name = Smi::cast(_raw_##name)->value();               \
  if (0 > _value_##name || _value_##name > UINT8_MAX) OUT_OF_RANGE;   \
  uint8 name = (uint8) _value_##name;

#define _A_T_uint16(N, name)                                          \
  Object* _raw_##name = __args[-(N)];                                 \
  if (!_raw_##name->is_smi()) {                                       \
    if (_raw_##name->is_large_integer()) OUT_OF_RANGE;                \
    else WRONG_TYPE;                                                  \
  }                                                                   \
  word _value_##name = Smi::cast(_raw_##name)->value();               \
  if (0 > _value_##name || _value_##name > UINT16_MAX) OUT_OF_RANGE;  \
  uint16 name = (uint16) _value_##name;

#define _A_T_int32(N, name)                                                  \
  Object* _raw_##name = __args[-(N)];                                        \
  int64 _value_##name;                                                       \
  if (_raw_##name->is_smi()) {                                               \
    _value_##name = Smi::cast(_raw_##name)->value();                         \
  } else if (_raw_##name->is_large_integer()) {                              \
    _value_##name = LargeInteger::cast(_raw_##name)->value();                \
  } else {                                                                   \
    WRONG_TYPE;                                                              \
  }                                                                          \
  if (_value_##name < INT32_MIN || _value_##name > INT32_MAX) OUT_OF_RANGE;  \
  int32 name = (int32) _value_##name;

#define _A_T_uint32(N, name)                                                 \
  Object* _raw_##name = __args[-(N)];                                        \
  int64 _value_##name;                                                       \
  if (_raw_##name->is_smi()) {                                               \
    _value_##name = Smi::cast(_raw_##name)->value();                         \
  } else if (_raw_##name->is_large_integer()) {                              \
    _value_##name = LargeInteger::cast(_raw_##name)->value();                \
  } else {                                                                   \
    WRONG_TYPE;                                                              \
  }                                                                          \
  if (_value_##name < 0 || _value_##name > UINT32_MAX) OUT_OF_RANGE;\
  uint32 name = (uint32) _value_##name;


#define _A_T_int64(N, name)                             \
  Object* _raw_##name = __args[-(N)];                   \
  int64 name;                                           \
  if (_raw_##name->is_smi()) {                          \
    name = (int64) Smi::cast(_raw_##name)->value();     \
  } else if (_raw_##name->is_large_integer()) {         \
    name = LargeInteger::cast(_raw_##name)->value();    \
  } else {                                              \
    WRONG_TYPE;                                         \
  }

#define _A_T_word(N, name)                \
  Object* _raw_##name = __args[-(N)];     \
  if (!_raw_##name->is_smi()) WRONG_TYPE; \
  word name = Smi::cast(_raw_##name)->value();

#define _A_T_double(N, name)                 \
  Object* _raw_##name = __args[-(N)];        \
  if (!_raw_##name->is_double()) WRONG_TYPE; \
  double name = Double::cast(_raw_##name)->value();

#define _A_T_to_double(N, name)                                \
  Object* _raw_##name = __args[-(N)];                          \
  double name;                                                 \
  if (_raw_##name->is_smi()) {                                 \
    name = (double) Smi::cast(_raw_##name)->value();           \
  }                                                            \
  else if (_raw_##name->is_large_integer()) {                  \
    name = (double) LargeInteger::cast(_raw_##name)->value();  \
  }                                                            \
  else if (_raw_##name->is_double()) {                         \
    name = Double::cast(_raw_##name)->value();                 \
  } else WRONG_TYPE;

#define _A_T_bool(N, name)                   \
  Object* _raw_##name = __args[-(N)];        \
  bool name = true;                          \
  if (_raw_##name == process->program()->true_object()) {}       \
  else if (_raw_##name == process->program()->false_object()) {  \
    name = false;                                                \
  } else WRONG_TYPE;

#define _A_T_cstring(N, name)                                           \
  Object* _raw_##name = __args[-(N)];                                   \
  const char* name;                                                     \
  if (_raw_##name->is_string()) {                                       \
    name = String::cast(_raw_##name)->as_cstr();                        \
  } else if (_raw_##name == process->program()->null_object()) {        \
    name = null;                                                        \
  } else WRONG_TYPE;

#define _A_T_StringOrSlice(N, name)                               \
  Object* _raw_##name = __args[-(N)];                             \
  Blob name;                                                      \
  if (!_raw_##name->byte_content(process->program(), &name, STRINGS_ONLY)) WRONG_TYPE;

#define _A_T_Blob(N, name)                                        \
  Object* _raw_##name = __args[-(N)];                             \
  Blob name;                                                      \
  if (!_raw_##name->byte_content(process->program(), &name, STRINGS_OR_BYTE_ARRAYS)) WRONG_TYPE;

#define _A_T_MutableBlob(N, name)                                                                  \
  Object* _raw_##name = __args[-(N)];                                                              \
  MutableBlob name;                                                                                \
  Error* _mutable_blob_error_##name;                                                               \
  if (!_raw_##name->mutable_byte_content(process, &name, &_mutable_blob_error_##name)) WRONG_TYPE; \
  if (name.address() == null) return _mutable_blob_error_##name;

#define MAKE_UNPACKING_MACRO(Type, N, name)                      \
  __ARG__(N, name##_proxy, ByteArray, is_byte_array)             \
  if (!name##_proxy->has_external_address() ||                   \
      name##_proxy->external_tag() < Type::tag_min ||            \
      name##_proxy->external_tag() > Type::tag_max)              \
    WRONG_TYPE;                                                  \
  Type* name = name##_proxy->as_external<Type>();                \
  if (!name) ALREADY_CLOSED;                                     \

#define _A_T_DNSResourceGroup(N, name)    MAKE_UNPACKING_MACRO(DNSResourceGroup, N, name)
#define _A_T_SimpleResourceGroup(N, name) MAKE_UNPACKING_MACRO(SimpleResourceGroup, N, name)
#define _A_T_GPIOResourceGroup(N, name)   MAKE_UNPACKING_MACRO(GPIOResourceGroup, N, name)
#define _A_T_I2CResourceGroup(N, name)    MAKE_UNPACKING_MACRO(I2CResourceGroup, N, name)
#define _A_T_I2SResourceGroup(N, name)    MAKE_UNPACKING_MACRO(I2SResourceGroup, N, name)
#define _A_T_PersistentResourceGroup(N, name)  MAKE_UNPACKING_MACRO(PersistentResourceGroup, N, name)
#define _A_T_PipeResourceGroup(N, name)   MAKE_UNPACKING_MACRO(PipeResourceGroup, N, name)
#define _A_T_SubprocessResourceGroup(N, name)  MAKE_UNPACKING_MACRO(SubprocessResourceGroup, N, name)
#define _A_T_ResourceGroup(N, name)       MAKE_UNPACKING_MACRO(ResourceGroup, N, name)
#define _A_T_SPIDevice(N, name)           MAKE_UNPACKING_MACRO(SPIDevice, N, name)
#define _A_T_SPIResourceGroup(N, name)    MAKE_UNPACKING_MACRO(SPIResourceGroup, N, name)
#define _A_T_SignalResourceGroup(N, name) MAKE_UNPACKING_MACRO(SignalResourceGroup, N, name)
#define _A_T_SocketResourceGroup(N, name) MAKE_UNPACKING_MACRO(SocketResourceGroup, N, name)
#define _A_T_TCPResourceGroup(N, name)    MAKE_UNPACKING_MACRO(TCPResourceGroup, N, name)
#define _A_T_MbedTLSResourceGroup(N, name)MAKE_UNPACKING_MACRO(MbedTLSResourceGroup, N, name)
#define _A_T_TimerResourceGroup(N, name)  MAKE_UNPACKING_MACRO(TimerResourceGroup, N, name)
#define _A_T_UDPResourceGroup(N, name)    MAKE_UNPACKING_MACRO(UDPResourceGroup, N, name)
#define _A_T_UARTResourceGroup(N, name)   MAKE_UNPACKING_MACRO(UARTResourceGroup, N, name)
#define _A_T_WifiResourceGroup(N, name)   MAKE_UNPACKING_MACRO(WifiResourceGroup, N, name)
#define _A_T_EthernetResourceGroup(N, name)   MAKE_UNPACKING_MACRO(EthernetResourceGroup, N, name)
#define _A_T_BLEResourceGroup(N, name)    MAKE_UNPACKING_MACRO(BLEResourceGroup, N, name)
#define _A_T_X509ResourceGroup(N, name)   MAKE_UNPACKING_MACRO(X509ResourceGroup, N, name)
#define _A_T_PWMResourceGroup(N, name)    MAKE_UNPACKING_MACRO(PWMResourceGroup, N, name)
#define _A_T_RpcResourceGroup(N, name)    MAKE_UNPACKING_MACRO(RpcResourceGroup, N, name)

#define _A_T_Resource(N, name)            MAKE_UNPACKING_MACRO(Resource, N, name)
#define _A_T_Directory(N, name)           MAKE_UNPACKING_MACRO(Directory, N, name)
#define _A_T_DNSLookupRequest(N, name)    MAKE_UNPACKING_MACRO(DNSLookupRequest, N, name)
#define _A_T_Font(N, name)                MAKE_UNPACKING_MACRO(Font, N, name)
#define _A_T_ImageOutputStream(N, name)   MAKE_UNPACKING_MACRO(ImageOutputStream, N, name)
#define _A_T_I2CCommand(N, name)          MAKE_UNPACKING_MACRO(I2CCommand, N, name)
#define _A_T_IntResource(N, name)         MAKE_UNPACKING_MACRO(IntResource, N, name)
#define _A_T_LookupResult(N, name)        MAKE_UNPACKING_MACRO(LookupResult, N, name)
#define _A_T_LwIPSocket(N, name)          MAKE_UNPACKING_MACRO(LwIPSocket, N, name)
#define _A_T_Timer(N, name)               MAKE_UNPACKING_MACRO(Timer, N, name)
#define _A_T_UDPSocket(N, name)           MAKE_UNPACKING_MACRO(UDPSocket, N, name)
#define _A_T_WifiEvents(N, name)          MAKE_UNPACKING_MACRO(WifiEvents, N, name)
#define _A_T_EthernetEvents(N, name)      MAKE_UNPACKING_MACRO(EthernetEvents, N, name)
#define _A_T_IPEvents(N, name)            MAKE_UNPACKING_MACRO(IPEvents, N, name)
#define _A_T_MbedTLSSocket(N, name)       MAKE_UNPACKING_MACRO(MbedTLSSocket, N, name)
#define _A_T_BaseMbedTLSSocket(N, name)   MAKE_UNPACKING_MACRO(BaseMbedTLSSocket, N, name)
#define _A_T_SslSession(N, name)          MAKE_UNPACKING_MACRO(SslSession, N, name)
#define _A_T_X509Certificate(N, name)     MAKE_UNPACKING_MACRO(X509Certificate, N, name)
#define _A_T_AesCbcContext(N, name)       MAKE_UNPACKING_MACRO(AesCbcContext, N, name)
#define _A_T_Sha1(N, name)                MAKE_UNPACKING_MACRO(Sha1, N, name)
#define _A_T_Sha256(N, name)              MAKE_UNPACKING_MACRO(Sha256, N, name)
#define _A_T_Adler32(N, name)             MAKE_UNPACKING_MACRO(Adler32, N, name)
#define _A_T_ZlibRle(N, name)             MAKE_UNPACKING_MACRO(ZlibRle, N, name)
#define _A_T_UARTResource(N, name)        MAKE_UNPACKING_MACRO(UARTResource, N, name)
#define _A_T_ADCState(N, name)            MAKE_UNPACKING_MACRO(ADCState, N, name)
#define _A_T_PWMResource(N, name)         MAKE_UNPACKING_MACRO(PWMResource, N, name)
#define _A_T_GAPResource(N, name)         MAKE_UNPACKING_MACRO(GAPResource, N, name)
#define _A_T_GATTResource(N, name)        MAKE_UNPACKING_MACRO(GATTResource, N, name)
#define _A_T_ServiceDescription(N, name)  MAKE_UNPACKING_MACRO(ServiceDescription, N, name)
#define _A_T_Peer(N, name)                MAKE_UNPACKING_MACRO(Peer, N, name)
#define _A_T_Channel(N, name)             MAKE_UNPACKING_MACRO(Channel, N, name)

// ARGS is expanded to one of the following depending on number of passed parameters.
#define _ODD ARGS cannot take odd number of arguments

#define _A_2(t1, n1) \
  _A_T_##t1(0, n1);

#define _A_4(t1, n1, t2, n2) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2);

#define _A_6(t1, n1, t2, n2, t3, n3) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2); \
  _A_T_##t3(2, n3);

#define _A_8(t1, n1, t2, n2, t3, n3, t4, n4) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2); \
  _A_T_##t3(2, n3); \
  _A_T_##t4(3, n4);

#define _A_10(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2); \
  _A_T_##t3(2, n3); \
  _A_T_##t4(3, n4); \
  _A_T_##t5(4, n5);

#define _A_12(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2); \
  _A_T_##t3(2, n3); \
  _A_T_##t4(3, n4); \
  _A_T_##t5(4, n5); \
  _A_T_##t6(5, n6);

#define _A_14(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2); \
  _A_T_##t3(2, n3); \
  _A_T_##t4(3, n4); \
  _A_T_##t5(4, n5); \
  _A_T_##t6(5, n6); \
  _A_T_##t7(6, n7);

#define _A_16(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2); \
  _A_T_##t3(2, n3); \
  _A_T_##t4(3, n4); \
  _A_T_##t5(4, n5); \
  _A_T_##t6(5, n6); \
  _A_T_##t7(6, n7); \
  _A_T_##t8(7, n8);

#define _A_18(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2); \
  _A_T_##t3(2, n3); \
  _A_T_##t4(3, n4); \
  _A_T_##t5(4, n5); \
  _A_T_##t6(5, n6); \
  _A_T_##t7(6, n7); \
  _A_T_##t8(7, n8); \
  _A_T_##t9(8, n9);

#define _A_20(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2); \
  _A_T_##t3(2, n3); \
  _A_T_##t4(3, n4); \
  _A_T_##t5(4, n5); \
  _A_T_##t6(5, n6); \
  _A_T_##t7(6, n7); \
  _A_T_##t8(7, n8); \
  _A_T_##t9(8, n9); \
  _A_T_##t10(9, n10);

#define _A_22(t1, n1, t2, n2, t3, n3, t4, n4, t5, n5, t6, n6, t7, n7, t8, n8, t9, n9, t10, n10, t11, n11) \
  _A_T_##t1(0, n1); \
  _A_T_##t2(1, n2); \
  _A_T_##t3(2, n3); \
  _A_T_##t4(3, n4); \
  _A_T_##t5(4, n5); \
  _A_T_##t6(5, n6); \
  _A_T_##t7(6, n7); \
  _A_T_##t8(7, n8); \
  _A_T_##t9(8, n9); \
  _A_T_##t10(9, n10); \
  _A_T_##t11(10, n11);

#define _OVERRIDE(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, NAME, ...) NAME

#define ARGS(...)        \
  _OVERRIDE(__VA_ARGS__, \
    _A_22, _ODD,         \
    _A_20, _ODD,         \
    _A_18, _ODD,         \
    _A_16, _ODD,         \
    _A_14, _ODD,         \
    _A_12, _ODD,         \
    _A_10, _ODD,         \
    _A_8,  _ODD,         \
    _A_6,  _ODD,         \
    _A_4,  _ODD,         \
    _A_2,  _ODD)(__VA_ARGS__)

// Macro for return a boolean object.
#define BOOL(value) ((value) ? process->program()->true_object() : process->program()->false_object())

#define ALLOCATION_FAILED return Primitive::mark_as_error(process->program()->allocation_failed())
#define ALREADY_EXISTS return Primitive::mark_as_error(process->program()->already_exists())
#define FILE_NOT_FOUND return Primitive::mark_as_error(process->program()->file_not_found())
#define HARDWARE_ERROR return Primitive::mark_as_error(process->program()->hardware_error())
#define ILLEGAL_UTF_8 return Primitive::mark_as_error(process->program()->illegal_utf_8())
#define INVALID_ARGUMENT return Primitive::mark_as_error(process->program()->invalid_argument())
#define MALLOC_FAILED return Primitive::mark_as_error(process->program()->malloc_failed())
#define NEGATIVE_ARGUMENT return Primitive::mark_as_error(process->program()->negative_argument())
#define OUT_OF_BOUNDS return Primitive::mark_as_error(process->program()->out_of_bounds())
#define OUT_OF_RANGE return Primitive::mark_as_error(process->program()->out_of_range())
#define ALREADY_IN_USE return Primitive::mark_as_error(process->program()->already_in_use())
#define OVERFLOW_ return Primitive::mark_as_error(process->program()->overflow())
#define PERMISSION_DENIED return Primitive::mark_as_error(process->program()->permission_denied())
#define QUOTA_EXCEEDED return Primitive::mark_as_error(process->program()->quota_exceeded())
#define UNIMPLEMENTED_PRIMITIVE return Primitive::mark_as_error(process->program()->unimplemented())
#define WRONG_TYPE return Primitive::mark_as_error(process->program()->wrong_object_type())
#define ALREADY_CLOSED return Primitive::mark_as_error(process->program()->already_closed())

#define OTHER_ERROR return Primitive::mark_as_error(process->program()->error())

// Support for validating a primitive is only invoked from the system process.
#define PRIVILEGED \
  if (!process->is_privileged()) return Primitive::mark_as_error(process->program()->privileged_primitive());

// ----------------------------------------------------------------------------

struct PrimitiveEntry {
  void* function;
  int arity;
};

class Primitive {
 public:
  typedef Object* Entry(Process* process, Object** arguments);

  static void set_up();

  // Use temporary tagging for marking an error.
  static bool is_error(Object* object) { return object->is_marked(); }
  static HeapObject* mark_as_error(String* string) { return string->mark(); }
  static String* unmark_from_error(Object* object) { return String::cast(object->unmark()); }
  static Object* os_error(int error, Process* process);

  // Module-specific primitive lookup. May return null if the primitive isn't linked in.
  static const PrimitiveEntry* at(unsigned module, unsigned index) {
    const PrimitiveEntry* table = _primitives[module];
    return (table == null) ? null : &table[index];
  }

  // Allocates or returns allocation failure.
  static Object* allocate_double(double value, Process* process);
  static Object* allocate_large_integer(int64 value, Process* process);
  static Object* allocate_array(int length, Object* filler, Process* process);

  static Object* integer(int64 value, Process* process) {
    if (Smi::is_valid(value)) return Smi::from((word) value);
    return allocate_large_integer(value, process);
  }

 private:
  static const PrimitiveEntry* _primitives[];
};

} // namespace toit
