#include "CommonFunc.h"
#include "BaseObject.h"
#include "TileMap.h"
#include "MainObject.h"
#include "ImpTimer.h"
#include "ThreatsObject.h"
#include "TextObject.h"

BaseObject g_background;
TTF_Font* font_time = NULL;

#define T_BIRD_NUM 5

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)	return false;
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");

	g_window = SDL_CreateWindow("Project", 
								SDL_WINDOWPOS_UNDEFINED, 
								SDL_WINDOWPOS_UNDEFINED,
								SCREEN_WIDTH, 
								SCREEN_HEIGHT, 
								SDL_WINDOW_SHOWN);

	if (g_window == NULL) {
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL) success = false;
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
				success = false;
		}
		if (TTF_Init() == -1)
		{
			success = false;
		}

		font_time = TTF_OpenFont("font//dlxfont_.ttf", 15);
		if (font_time == NULL)
		{
			success = false;
		}
	}

	return success;
}

bool LoadBackGround() 
{
	bool ret = g_background.LoadImg("img//background.png", g_screen);
	if (ret == false) return false;

	return true;
}

void close()
{
	g_background.Free();

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}

std::vector<ThreatsObject*> MakeThreadList()
{
	std::vector<ThreatsObject*> list_threats;
	ThreatsObject* dynamic_threats = new ThreatsObject[2];
	for (int i = 0; i < 1; i++)
	{
		ThreatsObject* p_threat = (dynamic_threats + i);
		if (p_threat != NULL)
		{	
			p_threat->LoadImg("img//threat_left.png", g_screen);
			p_threat->set_clips();
			p_threat->set_type_move(ThreatsObject::MOVE_IN_SPACE_THREAT);
			p_threat->set_x_pos(300);
			p_threat->set_y_pos(500);

			int pos1 = p_threat->get_x_pos() - 60;
			int pos2 = p_threat->get_y_pos() + 60;
			p_threat->SetAnimationPos(400, 10);
			p_threat->set_input_left(1);
			list_threats.push_back(p_threat);
		}
	}


#if 0
	ThreatsObject* threats_objs = new ThreatsObject[2];

	for (int i = 0; i < 2; i++)
	{
		ThreatsObject* p_threat = (threats_objs + i);
		if (p_threat != NULL)
		{
			p_threat->LoadImg("img//threat_level.png", g_screen);
			p_threat->set_clips();
			p_threat->set_x_pos(i * 1200);
			p_threat->set_y_pos(0);
			p_threat->set_type_move(ThreatsObject::STATIC_THREAT);
			p_threat->set_input_left(0);

			BulletObject* p_bullet = new BulletObject();
			p_threat->InitBullet(p_bullet, g_screen);
			
			list_threats.push_back(p_threat);
		}
	}
#endif

	return list_threats;
}

std::vector<TObject*> MakeTobjectList()
{
	std::vector<TObject*> tlist;
	TObject* bObject = new TObject[T_BIRD_NUM];
	int xList[T_BIRD_NUM] = { 300, 800, 1200, 1700, 2000 };
	for (int i = 0; i < T_BIRD_NUM; i++)
	{
		TObject* p_bird = (bObject + i);
		if (p_bird != NULL)
		{
			bool ret = p_bird->LoadImg("img//threat_left.png", g_screen);
			if (ret == true)
			{
				p_bird->SetType(TObject::T_BIRD);

				p_bird->set_x_pos(xList[i]);
				p_bird->set_y_pos(450);
				p_bird->set_type_move(false);
				p_bird->set_x_val(5);
				tlist.push_back(p_bird);
			}
		}
	}

	TObject* p_Boss = new TObject();
	bool ret = p_Boss->LoadImg("img//Boss.png", g_screen);
	if (ret == true)
	{
		p_Boss->SetType(TObject::T_BOSS);

		p_Boss->set_x_pos(SCREEN_WIDTH - 200);
		p_Boss->set_y_pos(50);
		p_Boss->InitBullet(g_screen, 1);  // RIGHT
		tlist.push_back(p_Boss);
	}

	TObject* p_Boss2 = new TObject();
	bool ret2 = p_Boss2->LoadImg("img//Boss.png", g_screen);
	if (ret2 == true)
	{
		p_Boss2->SetType(TObject::T_BOSS);

		p_Boss2->set_x_pos(50);
		p_Boss2->set_y_pos(50);
		p_Boss2->InitBullet(g_screen); // LEFT
		tlist.push_back(p_Boss2);
	}

	return tlist;
}


