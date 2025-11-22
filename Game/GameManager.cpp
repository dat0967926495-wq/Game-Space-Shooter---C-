#include "GameManager.h"
#include "PowerUp.h"
#include <iostream>
#include <algorithm>
#include <cstdlib> // Dùng cho rand()
#include <ctime>   // Dùng cho time()
#include <string>
#include <fstream>


// Kích thước cửa sổ
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

// Hàm khởi tạo GameManager
GameManager::GameManager() :
    window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "SFML Space Shooter")
{
    // Cài đặt giới hạn tốc độ khung hình (FPS)
    window.setFramerateLimit(60);
    // Khởi tạo seed cho hàm random (dùng để sinh kẻ địch)
    std::srand(std::time(0));
    loadHighScore();
    initAssets();
}

// Khởi tạo các tài nguyên
void GameManager::initAssets() {

    // Player texture đã được load trong Player::Player() (Giả định path hợp lệ)
    // Tải texture đạn Player (Player.cpp sử dụng "assets/bullet.png" cho đạn Player)
    // Giả sử có một texture chung cho đạn kẻ địch
    if (!playerBulletTexture.loadFromFile("assets/bullet.png")) {
        std::cout << "Không thể load bullet texture.\n";
    }

    // Tải texture kẻ địch
    if (!enemyTexture.loadFromFile("assets/enemy.png")) {
        std::cout << "Không thể load enemy texture.png\n";
    }
    //  THÊM KHỐI LỆNH NÀY ĐỂ TẢI HÌNH BOSS 
    if (!bossTexture.loadFromFile("assets/boss.png")) { // <-- Thay "assets/boss.png" bằng đường dẫn tệp của bạn
        std::cout << "Không thể load boss texture.png\n";
    }
    //  THÊM KHỐI LỆNH NÀY ĐỂ TẢI HÌNH BOSS 2 
    if (!boss2Texture.loadFromFile("assets/boss2.png")) { // <-- Tên tệp hình Boss 2
        std::cout << "Không thể load boss2 texture.png\n";
    }
    // Tải ảnh nền
    if (!backgroundTexture.loadFromFile("assets/background.png")) {
        std::cout << "Không thể load background.png!\n";
    }
    backgroundSprite.setTexture(backgroundTexture);
    if (!backgroundMusic.openFromFile("assets/background_music.ogg")) {
        std::cout << "ERROR: Khong the load file nhac nen background_music.ogg\n";
    }
    backgroundMusic.setLoop(true); // Cho phép nhạc lặp lại vô tận
    backgroundMusic.setVolume(50); // Chỉnh âm lượng (0-100), ví dụ 50%
    backgroundMusic.play();        // Bật nhạc
    if (!powerUpTexture.loadFromFile("assets/bullet.png")) { // Dùng tạm ảnh 'bullet.png'
        std::cout << "Không thể load powerUpTexture.\n";
    }
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cout << "ERROR: Khong the load file font arial.ttf\n";
    }
    // Gọi hàm khởi tạo menu
    initMenu();
    // Gọi hàm gameover
    initGameOver();
    // GỌI HÀM NHẬP TÊN
    initNameInput();
    initPauseScreen();

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.f, 10.f); // Đặt ở góc trên bên trái
    highScoreText.setFont(font);
    highScoreText.setCharacterSize(24);
    highScoreText.setFillColor(sf::Color::Cyan); // Cho màu khác để dễ phân biệt

    // CẬP NHẬT HIỂN THỊ ĐIỂM CAO
    highScoreText.setString("High Score: " + highScoreName + " - " + std::to_string(highScore));
    // Căn lề phải
    sf::FloatRect hsRect = highScoreText.getLocalBounds();
    highScoreText.setOrigin(hsRect.left + hsRect.width, hsRect.top);
    highScoreText.setPosition(sf::Vector2f(WINDOW_WIDTH - 10.f, 10.f));

}
// Hàm mới để khởi tạo các thành phần của Menu
void GameManager::initMenu() {
    // Tải font
    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cout << "ERROR: Khong the load file font arial.ttf\n";
    }

    // Cài đặt chữ tiêu đề
    titleText.setFont(font);
    titleText.setString("SPACE SHOOTER");
    titleText.setCharacterSize(64); // Kích thước chữ
    titleText.setFillColor(sf::Color::White);
    titleText.setStyle(sf::Text::Bold);
    // Căn giữa tiêu đề
    sf::FloatRect titleRect = titleText.getLocalBounds();
    titleText.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    titleText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 4.0f));

    // Cài đặt chữ hướng dẫn
    instructionText.setFont(font);
    instructionText.setString("Press Enter to Start");
    instructionText.setCharacterSize(24);
    instructionText.setFillColor(sf::Color::White);
    // Căn giữa hướng dẫn
    sf::FloatRect instructionRect = instructionText.getLocalBounds();
    instructionText.setOrigin(instructionRect.left + instructionRect.width / 2.0f, instructionRect.top + instructionRect.height / 2.0f);
    instructionText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
}
// HÀM MỚI: Khởi tạo các thành phần của màn hình Game Over
void GameManager::initGameOver() {
    // Font đã được load ở initMenu(), ta chỉ cần dùng lại
    gameOverText.setFont(font);
    gameOverText.setString("GAME OVER");
    gameOverText.setCharacterSize(72);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);
    // Căn giữa
    sf::FloatRect gameOverRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(gameOverRect.left + gameOverRect.width / 2.0f, gameOverRect.top + gameOverRect.height / 2.0f);
    gameOverText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 4.0f));

    // <-- THÊM KHỞI TẠO CHO ĐIỂM SỐ CUỐI CÙNG
    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(32);
    finalScoreText.setFillColor(sf::Color::White);

    // Cài đặt chữ hướng dẫn restart
    restartText.setFont(font);
    restartText.setString("Press Enter for Menu");
    restartText.setCharacterSize(24);
    restartText.setFillColor(sf::Color::White);
    // Căn giữa
    sf::FloatRect restartRect = restartText.getLocalBounds();
    restartText.setOrigin(restartRect.left + restartRect.width / 2.0f, restartRect.top + restartRect.height / 2.0f);
    restartText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 80.f));
}
// HÀM MỚI: Khởi tạo màn hình nhập tên
void GameManager::initNameInput() {
    // Tiêu đề
    nameInputTitle.setFont(font);
    nameInputTitle.setString("Enter Your Name (max 10):");
    nameInputTitle.setCharacterSize(40);
    nameInputTitle.setFillColor(sf::Color::White);
    sf::FloatRect titleRect = nameInputTitle.getLocalBounds();
    nameInputTitle.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    nameInputTitle.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 4.0f));

    // Tên người chơi
    playerNameText.setFont(font);
    playerNameText.setString(""); // Ban đầu rỗng
    playerNameText.setCharacterSize(48);
    playerNameText.setFillColor(sf::Color::Yellow);
    // Căn giữa (sẽ được cập nhật trong handleEvents)
    sf::FloatRect textRect = playerNameText.getLocalBounds();
    playerNameText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    playerNameText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
}
void GameManager::initPauseScreen() {
    // Cài đặt chữ "PAUSED"
    pauseText.setFont(font);
    pauseText.setString("PAUSED");
    pauseText.setCharacterSize(64);
    pauseText.setFillColor(sf::Color::Yellow);
    pauseText.setStyle(sf::Text::Bold);
    // Căn giữa
    sf::FloatRect pauseRect = pauseText.getLocalBounds();
    pauseText.setOrigin(pauseRect.left + pauseRect.width / 2.0f, pauseRect.top + pauseRect.height / 2.0f);
    pauseText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
}

