#pragma once
#include "SpriteGo.h"
class ZombieDieEffect :
    public SpriteGo
{ 
protected:
    float timer = 10;

public:
    ZombieDieEffect();
    ~ZombieDieEffect() = default;
    ZombieDieEffect(const ZombieDieEffect& other) = default;
    ZombieDieEffect& operator=(const ZombieDieEffect& other) = default;
    void Update(float dt) override;

    void SetTimer();
};

