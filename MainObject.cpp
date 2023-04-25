﻿#include "MainObject.h"

MainObject::MainObject()
{
	frame_ = 0;
	x_pos_ = 600;
	y_pos_ = 500;
	x_val_ = 0;
	y_val_ = 0;
	width_frame_ = 0;
	height_frame_ = 0;
	status_ = WALK_NONE;
	input_type_.left_ = 0;
	input_type_.right_ = 0;
	input_type_.down_ = 0;
	input_type_.up_ = 0;
	on_ground = false;
}

MainObject::~MainObject()
{

}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret == true)
	{
		width_frame_ = rect_.w / 8;
		height_frame_ = rect_.h;

	}
	return ret;
}

void MainObject::set_clips()
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

		frame_clip[2].x = 2*width_frame_;
		frame_clip[2].y = 0;
		frame_clip[2].w = width_frame_;
		frame_clip[2].h = height_frame_;

		frame_clip[3].x = 3*width_frame_;
		frame_clip[3].y = 0;
		frame_clip[3].w = width_frame_;
		frame_clip[3].h = height_frame_;

		frame_clip[4].x = 4*width_frame_;
		frame_clip[4].y = 0;
		frame_clip[4].w = width_frame_;
		frame_clip[4].h = height_frame_;

		frame_clip[5].x = 5*width_frame_;
		frame_clip[5].y = 0;
		frame_clip[5].w = width_frame_;
		frame_clip[5].h = height_frame_;

		frame_clip[6].x = 6*width_frame_;
		frame_clip[6].y = 0;
		frame_clip[6].w = width_frame_;
		frame_clip[6].h = height_frame_;

		frame_clip[7].x = 7*width_frame_;
		frame_clip[7].y = 0;
		frame_clip[7].w = width_frame_;
		frame_clip[7].h = height_frame_;

	}
}

void MainObject::Show(SDL_Renderer* des)
{
	UpdateImgPlayer(des);

	if (input_type_.left_ == 1 || input_type_.right_ == 1)
	{
		frame_++;
	}
	else
	{
		frame_ = 0;
	}

	if (frame_ >= 8)
	{
		frame_ = 0;
	}

	rect_.x = x_pos_;
	rect_.y = y_pos_;

	SDL_Rect* current_clip = &frame_clip[frame_];

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };

	SDL_RenderCopy(des, p_object_, current_clip, &renderQuad);
}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen)
{
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 1;
			input_type_.left_ = 0;
			UpdateImgPlayer(screen);
		}
		break;

		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 1;
			input_type_.right_ = 0;
			UpdateImgPlayer(screen);
		}
		break;
		case SDLK_UP:
		{
			if( on_ground == true)
			input_type_.jump_ ++;
		}
		break;
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
		case SDLK_RIGHT:
		{
			status_ = WALK_RIGHT;
			input_type_.right_ = 0;
		}
		break;

		case SDLK_LEFT:
		{
			status_ = WALK_LEFT;
			input_type_.left_ = 0;
		}
		break;
		}
	}

}

void MainObject::DoPlayer(Map& map_data)
{
	x_val_ = 0;
	y_val_ += GRAVITY_SPEED;

	if (y_val_ >= MAX_FALL_SPEED)
	{
		y_val_ = MAX_FALL_SPEED;
	}

	if (input_type_.left_ == 1)
	{
		x_val_ -= PLAYER_SPEED;
	}

	if (input_type_.right_ == 1)
	{
		x_val_ += PLAYER_SPEED;
	}
	if (input_type_.jump_ != 0) // Kiểm tra nếu người chơi nhấn phím lên
	{
		if (on_ground == true)
		{
			y_val_ = - PLAYER_JUMP_VAL;
			on_ground = false;
			// Reset biến nhảy
			input_type_.jump_ = 0;
		}
	}
	CheckToMap(map_data);
}

void MainObject::CheckToMap(Map& map_data)
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

				on_ground = true;
				if (status_ == WALK_NONE)
				{
					status_ = WALK_RIGHT;
				}		
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
				on_ground = true;
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

void MainObject::UpdateImgPlayer(SDL_Renderer* des)
{
	if (on_ground == true)
	{
		if (status_ == WALK_LEFT)
		{
			LoadImg("img//player_left.png", des);
		}
		else
		{
			LoadImg("img//player_right.png", des);
		}
	}
	else
	{
		if (status_ == WALK_LEFT)
		{
			LoadImg("img//jump_left.png", des);
		}
		else
		{
			LoadImg("img//jump_right.png", des);
		}
	}
}

SDL_Rect MainObject::GetRectFrame()
{
	SDL_Rect rect;
	rect.x = rect_.x;
	rect.y = rect_.y;
	rect.w = width_frame_;
	rect.h = height_frame_;

	return rect;
}