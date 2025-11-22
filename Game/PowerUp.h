#pragma once
#include <SFML/Graphics.hpp>

// Định nghĩa các loại power-up
enum class PowerUpType {
    Shield,     // Khiên bảo vệ
    DoubleShot, // Bắn 2 đạn
    Heal        // Hồi máu
};

class PowerUp {
private:
    sf::Sprite sprite;
    float speed;
    bool alive;
    PowerUpType type;

public:
    // Hàm khởi tạo
    PowerUp(const sf::Texture& texture, sf::Vector2f position, PowerUpType type);

    void update(float dt);
    void draw(sf::RenderWindow& window) const;

    bool isAlive() const;
    sf::FloatRect getBounds() const;
    PowerUpType getType() const; // Rất quan trọng: để biết là loại nào
};