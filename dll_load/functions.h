#pragma once

#include "stdafx.h"
#include <stdio.h>
#include "structs.h"
#include "hudelems.h"

#define DPAD_UP "n 15"
#define DPAD_DOWN "n 17"
#define DPAD_LEFT "n 19"
#define DPAD_RIGHT "n 21"
#define BUTTON_X "n 11"
#define STICK_RIGHT "n 27"


// this is in xam.h just putting it here so you know the area types
// Notification Areas
//
/*
#define XNOTIFY_SYSTEM                  (0x00000001)
#define XNOTIFY_LIVE                    (0x00000002)
#define XNOTIFY_FRIENDS                (0x00000004)
#define XNOTIFY_CUSTOM                  (0x00000008)
#define XNOTIFY_XMP                    (0x00000020)
#define XNOTIFY_MSGR                    (0x00000040)
#define XNOTIFY_PARTY                  (0x00000080)
#define XNOTIFY_ALL                    (XNOTIFY_SYSTEM | XNOTIFY_LIVE | XNOTIFY_FRIENDS | XNOTIFY_CUSTOM | XNOTIFY_XMP | XNOTIFY_MSGR | XNOTIFY_PARTY)
*/
 
//All this can go in a header file,
 
#pragma comment(lib, "xav")
 
typedef enum {
    XNOTIFYUI_TYPE_FRIENDONLINE = 0,
    XNOTIFYUI_TYPE_GAMEINVITE = 1,
    XNOTIFYUI_TYPE_FRIENDREQUEST = 2,
    XNOTIFYUI_TYPE_GENERIC = 3, // mail icon
    XNOTIFYUI_TYPE_MULTIPENDING = 4,
    XNOTIFYUI_TYPE_PERSONALMESSAGE = 5,
    XNOTIFYUI_TYPE_SIGNEDOUT = 6,
    XNOTIFYUI_TYPE_SIGNEDIN = 7,
    XNOTIFYUI_TYPE_SIGNEDINLIVE = 8,
    XNOTIFYUI_TYPE_SIGNEDINNEEDPASS = 9,
    XNOTIFYUI_TYPE_CHATREQUEST = 10,
    XNOTIFYUI_TYPE_CONNECTIONLOST = 11,
    XNOTIFYUI_TYPE_DOWNLOADCOMPLETE = 12,
    XNOTIFYUI_TYPE_SONGPLAYING = 13, // music icon
    XNOTIFYUI_TYPE_PREFERRED_REVIEW = 14, // happy face icon
    XNOTIFYUI_TYPE_AVOID_REVIEW = 15, // sad face icon
    XNOTIFYUI_TYPE_COMPLAINT = 16, // hammer icon
    XNOTIFYUI_TYPE_CHATCALLBACK = 17,
    XNOTIFYUI_TYPE_REMOVEDMU = 18,
    XNOTIFYUI_TYPE_REMOVEDGAMEPAD = 19,
    XNOTIFYUI_TYPE_CHATJOIN = 20,
    XNOTIFYUI_TYPE_CHATLEAVE = 21,
    XNOTIFYUI_TYPE_GAMEINVITESENT = 22,
    XNOTIFYUI_TYPE_CANCELPERSISTENT = 23,
    XNOTIFYUI_TYPE_CHATCALLBACKSENT = 24,
    XNOTIFYUI_TYPE_MULTIFRIENDONLINE = 25,
    XNOTIFYUI_TYPE_ONEFRIENDONLINE = 26,
    XNOTIFYUI_TYPE_ACHIEVEMENT = 27,
    XNOTIFYUI_TYPE_HYBRIDDISC = 28,
    XNOTIFYUI_TYPE_MAILBOX = 29, // mailbox icon
    // missing a few
    XNOTIFYUI_TYPE_EXCLAIM = 34, // exclamation mark icon
    XNOTIFYUI_TYPE_GARBAGE = 68, // garbage can icon
} XNOTIFYQUEUEUI_TYPE;
 
