#include "Player.h"
#include <iostream>
#include <algorithm>
// Kích thước cửa sổ mới để tham khảo
const float WINDOW_WIDTH = 1280.f;
const float WINDOW_HEIGHT = 720.f;

Player::Player() {
    // Load hình nhân vật
    if (!texture.loadFromFile("assets/player.png"))
        std::cout << "Không thể load player.png\n";
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f); // giảm kích thước
    // Căn giữa theo chiều ngang và đặt gần cuối màn hình theo chiều dọc
    sprite.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 100.f);

    speed = 350.f;          // tốc độ di chuyển
    shootCooldown = 0.3f;   // 0.3 giây giữa các lần bắn
    currentCooldown = 0.f;  // cooldown ban đầu

    // Load texture đạn
    if (!bulletTexture.loadFromFile("assets/bullet.png"))
        std::cout << "Không thể load bullet.png\n";

    // Thiết lập máu
    maxHealth = 150;
    currentHealth = 150;

    // Nền thanh máu
    healthBarBack.setSize(sf::Vector2f(60, 8));
    healthBarBack.setFillColor(sf::Color(50, 50, 50));
    // Thanh máu hiển thị
    healthBar.setSize(sf::Vector2f(60, 8));
    healthBar.setFillColor(sf::Color::Green);
    if (!shootBuffer.loadFromFile("assets/shoot.ogg")) {
        std::cout << "ERROR: Khong the load file am thanh shoot.ogg\n";
    }
    shootSound.setBuffer(shootBuffer);

    shootSound.setVolume(10.f);
    shieldActive = false;
    shieldTimer = 0.f;
    doubleShotActive = false;
    doubleShotTimer = 0.f;

    // Cài đặt hình ảnh cho khiên
    shieldSprite.setRadius(sprite.getGlobalBounds().width / 2.f + 5.f);
    shieldSprite.setFillColor(sf::Color(0, 150, 255, 70)); // Xanh trong suốt
    shieldSprite.setOutlineColor(sf::Color::Cyan);
    shieldSprite.setOutlineThickness(2.f);
}

// Xử lý phím nhấn
void Player::handleInput(float dt) {
    float moveX = 0.f;
    float moveY = 0.f;

    // Di chuyển trái
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        moveX -= speed * dt;

    // Di chuyển phải
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        moveX += speed * dt;

    // Di chuyển lên
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        moveY -= speed * dt;

    // Di chuyển xuống
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        moveY += speed * dt;

    sprite.move(moveX, moveY);

    // Giới hạn nhân vật trong màn hình
    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect playerBounds = sprite.getGlobalBounds();

    // Giới hạn bên trái (X)
    pos.x = std::max(0.f, std::min(pos.x, WINDOW_WIDTH - playerBounds.width));

    // Giới hạn bên trên và dưới (Y)
    pos.y = std::max(0.f, std::min(pos.y, WINDOW_HEIGHT - playerBounds.height));

    sprite.setPosition(pos);

    // Giữ Space bắn từng viên theo cooldown
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        if (canShoot()) {
            shoot();
        }
    }
}

// Cập nhật logic: cooldown, di chuyển đạn, thanh máu
void Player::update(float dt, sf::Vector2f playerPosition) {
    // Cập nhật thời gian hồi chiêu
    if (shootCooldown > 0.f) {
        // Giảm cooldown theo thời gian
        if (currentCooldown > 0.f)
            currentCooldown -= dt;

        // Di chuyển đạn lên
        for (auto& bullet : bullets)
            bullet.move(0.f, -700.f * dt); // tốc độ đạn 700px/s

        // Xóa đạn ra khỏi màn hình
        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(), [](sf::Sprite& b) {
                return b.getPosition().y < -10.f;
                }),
            bullets.end()
        );

        // Cập nhật thanh máu
        sf::Vector2f pos = sprite.getPosition();

        // *** DÒNG SỬA LỖI: Khai báo lại playerBounds ở đây ***
        sf::FloatRect playerBounds = sprite.getGlobalBounds();

        // Căn giữa thanh máu dựa trên playerBounds
        healthBarBack.setPosition(pos.x + (playerBounds.width / 2.f) - (healthBarBack.getSize().x / 2.f), pos.y - 15.f);
        healthBar.setPosition(pos.x + (playerBounds.width / 2.f) - (healthBarBack.getSize().x / 2.f), pos.y - 15.f);


        float healthPercent = static_cast<float>(currentHealth) / maxHealth;
        healthBar.setSize(sf::Vector2f(60.f * healthPercent, 8));

        // Đổi màu thanh máu
        if (healthPercent > 0.6f)
            healthBar.setFillColor(sf::Color::Green);
        else if (healthPercent > 0.3f)
            healthBar.setFillColor(sf::Color::Yellow);
        else
            healthBar.setFillColor(sf::Color::Red);
    }
    if (shieldActive) {
        shieldTimer -= dt;
        if (shieldTimer <= 0.f) {
            shieldActive = false;
        }
        // Cập nhật vị trí khiên đi theo player
        shieldSprite.setOrigin(shieldSprite.getRadius(), shieldSprite.getRadius());
        shieldSprite.setPosition(sprite.getPosition().x + sprite.getGlobalBounds().width / 2.f,
            sprite.getPosition().y + sprite.getGlobalBounds().height / 2.f);
    }

    if (doubleShotActive) {
        doubleShotTimer -= dt;
        if (doubleShotTimer <= 0.f) {
            doubleShotActive = false;
        }
    }
}


