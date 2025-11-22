#pragma once
#include<SFML/Graphics.hpp>

class Bullet{
private:
	sf::Sprite sprite; // Luu hinh anh va vi tri vien đan
	float speed; // Toc do vien dan
	bool alive = true; // Vien dan con ton tai
public:
	Bullet(const sf::Texture& texture, float startX, float startY, float speed, float scale); // ham khoi tao hinh anh, vi tri va toc do, ti le kich thuoc
	void update(float dt); // cap nhat vi tri vien dan
	void draw(sf::RenderWindow& window) const; // ve hinh anh vien dan
	sf::FloatRect getBounds() const;
	bool isAlive() const;
	void destroy();
};

