#pragma once
#ifndef THREATS_OBJECT_H_
#define THREATS_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"
#include "BulletObject.h"

#define THREAT_FRAME_NUM 8
#define THREAT_GRAVITY_SPEED 0.8
#define THREAT_MAX_FALL_SPEED 10
#define THREAT_SPEED 3

#define MAX_SPEED 20

class ThreatsObject : public BaseObject
{
public:
	ThreatsObject();
	~ThreatsObject();

	enum TypeMove
	{
		STATIC_THREAT = 0,
		MOVE_IN_SPACE_THREAT =1,
	};

	void set_x_val(const int& xVal) { x_val_ = xVal; }
	void set_y_val(const int& yVal) { y_val_ = yVal; }

	void set_x_pos(const float& xp) { x_pos_ = xp; }
	void set_y_pos(const float& yp) { y_pos_ = yp; }
	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }
	void SetMapXY(const int& mp_x, const int& mp_y) { map_x_ = mp_x; map_y_ = mp_y; }

	void set_clips();
	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }
	void Doplayer(Map& gMap);
	void CheckToMap(Map& gMap);
	void InitThreat();
	void set_type_move(const int& typeMove) { type_move_ = typeMove; }
	void SetAnimationPos(const int& pos_a, const int& pos_b) { animation_a_ = pos_a, animation_b_ = pos_b; }
	void set_input_left(const int& ipLeft) { input_type_.left_ = ipLeft; };
	void ImpMoveType(SDL_Renderer* screen);

	std::vector<BulletObject*> get_bullet_list() const { return bullet_list_; }
	void set_bullet_list(std::vector<BulletObject*> bl_list) { bullet_list_ = bl_list; }

	void InitBullet(BulletObject* p_bullet, SDL_Renderer* screen);
	void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
	SDL_Rect GetRectFrame();
	void RemoveBullet(const int& idx);
private:
	SDL_Rect frame_clip[THREAT_FRAME_NUM];
	float x_pos_;
	float y_pos_;
	bool on_ground_;
	int come_back_time_;
	int width_frame_;
	int height_frame_;
	int frame_;
	float x_val_;
	float y_val_;
	int map_x_;
	int map_y_;

	int type_move_;
	int animation_a_;
	int animation_b_;
	Input input_type_;

	std::vector<BulletObject*> bullet_list_;

};



class TObject : public BaseObject
{
public:
	TObject();
	~TObject();

	enum T_Type
	{
		T_BIRD = 0,
		T_BOSS = 1,
	};

	void set_x_val(const int& xVal) { x_val_ = xVal; }
	void set_y_val(const int& yVal) { y_val_ = yVal; }

	void set_x_pos(const float& xp) { x_pos_ = xp; }
	void set_y_pos(const float& yp) { y_pos_ = yp; }
	float get_x_pos() const { return x_pos_; }
	float get_y_pos() const { return y_pos_; }
	void SetMapXY(const int& mp_x, const int& mp_y) { map_x_ = mp_x; map_y_ = mp_y; }


	bool LoadImg(std::string path, SDL_Renderer* screen);
	void Show(SDL_Renderer* des);
	int get_width_frame() const { return width_frame_; }
	int get_height_frame() const { return height_frame_; }
	void set_type_move(const int& typeMove) { type_move_ = typeMove; }
	void set_input_left(const int& ipLeft) { input_type_.left_ = ipLeft; };
	void UpdateMoving(SDL_Renderer* screen);
	void SetType(int type) { m_Type = type; }
	int GetType() { return m_Type; }
	std::vector<BulletObject*> get_bullet_list() const { return bullet_list_; }
	void set_bullet_list(std::vector<BulletObject*> bl_list) { bullet_list_ = bl_list; }

	void InitBullet(SDL_Renderer* screen, int bl_type = 0);
	void MakeBullet(SDL_Renderer* screen, const int& x_limit, const int& y_limit);
	SDL_Rect GetRectFrame();
	void RemoveBullet(const int& idx);
	bool GetAlive() { return m_alive; }
	void SetAlive(bool alive) { m_alive = alive; }
protected:
	void set_clips();
private:
	SDL_Rect frame_clip[THREAT_FRAME_NUM];
	float x_pos_;   // toa do thuc cua doi tuong so voi tong chieu dai ban do
	float y_pos_; 
	int width_frame_;
	int height_frame_;
	int frame_;
	int speed_;
	float x_val_;
	float y_val_;
	int map_x_;
	int map_y_;
	int m_Type;
	bool type_move_;  // 0 From Left-to-Right, 1 From Right-to-Left
	int m_time_alive;
	Input input_type_;
	bool m_alive;
	std::vector<BulletObject*> bullet_list_;

};


#endif // !THREATS_OBJECT_H_
