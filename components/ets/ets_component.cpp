#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

#include "ets_component.h"

namespace esphome {
namespace ets {

static const char *const TAG = "ets_api";
static const char *const NOT_IMPLEMENTED = "not implemented yet";

void ETSComponent::dump_config_(const char *TAG) const {
  LOG_SENSOR("  ", "Floor Temperature", this->floor_temp_);
  ESP_LOGCONFIG(TAG, "  Temperature control type: %s", NOT_IMPLEMENTED);
  ESP_LOGCONFIG(TAG, "  Floor Sensor: %s", NOT_IMPLEMENTED);
  ESP_LOGCONFIG(TAG, "  Antifreeze: %s", NOT_IMPLEMENTED);
  ESP_LOGCONFIG(TAG, "  Brightness: %s", NOT_IMPLEMENTED);
  ESP_LOGCONFIG(TAG, "  Open Window Mode: %s", NOT_IMPLEMENTED);
  ESP_LOGCONFIG(TAG, "  Child Lock: %s", NOT_IMPLEMENTED);
}

}  // namespace ets
}  // namespace esphome
