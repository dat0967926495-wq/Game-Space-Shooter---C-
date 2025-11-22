#include "Bullet.h"

Bullet::Bullet(const sf::Texture& texture, float startX, float startY, float speed, float scale) {
    sprite.setTexture(texture);       // Gán hình ảnh
    sprite.setPosition(startX, startY); // Đặt vị trí xuất phát
    this->speed = speed;              // Gán tốc độ
    alive = true;                     // Đánh dấu viên đạn còn tồn tại
    sprite.setScale(scale, scale); // Đặt kích thước
}

// Cập nhật vị trí viên đạn mỗi frame
void Bullet::update(float dt) {
    // Di chuyển lên trên theo trục Y
    // dt: delta time giữa 2 frame để tốc độ ổn định
    sprite.move(0.f, -speed * dt);

    // Nếu viên đạn bay ra ngoài màn hình → đánh dấu dead
    if (sprite.getPosition().y + sprite.getGlobalBounds().height < 0) {
        destroy();
    }
}

// Vẽ viên đạn ra màn hình
void Bullet::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

// Lấy vùng bao quanh viên đạn (dùng để check va chạm)
sf::FloatRect Bullet::getBounds() const {
    return sprite.getGlobalBounds();
}

// Hàm kiểm tra viên đạn còn tồn tại không
bool Bullet::isAlive() const {
    return alive;
}

// Đánh dấu viên đạn đã chết
void Bullet::destroy() {
    alive = false;
}
