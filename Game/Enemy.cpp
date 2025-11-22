#include "Enemy.h"
#include <cstdlib>
#include <cmath>

// Kích thước cửa sổ (để giới hạn di chuyển ngang)
const int WINDOW_WIDTH = 1280;

// *** CẬP NHẬT HÀM KHỞI TẠO ***
Enemy::Enemy(const sf::Texture& texture, float startX, float startY, float speed, int maxHealth, EnemyType type) {
    sprite.setTexture(texture);
    sprite.setPosition(startX, startY);
    this->speed = speed;
    this->maxHealth = maxHealth;
    this->health = maxHealth;
    this->type = type; // <-- Gán loại kẻ địch

    // Khởi tạo hướng zigzag ngẫu nhiên (trái hoặc phải)
    if (std::rand() % 2 == 0) {
        this->zigzagDirection = -1.0f;
    }
    else {
        this->zigzagDirection = 1.0f;
    }
    this->zigzagTimer = 0.f; // Reset bộ đếm

    // Tạo thanh máu
    healthBarBack.setSize(sf::Vector2f(40.f, 5.f));
    healthBarBack.setFillColor(sf::Color(50, 50, 50));
    healthBar.setSize(sf::Vector2f(40.f, 5.f));
    healthBar.setFillColor(sf::Color::Red);
}

void Enemy::update(float dt, sf::Vector2f playerPosition) {

    // *** CẬP NHẬT LOGIC DI CHUYỂN DỰA TRÊN LOẠI KẺ ĐỊCH ***
    float moveX = 0.f;
    float moveY = 0.f; // Luôn di chuyển xuống

    switch (type) {
    case EnemyType::Normal:
        // Kiểu Normal: Chỉ di chuyển xuống (moveX = 0)
        moveY = speed * dt;
        break;

    case EnemyType::Zigzag:
        // Kiểu Zigzag: Di chuyển ngang
        moveY = speed * dt; // Vẫn di chuyển xuống
        moveX = 80.f * zigzagDirection * dt; // Tốc độ di chuyển ngang
        zigzagTimer += dt;

        // Đổi hướng sau mỗi 1.2 giây
        if (zigzagTimer > 1.2f) {
            zigzagDirection *= -1.f; // Đảo ngược hướng
            zigzagTimer = 0.f;       // Reset bộ đếm
        }
        break;
        // *** THÊM LOGIC CHO KAMIKAZE ***
    case EnemyType::Kamikaze:
    { // Tạo scope mới cho biến direction và length
        // Tính vector hướng từ địch tới người chơi
        sf::Vector2f direction = playerPosition - sprite.getPosition();

        // Chuẩn hóa vector (lấy vector đơn vị)
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        if (length != 0) {
            direction.x /= length;
            direction.y /= length;
        }

        // Di chuyển theo vector hướng đã chuẩn hóa
        moveX = direction.x * speed * dt;
        moveY = direction.y * speed * dt;
    }
    break;
    }

    // Áp dụng di chuyển
    sprite.move(moveX, moveY);

    // Xử lý khi bay ra khỏi màn hình (dưới)
    if (sprite.getPosition().y > 720)
        destroy();

    // Cập nhật bộ đếm bắn
    shootTimer += dt;

    // Lấy vị trí hiện tại
    sf::Vector2f pos = sprite.getPosition();

    // *** XỬ LÝ KHI CHẠM 2 LỀ (CHO ZIGZAG) ***
    if (type == EnemyType::Zigzag) {
        sf::FloatRect bounds = sprite.getGlobalBounds();
        // Chạm lề trái
        if (pos.x < 0.f) {
            pos.x = 0.f;
            zigzagDirection = 1.0f; // Đổi hướng sang phải
            zigzagTimer = 0.f;
        }
        // Chạm lề phải
        else if (pos.x + bounds.width > WINDOW_WIDTH) {
            pos.x = WINDOW_WIDTH - bounds.width;
            zigzagDirection = -1.0f; // Đổi hướng sang trái
            zigzagTimer = 0.f;
        }
        // Cập nhật lại vị trí nếu có va chạm lề
        sprite.setPosition(pos.x, pos.y);
    }

    // Cap nhat vi trí thanh máu di theo enemy
    if (type != EnemyType::Kamikaze) {
        healthBarBack.setPosition(pos.x, pos.y - 10.f);
        healthBar.setPosition(pos.x, pos.y - 10.f);

        // Cap nhat do dài thanh máu theo phan tram
        float healthPercent = static_cast<float>(health) / maxHealth;
        healthBar.setSize(sf::Vector2f(40.f * healthPercent, 5.f));
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    // *** Kamikaze không cần vẽ thanh máu ***
    if (type != EnemyType::Kamikaze) {
        window.draw(healthBarBack);
        window.draw(healthBar);
    }
}

void Enemy::shoot(std::vector<Bullet>& enemyBullets, const sf::Texture& bulletTexture) {
    // *** KAMIKAZE KHÔNG BAO GIỜ BẮN ***
    if (type == EnemyType::Kamikaze) {
        return;
    }
    // Kẻ địch Zigzag có thể bắn nhanh hơn
    float shootInterval = (type == EnemyType::Zigzag) ? 1.5f : 2.0f;

    if (shootTimer > shootInterval) {
        enemyBullets.push_back(
            Bullet(bulletTexture,
                sprite.getPosition().x + sprite.getGlobalBounds().width / 2 - 5,
                sprite.getPosition().y + sprite.getGlobalBounds().height,
                -150.f, 0.5f)
        );
        shootTimer = 0.f;
    }
}


void Enemy::takeDamage(int damage) {
    health -= damage;
    if (health <= 0)
        destroy();
}