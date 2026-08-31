#pragma once

#include "../rka_api/rka_api.h"
#include "vport/ets_vport.h"
#include "ets_data.h"

namespace esphome {
namespace ets {

class ETSListener : public rka_api::RKAListener<ets_state_t> {
 public:
  // on_frame обрабатывается в базовом классе
  // on_state переопределяется в ETSClimate
};

using ETSApiBase = rka_api::RKAApi<ETSVPort>;

class ETSApi : public ETSApiBase {
 public:
  explicit ETSApi(ETSVPort *vport) : ETSApiBase(vport) {}

  void init_unk0C(uint16_t unk0C) {
    this->unk0C_ = unk0C;
    ESP_LOGD("ets_api", "init_unk0C: 0x%04X", unk0C);
  }

  void set_mode(bool *state, float target_temp, float air_temp) const;

 protected:
  uint16_t unk0C_{};
};

}  // namespace ets
}  // namespace esphome