#include "esphome/core/log.h"

#include "ets_climate.h"

namespace esphome {
namespace ets {

static const char *const TAG = "ets_climate";

void ETSClimate::dump_config() {
  LOG_CLIMATE("", "Electrolux Thermotronic Smart", this);
  this->dump_config_(TAG);
}

ClimateTraits ETSClimate::traits() {
  auto traits = climate::ClimateTraits();

  // ⚠️ Удалено: set_supports_current_temperature больше не существует в ESPHome 2026.8.0
  // traits.set_supports_current_temperature(true);

  traits.set_visual_min_temperature(5.0f);
  traits.set_visual_max_temperature(45.0f);
  traits.set_visual_temperature_step(0.1f);

  traits.set_supported_modes({
      ClimateMode::CLIMATE_MODE_OFF,
      ClimateMode::CLIMATE_MODE_HEAT,
  });

  return traits;
}

void ETSClimate::control(const ClimateCall &call) {
  if (call.get_target_temperature().has_value()) {
    this->target_temperature = *call.get_target_temperature();
    this->api_->set_mode(nullptr, this->target_temperature, this->current_temperature);
  }

  if (call.get_mode().has_value()) {
    this->mode = *call.get_mode();
    bool mode = this->mode != ClimateMode::CLIMATE_MODE_OFF;
    this->api_->set_mode(&mode, this->target_temperature, this->current_temperature);
  }
}

void ETSClimate::on_state(const ets_state_t &state) {
  ESP_LOGI(TAG, "=== on_state called ===");
  ESP_LOGI(TAG, "  state_: 0x%02X", state.state_);
  ESP_LOGI(TAG, "  unk01: 0x%02X", state.unk01);
  ESP_LOGI(TAG, "  target_temp_: 0x%04X", state.target_temp_);
  ESP_LOGI(TAG, "  air_temp_: 0x%04X", state.air_temp_);
  ESP_LOGI(TAG, "  floor_temp_: 0x%04X", state.floor_temp_);
  ESP_LOGI(TAG, "  unk08: 0x%04X", state.unk08);
  ESP_LOGI(TAG, "  ctl_type: 0x%02X", state.ctl_type);
  ESP_LOGI(TAG, "  sens_type: 0x%02X", state.sens_type);
  ESP_LOGI(TAG, "  unk0C: 0x%04X", state.unk0C);
  ESP_LOGI(TAG, "  unk0E: 0x%02X", state.unk0E);
  ESP_LOGI(TAG, "  antifreeze: 0x%02X", state.antifreeze);
  ESP_LOGI(TAG, "  unk10: 0x%02X", state.unk10);
  ESP_LOGI(TAG, "  open_wnd_mode: 0x%02X", state.open_wnd_mode);
  ESP_LOGI(TAG, "  chld_lck: 0x%02X", state.chld_lck);

  // Инициализируем unk0C
  this->api_->init_unk0C(state.unk0C);

  this->target_temperature = state.target_temp();
  this->current_temperature = state.floor_temp();
  this->mode = state.is_off() ? climate::CLIMATE_MODE_OFF : climate::CLIMATE_MODE_HEAT;
  this->publish_state();

  if (this->floor_temp_) {
    float floor_temp = state.floor_temp();
    if (floor_temp > -10.0f) {
      this->floor_temp_->publish_state(floor_temp);
    }
  }
}

}  // namespace ets
}  // namespace esphome