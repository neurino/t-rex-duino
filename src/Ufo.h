/*
 * Project name: T-rex-duino
 * Description: T-rex game from Chrome brower rewritten for Arduino
 * Project page: https://github.com/neurino/t-rex-duino
 * Author: github.com/AlexIII and github.com/neurino
 * License: MIT
*/

#ifndef _UFO_H_
#define _UFO_H_

#include "assets.h"
#include "engine.h"

#define UFO_POSITION_Y1 15
#define UFO_POSITION_Y2 25
#define UFO_POSITION_Y3 35

static const BitmapMasked* const ufo_sprites[] = { &ufo_1, &ufo_2 };
static const int8_t ufo_y_positions[] = {UFO_POSITION_Y1, UFO_POSITION_Y2, UFO_POSITION_Y2, UFO_POSITION_Y3};

struct Ufo : SpireScrollingToLeft {
  Ufo(SpawnHold &spawnHolder) : SpireScrollingToLeft(ufo_sprites[0], UFO_SPEED, UFO_POSITION_Y1), spawnHolder(spawnHolder) { }

  virtual void step() override {
    SpireScrollingToLeft::step();
    animationStep();
    if (!isActive()) {
      if (respawnWait) {
        --respawnWait;
        return;
      }

      if (!spawnHolder.tryAcquire(this, PLAYER_SAFE_ZONE_WIDTH * 2))
        return;

      const uint16_t r = rand();
      position.y = ufo_y_positions[r & 3];
      respawnWait = scaleValue(r, UFO_RESPAWN_RATE / 2) + UFO_RESPAWN_RATE / 2;
      rearm();
    }
  }

private:
  uint16_t respawnWait = 0;
  SpawnHold &spawnHolder;

  uint8_t animationSkip = 0;
  void animationStep() {
    if (animationSkip) {
      --animationSkip;
      return;
    }
    animationSkip = 10;
    bitmap = bitmap == ufo_sprites[0] ? ufo_sprites[1] : ufo_sprites[0];
  }
};

#endif