// HÀM MỚI: Reset toàn bộ game về trạng thái ban đầu
void GameManager::resetGame() {
    player.reset();          // Reset người chơi
    enemies.clear();         // Xóa hết kẻ địch
    enemyBullets.clear();    // Xóa hết đạn của địch
    score = 0; // <-- RESET ĐIỂM VỀ 0
    powerUps.clear();

    // *** RESET BOSS ***
    boss.reset(); // Xóa boss hi?n t?i
    bossActive = false; // T?t c? boss
    boss1Defeated = false; // Reset cờ Boss 1
    boss2Defeated = false; // Reset cờ Boss 2

    gameState = GameState::Menu; // Quay về màn hình Menu
}

// Xử lý sự kiện cửa sổ (ví dụ: đóng cửa sổ)
void GameManager::handleEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        // Thoát game khi nhấn ESC
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            window.close();
        // *** THÊM LOGIC PAUSE/UNPAUSE BẰNG PHÍM 'P' ***
        if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::P) {
            if (gameState == GameState::Running) {
                // Nếu đang chơi -> Tạm dừng
                gameState = GameState::Paused;
                // Có thể thêm: Dừng nhạc nền
                // backgroundMusic.pause();
            }
            else if (gameState == GameState::Paused) {
                // Nếu đang tạm dừng -> Chơi tiếp
                gameState = GameState::Running;
                // Có thể thêm: Bật lại nhạc nền
                // backgroundMusic.play();
            }
        }

        // **Logic xử lý cho Menu**
        if (gameState == GameState::Menu) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                // Xóa tên cũ và chuyển sang màn hình nhập tên
                currentPlayerName.clear();
                playerNameText.setString("");
                gameState = GameState::NameInput; // Chuyển sang NameInput
            }
        }
        // **Logic xử lý cho NameInput**
        else if (gameState == GameState::NameInput) {

            // --- PHẦN 1: CHỈ XỬ LÝ VIỆC NHẬP VĂN BẢN ---
            if (event.type == sf::Event::TextEntered) {
                // Chỉ nhận các ký tự in được (bỏ qua Backspace, Enter, v.v...)
                if (event.text.unicode >= 32 && event.text.unicode <= 126 && currentPlayerName.length() < 10) {

                    currentPlayerName += static_cast<char>(event.text.unicode);

                    // *** CĂN LỀ (PHẢI CÓ) ***
                    playerNameText.setString(currentPlayerName);
                    sf::FloatRect textRect = playerNameText.getLocalBounds();
                    playerNameText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                    playerNameText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
                }
            }

            // --- PHẦN 2: CHỈ XỬ LÝ CÁC PHÍM ĐIỀU KHIỂN ---
            if (event.type == sf::Event::KeyPressed) {

                // Xử lý phím Enter
                if (event.key.code == sf::Keyboard::Enter && !currentPlayerName.empty()) {
                    // newHighScoreSet = false; // Bạn đã xóa biến này, OK
                    gameState = GameState::Running;
                }
                // Xử lý phím Backspace (Xóa lùi)
                else if (event.key.code == sf::Keyboard::BackSpace && !currentPlayerName.empty()) {

                    currentPlayerName.pop_back(); // Xóa ký tự cuối

                    // *** CĂN LỀ (PHẢI CÓ) ***
                    // Đây là 3 dòng code bạn có thể đã quên
                    playerNameText.setString(currentPlayerName);
                    sf::FloatRect textRect = playerNameText.getLocalBounds();
                    playerNameText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
                    playerNameText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
                }
            }
        }
        // **LOGIC CHO MÀN HÌNH GAME OVER**
        else if (gameState == GameState::GameOver) {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                resetGame(); // Gọi hàm reset khi nhấn Enter
            }
        }
    }
}