#ifdef __cplusplus
extern "C" {
#endif
 
    VOID XNotifyQueueUI(
        XNOTIFYQUEUEUI_TYPE exnq,
        DWORD dwUserIndex,
        ULONGLONG qwAreas,
        PWCHAR displayText,
        PVOID contextData
    );
#ifdef __cplusplus
}
#endif

BOOL Loaded = FALSE;

VOID (_cdecl *SV_GameSendServerCommand)(INT client, INT type, const PCHAR string) = (VOID(*)(INT,INT,const PCHAR))0x82254940;
BOOL (_cdecl *Dvar_GetBool)(const PCHAR string) = (BOOL(*)(const PCHAR))0x8229EF58;
const PCHAR (_cdecl *Dvar_GetString)(const PCHAR string) = (const PCHAR(*)(const PCHAR))0x8229F118;
FLOAT (_cdecl *Dvar_GetFloat)(const PCHAR string) = (FLOAT(*)(const PCHAR))0x8229F028;
INT (_cdecl *Dvar_GetInt)(const PCHAR string) = (INT(*)(const PCHAR))0x8229EFC0;
DWORD (_cdecl *getClientState)(INT client) = (DWORD(*)(INT))0x821E66A8;
VOID (_cdecl *SV_Cmd_TokenizeString)(const PCHAR string) = (VOID(*)(const PCHAR))0x82225348;
VOID (_cdecl *ClientCommand)(DWORD client) = (VOID(*)(DWORD))0x821DC638;
VOID (_cdecl *SV_Cmd_EndTokenizedString)() = (VOID(*)())0x82225368;

DWORD gEntity(INT client) { return 0x82F03600 + (client * 0x280); }
DWORD gClient(INT client) { return 0x83620380 + (client * 0x97F80); }
DWORD getPlayerState(INT client) { return 0x830CBF80 + (client * 0x3700); }
typedef bool (*Dvar_GetBool_t)(const char *dvarName);



BOOL comparestring(const CHAR *Str1, const CHAR *Str2) {
	INT Result = 0;
	while (!(Result = *(unsigned char*) Str1 - *(unsigned char*) Str2) && *Str2) ++Str1, ++Str2;
	if (Result == 0) return TRUE;
	else return FALSE;
}

struct XenonUserData{
	INT signInState;
	CHAR Name[0x20];
};

