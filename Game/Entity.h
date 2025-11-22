#pragma once
#include <SFML/Graphics.hpp>

class Entity {
protected:
    // Thuộc tính chung: hình ảnh, sprite
    sf::Sprite sprite;

public:
    // Hàm khởi tạo ảo (quan trọng cho kế thừa)
    virtual ~Entity() {}

    // Hàm ảo thuần túy (pure virtual)
    // Cập nhật: Thêm tham số playerPosition
    virtual void update(float dt, sf::Vector2f playerPosition) = 0;

    // Hàm ảo (virtual)
    // Cung cấp một định nghĩa mặc định, lớp con có thể viết đè (override)
    virtual void draw(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    // Hàm chung, không cần ảo
    sf::FloatRect getBounds() const {
        return sprite.getGlobalBounds();
    }

    // *** SỬA LỖI: Thêm phần thân hàm vào đây ***
    sf::Vector2f getPosition() const {
        return sprite.getPosition();
    }
};