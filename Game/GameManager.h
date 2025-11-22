#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory> // Dùng cho std::unique_ptr
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Boss.h"
#include "PowerUp.h"

// Định nghĩa trạng thái game
enum class GameState {
    Menu,
    NameInput,
    Running,
    GameOver,
    Paused
};

class GameManager {
private:
    // Cửa sổ game
    sf::RenderWindow window;
    // Đồng hồ để tính thời gian delta time (dt)
    sf::Clock clock;
    sf::Font font;
    sf::Text titleText;
    sf::Text instructionText;
    sf::Text gameOverText;
    sf::Text restartText;
    sf::Text pauseText;
    sf::Text nameInputTitle; // Chữ "Enter Your Name:"
    sf::Text playerNameText; // Tên người chơi đang gõ
    std::string currentPlayerName;
    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    sf::Music backgroundMusic;
    int score;
    int highScore;
    std::string highScoreName;
    sf::Text scoreText;
    sf::Text highScoreText;
    sf::Text finalScoreText; // Để hiển thị điểm ở màn hình GameOver
    // Các đối tượng trong game
    Player player; // Người chơi
    std::vector<Enemy> enemies; // Danh sách kẻ địch
    std::vector<Bullet> enemyBullets; // Đạn của kẻ địch
    
    //  BIẾN MỚI CHO POWER-UP 
    std::vector<PowerUp> powerUps;
    sf::Texture powerUpTexture;

    // Texture chung cho đạn của cả Player và Enemy
    sf::Texture playerBulletTexture;
    sf::Texture enemyTexture;
    sf::Texture bossTexture;
    sf::Texture boss2Texture; // <--- THÊM DÒNG NÀY (CHO BOSS 2)
    // Quản lý Boss
    std::unique_ptr<Boss> boss;
    bool bossActive = false;
    bool boss1Defeated = false; // <--- THÊM DÒNG NÀY (ĐỂ BIẾT BOSS 1 ĐÃ CHẾT)
    bool boss2Defeated = false;


    // Trạng thái game
    GameState gameState = GameState::Menu;

    // Hàm riêng tư:
    // Khởi tạo các tài nguyên (textures, fonts, v.v.)
    void initAssets();
    // Xử lý sự kiện (đóng cửa sổ, v.v.)
    void handleEvents();
    void initMenu(); // <-- Thêm hàm khởi tạo menu
    void initGameOver();
    void initNameInput();
    void initPauseScreen();
    void resetGame();
    void loadHighScore();
    void saveHighScore();
    // Cập nhật logic game (di chuyển, va chạm)
    void update(float dt);
    // Vẽ tất cả lên màn hình
    void render();

    // Logic sinh kẻ địch mới
    void spawnEnemy();
    // *** THÊM HÀM SINH BOSS ***
    void spawnBoss(int bossNumber); // <-- Thêm tham số
    // Logic kiểm tra va chạm giữa đạn người chơi và kẻ địch
    void checkPlayerBulletEnemyCollision();
    // Logic kiểm tra va chạm giữa đạn kẻ địch và người chơi
    void checkEnemyBulletPlayerCollision();
    void checkPlayerEnemyCollision();
    // Xóa các viên đạn, kẻ địch đã chết hoặc ra khỏi màn hình
    void cleanUp();
    bool newHighScoreSet = false; // Cờ để biết khi nào cần lưu file
    // *** THÊM CÁC HÀM PRIVATE MỚI ***
    void spawnPowerUp(sf::Vector2f position);
    void checkPlayerPowerUpCollision();

public:
    // Hàm khởi tạo
    GameManager();
    // Vòng lặp chính của game
    void run();
};