int main(int argc, char* argv[])
{
	ImpTimer fps_timer;

	if (InitData() == false)
		return -1;

	if (LoadBackGround() == false)
		return -1;

	GameMap game_map;
	game_map.LoadMap((char*)"map/map01.dat");
	game_map.LoadTiles(g_screen);


	MainObject p_player;
	p_player.LoadImg("img//player_right.png", g_screen);
	p_player.set_clips();

	std::vector<TObject*> threats_list = MakeTobjectList();

	//Time text
	TextObject time_game;
	time_game.SetColor(TextObject::WHITE_TEXT);

	bool is_quit = false;

	int frame_3s = 0;
	int frame_count = 0;
	while (!is_quit)
	{
		fps_timer.start();

		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}

			p_player.HandleInputAction(g_event, g_screen);
		}

		frame_count++;
		if (frame_count == FRAME_PER_SECOND)
		{
			frame_3s++;
			frame_count = 0;
		}

		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);
		
		g_background.Render(g_screen, NULL);
		Map map_data = game_map.getMap();
		game_map.DrawMap(g_screen);
		

		p_player.DoPlayer(map_data);
		p_player.Show(g_screen);
		


		SDL_Rect rect_player = p_player.GetRectFrame(); // ko phu thuoc vao for thi nen goi o ngoai

		for (int i = 0; i < threats_list.size(); i++)
		{
			TObject* p_tobj = threats_list.at(i);
			if (p_tobj != NULL)
			{
				p_tobj->SetMapXY(map_data.start_x_, map_data.start_y_);

				if (frame_3s > 3)
				{
					p_tobj->UpdateMoving(g_screen);
					p_tobj->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);
				}
				p_tobj->Show(g_screen);
				//p_tobj->MakeBullet(g_screen, SCREEN_WIDTH, SCREEN_HEIGHT);

				// Check va cham
				bool bCol1 = false;

				if (p_tobj->GetType() == TObject::T_BOSS)
				{
					std::vector<BulletObject*> tBullet_list = p_tobj->get_bullet_list();
					for (int jj = 0; jj < tBullet_list.size(); ++jj)
					{
						BulletObject* pt_bullet = tBullet_list.at(jj);
						if (pt_bullet)
						{
							SDL_Rect bRect = pt_bullet->GetRect();
							bRect.y -= 5; // ma giao
							bCol1 = SDLCommonFunc::CheckCollision(bRect, rect_player);
							if (bCol1)
							{
								pt_bullet->set_is_move(false);
								break;
							}
						}
					}
				}

				SDL_Rect rect_threat = p_tobj->GetRectFrame();
				rect_threat.x += 20; // ma giao
				bool bCol2 = SDLCommonFunc::CheckCollision(rect_player, rect_threat);

				bCol1 = false;
				bCol2 = false;

				if (bCol1 || bCol2)
				{
					if (MessageBox(NULL, L"GAME_OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
					{
						is_quit = true;
						continue;
					}
				}
			}
		}


		//Show Game Time
		std::string str_time = "Time: ";
		Uint32 time_val = SDL_GetTicks() / 1000;
		Uint32 val_time = 300 - time_val;
		if (time_val < 0)
		{
			if (MessageBox(NULL, L"GAME_OVER", L"Info", MB_OK | MB_ICONSTOP) == IDOK)
			{
				is_quit = true;
				break;
			}
		}
		else
		{
			std::string str_val = std::to_string(val_time);
			str_time += str_val;

			time_game.SetText(str_time);
			time_game.LoadFromRenderText(font_time, g_screen);
			time_game.RenderText(g_screen, SCREEN_WIDTH - 200, 15 );
		}

		SDL_RenderPresent(g_screen);

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND; // ms

		if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			if(delay_time >= 0)	SDL_Delay(delay_time);
		}
	}

	for (int i = 0; i < threats_list.size(); i++)
	{
		TObject* p_tobj = threats_list.at(i);
		if (p_tobj)
		{
			p_tobj->Free();
			p_tobj = NULL;
		}
	}
	threats_list.clear();
	close();
	return 0;
}