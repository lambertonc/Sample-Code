#pragma once

#include "stdafx.h"
#include "hudelems.h"

enum Verification {
	Unverified,
	Verified,
	VIP,
	Admin,
	Host
};

struct _Client {
	Verification Status;
	struct _Menu {
		bool Open;
		int Scroll;
		int Index;
		int subMenu;
	} Menu;
	struct _Hud {
		game_hudelem_s *MenuShader;
		game_hudelem_s *NewsbarShader;
		game_hudelem_s *Scrollbar;
		game_hudelem_s *MenuTitle;
		game_hudelem_s *Item[8];
		game_hudelem_s *LolWut;
		game_hudelem_s *y;
		game_hudelem_s *x;
		game_hudelem_s *shader;
		game_hudelem_s *item;
	} Hud;
} Client[18];