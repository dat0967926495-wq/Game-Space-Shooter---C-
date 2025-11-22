#include "Boss.h"
#include <iostream>

// Kích th??c c?a s? ?? tham kh?o
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

Boss::Boss(const sf::Texture& texture, float scale) { // <-- Nh?n thêm scale
    sprite.setTexture(texture);
    // Làm cho Boss l?n h?n quái th??ng
    sprite.setScale(scale, scale); // <-- S? d?ng scale ???c truy?n vào
    // ??t ? v? trí trên cùng, chính gi?a
    sprite.setPosition((WINDOW_WIDTH - sprite.getGlobalBounds().width) / 2.f, 50.f);

    maxHealth = 50; // Boss có 50 máu
    health = maxHealth;
    speed = 100.f; // T?c ?? di chuy?n ngang
    direction = 1; // B?t ??u di chuy?n sang ph?i
    shootTimer = 0.f;

    // Thanh máu (??t ? trên cùng màn hình)
    healthBarBack.setSize(sf::Vector2f(WINDOW_WIDTH - 20.f, 20.f));
    healthBarBack.setFillColor(sf::Color(50, 50, 50));
    healthBarBack.setPosition(10.f, 10.f);

    healthBar.setSize(sf::Vector2f(WINDOW_WIDTH - 20.f, 20.f));
    healthBar.setFillColor(sf::Color::Red);
    healthBar.setPosition(10.f, 10.f);
}

void Boss::update(float dt, sf::Vector2f playerPosition) {
    // 1. C?p nh?t di chuy?n
    sprite.move(speed * direction * dt, 0.f);

    // Ki?m tra va ch?m l?
    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();

    if (pos.x <= 0.f) { // Ch?m l? trái
        direction = 1; // ??i h??ng ph?i
    }
    else if (pos.x + bounds.width >= WINDOW_WIDTH) { // Ch?m l? ph?i
        direction = -1; // ??i h??ng trái
    }

    // 2. C?p nh?t thanh máu
    float healthPercent = static_cast<float>(health) / maxHealth;
    healthBar.setSize(sf::Vector2f((WINDOW_WIDTH - 20.f) * healthPercent, 20.f));

    // 3. C?p nh?t th?i gian b?n
    shootTimer += dt;
}

// Boss b?n 3 viên ??n cùng lúc t? 3 v? trí
void Boss::shoot(std::vector<Bullet>& enemyBullets, const sf::Texture& bulletTexture) {
    if (shootTimer > 2.f) { // Ban moii 2 giây
        sf::FloatRect bounds = sprite.getGlobalBounds();
        sf::Vector2f pos = sprite.getPosition();

        // V? trí 3 "súng"
        float gunLeft = pos.x + bounds.width * 0.2f;
        float gunMid = pos.x + bounds.width * 0.5f;
        float gunRight = pos.x + bounds.width * 0.8f;
        float gunY = pos.y + bounds.height;

        // T?o 3 viên ??n (speed -150.f ?? bay xu?ng, gi?ng Enemy.cpp)
        enemyBullets.push_back(Bullet(bulletTexture, gunLeft, gunY, -100.f, 0.5f));
        enemyBullets.push_back(Bullet(bulletTexture, gunMid, gunY, -100.f, 0.5f));
        enemyBullets.push_back(Bullet(bulletTexture, gunRight, gunY, -100.f, 0.5f));

        shootTimer = 0.f; // Reset th?i gian ch?
    }
}

void Boss::takeDamage(int damage) {
    health -= damage;
    if (health < 0) {
        health = 0;
    }
    std::cout << "Boss health: " << health << std::endl;
}

bool Boss::isAlive() const {
    return health > 0;
}

// Override hàm draw  thêm thanh máu
void Boss::draw(sf::RenderWindow& window) {
    window.draw(sprite); // Vo thân Boss
    // Vo thanh máu
    window.draw(healthBarBack);
    window.draw(healthBar);
}