// Sinh kẻ địch
// Sinh kẻ địch
void GameManager::spawnEnemy() {
    // --- Phần 1: Các điều kiện dừng (Giữ nguyên như cũ) ---

    // NẾU BOSS ĐANG HOẠT ĐỘNG, KHÔNG SINH QUÁI THƯỜNG
    // (Lưu ý: bạn có thể đã dùng 'bossActive' hoặc 'boss',
    // hãy đảm bảo nó khớp với code của bạn)
    if (bossActive && boss)
        return;

    // GIỚI HẠN SỐ LƯỢNG QUÁI (Giữ nguyên)
    if (enemies.size() >= 5)
        return;

    // --- Phần 2: Khởi tạo (Giữ nguyên) ---
    float startX = static_cast<float>(std::rand() % (WINDOW_WIDTH - 50));
    float startY = -50.f;

    EnemyType type;
    float speed;
    int health;

    // --- Phần 3: LOGIC CHỌN QUÁI (Đây là phần thay đổi) ---

    // Chúng ta sẽ kiểm tra xem game đang ở giai đoạn nào

    if (boss2Defeated) {
        // *** GIAI ĐOẠN 3: SAU KHI HẠ BOSS 2 (Chế độ vô tận) ***
        // Quái sẽ nhanh hơn và tỉ lệ quái khó cao hơn.

        int typeRoll = std::rand() % 100;

        if (typeRoll < 40) { // 40% cơ hội ra Kamikaze
            type = EnemyType::Kamikaze;
            speed = 200.f + (std::rand() % 50); // Nhanh hơn trước
            health = 1;
        }
        else if (typeRoll < 80) { // 40% cơ hội ra Zigzag
            type = EnemyType::Zigzag;
            speed = 100.f + (std::rand() % 40); // Cũng nhanh hơn
            health = 5;
        }
        else { // 20% cơ hội ra Normal
            type = EnemyType::Normal;
            speed = 90.f + (std::rand() % 30);
            health = 3;
        }
    }
    else if (boss1Defeated) {
        // *** GIAI ĐOẠN 2: SAU KHI HẠ BOSS 1 (Trước Boss 2) ***
        // Đây chính là logic cũ của bạn, cân bằng 3 loại quái.

        int typeRoll = std::rand() % 100;

        if (typeRoll < 50) {
            type = EnemyType::Normal;
            speed = 70.f + (std::rand() % 30);
            health = 3;
        }
        else if (typeRoll < 85) {
            type = EnemyType::Zigzag;
            speed = 80.f + (std::rand() % 40);
            health = 5;
        }
        else {
            type = EnemyType::Kamikaze;
            speed = 180.f + (std::rand() % 50);
            health = 1;
        }
    }
    else {
        // *** GIAI ĐOẠN 1: MỚI BẮT ĐẦU GAME (Trước Boss 1) ***
        // Giai đoạn dễ nhất, không có Kamikaze.

        int typeRoll = std::rand() % 100;

        if (typeRoll < 60) { // 60% cơ hội ra Normal
            type = EnemyType::Normal;
            speed = 70.f + (std::rand() % 30);
            health = 3;
        }
        else { // 40% cơ hội ra Zigzag
            type = EnemyType::Zigzag;
            speed = 80.f + (std::rand() % 40);
            health = 5;
        }
    }

    // --- Phần 4: Thêm kẻ địch vào game (Giữ nguyên) ---
    enemies.emplace_back(enemyTexture, startX, startY, speed, health, type);
}


