#pragma once
#include "../rka_api/rka_data.h"

namespace esphome {
namespace ets {

#pragma pack(push, 1)

enum ets_ctl_type_t : uint8_t {
  CT_FLOOR = 0,      // датчик пола
  CT_AIR = 1,        // датчик воздуха
  CT_FLOOR_AIR = 2,  // датчик пола и воздуха
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
// Ответ состояния (команда 0x0A)
// ============================================================
struct ets_state_t {
  // Байт 0
  uint8_t state_;          // 00=off, 01=on
  
  // Байт 1
  uint8_t unk01{0x7F};     // всегда 0x7F
  
  // Байты 2-3
  int16_t target_temp_;    // целевая (Big Endian, ×10)
  
  // Байты 4-5
  int16_t air_temp_;       // воздух (Big Endian, ×10)
  
  // Байты 6-7
  int16_t floor_temp_;     // пол (Big Endian, ×10)
  
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
    int16_t val = (target_temp_ >> 8) | (target_temp_ << 8);
    return val * 0.1f;
  }
  float air_temp() const {
    int16_t val = (air_temp_ >> 8) | (air_temp_ << 8);
    return val * 0.1f;
  }
  float floor_temp() const {
    int16_t val = (floor_temp_ >> 8) | (floor_temp_ << 8);
    return val * 0.1f;
  }
  
  bool is_antifreeze() const { return antifreeze == 0x32; }
  bool is_window_open() const { return open_wnd_mode == 0x01; }
  bool is_locked() const { return chld_lck == 0x00; }
};

// ============================================================
// Команда изменения (команда 0x0A для отправки)
// ============================================================
struct ets_mode_t {
  enum { UNCHANGED = 0x7F };
  
  // Байт 0
  uint8_t state_{UNCHANGED};  // 7F=unchanged, 00=off, 01=on
  
  // Байт 1
  uint8_t unk01{UNCHANGED};   // всегда 0x7F
  
  // Байты 2-3
  int16_t target_temp_;       // целевая (Big Endian, ×10)
  
  // Байты 4-5
  int16_t air_temp_;          // воздух (Big Endian, ×10)
  
  // Байты 6-7
  int16_t floor_temp_;        // пол (Big Endian, ×10)
  
  // Байты 8-9
  uint16_t unk08{0};          // всегда 0x0000
  
  // Байт 10
  uint8_t ctl_type{UNCHANGED};
  
  // Байт 11
  uint8_t sens_type{UNCHANGED};
  
  // Байты 12-13
  uint16_t unk0C{0x01C2};     // всегда 0x01C2
  
  // Байт 14
  uint8_t unk0E{0};           // всегда 0x00
  
  // Байт 15
  uint8_t antifreeze{UNCHANGED};  // 32=on, 00=off
  
  // Байт 16
  uint8_t unk10{UNCHANGED};   // всегда 0x7F
  
  // Байт 17
  uint8_t open_wnd_mode{UNCHANGED};  // 00=off, 01=on
  
  // Байт 18
  uint8_t chld_lck{UNCHANGED};  // 00=on, 01=off
  
  // --- Методы ---
  void set_state(bool is_on) { state_ = is_on ? 1 : 0; }
  void set_unchanged_state() { state_ = UNCHANGED; }
  
  void set_target_temp(float value) {
    int16_t val = (int16_t)(value * 10);
    target_temp_ = (val >> 8) | (val << 8);
  }
  void set_air_temp(float value) {
    int16_t val = (int16_t)(value * 10);
    air_temp_ = (val >> 8) | (val << 8);
  }
  void set_floor_temp(float value) {
    int16_t val = (int16_t)(value * 10);
    floor_temp_ = (val >> 8) | (val << 8);
  }
  
  void set_antifreeze(bool on) { antifreeze = on ? 0x32 : 0x00; }
  void set_window_open(bool on) { open_wnd_mode = on ? 0x01 : 0x00; }
  void set_locked(bool on) { chld_lck = on ? 0x00 : 0x01; }
  
  // Сброс всех параметров в UNCHANGED
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