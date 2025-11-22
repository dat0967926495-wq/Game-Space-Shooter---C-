#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.h"
#include "Bullet.h"
#include <vector>

class Boss : public Entity {
private:
    int health;
    int maxHealth;
    float speed;
    int direction; // 1 = sang phai, -1 = sang trái
    float shootTimer;

    // Thanh máu c?a Boss
    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBar;

public:
    Boss(const sf::Texture& texture, float scale);
    void update(float dt, sf::Vector2f playerPosition) override;
    void draw(sf::RenderWindow& window) override; // Phai override thêm thanh máu

    void shoot(std::vector<Bullet>& enemyBullets, const sf::Texture& bulletTexture);
    void takeDamage(int damage);
    bool isAlive() const;
};