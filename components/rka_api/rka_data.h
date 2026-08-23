#pragma once

#include <cstdint>
#include <cstddef>

namespace esphome {
namespace rka_api {

#pragma pack(push, 1)

enum rka_packet_type_t : uint8_t {
  PACKET_REQ_DEV_TYPE = 0x01,
  PACKET_REQ_SAVE_DATA = 0x06,
  PACKET_REQ_LOAD_DATA = 0x07,
  PACKET_REQ_STATE = 0x08,
  PACKET_REQ_SET_COMMAND = 0x0A,
  _PACKET_RSP_CMD_MASK = 0x80,
  PACKET_RSP_DEV_TYPE = _PACKET_RSP_CMD_MASK | PACKET_REQ_DEV_TYPE,
  PACKET_RSP_SAVE_DATA = _PACKET_RSP_CMD_MASK | PACKET_REQ_SAVE_DATA,
  PACKET_RSP_LOAD_DATA = _PACKET_RSP_CMD_MASK | PACKET_REQ_LOAD_DATA,
  PACKET_RSP_STATE = _PACKET_RSP_CMD_MASK | PACKET_REQ_STATE,  // 0x88
  PACKET_RSP_SET_COMMAND = _PACKET_RSP_CMD_MASK | PACKET_REQ_SET_COMMAND,  // 0x8A
  PACKET_CMD_STATE = 0x09,
  PACKET_CMD_ERROR = 0x05,
  
  // ДОБАВЛЕНО для ETS термостата
  PACKET_RSP_STATE_ETS = 0x0A,
};

struct rka_error_t {
  enum { RSP_FRAME_TYPE = PACKET_CMD_ERROR };
  enum Code : uint8_t { CODE_BAD_CRC = 1, CODE_BAD_COMMAND = 2 } code;
};

struct rka_dev_type_t {
  enum { RSP_FRAME_TYPE = PACKET_RSP_DEV_TYPE };
  uint32_t unknown;
  enum : uint16_t { EWH = 0x1100, BWH = 0x0400, ETS = 0x0501, EHU = 0x0700 } type;
};

struct rka_result_t {
  enum { RSP_FRAME_TYPE = PACKET_RSP_SAVE_DATA };
  enum : uint8_t { RESULT_OK = 1 };
  uint8_t result;
};

struct rka_data_t {
  enum {
    RSP_FRAME_TYPE = PACKET_RSP_LOAD_DATA,
    REQ_FRAME_TYPE = PACKET_REQ_SAVE_DATA,
  };
  uint8_t data[16];
};

#pragma pack(pop)

// Размер буфера UART. Увеличено до 32 для поддержки пакетов ETS (20+ байт).
struct rka_max_data_size_t {
  static constexpr size_t value = 32;
};

}  // namespace rka_api
}  // namespace esphome