#pragma once
#include "GameObject.h"

class Indicator :
    public GameObject
{
protected:
    float timer = 1;
    sf::Vector2f speed = { 0.f, -30.f };
    sf::Text body;
    std::string string = "defaultString";

public:
    Indicator();
    ~Indicator();
    Indicator(const Indicator& other) = default;
    Indicator& operator=(const Indicator& other) = default;
    
    void Init()    override;
    void Release() override;    
    void Reset()   override;
    void Update(float dt) override;
    void Draw(sf::RenderWindow& window) override;
    void SetTimer();
    void SetIndicator(int info, sf::Sprite sp, sf::Color color = sf::Color::Red);
};