// Kiểm tra có thể bắn không
bool Player::canShoot() const {
    return currentCooldown <= 0.f;
}

// Bắn đạn
void Player::shoot() {
    currentCooldown = shootCooldown; // reset cooldown
    shootSound.play();

    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();

    if (doubleShotActive) {
        // Bắn 2 viên
        sf::Sprite bulletLeft;
        bulletLeft.setTexture(bulletTexture);
        bulletLeft.setScale(0.2f, 0.2f);
        bulletLeft.setPosition(pos.x, pos.y);
        bullets.push_back(bulletLeft);

        sf::Sprite bulletRight;
        bulletRight.setTexture(bulletTexture);
        bulletRight.setScale(0.2f, 0.2f);
        bulletRight.setPosition(pos.x + bounds.width - bulletRight.getGlobalBounds().width, pos.y);
        bullets.push_back(bulletRight);

        std::cout << "Ban 2 dan! So dan: " << bullets.size() << std::endl;
    }
    else {
        // Bắn 1 viên (như cũ)
        sf::Sprite bullet;
        bullet.setTexture(bulletTexture);
        bullet.setScale(0.2f, 0.2f);
        bullet.setPosition(pos.x + bounds.width / 2 - 5, pos.y);
        bullets.push_back(bullet);

        std::cout << "Ban 1 dan! So dan: " << bullets.size() << std::endl;
    }
}

// Vẽ nhân vật, đạn, thanh máu
void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    for (auto& bullet : bullets)
        window.draw(bullet);
    window.draw(healthBarBack);
    window.draw(healthBar);
    if (shieldActive) {
        window.draw(shieldSprite);
    }
}

// Nhận sát thương
void Player::takeDamage(int amount) {
    // Nếu là hồi máu (amount < 0)
    if (amount < 0) {
        currentHealth -= amount; // (ví dụ: currentHealth -= -20, tức là + 20)
        if (currentHealth > maxHealth) {
            currentHealth = maxHealth;
        }
        return; // Hồi máu xong thì thoát
    }

    // Nếu đang có khiên và bị tấn công
    if (shieldActive) {
        shieldActive = false; // Khiên vỡ
        shieldTimer = 0.f;
        std::cout << "Khiên đã chặn sát thương!\n";
        return; // Không nhận sát thương
    }

    // Nếu không có khiên
    currentHealth -= amount;
    if (currentHealth < 0)
        currentHealth = 0;
}

// Kiểm tra đã chết chưa
bool Player::isDead() const {
    return currentHealth <= 0;
}
// HÀM MỚI: Reset trạng thái người chơi
void Player::reset() {
    currentHealth = maxHealth;
    // Đặt lại vị trí mặc định
    sprite.setPosition(WINDOW_WIDTH / 2.f, WINDOW_HEIGHT - 100.f);
    bullets.clear();                  // Xóa hết đạn cũ
    currentCooldown = 0.f;            // Reset cooldown
    // *** THÊM RESET POWER-UP ***
    shieldActive = false;
    shieldTimer = 0.f;
    doubleShotActive = false;
    doubleShotTimer = 0.f;
}
void Player::activateShield(float duration) {
    shieldActive = true;
    shieldTimer = duration; // Đặt lại thời gian
}

void Player::activateDoubleShot(float duration) {
    doubleShotActive = true;
    doubleShotTimer = duration; // Đặt lại thời gian
}