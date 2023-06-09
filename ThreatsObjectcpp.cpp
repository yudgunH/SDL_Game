﻿#include "ThreatsObject.h"

ThreatsObject::ThreatsObject()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 0.0;
	y_pos_ = 0.0;
	on_ground_ = false;
	come_back_time_ = 0;
	frame_ = 0;

	animation_a_ = 0;
	animation_b_ = 0;
	input_type_.left_ = 0;
	type_move_ = STATIC_THREAT;
}

ThreatsObject::~ThreatsObject()
{

}

bool ThreatsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / THREAT_FRAME_NUM;
		height_frame_ = rect_.h;
	}
	return ret;
}

void ThreatsObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		frame_clip[0].x = 0;
		frame_clip[0].y = 0;
		frame_clip[0].w = width_frame_;
		frame_clip[0].h = height_frame_;

		frame_clip[1].x = width_frame_;
		frame_clip[1].y = 0;
		frame_clip[1].w = width_frame_;
		frame_clip[1].h = height_frame_;

		frame_clip[2].x = 2 * width_frame_;
		frame_clip[2].y = 0;
		frame_clip[2].w = width_frame_;
		frame_clip[2].h = height_frame_;

		frame_clip[3].x = 3 * width_frame_;
		frame_clip[3].y = 0;
		frame_clip[3].w = width_frame_;
		frame_clip[3].h = height_frame_;

		frame_clip[4].x = 4 * width_frame_;
		frame_clip[4].y = 0;
		frame_clip[4].w = width_frame_;
		frame_clip[4].h = height_frame_;

		frame_clip[5].x = 5 * width_frame_;
		frame_clip[5].y = 0;
		frame_clip[5].w = width_frame_;
		frame_clip[5].h = height_frame_;

		frame_clip[6].x = 6 * width_frame_;
		frame_clip[6].y = 0;
		frame_clip[6].w = width_frame_;
		frame_clip[6].h = height_frame_;

		frame_clip[7].x = 7 * width_frame_;
		frame_clip[7].y = 0;
		frame_clip[7].w = width_frame_;
		frame_clip[7].h = height_frame_;
	}
}

void ThreatsObject::Show(SDL_Renderer* des)
{
	if (come_back_time_ == 0)
	{
		rect_.x = x_pos_ - map_x_;
		rect_.y = y_pos_ - map_y_;
		frame_++;
		if (frame_ > 8)
		{
			frame_ = 0;
		}

		SDL_Rect* currentClip = &frame_clip[frame_];
		SDL_Rect rendQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
		SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
	}
}

void ThreatsObject::Doplayer(Map& gMap)
{
	if (come_back_time_ == 0)
	{
		x_val_ = 0;
		y_val_ += THREAT_GRAVITY_SPEED;
		if (y_val_ >= THREAT_MAX_FALL_SPEED)
		{
			y_val_ = THREAT_MAX_FALL_SPEED;
		}

		if (input_type_.left_ == 1)
		{
			x_val_ -= THREAT_SPEED;
		}
		else if (input_type_.right_ == 1)
		{
			x_val_ += THREAT_SPEED;
		}
	
		CheckToMap(gMap);  
	}
	else if (come_back_time_ > 0)
	{
		come_back_time_--;
		if (come_back_time_ == 0)
		{
			InitThreat();
		}
	}
}

void ThreatsObject::InitThreat()
{
	x_val_ = 0;
	y_val_ = 0;
	if (x_pos_ > 256)
	{
		x_pos_ -= 256;
		animation_a_ -= 256;
		animation_b_ -= 256;
	}
	else
	{
		x_pos_ = 0;
	}
	y_pos_ = 0;
	come_back_time_ = 0;
	input_type_.left_ = 1;
}

