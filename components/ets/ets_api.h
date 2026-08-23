#pragma once

#include "../rka_api/rka_api.h"
#include "vport/ets_vport.h"
#include "ets_data.h"

namespace esphome {
namespace ets {

class ETSListener : public rka_api::RKAListener<ets_state_t> {
 public:
  void on_frame(const rka_api::rka_any_frame_t &frame, size_t size) override {
    // Ваш термостат отвечает командой 0x0A (а не 0x88)
    if (frame.type == 0x0A) {  // ← ИСПРАВЛЕНО
      this->on_state(frame.data, size);
    } else {
      rka_api::RKAListener<ets_state_t>::on_frame(frame, size);
    }
  }
};

using ETSApiBase = rka_api::RKAApi<ETSVPort>;

class ETSApi : public ETSApiBase {
 public:
  explicit ETSApi(ETSVPort *vport) : ETSApiBase(vport) {}

  void init_unk0C(uint16_t unk0C) { this->unk0C_ = unk0C; }

  void set_mode(bool *state, float target_temp, float air_temp) const;

 protected:
  uint16_t unk0C_{};
};

}  // namespace ets
}  // namespace esphome