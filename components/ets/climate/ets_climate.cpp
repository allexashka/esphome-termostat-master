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
  // Инициализируем unk0C
  this->api_->init_unk0C(state.unk0C);

  // ============================================================
  // Конвертация Big Endian
  // ============================================================
  auto convert_temp = [](int16_t value) -> float {
    uint16_t raw = (uint16_t)value;
    uint16_t swapped = ((raw & 0xFF) << 8) | ((raw >> 8) & 0xFF);
    return swapped * 0.1f;
  };

  float target_temp = convert_temp(state.target_temp_);
  float air_temp = convert_temp(state.air_temp_);      // ← байты 4-5
  float floor_temp = convert_temp(state.floor_temp_);  // ← байты 6-7

  // ============================================================
  // Отладка
  // ============================================================
  ESP_LOGI(TAG, "=== on_state called ===");
  ESP_LOGI(TAG, "  state_: 0x%02X (ВКЛ=%d)", state.state_, state.state_ == 0x01);
  ESP_LOGI(TAG, "  target_temp_: raw=0x%04X → %.1f°C", (uint16_t)state.target_temp_, target_temp);
  ESP_LOGI(TAG, "  air_temp_: raw=0x%04X → %.1f°C", (uint16_t)state.air_temp_, air_temp);
  ESP_LOGI(TAG, "  floor_temp_: raw=0x%04X → %.1f°C", (uint16_t)state.floor_temp_, floor_temp);
  ESP_LOGI(TAG, "  unk0C: 0x%04X", state.unk0C);
  ESP_LOGI(TAG, "  antifreeze: 0x%02X %s", state.antifreeze, state.is_antifreeze() ? "ВКЛ" : "ВЫКЛ");
  ESP_LOGI(TAG, "  open_wnd_mode: 0x%02X %s", state.open_wnd_mode, state.is_window_open() ? "ВКЛ" : "ВЫКЛ");
  ESP_LOGI(TAG, "  chld_lck: 0x%02X %s", state.chld_lck, state.is_locked() ? "ВКЛ" : "ВЫКЛ");

  // ============================================================
  // Обновляем климат
  // ============================================================
  this->target_temperature = target_temp;
  this->current_temperature = air_temp;  // ← текущая = температура ВОЗДУХА
  this->mode = state.is_off() ? climate::CLIMATE_MODE_OFF : climate::CLIMATE_MODE_HEAT;
  this->publish_state();

  // ============================================================
  // Обновляем сенсор температуры пола (если есть)
  // ============================================================
  if (this->floor_temp_) {
    if (floor_temp > -10.0f) {
      this->floor_temp_->publish_state(floor_temp);
    }
  }
}

}  // namespace ets
}  // namespace esphome