// Kiểm tra va chạm đạn người chơi và kẻ địch
void GameManager::checkPlayerBulletEnemyCollision() {
    auto& playerBullets = player.getBullets(); // Lấy danh sách đạn của người chơi

    // Duyệt qua tất cả đạn của người chơi
    for (int i = 0; i < playerBullets.size(); ) {
        bool hit = false;
        // Lấy vùng va chạm của đạn
        sf::FloatRect bulletBounds = playerBullets[i].getGlobalBounds();

        // *** KIỂM TRA VA CHẠM VỚI BOSS ***
        if (bossActive && boss && boss->isAlive()) {
            if (bulletBounds.intersects(boss->getBounds())) {
                boss->takeDamage(1); // Boss m?t 1 máu
                hit = true;
                // N?u Boss ch?t, logic ???c x? lý trong update()
            }
        }

        // Duyệt qua tất cả kẻ địch (n?u ch?a b?n trúng Boss)
        if (!hit) {
            for (auto& enemy : enemies) {
                if (enemy.isAlive() && bulletBounds.intersects(enemy.getBounds())) {
                    enemy.takeDamage(1); // Gây 1 sát thương cho kẻ địch
                    // <-- NẾU KẺ ĐỊCH CHẾT, CỘNG ĐIỂM
                    if (!enemy.isAlive()) {
                        score += 100; // Cộng 100 điểm cho mỗi kẻ địch
                        // *** THÊM: TẠO POWER-UP KHI QUÁI CHẾT ***
                        // 20% cơ hội rơi ra
                        if (std::rand() % 100 < 40) {
                            spawnPowerUp(enemy.getPosition());
                        }
                    }
                    hit = true;
                    break; // Thoát vòng lặp kẻ địch vì đạn chỉ bắn trúng 1 mục tiêu
                }
            }
        }

        // Nếu đạn bắn trúng, xóa viên đạn đó
        if (hit) {
            // Hoán đổi với phần tử cuối cùng rồi xóa (Tối ưu hơn erase(i))
            std::swap(playerBullets[i], playerBullets.back());
            playerBullets.pop_back();
        }
        else {
            // Nếu không trúng, chuyển sang viên đạn tiếp theo
            i++;
        }
    }
}

