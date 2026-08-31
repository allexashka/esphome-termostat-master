#pragma once
#include "../rka_api/rka_data.h"

namespace esphome {
namespace ets {

#pragma pack(push, 1)

enum ets_ctl_type_t : uint8_t {
  CT_FLOOR = 0,
  CT_AIR = 1,
  CT_FLOOR_AIR = 2,
};

enum ets_sens_type_t : uint8_t {
  ST_CALEO_5KOHM = 0,
  ST_TEPLOLUX_6_8KOHM = 1,
  ST_ELECTROLUX_10KOHM = 2,
  ST_RAYCHEM_13KOHM = 3,
  ST_DEVI_15KOHM = 4,
  ST_EBERIE_33KOHM = 5,
};

// ============================================================
// Ответ состояния (19 байт)
// ============================================================
struct ets_state_t {
  // Байт 0
  uint8_t state_;          // 00=off, 01=on

  // Байт 1
  uint8_t unk01{0x7F};     // всегда 0x7F

  // Байты 2-3
  int16_t target_temp_;    // целевая (Big Endian, ×10)

  // Байты 4-5
  int16_t air_temp_;       // температура ВОЗДУХА (Big Endian, ×10)

  // Байты 6-7
  int16_t floor_temp_;     // температура ПОЛА (Big Endian, ×10)

  // Байты 8-9
  uint16_t unk08{0};       // всегда 0x0000

  // Байт 10
  uint8_t ctl_type{0x7F};  // тип управления (7F=unchanged)

  // Байт 11
  ets_sens_type_t sens_type{ST_ELECTROLUX_10KOHM};

  // Байты 12-13
  uint16_t unk0C{0x01C2};  // всегда 0x01C2

  // Байт 14
  uint8_t unk0E{0};        // всегда 0x00

  // Байт 15
  uint8_t antifreeze{0};   // 00=off, 32=on

  // Байт 16
  uint8_t unk10{0x7F};     // всегда 0x7F

  // Байт 17
  uint8_t open_wnd_mode{0}; // 00=off, 01=on

  // Байт 18
  uint8_t chld_lck{0x7F};  // 00=on, 01=off

  // --- Методы ---
  bool is_off() const { return state_ == 0; }
  bool is_on() const { return state_ == 1; }

  float target_temp() const {
    uint16_t raw = (uint16_t)target_temp_;
    uint16_t swapped = ((raw & 0xFF) << 8) | ((raw >> 8) & 0xFF);
    return swapped * 0.1f;
  }

  float air_temp() const {
    uint16_t raw = (uint16_t)air_temp_;
    uint16_t swapped = ((raw & 0xFF) << 8) | ((raw >> 8) & 0xFF);
    return swapped * 0.1f;
  }

  float floor_temp() const {
    uint16_t raw = (uint16_t)floor_temp_;
    uint16_t swapped = ((raw & 0xFF) << 8) | ((raw >> 8) & 0xFF);
    return swapped * 0.1f;
  }

  bool is_antifreeze() const { return antifreeze == 0x32; }
  bool is_window_open() const { return open_wnd_mode == 0x01; }
  bool is_locked() const { return chld_lck == 0x00; }
};

// ============================================================
// Команда изменения состояния (19 байт)
// ============================================================
struct ets_mode_t {
  enum { UNCHANGED = 0x7F };

  // Байт 0: состояние (7F=unchanged, 00=off, 01=on)
  uint8_t state_{UNCHANGED};

  // Байт 1: всегда 0x7F
  uint8_t unk01{UNCHANGED};

  // Байты 2-3: целевая температура (Big Endian, ×10)
  int16_t target_temp_;

  // Байты 4-5: всегда 0x0000
  uint16_t unk04{0};

  // Байты 6-7: температура воздуха (Big Endian, ×10)
  int16_t air_temp_;

  // Байты 8-9: всегда 0x0000
  uint16_t unk06{0};

  // Байт 10: тип управления
  uint8_t ctl_type{UNCHANGED};

  // Байт 11: тип датчика
  uint8_t sens_type{UNCHANGED};

  // Байты 12-13: unk0C (берётся из состояния!)
  uint16_t unk0C{0x01C2};

  // Байт 14: всегда 0x00
  uint8_t unk0E{0};

  // Байт 15: антизамерзание
  uint8_t antifreeze{UNCHANGED};

  // Байт 16: всегда 0x7F
  uint8_t unk10{UNCHANGED};

  // Байт 17: открытое окно
  uint8_t open_wnd_mode{UNCHANGED};

  // Байт 18: блокировка
  uint8_t chld_lck{UNCHANGED};

  // --- Методы ---
  void set_state(bool is_on) {
    state_ = is_on ? 0x01 : 0x00;
  }

  void set_unchanged_state() {
    state_ = UNCHANGED;
  }

  void set_target_temp(float value) {
    uint16_t val = (uint16_t)(value * 10);
    target_temp_ = (int16_t)((val >> 8) | (val << 8));
  }

  void set_air_temp(float value) {
    uint16_t val = (uint16_t)(value * 10);
    air_temp_ = (int16_t)((val >> 8) | (val << 8));
  }

  void set_antifreeze(bool on) {
    antifreeze = on ? 0x32 : 0x00;
  }

  void set_window_open(bool on) {
    open_wnd_mode = on ? 0x01 : 0x00;
  }

  void set_locked(bool on) {
    chld_lck = on ? 0x00 : 0x01;
  }

  void reset() {
    state_ = UNCHANGED;
    unk01 = UNCHANGED;
    ctl_type = UNCHANGED;
    sens_type = UNCHANGED;
    antifreeze = UNCHANGED;
    unk10 = UNCHANGED;
    open_wnd_mode = UNCHANGED;
    chld_lck = UNCHANGED;
    unk0E = 0;
    unk04 = 0;
    unk06 = 0;
  }
};

#pragma pack(pop)

static_assert(sizeof(ets_state_t) == 19, "ets_state_t must be exactly 19 bytes!");
static_assert(sizeof(ets_mode_t) == 19, "ets_mode_t must be exactly 19 bytes!");

}  // namespace ets
}  // namespace esphome