int _Host() {
	INT Host;
	CHAR Temp[18][15];
	XenonUserData *xenonUserData = (XenonUserData*)0x838BA820;
	for (INT i = 0; i < 18; i++) {
		strcpy(Temp[i], (PCHAR)getClientState(i) + 0x44);
		if (comparestring(xenonUserData[0].Name, Temp[i]))
			Host = i;
	}
	return Host;
}
void XNotify(XNOTIFYQUEUEUI_TYPE type, LPCWSTR text)
{
    WCHAR buffer[512];
    swprintf(buffer, text);
    XNotifyQueueUI(type, 0xFF, XNOTIFY_SYSTEM, buffer, 0);
}
VOID PlaySound(int clientIndex, const char *Soundname){
	typedef const char * (*G_SoundAliasIndex_t)(const char *name);
	G_SoundAliasIndex_t G_SoundAliasIndex = (G_SoundAliasIndex_t)0x8220D048;
	char buf[50];
	const char *index = G_SoundAliasIndex(Soundname);
	sprintf(buf, "p %i", index);
	SV_GameSendServerCommand(clientIndex,0, (char* const)buf);
}
VOID InitMenu() {
	SV_GameSendServerCommand(-1, 1, "s loc_warnings \"0\" loc_warningsUI \"0\" scr_gameEnded \"1\"");
	for (INT i = 0; i < 18; i++) {
		Client[i].Hud.MenuShader = HudElem_Alloc(i, 0);
		Client[i].Hud.NewsbarShader = HudElem_Alloc(i, 0);
		Client[i].Hud.Scrollbar = HudElem_Alloc(i, 0);
		Client[i].Hud.MenuTitle = HudElem_Alloc(i, 0);
		for (int a = 0; a < 8; a++) { Client[i].Hud.Item[a] = HudElem_Alloc(i, 0); }
		Client[i].Hud.LolWut = HudElem_Alloc(i, 0);
		setShader(Client[i].Hud.MenuShader, "black", 900, 320, 190, 280, 5, 5, 0.1, 255, 255, 255, 160);
		setShader(Client[i].Hud.NewsbarShader, "blue", 350, 695, 1000, 28, 5, 5, 0.1, 255, 255, 255, 160);
		setShader(Client[i].Hud.Scrollbar, "white", 900, 130, 180, 18, 5, 5, 0.2, 0, 195, 255, 180);
		setText(Client[i].Hud.MenuTitle, "LambChops", 6, 0.8, 900, 85, 5, 5, 0.3, 255, 255, 255, 255);
		setText(Client[i].Hud.Item[0], "Main Mods", 3, 1.5, 900, 130, 5, 5, 0.3, 255, 255, 255, 255);
		setText(Client[i].Hud.Item[1], "Player Mods", 3, 1.5, 900, 160, 5, 5, 0.3, 255, 255, 255, 255);
		setText(Client[i].Hud.Item[2], "Account Mods", 3, 1.5, 900, 190, 5, 5, 0.3, 255, 255, 255, 255);
		setText(Client[i].Hud.Item[3], "Weapons Menu", 3, 1.5, 900, 220, 5, 5, 0.3, 255, 255, 255, 255);
		setText(Client[i].Hud.Item[4], "Model Menu", 3, 1.5, 900, 250, 5, 5, 0.3, 255, 255, 255, 255);
		setText(Client[i].Hud.Item[5], "VIP", 3, 1.5, 900, 280, 5, 5, 0.3, 255, 255, 255, 255);
		setText(Client[i].Hud.Item[6], "Co-Host", 3, 1.5, 900, 310, 5, 5, 0.3, 255, 255, 255, 255);
		setText(Client[i].Hud.Item[7], "Host", 3, 1.5, 900, 340, 5, 5, 0.3, 255, 255, 255, 255);
		setShader(Client[i].Hud.LolWut, "ui_scrollbar_thumb", 900, 320, 800, 320, 5, 5, 0.0, 0, 195, 255, 150);
		Client[i].Hud.MenuShader->elem.flags = HUDELEMFLAG_HIDEWHENINMENU | HUDELEMFLAG_HIDEWHENDEAD;
		Client[i].Hud.NewsbarShader->elem.flags = HUDELEMFLAG_HIDEWHENINMENU | HUDELEMFLAG_HIDEWHENDEAD;
		Client[i].Hud.Scrollbar->elem.flags = HUDELEMFLAG_HIDEWHENINMENU | HUDELEMFLAG_HIDEWHENDEAD;
		Client[i].Hud.MenuTitle->elem.flags = HUDELEMFLAG_HIDEWHENINMENU | HUDELEMFLAG_HIDEWHENDEAD;
		for (INT b = 0; b < 8; b++) { Client[i].Hud.Item[b]->elem.flags = HUDELEMFLAG_HIDEWHENINMENU | HUDELEMFLAG_HIDEWHENDEAD; }
		if (i == _Host()) Client[i].Status = Host;
		else Client[i].Status = Unverified;
	}
	Loaded = TRUE;
}


void ElemMove(int ElemAddress, float x, float y, int cli)
{
	*(float *)(ElemAddress + Client[cli].Hud.x) = x;
    *(float *)(ElemAddress + Client[cli].Hud.x) = y;
}
 



VOID Revert(INT client) {
	Client[client].Menu.Scroll = 0;
	Client[client].Menu.Index = 7;
	Client[client].Menu.subMenu = 0;
	Client[client].Hud.MenuTitle->elem.text = G_LocalizedStringIndex("LambChops!");
	Client[client].Hud.Item[0]->elem.text = G_LocalizedStringIndex("Main Mods");
	Client[client].Hud.Item[1]->elem.text = G_LocalizedStringIndex("Player Mods");
	Client[client].Hud.Item[2]->elem.text = G_LocalizedStringIndex("Account Mods");
	Client[client].Hud.Item[3]->elem.text = G_LocalizedStringIndex("Weapons Menu");
	Client[client].Hud.Item[4]->elem.text = G_LocalizedStringIndex("Model Menu");
	Client[client].Hud.Item[5]->elem.text = G_LocalizedStringIndex("VIP");
	Client[client].Hud.Item[6]->elem.text = G_LocalizedStringIndex("Co-Host");
	Client[client].Hud.Item[7]->elem.text = G_LocalizedStringIndex("Host"); 
}