void ThreatsObject::CheckToMap(Map& map_data)
{
	int x1 = 0;
	int x2 = 0;
	  
	int y1 = 0;
	int y2 = 0;

	//Check horizontal
	int height_min = height_frame_ < TILE_SIZE ? height_frame_ : TILE_SIZE;

	x1 = (x_pos_ + x_val_) / TILE_SIZE;
	x2 = (x_pos_ + x_val_ + width_frame_ - 1) / TILE_SIZE;

	y1 = (y_pos_) / TILE_SIZE;
	y2 = (y_pos_ + height_min - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 <= MAX_MAP_X && y1 > 0 && y2 <= MAX_MAP_Y)
	{
		if (x_val_ > 0) // nhân vật đang chạy sang phải
		{
			if (map_data.tile[y1][x2] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
			{
				x_pos_ = x2 * TILE_SIZE;
				x_pos_ -= width_frame_ + 1;
				x_val_ = 0;

				on_ground_ = true;
				
			}
		}
		else if (x_val_ < 0)
		{
			if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y2][x1] != BLANK_TILE)
			{
				x_pos_ = (x1 + 1) * TILE_SIZE;
				x_val_ = 0;
			}
		}
	}


	//Check vertical

	int width_min = width_frame_ < TILE_SIZE ? width_frame_ : TILE_SIZE;
	x1 = (x_pos_) / TILE_SIZE;
	x2 = (x_pos_ + width_min) / TILE_SIZE;

	y1 = (y_pos_ + y_val_) / TILE_SIZE;
	y2 = (y_pos_ + y_val_ + height_frame_ - 1) / TILE_SIZE;

	if (x1 >= 0 && x2 < MAX_MAP_X && y1 >= 0 && y2 < MAX_MAP_Y)
	{
		if (y_val_ > 0)
		{
			if (map_data.tile[y2][x1] != BLANK_TILE || map_data.tile[y2][x2] != BLANK_TILE)
			{
				y_pos_ = y2 * TILE_SIZE;
				y_pos_ -= (height_frame_ + 1);
				y_val_ = 0;
				on_ground_ = true;
			}
		}
		else if (y_val_ < 0)
		{
			if (map_data.tile[y1][x1] != BLANK_TILE || map_data.tile[y1][x2] != BLANK_TILE)
			{
				y_pos_ = (y1 + 1) * TILE_SIZE;
				y_val_ = 0;
			}
		}
	}

	x_pos_ += x_val_;
	y_pos_ += y_val_;

	if (x_pos_ < 0)
	{
		x_pos_ = 0;
	}
	else if (x_pos_ + width_frame_ > map_data.max_x_)
	{
		x_pos_ = map_data.max_x_ - width_frame_ - 1;
	}

}

void ThreatsObject::ImpMoveType(SDL_Renderer* screen)
{
	if (type_move_ == STATIC_THREAT)
	{
		;//
	}
	else
	{
		if (on_ground_ == true)
		{
			if (x_pos_ > animation_b_)
			{
				input_type_.left_ = 1;
				input_type_.right_ = 0;
				LoadImg("img//threat_left.png", screen);
			}
			else if (x_pos_ < animation_b_)
			{
				input_type_.left_ = 0;
				input_type_.right_ = 1;
				LoadImg("img//threat_right.png", screen);
			}
		}
		else
		{
			if (input_type_.left_ == 1)
			{
				LoadImg("img//threat_left.png", screen);
			}
		}
	}
}

void ThreatsObject::InitBullet(BulletObject* p_bullet, SDL_Renderer* screen)
{
	if (p_bullet != NULL)
	{
		p_bullet->set_bullet_type(BulletObject::LASER_BULLET);
		bool ret = p_bullet->LoadImgBullet(screen);
		if (ret)
		{
			p_bullet->set_is_move(true);
			p_bullet->set_bullet_dir(BulletObject::DIR_LEFT);
			p_bullet->SetRect(rect_.x + 10, rect_.y + 10);
			p_bullet->set_x_val(15);
			bullet_list_.push_back(p_bullet);
		}
	}
}

void ThreatsObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
	for (int i = 0; i < bullet_list_.size(); i++)
	{
		BulletObject* p_bullet = bullet_list_.at(i);
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move())
			{
				int bullet_distance = rect_.x + width_frame_ - p_bullet->GetRect().x;
				if (bullet_distance < 300 && bullet_distance > 0)
				{
					p_bullet->HandleMove(x_limit, y_limit);
					p_bullet->Render(screen);
				}
				else
				{
					p_bullet->set_is_move(false);
				}
				
			}
			else
			{
				p_bullet->set_is_move(true);
				p_bullet->SetRect(rect_.x + 10, rect_.y + 10);
			}

		}
	}
}

void ThreatsObject::RemoveBullet(const int& idx)
{
	int size = bullet_list_.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = bullet_list_.at(idx);
		bullet_list_.erase(bullet_list_.begin() + idx);
		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

SDL_Rect ThreatsObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;

	return rect;
}


////////////////////////////////////////////////////////////////////

TObject::TObject()
{
	width_frame_ = 0;
	height_frame_ = 0;
	x_val_ = 0.0;
	y_val_ = 0.0;
	x_pos_ = 0.0;
	y_pos_ = 0.0;
	frame_ = 0;
	input_type_.left_ = 0;
	type_move_ = 0;
	m_Type = T_BIRD;
	m_alive = true;
	m_time_alive = 0;
	speed_ = 0;
}

TObject::~TObject()
{

}

