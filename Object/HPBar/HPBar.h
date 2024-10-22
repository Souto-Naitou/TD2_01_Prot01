#pragma once
#include "Vector2.h"
class HPBar
{
public:
	HPBar() = default;
	~HPBar() = default;

	void Init(float _maxHP, Vector2 _pos, float _maxHPwidth, float _height, float _rotation, unsigned int _color);

	void Update(float _hp);

	void Draw();

	void SetHP(float _hp) { hp_ = _hp; }
	float GetHP() { return hp_; }

	void SetMaxHP(float _maxHP) { maxHP_ = _maxHP; }
	float GetMaxHP() { return maxHP_; }

	void SetPos(Vector2 _pos) { pos_ = _pos; }
	Vector2 GetPos() { return pos_; }

	void SetMaxHPWidth(float _maxHPwidth) { maxHPWidth_ = _maxHPwidth; }
	float GetMaxHPWidth() { return maxHPWidth_; }

	void SetHeight(float _height) { height_ = _height; }
	float GetHeight() { return height_; }

	void SetWidth(float _width) { width_ = _width; }
	float GetWidth() { return width_; }

	void SetRotation(float _rotation) { rotation_ = _rotation; }
	float GetRotation() { return rotation_; }

	void SetColor(unsigned int _color) { color_ = _color; }
	unsigned int GetColor() { return color_; }

private:

	float hp_;
	float maxHP_;

	Vector2 pos_ ;

	float maxHPWidth_;

	float height_;
	float width_;

	unsigned int color_;

	float rotation_;
};