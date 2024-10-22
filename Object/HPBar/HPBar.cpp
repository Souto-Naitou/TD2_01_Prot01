#include "HPBar.h"
#include <Novice.h>
#include <cmath>
#include <array>

void HPBar::Init(float _maxHP, Vector2 _pos, float _maxHPwidth, float _height, float _rotation, unsigned int _color)
{
	hp_ = _maxHP;
	maxHP_ = _maxHP;

	pos_ = _pos;

	maxHPWidth_ = _maxHPwidth;

	height_ = _height;
	width_ = _maxHPwidth;

	rotation_ = _rotation;
	color_ = _color;
}

void HPBar::Update(float _hp)
{
	width_ = (_hp / maxHP_) * maxHPWidth_;
}

void HPBar::Draw()
{
	Novice::DrawBox(static_cast<int>(pos_.x), static_cast<int>(pos_.y), static_cast<int>(width_), static_cast<int>(height_), rotation_, color_, kFillModeSolid);

	// outline
	float offset = 2.0f;
	
	float left = -offset;
	float top = -offset;
	float right = maxHPWidth_ + offset;
	float bottom = height_ + offset;

	// 頂点データ
	std::array vertices = {
		Vector2{left,  top   }, // 左上
		Vector2{right, top   }, // 右上
		Vector2{left,  bottom}, // 左下
		Vector2{right, bottom}, // 右下
	};

	// 回転
	for (auto& vertex : vertices) {
		// 回転
		vertex = {
			vertex.x * cosf(rotation_) + vertex.y * -sinf(rotation_),
			vertex.x * sinf(rotation_) + vertex.y * cosf(rotation_) };
		// 平行移動
		vertex.x += pos_.x;
		vertex.y += pos_.y;
	}

	Novice::DrawLine(static_cast<int>(vertices[0].x), static_cast<int>(vertices[0].y), static_cast<int>(vertices[1].x), static_cast<int>(vertices[1].y), WHITE);
	Novice::DrawLine(static_cast<int>(vertices[0].x), static_cast<int>(vertices[0].y), static_cast<int>(vertices[2].x), static_cast<int>(vertices[2].y), WHITE);
	Novice::DrawLine(static_cast<int>(vertices[1].x), static_cast<int>(vertices[1].y), static_cast<int>(vertices[3].x), static_cast<int>(vertices[3].y), WHITE);
	Novice::DrawLine(static_cast<int>(vertices[2].x), static_cast<int>(vertices[2].y), static_cast<int>(vertices[3].x), static_cast<int>(vertices[3].y), WHITE);
}