bool TObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);
	if (ret)
	{
		width_frame_ = rect_.w / THREAT_FRAME_NUM;
		height_frame_ = rect_.h;

		set_clips();
	}
	return ret;
}

void TObject::set_clips()
{
	if (width_frame_ > 0 && height_frame_ > 0)
	{
		for (int i = 0; i < THREAT_FRAME_NUM; i++)
		{
			frame_clip[i].x = i * width_frame_;
			frame_clip[i].y = 0;
			frame_clip[i].w = width_frame_;
			frame_clip[i].h = height_frame_;
		}

	}
}

void TObject::Show(SDL_Renderer* des)
{
	if (m_alive == false)
		return;

	rect_.x = x_pos_ - map_x_;
	rect_.y = y_pos_ - map_y_;
	frame_++;
	if (frame_ > 8)
	{
		frame_ = 0;
	}

	SDL_Rect* currentClip = &frame_clip[frame_];
	SDL_Rect rendQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
	SDL_RenderCopy(des, p_object_, currentClip, &rendQuad);
}

void TObject::UpdateMoving(SDL_Renderer* screen)
{
	if (m_Type == T_BIRD)
	{

		if (m_alive == true)  // khi dang song thi moi xu ly di chuyen
		{
			if (type_move_ == true) // left-to-right
			{
				x_pos_ += (x_val_ + speed_);

				int rX = x_pos_ - map_x_;
				if (rX > SCREEN_WIDTH) // di chuyen het ve ben mep phai
				{
					m_alive = false; // tam thoi coi nhu chet
					type_move_ = !type_move_;  // dao chieu di chuyen
					m_time_alive = 100;  // set thoi gian hoi sinh = 100

					input_type_.right_ = 0;
					input_type_.left_ = 1;
					LoadImg("img//threat_left.png", screen);
				}
			}
			else
			{
				x_pos_ -= (x_val_ + speed_);

				// rx la toa do so voi man hinh
				int rX = x_pos_ - map_x_;

				if (rX + width_frame_ < 0) // sau khi di chuyen het mep ben trai
				{
					// dao chieu di chuyen
					m_alive = false;
					type_move_ = !type_move_;
					m_time_alive = 100;
					input_type_.right_ = 1;
					input_type_.left_ = 0;
					LoadImg("img//threat_right.png", screen);
				}
			}
		}
		else
		{
			// neu dang chet, thi bat dau - thoi gian hoi sinh
			m_time_alive--;

			// khi nao m_time_alive = 0, nghia la thoi gian cho hoi sinh da het
			if (m_time_alive == 0)
			{
				// bat dau hoi sinh cho con chim
				m_alive = true;
				speed_ += 2;
				if (speed_ > MAX_SPEED)
				{
					speed_ = MAX_SPEED;
				}
			}
		}
	}
}

void TObject::InitBullet(SDL_Renderer* screen, int bl_type)
{
	BulletObject* pBul = new BulletObject();
	pBul->set_bullet_type(BulletObject::LASER_BULLET);

	bool ret = pBul->LoadImgBullet(screen);
	if (ret)
	{
		pBul->set_is_move(true);
		if (bl_type == 0) // left-to_right 
		{
			pBul->set_bullet_dir(BulletObject::DIR_LEFT_XY);
		}
		else
		{
			pBul->set_bullet_dir(BulletObject::DIR_RIGHT_XY);
		}

		int xb = x_pos_ - map_x_;
		int yb = y_pos_ - map_y_;

		pBul->SetRect(xb + 10, yb + 10);
		pBul->set_x_val(8);
		pBul->set_y_val(8);
		bullet_list_.push_back(pBul);
	}
}

void TObject::MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit)
{
	if (m_Type == T_BOSS)
	{
		for (int i = 0; i < bullet_list_.size(); i++)
		{
			BulletObject* p_bullet = bullet_list_.at(i);
			if (p_bullet != NULL)
			{
				if (p_bullet->get_is_move())
				{
					p_bullet->HandleMove(x_limit, y_limit);
					p_bullet->Render(screen);
				}
				else
				{
					p_bullet->set_is_move(true);
					int xb = x_pos_ - map_x_;
					int yb = y_pos_ - map_y_;
					p_bullet->SetRect(xb + 10, yb + 10);
				}

			}
		}
	}
}

void TObject::RemoveBullet(const int& idx)
{
	int size = bullet_list_.size();
	if (size > 0 && idx < size)
	{
		BulletObject* p_bullet = bullet_list_.at(idx);
		bullet_list_.erase(bullet_list_.begin() + idx);
		if (p_bullet)
		{
			delete p_bullet;
			p_bullet = NULL;
		}
	}
}

SDL_Rect TObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;

	return rect;
}