int GB(int Client)
{
	return *(int *)(getClientState(Client) + 0x3438);
}

void DoubleJump(int client)
      {
          
            if (GB((client)) == 40000)
            {
                Sleep(15);
                float Velocity = *(float *)(getClientState(client) + 0x30);
                Velocity += 39;
                *(float *)(getClientState(Host) + 0x30) = Velocity;
            }
 
      }

FLOAT ScrollY(INT client) {
	switch(Client[client].Menu.Scroll) {
	case 0: return 130;
	case 1: return 160;
	case 2: return 190;
	case 3: return 220;
	case 4: return 250;
	case 5: return 280;
	case 6: return 310;
	case 7: return 340;
	case 8: return 370;
	case 9: return 400;
	}
	return 0;
}
void toggleNoclip(void* playerState)
{
    if(playerState)
    //Lrn 2 bitflag
          *(int*)((int)playerState + 0x3420) ^= 1;

}
void SetPulseFX( game_hudelem_s * elem, int speed, int decayStart, int decayDuration )
{
    if ( speed <= 0 )
    {
        printf( "Time (%i) must be greater than 0.\n", speed );
        return;
    }
    else if ( decayStart <= 0 || decayDuration <= 0 )
        return;
 
    int * levelTime = (int*)0x8310A12C;
    elem->elem.fxBirthTime = *levelTime;
    elem->elem.fxLetterTime = speed;
    elem->elem.fxDecayStartTime = decayStart;
    elem->elem.fxDecayDuration = decayDuration;
}
void SelectOpt(INT client, INT option, INT index){
	
	
	
	//WCHAR buffer[512];
	//swprintf(buffer, L"whore");
	//XNotifyQueueUI(XNOTIFYUI_TYPE_ACHIEVEMENT, 0xFF, XNOTIFY_SYSTEM, buffer, 0);
	/*if(i == 1){
		if(cli == 0)
		{
			*(byte*)0x82104093 = 01;//Laser
		}
	}*/
}
void Select(INT client, INT option, INT index) {
	int num;
	if (Client[client].Menu.subMenu == 0){
		switch (index) {
		case 0:

			switch (option)
			{
			case 0:
				Client[client].Menu.Index = 7;
				Client[client].Menu.subMenu = 1;
				Client[client].Menu.Scroll = 0;
				Client[client].Hud.MenuTitle->elem.text = G_LocalizedStringIndex("Main Mods");
				Client[client].Hud.Item[0]->elem.text = G_LocalizedStringIndex("Infinite Ammo");
				Client[client].Hud.Item[1]->elem.text = G_LocalizedStringIndex("Double Jump");
				Client[client].Hud.Item[2]->elem.text = G_LocalizedStringIndex("Wallhack");
				Client[client].Hud.Item[3]->elem.text = G_LocalizedStringIndex("Option 4");
				Client[client].Hud.Item[4]->elem.text = G_LocalizedStringIndex("Option 5");
				Client[client].Hud.Item[5]->elem.text = G_LocalizedStringIndex("Option 6");
				Client[client].Hud.Item[6]->elem.text = G_LocalizedStringIndex("Option 7");
				Client[client].Hud.Item[7]->elem.text = G_LocalizedStringIndex("Option 8");
				num = 1;
				break;
			case 1:
				Client[client].Menu.Index = 7;
				Client[client].Menu.subMenu = 2;
				Client[client].Menu.Scroll = 0;
				Client[client].Hud.MenuTitle->elem.text = G_LocalizedStringIndex("Player Mods");
				Client[client].Hud.Item[0]->elem.text = G_LocalizedStringIndex("Option 1");
				Client[client].Hud.Item[1]->elem.text = G_LocalizedStringIndex("Option 2");
				Client[client].Hud.Item[2]->elem.text = G_LocalizedStringIndex("Option 3");
				Client[client].Hud.Item[3]->elem.text = G_LocalizedStringIndex("Option 4");
				Client[client].Hud.Item[4]->elem.text = G_LocalizedStringIndex("Option 5");
				Client[client].Hud.Item[5]->elem.text = G_LocalizedStringIndex("Option 6");
				Client[client].Hud.Item[6]->elem.text = G_LocalizedStringIndex("Option 7");
				Client[client].Hud.Item[7]->elem.text = G_LocalizedStringIndex("Option 8");
				num = 2;
				break;
			case 2:
				Client[client].Menu.Index = 7;
				Client[client].Menu.subMenu = 3;
				Client[client].Menu.Scroll = 0;
				Client[client].Hud.MenuTitle->elem.text = G_LocalizedStringIndex("Account");
				Client[client].Hud.Item[0]->elem.text = G_LocalizedStringIndex("Option 1");
				Client[client].Hud.Item[1]->elem.text = G_LocalizedStringIndex("Option 2");
				Client[client].Hud.Item[2]->elem.text = G_LocalizedStringIndex("Option 3");
				Client[client].Hud.Item[3]->elem.text = G_LocalizedStringIndex("Option 4");
				Client[client].Hud.Item[4]->elem.text = G_LocalizedStringIndex("Option 5");
				Client[client].Hud.Item[5]->elem.text = G_LocalizedStringIndex("Option 6");
				Client[client].Hud.Item[6]->elem.text = G_LocalizedStringIndex("Option 7");
				Client[client].Hud.Item[7]->elem.text = G_LocalizedStringIndex("Option 8");
				num = 3;
				break;
			case 3:
				Client[client].Menu.Index = 7;
				Client[client].Menu.subMenu = 4;
				Client[client].Menu.Scroll = 0;
				Client[client].Hud.MenuTitle->elem.text = G_LocalizedStringIndex("Weapons");
				Client[client].Hud.Item[0]->elem.text = G_LocalizedStringIndex("Option 1");
				Client[client].Hud.Item[1]->elem.text = G_LocalizedStringIndex("Option 2");
				Client[client].Hud.Item[2]->elem.text = G_LocalizedStringIndex("Option 3");
				Client[client].Hud.Item[3]->elem.text = G_LocalizedStringIndex("Option 4");
				Client[client].Hud.Item[4]->elem.text = G_LocalizedStringIndex("Option 5");
				Client[client].Hud.Item[5]->elem.text = G_LocalizedStringIndex("Option 6");
				Client[client].Hud.Item[6]->elem.text = G_LocalizedStringIndex("Option 7");
				Client[client].Hud.Item[7]->elem.text = G_LocalizedStringIndex("Option 8");
				num = 4;
				break;
			case 4:
				Client[client].Menu.Index = 7;
				Client[client].Menu.subMenu = 5;
				Client[client].Menu.Scroll = 0;
				Client[client].Hud.MenuTitle->elem.text = G_LocalizedStringIndex("Model Menu");
				Client[client].Hud.Item[0]->elem.text = G_LocalizedStringIndex("Option 1");
				Client[client].Hud.Item[1]->elem.text = G_LocalizedStringIndex("Option 2");
				Client[client].Hud.Item[2]->elem.text = G_LocalizedStringIndex("Option 3");
				Client[client].Hud.Item[3]->elem.text = G_LocalizedStringIndex("Option 4");
				Client[client].Hud.Item[4]->elem.text = G_LocalizedStringIndex("Option 5");
				Client[client].Hud.Item[5]->elem.text = G_LocalizedStringIndex("Option 6");
				Client[client].Hud.Item[6]->elem.text = G_LocalizedStringIndex("Option 7");
				Client[client].Hud.Item[7]->elem.text = G_LocalizedStringIndex("Option 8");
				num = 5;
				break;
			case 5:
				Client[client].Menu.Index = 7;
				Client[client].Menu.subMenu = 6;
				Client[client].Menu.Scroll = 0;
				Client[client].Hud.MenuTitle->elem.text = G_LocalizedStringIndex("VIP");
				Client[client].Hud.Item[0]->elem.text = G_LocalizedStringIndex("Option 1");
				Client[client].Hud.Item[1]->elem.text = G_LocalizedStringIndex("Option 2");
				Client[client].Hud.Item[2]->elem.text = G_LocalizedStringIndex("Option 3");
				Client[client].Hud.Item[3]->elem.text = G_LocalizedStringIndex("Option 4");
				Client[client].Hud.Item[4]->elem.text = G_LocalizedStringIndex("Option 5");
				Client[client].Hud.Item[5]->elem.text = G_LocalizedStringIndex("Option 6");
				Client[client].Hud.Item[6]->elem.text = G_LocalizedStringIndex("Option 7");
				Client[client].Hud.Item[7]->elem.text = G_LocalizedStringIndex("Option 8");
				num = 6;
				break;
			case 6:
				Client[client].Menu.Index = 7;
				Client[client].Menu.subMenu = 7;
				Client[client].Menu.Scroll = 0;
				Client[client].Hud.MenuTitle->elem.text = G_LocalizedStringIndex("CO-Host");
				Client[client].Hud.Item[0]->elem.text = G_LocalizedStringIndex("Option 1");
				Client[client].Hud.Item[1]->elem.text = G_LocalizedStringIndex("Option 2");
				Client[client].Hud.Item[2]->elem.text = G_LocalizedStringIndex("Option 3");
				Client[client].Hud.Item[3]->elem.text = G_LocalizedStringIndex("Option 4");
		Client[client].Hud.Item[4]->elem.text = G_LocalizedStringIndex("Option 5");
				Client[client].Hud.Item[5]->elem.text = G_LocalizedStringIndex("Option 6");
				Client[client].Hud.Item[6]->elem.text = G_LocalizedStringIndex("Option 7");
				Client[client].Hud.Item[7]->elem.text = G_LocalizedStringIndex("Option 8");
				num = 7;
				break;
			case 7:
				Client[client].Menu.Index = 7;
				Client[client].Menu.subMenu = 8;
				Client[client].Menu.Scroll = 0;
				Client[client].Hud.MenuTitle->elem.text = G_LocalizedStringIndex("Host");
				Client[client].Hud.Item[0]->elem.text = G_LocalizedStringIndex("Player Verification");
				Client[client].Hud.Item[1]->elem.text = G_LocalizedStringIndex("Lobby Settings");
				Client[client].Hud.Item[2]->elem.text = G_LocalizedStringIndex("");
				Client[client].Hud.Item[3]->elem.text = G_LocalizedStringIndex("Option 4");
				Client[client].Hud.Item[4]->elem.text = G_LocalizedStringIndex("Option 5");
				Client[client].Hud.Item[5]->elem.text = G_LocalizedStringIndex("Option 6");
				Client[client].Hud.Item[6]->elem.text = G_LocalizedStringIndex("Option 7");
				Client[client].Hud.Item[7]->elem.text = G_LocalizedStringIndex("Option 8");
				num = 8;
				break;
			}break;
		}
	}
	else
	{
		switch(Client[client].Menu.subMenu){
			case 1:
				switch(Client[client].Menu.Scroll){
					case 0:
						*(byte*)0x82104093 = 01;
				}
		}
	}
}


VOID PatchInJump(PDWORD Address, DWORD Destination, BOOL Linked) {
	Address[0] = 0x3D600000 + ((Destination >> 16) & 0xFFFF);
	if (Destination & 0x8000) Address[0] += 1;
	Address[1] = 0x396B0000 + (Destination & 0xFFFF);
	Address[2] = 0x7D6903A6;
	Address[3] = Linked ? 0x4E800421 : 0x4E800420;
}

