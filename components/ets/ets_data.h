// ============================================================
// Методы для ets_state_t
// ============================================================
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