// Kiểm tra va chạm đạn kẻ địch và người chơi
void GameManager::checkEnemyBulletPlayerCollision() {
    sf::FloatRect playerBounds = player.getBounds();

    // Lọc và xử lý va chạm
    enemyBullets.erase(
        std::remove_if(enemyBullets.begin(), enemyBullets.end(), [&](Bullet& bullet) {
            if (bullet.isAlive() && bullet.getBounds().intersects(playerBounds)) {
                player.takeDamage(10); // Người chơi mất 10 máu
                std::cout << "Player bị trúng đạn! Máu còn: " << player.getCurrentHealth() << "\n";
                // Đánh dấu đạn đã trúng để xóa
                return true;
            }
            return false;
            }),
        enemyBullets.end()
    );
}

//  KIỂM TRA VA CHẠM GIỮA PLAYER VÀ ENEMY ***
void GameManager::checkPlayerEnemyCollision() {
    // Chỉ kiểm tra khi đang chơi
    if (gameState != GameState::Running)
        return;

    sf::FloatRect playerBounds = player.getBounds();

    // *** KIỂM TRA VA CHẠM VỚI QUÁI THƯỜNG ***
    for (auto& enemy : enemies) {
        // Chỉ kiểm tra va chạm nếu kẻ địch còn sống
        if (enemy.isAlive() && playerBounds.intersects(enemy.getBounds())) {

            // Player nhận sát thương 
            player.takeDamage(20);

            // Kẻ địch bị phá hủy ngay lập tức
            enemy.destroy();

            // In ra console để debug
            std::cout << "Player VA CHAM Enemy! Mau con: " << player.getCurrentHealth() << "\n";
        }
    }

    // *** KIỂM TRA VA CHẠM VỚI BOSS ***
    if (bossActive && boss && boss->isAlive()) {
        if (playerBounds.intersects(boss->getBounds())) {
            // Va ch?m v?i Boss m?t nhi?u máu h?n
            player.takeDamage(50);
            std::cout << "Player VA CHAM BOSS! Mau con: " << player.getCurrentHealth() << "\n";
            // Boss không b? phá h?y khi va ch?m, ch? ng??i ch?i m?t máu
        }
    }
}


// Dọn dẹp: Xóa các đối tượng đã chết/hết hạn
void GameManager::cleanUp() {
    // Xóa kẻ địch đã chết
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(), [](const Enemy& e) {
            return !e.isAlive();
            }),
        enemies.end()
    );

    // Xóa đạn kẻ địch đã chết (ví dụ: bay ra ngoài màn hình)
    enemyBullets.erase(
        std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](const Bullet& b) {
            return !b.isAlive();
            }),
        enemyBullets.end()
    );
    // *** THÊM: XÓA POWER-UP ĐÃ BIẾN MẤT ***
    powerUps.erase(
        std::remove_if(powerUps.begin(), powerUps.end(), [](const PowerUp& p) {
            return !p.isAlive();
            }),
        powerUps.end()
    );
}

