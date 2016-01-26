#pragma once

#include "stdafx.h"

enum hudelem_type {
    HUDELEMTYPE_NONE = 0x00,
    HUDELEMTYPE_TEXT = 0x01,
    HUDELEMTYPE_VALUE = 0x02,
    HUDELEMTYPE_PLAYERNAME = 0x03,
    HUDELEMTYPE_MATERIAL = 0x04,
    HUDELEMTYPE_TIMER_DOWN = 0x05,
    HUDELEMTYPE_TIMER_UP = 0x06,
    HUDELEMTYPE_TIMER_STATIC = 0x07,
    HUDELEMTYPE_TIMERTENTHS_DOWN = 0x08,
    HUDELEMTYPE_TIMERTENTHS_UP = 0x09,
    HUDELEMTYPE_TIMERTENTHS_STATIC = 0x0A,
    HUDELEMTYPE_CLOCK_DOWN = 0x0B,
    HUDELEMTYPE_CLOCK_UP = 0x0C,
    HUDELEMTYPE_WAYPOINT = 0x0D
};

enum hudelem_flags {
    HUDELEMFLAG_FOREGROUND = 0x01,
    HUDELEMFLAG_HIDEWHENDEAD = 0x02,
    HUDELEMFLAG_HIDEWHENINMENU = 0x04,
    HUDELEMFLAG_SPLATTER = 0x80,
    HUDELEMFLAG_LOWRESBACKGROUND = 0x200
};

struct hudelem_color {
	BYTE r;
	BYTE g;
	BYTE b;
	BYTE a;
};

struct hudelem_s {
    hudelem_type type;
    FLOAT y;
    FLOAT x;
    FLOAT z;
    INT targetEntNum;
    FLOAT fontScale;
    FLOAT fromFontScale;
    INT fontScaleStartTime;
    INT fontScaleTime;
    INT label;
    INT font;
    INT alignOrg;
    INT alignScreen;
    hudelem_color color;
    hudelem_color fromColor;
    INT fadeStartTime;
    INT fadeTime;
    INT height;
    INT width;
    INT materialIndex;
    INT fromHeight;
    INT fromWidth;
    INT scaleStartTime;
    INT scaleTime;
    FLOAT fromY;
    FLOAT fromX;
    INT fromAlignOrg;
    INT fromAlignScreen;
    INT moveStartTime;
    INT moveTime;
    FLOAT value;
    INT time;
    INT duration;
    INT text;
    FLOAT sort;
    hudelem_color glowColor;
    INT fxBirthTime;
    INT fxLetterTime;
    INT fxDecayStartTime;
    INT fxDecayDuration;
    INT soundId;
    INT flags;
};

struct game_hudelem_s {
    hudelem_s elem;
    INT client;
    INT team;
    INT archived;
};

game_hudelem_s* (__cdecl *HudElem_Alloc)(INT client, INT team) = (game_hudelem_s*(*)(INT,INT))0x821DF9C0;
INT (_cdecl *G_LocalizedStringIndex)(const PCHAR string) = (INT(*)(const PCHAR))0x8220C838;
INT (_cdecl *G_MaterialIndex)(const PCHAR material) = (INT(*)(const PCHAR))0x8220C9F8;

VOID setText(game_hudelem_s *elem, const PCHAR string, INT font, FLOAT fontScale, FLOAT x, FLOAT y, INT alignOrg, INT alignScreen, FLOAT sort, BYTE r, BYTE g, BYTE b, BYTE a) {
	elem->elem.type = HUDELEMTYPE_TEXT;
	elem->elem.alignOrg = alignOrg;
	elem->elem.alignScreen = alignScreen;
	elem->elem.font = font;
	elem->elem.fontScale = fontScale;
	elem->elem.x = x;
	elem->elem.y = y;
	elem->elem.color.r = r;
	elem->elem.color.g = g;
	elem->elem.color.b = b;
	elem->elem.color.a = a;
	elem->elem.sort = sort;
	elem->elem.text = G_LocalizedStringIndex(string);
}

VOID setShader(game_hudelem_s *elem, const PCHAR material, FLOAT x, FLOAT y, INT width, INT height, INT alignOrg, INT alignScreen, FLOAT sort, BYTE r, BYTE g, BYTE b, BYTE a) {
	elem->elem.type = HUDELEMTYPE_MATERIAL;
	elem->elem.alignOrg = alignOrg;
	elem->elem.alignScreen = alignScreen;
	elem->elem.x = x;
	elem->elem.y = y;
	elem->elem.color.r = r;
	elem->elem.color.g = g;
	elem->elem.color.b = b;
	elem->elem.color.a = a;
	elem->elem.width = width;
	elem->elem.height = height;
	elem->elem.sort = sort;
	elem->elem.materialIndex = G_MaterialIndex(material);
}

VOID MoveOverTime(game_hudelem_s *elem, INT time, FLOAT x, FLOAT y) {
	elem->elem.fromX = elem->elem.x;
	elem->elem.fromY = elem->elem.y;
	elem->elem.fromAlignOrg = elem->elem.alignOrg;
	elem->elem.fromAlignScreen = elem->elem.alignScreen;
	elem->elem.moveStartTime = *(PINT)0x8310A12C;
	elem->elem.moveTime = time;
	elem->elem.x = x;
	elem->elem.y = y;
}

VOID ScaleOverTime(game_hudelem_s *elem, INT time, INT width, INT height) {
	elem->elem.fromHeight = elem->elem.height;
	elem->elem.fromWidth = elem->elem.width;
	elem->elem.scaleStartTime = *(PINT)0x8310A12C;
	elem->elem.scaleTime = time;
	elem->elem.height = height;
	elem->elem.width = width;
}

VOID FadeOverTime(game_hudelem_s *elem, INT time, BYTE r, BYTE g, BYTE b, BYTE a) {
	elem->elem.fromColor = elem->elem.color;
	elem->elem.fadeStartTime = *(PINT)0x8310A12C;
	elem->elem.fadeTime = time;
	elem->elem.color.r = r;
	elem->elem.color.g = g;
	elem->elem.color.b = b;
	elem->elem.color.a = a;
}