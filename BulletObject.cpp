#include "BulletObject.h"

BulletObject::BulletObject()
{
	x_val_ = 0;
	y_val_ = 0;
	is_move_ = false;
	bullet_type_ = SPHERE_BULLET;
}

BulletObject::~BulletObject()
{

}

void BulletObject::HandleMove(const int& x_border, const int& y_border)
{
	if ( bullet_dir_ == DIR_RIGHT)
	{
		rect_.x += x_val_;
		if (rect_.x > x_border)
		{
			is_move_ = false;
		}
	}		
	else if ( bullet_dir_ == DIR_LEFT)
	{
		rect_.x -= x_val_;
		if (rect_.x < 0)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_LEFT_XY)
	{
		rect_.x += x_val_;
		rect_.y += y_val_;

		if (rect_.x > x_border || rect_.y > y_border)
		{
			is_move_ = false;
		}
	}
	else if (bullet_dir_ == DIR_RIGHT_XY)
	{
		rect_.x -= x_val_;
		rect_.y += y_val_;

		if (rect_.x < 0 || rect_.y > y_border)
		{
			is_move_ = false;
		}
	}
}

bool BulletObject::LoadImgBullet(SDL_Renderer* des)
{
	bool ret = false;
	if (bullet_type_ = LASER_BULLET)
	{
		ret = LoadImg("img//laser_bullet.png", des);
	}
	else
	{
		ret = LoadImg("img//sphere_bullet.png", des);
	}
	return ret;
}