// Cập nhật logic game
void GameManager::update(float dt) {
    // Chỉ cập nhật logic game khi đang chơi
    if (gameState == GameState::Running) {
        //  LẤY VỊ TRÍ PLAYER (ĐỂ TRUYỀN CHO CÁC HÀM UPDATE) 
        sf::Vector2f playerPos = player.getPosition();
        // Xử lý đầu vào người chơi
        player.handleInput(dt);
        // Cập nhật người chơi (cooldown, đạn, thanh máu)
        player.update(dt, playerPos);

        // *** CẬP NHẬT ĐIỂM SỐ (Chuy?n v? trí ?? Boss không ?è lên) ***
        // N?u Boss còn s?ng, ??t ?i?m s? ? d??i thanh máu Boss
        if (bossActive) {
            scoreText.setPosition(10.f, 35.f);
        }
        else {
            scoreText.setPosition(10.f, 10.f);
        }
        scoreText.setString("Score: " + std::to_string(score));
        // *** THÊM LOGIC CẬP NHẬT HIGH SCORE NGAY TẠI ĐÂY ***
        if (score > highScore) {
            newHighScoreSet = true; // Đánh dấu là có kỷ lục mới
            highScore = score;
            highScoreName = currentPlayerName;

            // Cập nhật cả Text hiển thị
            highScoreText.setString("High Score: " + highScoreName + " - " + std::to_string(highScore));
            // Căn lề lại
            sf::FloatRect hsRect = highScoreText.getLocalBounds();
            highScoreText.setOrigin(hsRect.left + hsRect.width, hsRect.top);
            highScoreText.setPosition(sf::Vector2f(WINDOW_WIDTH - 10.f, 10.f));
        }

        // Kiểm tra nếu người chơi chết
        if (player.isDead()) {
            gameState = GameState::GameOver;
            if (newHighScoreSet) {
                saveHighScore();
            }
            // Cập nhật và căn giữa điểm số cuối cùng
            finalScoreText.setString("Your Score: " + std::to_string(score));
            sf::FloatRect finalScoreRect = finalScoreText.getLocalBounds();
            finalScoreText.setOrigin(finalScoreRect.left + finalScoreRect.width / 2.0f, finalScoreRect.top + finalScoreRect.height / 2.0f);
            finalScoreText.setPosition(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 20.f));
            std::cout << "Game Over!\n";
            return;
        }

        // *** LOGIC KÍCH HOẠT BOSS 1 ***
        if (!bossActive && !boss1Defeated && score >= 1500 && !boss) {
            spawnBoss(1); // <-- Gọi spawnBoss(1)
        }
        // *** LOGIC KÍCH HOẠT BOSS 2 ***
        if (!bossActive && boss1Defeated && !boss2Defeated && score >= 7500 && !boss) {
            spawnBoss(2); // <-- Gọi spawnBoss(2)
        }

        // *** CẬP NHẬT BOSS HOẶC QUÁI THƯỜNG ***
        if (bossActive && boss) {
            // C?p nh?t Boss
            boss->update(dt, playerPos);
            boss->shoot(enemyBullets, playerBulletTexture);

            // Ki?m tra n?u Boss ch?t
            if (!boss->isAlive()) {
                score += 5000; // Th??ng 5000 ?i?m
                boss.reset(); // Xóa Boss
                bossActive = false;

                // KIỂM TRA XEM BOSS NÀO VỪA CHẾT
                if (!boss1Defeated) { // Nếu cờ boss1Defeated đang là false
                    // ==> Đây chắc chắn là Boss 1 vừa bị hạ
                    boss1Defeated = true; // <-- ĐÁNH DẤU BOSS 1 ĐÃ CHẾT
                    std::cout << "BOSS 1 DEFEATED!" << std::endl;
                }
                else {
                    boss2Defeated = true; // 1. Đánh dấu Boss 2 đã chết
                    std::cout << "BOSS 2 DEFEATED! The fight continues..." << std::endl;
                    // 2. Thưởng cho người chơi
                    spawnPowerUp(player.getPosition());
                    spawnPowerUp(player.getPosition());

                    // 3. KHÔNG 'return', KHÔNG đổi 'gameState'
                    //    Game sẽ tự động quay lại vòng lặp sinh quái
                }
            }
            
        }
        else {
            // Sinh kẻ địch theo một xác suất hoặc thời gian
            if (std::rand() % 100 < 5) // 5% cơ hội sinh mỗi frame (tùy chỉnh)
                spawnEnemy();

            // Cập nhật và xử lý bắn đạn của kẻ địch
            for (auto& enemy : enemies) {
                enemy.update(dt, playerPos);
                // Kẻ địch bắn đạn
                enemy.shoot(enemyBullets, playerBulletTexture); // Dùng chung texture
            }
        }
        // *** THÊM: CẬP NHẬT VỊ TRÍ POWER-UP (sau khi cập nhật enemies) ***
        for (auto& pu : powerUps) {
            pu.update(dt);
        }


        // Cập nhật vị trí đạn kẻ địch
        for (auto& bullet : enemyBullets) {
            bullet.update(dt);
        }

        // Xử lý va chạm
        checkPlayerBulletEnemyCollision();
        checkEnemyBulletPlayerCollision();
        checkPlayerEnemyCollision();
        checkPlayerPowerUpCollision();

        // Dọn dẹp các đối tượng không cần thiết
        cleanUp();
    }
    // Nếu là GameOver, Menu, NameInput, hàm sẽ không làm gì cả
}

