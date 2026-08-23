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

struct ets_state_t {
  uint8_t state_;
  uint8_t unk01{0x7F};
  int16_t target_temp_;
  int16_t air_temp_;
  int16_t floor_temp_;
  uint16_t unk08{0};
  uint8_t ctl_type{0x7F};
  ets_sens_type_t sens_type{ST_ELECTROLUX_10KOHM};
  uint16_t unk0C{0x01C2};
  uint8_t unk0E{0};
  uint8_t antifreeze{0};
  uint8_t unk10{0x7F};
  uint8_t open_wnd_mode{0};
  uint8_t chld_lck{0x7F};

  bool is_off() const { return state_ == 0; }
  bool is_on() const { return state_ == 1; }

  // ============================================================
  // ИСПРАВЛЕНО: ручная конвертация Big Endian
  // ============================================================
  float target_temp() const {
    // target_temp_ хранится в Big Endian: [старший байт][младший байт]
    // Например: 0x00D2 → 210 → 21.0°C
    uint16_t raw = (uint16_t)target_temp_;
    uint16_t swapped = (raw >> 8) | (raw << 8);
    return swapped * 0.1f;
  }
  
  float air_temp() const {
    uint16_t raw = (uint16_t)air_temp_;
    uint16_t swapped = (raw >> 8) | (raw << 8);
    return swapped * 0.1f;
  }
  
  float floor_temp() const {
    uint16_t raw = (uint16_t)floor_temp_;
    uint16_t swapped = (raw >> 8) | (raw << 8);
    return swapped * 0.1f;
  }

  bool is_antifreeze() const { return antifreeze == 0x32; }
  bool is_window_open() const { return open_wnd_mode == 0x01; }
  bool is_locked() const { return chld_lck == 0x00; }
};

struct ets_mode_t {
  enum { UNCHANGED = 0x7F };

  uint8_t state_{UNCHANGED};
  uint8_t unk01{UNCHANGED};
  int16_t target_temp_;
  int16_t air_temp_;
  int16_t floor_temp_;
  uint16_t unk08{0};
  uint8_t ctl_type{UNCHANGED};
  uint8_t sens_type{UNCHANGED};
  uint16_t unk0C{0x01C2};
  uint8_t unk0E{0};
  uint8_t antifreeze{UNCHANGED};
  uint8_t unk10{UNCHANGED};
  uint8_t open_wnd_mode{UNCHANGED};
  uint8_t chld_lck{UNCHANGED};

  void set_state(bool is_on) { state_ = is_on ? 1 : 0; }
  void set_unchanged_state() { state_ = UNCHANGED; }

  void set_target_temp(float value) {
    uint16_t val = (uint16_t)(value * 10);
    target_temp_ = (int16_t)((val >> 8) | (val << 8));
  }
  void set_air_temp(float value) {
    uint16_t val = (uint16_t)(value * 10);
    air_temp_ = (int16_t)((val >> 8) | (val << 8));
  }
  void set_floor_temp(float value) {
    uint16_t val = (uint16_t)(value * 10);
    floor_temp_ = (int16_t)((val >> 8) | (val << 8));
  }

  void set_antifreeze(bool on) { antifreeze = on ? 0x32 : 0x00; }
  void set_window_open(bool on) { open_wnd_mode = on ? 0x01 : 0x00; }
  void set_locked(bool on) { chld_lck = on ? 0x00 : 0x01; }

  void reset() {
    state_ = UNCHANGED;
    unk01 = UNCHANGED;
    ctl_type = UNCHANGED;
    sens_type = UNCHANGED;
    antifreeze = UNCHANGED;
    unk10 = UNCHANGED;
    open_wnd_mode = UNCHANGED;
    chld_lck = UNCHANGED;
  }
};

#pragma pack(pop)

}  // namespace ets
}  // namespace esphome