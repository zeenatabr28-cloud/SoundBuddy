#pragma once
#include "Config.h"

int getBatteryPct() {
#if VBAT_SIMULATE
  return VBAT_SIM_PCT;
#else
  static int     samples[8] = {0};
  static uint8_t idx        = 0;

  samples[idx] = analogRead(PIN_VBAT);
  idx = (idx + 1) % 8;

  long sum = 0;
  for (int i = 0; i < 8; i++) sum += samples[i];
  int raw = sum / 8;

  if (raw < 2000) return -1;  // Floating pin — no divider connected

  float v   = (raw / 4095.0f) * 3.3f * 2.0f;
  int   pct = (int)((v - 3.3f) / (4.2f - 3.3f) * 100.0f);
  pct = constrain(pct, 0, 100);

  // 2% deadband — ignore small changes to prevent display jitter
  static int lastPct = -1;
  if (lastPct >= 0 && abs(pct - lastPct) < 2) return lastPct;
  lastPct = pct;
  return pct;
#endif
}