// Vẽ tất cả lên màn hình
void GameManager::render() {
    window.clear(); // Xóa màn hình với màu đen

    // Vẽ phông nền trước (có thể vẽ cho mọi trạng thái)
    window.draw(backgroundSprite);

    if (gameState == GameState::Menu) {
        // Vẽ các thành phần của Menu
        window.draw(titleText);
        window.draw(instructionText);
        window.draw(highScoreText); // <-- VẼ ĐIỂM CAO Ở MENU
    }
    // VẼ MÀN HÌNH NHẬP TÊN
    else if (gameState == GameState::NameInput) {
        window.draw(nameInputTitle);
        window.draw(playerNameText);
        window.draw(highScoreText); // Cũng hiển thị highscore ở đây
    }
    else if (gameState == GameState::Running || gameState == GameState::Paused) {
        // Vẽ các đối tượng game
        player.draw(window);

        // V? quái th??ng
        for (auto& enemy : enemies) {
            enemy.draw(window);
        }

        // *** VẼ BOSS ***
        if (bossActive && boss) {
            boss->draw(window);
        }

        // V? ??n ??ch
        for (auto& bullet : enemyBullets) {
            bullet.draw(window);
        }
        // *** THÊM: VẼ POWER-UP ***
        for (auto& pu : powerUps) {
            pu.draw(window);
        }

        window.draw(scoreText);
        // Ch? v? HighScore n?u không có Boss
        if (!bossActive) {
            window.draw(highScoreText);
        }
        // *** CHỈ VẼ CHỮ "PAUSED" NẾU ĐANG PAUSE ***
        if (gameState == GameState::Paused) {
            window.draw(pauseText);
        }
    }
    else if (gameState == GameState::GameOver) {
        // Vẫn vẽ các đối tượng game ở trạng thái cuối cùng
        player.draw(window);
        for (auto& enemy : enemies) {
            enemy.draw(window);
        }
        // V? Boss n?u ng??i ch?i thua khi ?ang ?ánh Boss
        if (bossActive && boss) {
            boss->draw(window);
        }
        for (auto& bullet : enemyBullets) {
            bullet.draw(window);
        }
        // Vẽ chữ Game Over và hướng dẫn lên trên
        window.draw(gameOverText);
        window.draw(restartText);
        // <-- VẼ ĐIỂM SỐ CUỐI CÙNG
        window.draw(finalScoreText);
    }

    window.display(); // Hiển thị khung hình đã vẽ
}

