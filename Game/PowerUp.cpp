#include "PowerUp.h"

// Kích th??c c?a s?
const int WINDOW_HEIGHT = 720;

PowerUp::PowerUp(const sf::Texture& texture, sf::Vector2f position, PowerUpType type) {
    this->type = type;
    this->speed = 60.f; // T?c ?? r?i xu?ng
    this->alive = true;

    sprite.setTexture(texture);
    sprite.setPosition(position);
    sprite.setScale(0.5f, 0.5f); // Ch?nh kích th??c cho phù h?p

    // Tùy ch?nh màu s?c d?a trên lo?i
    switch (type) {
    case PowerUpType::Shield:
        sprite.setColor(sf::Color::Cyan); // Màu xanh bi?n
        break;
    case PowerUpType::DoubleShot:
        sprite.setColor(sf::Color::Yellow); // Màu vàng
        break;
    case PowerUpType::Heal:
        sprite.setColor(sf::Color::Green); // Màu xanh lá
        break;
    }
}

void PowerUp::update(float dt) {
    // Di chuy?n xu?ng
    sprite.move(0.f, speed * dt);

    // N?u ra kh?i màn hình thì bi?n m?t
    if (sprite.getPosition().y > WINDOW_HEIGHT) {
        alive = false;
    }
}

void PowerUp::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

bool PowerUp::isAlive() const {
    return alive;
}

sf::FloatRect PowerUp::getBounds() const {
    return sprite.getGlobalBounds();
}

PowerUpType PowerUp::getType() const {
    return type;
}