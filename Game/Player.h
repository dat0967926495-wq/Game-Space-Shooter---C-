#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include "Entity.h"

// Lớp Player: điều khiển nhân vật, bắn đạn, thanh máu
class Player : public Entity {
private:
    sf::Texture texture;         // Texture nhân vật
    float speed;                 // Tốc độ di chuyển
    float shootCooldown;         // Thời gian chờ giữa các lần bắn
    float currentCooldown;       // Bộ đếm cooldown hiện tại

    sf::Texture bulletTexture;   // Texture đạn
    std::vector<sf::Sprite> bullets; // Danh sách đạn đang bay
    sf::SoundBuffer shootBuffer; // Lưu trữ file âm thanh
    sf::Sound shootSound;        // Dùng để phát âm thanh

    int maxHealth;               // Máu tối đa
    int currentHealth;           // Máu hiện tại
    sf::RectangleShape healthBarBack; // Nền thanh máu
    sf::RectangleShape healthBar;     // Thanh máu hiển thị
    bool shieldActive;
    float shieldTimer;
    sf::CircleShape shieldSprite; // Hình tròn để vẽ khiên

    bool doubleShotActive;
    float doubleShotTimer;

public:
    Player();
    void reset();
    void handleInput(float dt);           // Xử lý phím nhấn

    // <<<--- XÓA DÒNG NÀY: void update(float dt) override;

    bool canShoot() const;                // Kiểm tra có thể bắn không
    void shoot();                         // Bắn viên đạn
    void draw(sf::RenderWindow& window);  // Vẽ nhân vật, đạn, thanh máu
    void takeDamage(int amount);          // Nhận sát thương
    bool isDead() const;                  // Kiểm tra đã chết chưa
    std::vector<sf::Sprite>& getBullets() { return bullets; }
    int getCurrentHealth() const { return currentHealth; }

    //  GIỮ LẠI DÒNG NÀY (Vì đây là hàm đúng của lớp cha Entity)
    void update(float dt, sf::Vector2f playerPosition) override;
    void activateShield(float duration = 5.f);
    void activateDoubleShot(float duration = 8.f);

};