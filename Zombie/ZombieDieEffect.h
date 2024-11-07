#pragma once
#include "SpriteGo.h"
class ZombieDieEffect :
    public SpriteGo
{ 
protected:
    float fadeoutTimer = 5;
    float timer = 10;

public:
    ZombieDieEffect();
    ~ZombieDieEffect() = default;
    ZombieDieEffect(const ZombieDieEffect& other) = default;
    ZombieDieEffect& operator=(const ZombieDieEffect& other) = default;
    void Update(float dt) override;

};

