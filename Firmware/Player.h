#pragma once
#include "config.h"
#include "state.h"

void playTrack(int t) {
  track       = constrain(t, 1, trackTotal > 0 ? trackTotal : 9999);
  playing     = true;
  trackStart  = millis();
  pausedAccum = 0;
  dirty       = true;
  if (dfReady) df.play(track);
}

void saveSettings() {
  prefs.begin("diypod", false);
  prefs.putInt("vol",          vol);
  prefs.putInt("eqIdx",        eqIdx);
  prefs.putBool("shuffle",     shuffle);
  prefs.putInt("repeatMd",     (int)repeatMd);
  prefs.putBool("autoPlay",    autoPlay);
  prefs.putInt("sleepTimeout", sleepTimeout);
  prefs.end();
}
.
void loadSettings() {
  prefs.begin("diypod", true);
  vol          = prefs.getInt("vol",          20);
  eqIdx        = prefs.getInt("eqIdx",        0);
  shuffle      = prefs.getBool("shuffle",     false);
  repeatMd     = (RepeatMode)prefs.getInt("repeatMd", 0);
  autoPlay     = prefs.getBool("autoPlay",    true);
  sleepTimeout = prefs.getInt("sleepTimeout", 0);
  prefs.end();
}

void onFinished() {
  // Repeat single track
  if (repeatMd == REP_ON) {
    if (dfReady) df.play(track);
    trackStart = millis(); pausedAccum = 0;
    return;
  }

  if (!autoPlay && repeatMd == REP_OFF) {
    playing = false; pausedAt = millis(); dirty = true;
    return;
  }

  int next;
  if (shuffle) {
    do { next = random(1, trackTotal + 1); } while (next == track && trackTotal > 1);
  } else {
    next = track + 1;
    // End of library — stop playback
    if (trackTotal > 0 && next > trackTotal) {
      playing = false; pausedAt = millis(); dirty = true;
      return;
    }
  }
  playTrack(next);
}