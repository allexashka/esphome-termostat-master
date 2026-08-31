#include "esphome/core/log.h"
#include "esphome/core/helpers.h"
#include "ets_api.h"

namespace esphome {
namespace ets {

static const char *const TAG = "ets_api";

void ETSApi::set_mode(bool *state, float target_temp, float air_temp) const {
  if (this->unk0C_ == 0) {
    ESP_LOGW(TAG, "Current state has not been received yet");
    return;
  }

  ets_mode_t mode;
  mode.reset();

  if (state) {
    mode.set_state(*state);
    ESP_LOGD(TAG, "Setting state: %s", *state ? "ON (0x01)" : "OFF (0x00)");
  } else {
    mode.set_unchanged_state();
    ESP_LOGD(TAG, "State unchanged (0x7F)");
  }

  mode.set_target_temp(target_temp);
  mode.set_air_temp(air_temp);
  mode.unk0C = this->unk0C_;

  ESP_LOGD(TAG, "Sending: state=0x%02X, target=%.1f, air=%.1f, unk0C=0x%04X",
           mode.state_, target_temp, air_temp, this->unk0C_);

  this->write_st(mode);
}

}  // namespace ets
}  // namespace esphome