// Vòng lặp chính của game
void GameManager::run() {
    while (window.isOpen()) {
        // Tính thời gian đã trôi qua kể từ khung hình trước (dt)
        float dt = clock.restart().asSeconds();

        handleEvents();
        update(dt);
        render();
    }
}
void GameManager::loadHighScore() {
    std::ifstream file("assets/highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.ignore(); // Bỏ qua ký tự newline còn lại sau khi đọc số
        std::getline(file, highScoreName); // Đọc tên (có thể rỗng)
        file.close();
        if (highScoreName.empty()) {
            highScoreName = "None";
        }
    }
    else {
        highScore = 0;
        highScoreName = "None"; // Mặc định
    }
}

void GameManager::saveHighScore() {
    std::ofstream file("assets/highscore.txt");
    if (file.is_open()) {
        file << highScore << std::endl; // Ghi điểm
        file << highScoreName;         // Ghi tên
        file.close();
    }
    else {
        std::cout << "ERROR: Khong the luu file highscore.txt!\n";
    }
}

// *** HÀM MỚI ĐỂ SINH BOSS ***
void GameManager::spawnBoss(int bossNumber) {
    if (bossNumber == 1) {
        // Tạo Boss 1 (với scale 2.0f như chúng ta đã sửa trước đó)
        boss = std::make_unique<Boss>(bossTexture, 2.0f);
        std::cout << "BOSS 1 HAS APPEARED!" << std::endl;
    }
    else if (bossNumber == 2) {
        // Tạo Boss 2 (dùng hình ảnh MỚI và kích thước MỚI, ví dụ 2.5f)
        boss = std::make_unique<Boss>(boss2Texture, 2.5f); // <-- Dùng boss2Texture và scale 2.5
        std::cout << "BOSS 2 HAS APPEARED!" << std::endl;
    }

    bossActive = true;
    // Xóa h?t quái th??ng và ??n ?? t?p trung ?ánh Boss
    enemies.clear();
    enemyBullets.clear();
}
// Hàm sinh PowerUp
void GameManager::spawnPowerUp(sf::Vector2f position) {
    // Random loại power-up
    int roll = std::rand() % 100;
    PowerUpType type;

    if (roll < 33) {
        type = PowerUpType::Heal;
    }
    else if (roll < 66) {
        type = PowerUpType::Shield;
    }
    else {
        type = PowerUpType::DoubleShot;
    }

    // Tạo và thêm vào danh sách
    powerUps.emplace_back(powerUpTexture, position, type);
    std::cout << "Da tao PowerUp!\n";
}

// Hàm kiểm tra va chạm Player và PowerUp
void GameManager::checkPlayerPowerUpCollision() {
    sf::FloatRect playerBounds = player.getBounds();

    powerUps.erase(
        std::remove_if(powerUps.begin(), powerUps.end(), [&](PowerUp& pu) {
            if (pu.getBounds().intersects(playerBounds)) {
                // Va chạm! Kích hoạt lợi ích
                switch (pu.getType()) {
                case PowerUpType::Heal:
                    player.takeDamage(-20); // Gửi số âm để hồi máu
                    std::cout << "Nham HEAL!\n";
                    break;
                case PowerUpType::Shield:
                    player.activateShield(12.f);
                    std::cout << "Nham SHIELD!\n";
                    break;
                case PowerUpType::DoubleShot:
                    player.activateDoubleShot(10.f);
                    std::cout << "Nham DOUBLE SHOT!\n";
                    break;
                }
                return true; // Trả về true để xóa power-up này
            }
            return false; // Không va chạm, giữ lại
            }),
        powerUps.end()
    );
}
