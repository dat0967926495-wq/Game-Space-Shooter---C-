#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Bullet.h"
#include "Entity.h"

// *** THÊM B? ??NH NGH?A LO?I K? ??CH ***
enum class EnemyType {
    Normal,
    Zigzag,
    Kamikaze
};

class Enemy : public Entity {
private:
    float speed;
    bool alive = true;
    float shootTimer = 0.f;

    int health;          // Máu
    int maxHealth;       // Máu t?i ?a
    sf::RectangleShape healthBarBack;
    sf::RectangleShape healthBar;

    // *** THÊM BI?N ?? QU?N LÝ LO?I K? ??CH ***
    EnemyType type;
    float zigzagDirection = 1.0f; // H??ng di chuy?n ngang (1 = ph?i, -1 = trái)
    float zigzagTimer = 0.f;      // B? ??m th?i gian ?? ??i h??ng

public:
    // *** C?P NH?T HÀM KH?I T?O ***
    Enemy(const sf::Texture& texture, float startX, float startY, float speed, int maxHealth, EnemyType type);

    void update(float dt, sf::Vector2f playerPosition) override;
    void draw(sf::RenderWindow& window) override;
    void shoot(std::vector<Bullet>& enemyBullets, const sf::Texture& bulletTexture);

    EnemyType getType() const { return type; }
    bool isAlive() const { return alive; }
    void destroy() { alive = false; }

    void takeDamage(int damage);
};