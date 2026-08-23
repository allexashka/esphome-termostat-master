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
// Ответ состояния (команда 0x0A)
// Длина данных: 20 байт
// ============================================================
struct ets_state_t {
  enum { RSP_FRAME_TYPE = 0x0A };

  uint8_t state_;                  // 0: 0x00=ВЫКЛ, 0x01=ВКЛ
  uint8_t unk01;                   // 1: Всегда 0x7F
  int16_t target_temp_;            // 2-3: Целевая температура ×10 (Big Endian)
  int16_t air_temp_;               // 4-5: Температура воздуха ×10 (Big Endian)
  int16_t floor_temp_;             // 6-7: Температура пола ×10 (Big Endian)
  uint16_t unk08;                  // 8-9: Всегда 0x0000
  uint8_t ctl_type;                // 10: Тип управления
  ets_sens_type_t sens_type;       // 11: Тип датчика
  uint16_t unk0C;                  // 12-13: Всегда 0x01C2
  uint8_t unk0E;                   // 14: Всегда 0x00
  uint8_t antifreeze;              // 15: 0x00=ВЫКЛ, 0x32=ВКЛ
  uint8_t brightness;              // 16: Всегда 0x7F
  uint8_t open_wnd_mode;           // 17: 0x00=ВЫКЛ, 0x01=ВКЛ
  uint8_t chld_lck;                // 18: 0x00=ВКЛ, 0x01=ВЫКЛ
  uint8_t unk10;                   // 19: Неизвестно / CRC

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
// Команда изменения режима (отправка на термостат)
// Длина данных: 20 байт
// ============================================================
struct ets_mode_t {
  enum { REQ_FRAME_TYPE = 0x0A };
  enum : uint8_t { UNCHANGED = 0x7F };

  uint8_t state_;                  // 0: 0x7F=не менять, 0x00=ВЫКЛ, 0x01=ВКЛ
  uint8_t unk01;                   // 1: Всегда 0x7F
  int16_t target_temp_;            // 2-3: Целевая температура ×10 (Big Endian)
  int16_t air_temp_;               // 4-5: Температура воздуха ×10 (Big Endian)
  int16_t floor_temp_;             // 6-7: Температура пола ×10 (Big Endian)
  uint16_t unk08;                  // 8-9: Всегда 0x0000
  uint8_t ctl_type;                // 10: 0x7F=не менять
  uint8_t sens_type;               // 11: 0x7F=не менять
  uint16_t unk0C;                  // 12-13: Всегда 0x01C2
  uint8_t unk0E;                   // 14: Всегда 0x00
  uint8_t antifreeze;              // 15: 0x7F=не менять, 0x32=ВКЛ, 0x00=ВЫКЛ
  uint8_t brightness;              // 16: 0x7F=не менять
  uint8_t open_wnd_mode;           // 17: 0x7F=не менять, 0x00=ВЫКЛ, 0x01=ВКЛ
  uint8_t chld_lck;                // 18: 0x7F=не менять, 0x00=ВКЛ, 0x01=ВЫКЛ
  uint8_t unk10;                   // 19: Неизвестно / CRC

  void set_state(bool on) { this->state_ = on ? 0x01 : 0x00; }
  void set_target_temp(float temp) {
    int16_t val = static_cast<int16_t>(temp * 10.0f);
    this->target_temp_ = (val >> 8) | (val << 8);
  }
  void set_air_temp(float temp) {
    int16_t val = static_cast<int16_t>(temp * 10.0f);
    this->air_temp_ = (val >> 8) | (val << 8);
  }
  void set_floor_temp(float temp) {
    int16_t val = static_cast<int16_t>(temp * 10.0f);
    this->floor_temp_ = (val >> 8) | (val << 8);
  }
  void reset() {
    this->state_ = UNCHANGED;
    this->unk01 = UNCHANGED;
    this->target_temp_ = 0;
    this->air_temp_ = 0;
    this->floor_temp_ = 0;
    this->unk08 = 0;
    this->ctl_type = UNCHANGED;
    this->sens_type = UNCHANGED;
    this->unk0C = 0x01C2;
    this->unk0E = 0x00;
    this->antifreeze = UNCHANGED;
    this->brightness = UNCHANGED;
    this->open_wnd_mode = UNCHANGED;
    this->chld_lck = UNCHANGED;
    this->unk10 = 0;
  }
};

#pragma pack(pop)

// Проверки размера ВЫНЕСЕНЫ за пределы структур
static_assert(sizeof(ets_state_t) == 20, "ets_state_t must be exactly 20 bytes!");
static_assert(sizeof(ets_mode_t) == 20, "ets_mode_t must be exactly 20 bytes!");

}  // namespace ets
}  // namespace esphome