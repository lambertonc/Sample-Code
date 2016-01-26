#include "stdafx.h"
#include "functions.h"
#include <stdio.h>

int selection;
VOID SV_ExecuteClientCommandHook(DWORD cl, const PCHAR s, BOOL clientOK) {
	DWORD clientIndex = (cl - *(PINT)0x83623B98) / 0x97F80;
	SV_Cmd_TokenizeString(s);
	ClientCommand(clientIndex);
	SV_Cmd_EndTokenizedString();
	INT i = (INT)clientIndex;
	
	if (Client[i].Status > Unverified) {
		if (strstr(s, DPAD_LEFT)) {
			if (!Client[i].Menu.Open) {
				PlaySound(-1,"mp_killstreak_nuclearstrike");
				Revert(i);
				Client[i].Menu.Index = 7;
				Client[i].Menu.Scroll = 0;
				Client[i].Menu.subMenu = 0;
				MoveOverTime(Client[i].Hud.LolWut, 220, 480, 320);
				MoveOverTime(Client[i].Hud.MenuShader, 200, 480, 320);
				MoveOverTime(Client[i].Hud.Scrollbar, 200, 480, 130);
				MoveOverTime(Client[i].Hud.MenuTitle, 220, 480, 85);
				MoveOverTime(Client[i].Hud.Item[0], 220, 480, 130);
				if (Client[i].Hud.MenuTitle->elem.x == 480) { FadeOverTime(Client[i].Hud.MenuTitle, 280, 255, 255, 255, 255); }
				for (INT a = 1; a < 8; a++) { INT b = a - 1; Client[i].Hud.Item[a]->elem.y = Client[i].Hud.Item[b]->elem.y + 30; }
				for (INT c = 1; c < 8; c++) { MoveOverTime(Client[i].Hud.Item[c], 220, 480, Client[i].Hud.Item[c]->elem.y); }
				*(PINT)(getPlayerState(i) + 0x3420) = 0x04;
				Client[i].Menu.Open = TRUE;
			}else{
				Client[i].Menu.Index = 0;
				Client[i].Menu.Scroll = 0;
				Client[i].Menu.subMenu = 0;
				MoveOverTime(Client[i].Hud.LolWut, 220, 900, 320);
				MoveOverTime(Client[i].Hud.MenuShader, 200, 900, 320);
				MoveOverTime(Client[i].Hud.Scrollbar, 200, 900, 130);
				MoveOverTime(Client[i].Hud.MenuTitle, 220, 900, 85);
				MoveOverTime(Client[i].Hud.Item[0], 220, 900, 130);
				if (Client[i].Hud.MenuTitle->elem.x == 480) { FadeOverTime(Client[i].Hud.MenuTitle, 280, 255, 255, 255, 0); }
				for (INT a = 1; a < 8; a++) { INT b = a - 1; Client[i].Hud.Item[a]->elem.y = Client[i].Hud.Item[b]->elem.y + 30; }
				for (INT c = 1; c < 8; c++) { MoveOverTime(Client[i].Hud.Item[c], 220, 900, Client[i].Hud.Item[c]->elem.y); }
				*(PINT)(getPlayerState(i) + 0x3420) = 0x00;
				Client[i].Menu.Open = FALSE;
			}
		}
		if (strstr(s, DPAD_UP) && Client[i].Menu.Open) {
			if (Client[i].Menu.Scroll == 0) Client[i].Menu.Scroll = Client[i].Menu.Index;
			else Client[i].Menu.Scroll--;
			MoveOverTime(Client[i].Hud.Scrollbar, 180, 480, ScrollY(i));
		}
		else if (strstr(s, DPAD_DOWN) && Client[i].Menu.Open) {
			if (Client[i].Menu.Scroll >= Client[i].Menu.Index) Client[i].Menu.Scroll = 0;
			else Client[i].Menu.Scroll++;
			MoveOverTime(Client[i].Hud.Scrollbar, 180, 480, ScrollY(i));
		}
		if (strstr(s, BUTTON_X) && Client[i].Menu.Open) {
			if(selection == 0)
				Select(i, Client[i].Menu.Scroll, Client[i].Menu.subMenu);
			Client[i].Menu.Scroll = 0;
		}
		if(strstr(s, BUTTON_X) && selection > 0){
			SelectOpt(selection, Client[i].Menu.Scroll);
		}
		if (strstr(s, "n 28") && Client[i].Menu.Open) {
			selection = 0;
			Revert(i);
		}
	}
}

const char *GetPlayerName(int client)
{
   DWORD player;
   player = getPlayerState(client);
   player += 0x44;
   return (char*)player;
}

DWORD XamInputGetStateHook(DWORD UserIndex, DWORD Flags, PXINPUT_STATE State) {
	if (Dvar_GetBool("cl_ingame")) {
		if (!Loaded) {
			*(PBOOL)(0x83109D9C) = TRUE;
			InitMenu();
		}
	}
	return XInputGetStateEx(UserIndex, Flags, State);
}

BOOL APIENTRY DllMain(HANDLE Module, DWORD Reason, LPVOID Reserved) {
	switch (Reason) {
	case DLL_PROCESS_ATTACH:
		*(PDWORD)(0x82168FB4) = 0x60000000;
		*(PDWORD)(0x8216907C) = 0x60000000;
		*(PDWORD)(0x821690F4) = 0x60000000;
		*(PDWORD)(0x821D6E60) = 0x60000000;
		PatchInJump((PDWORD)0x823B64A4, (DWORD)XamInputGetStateHook, FALSE);
		PatchInJump((PDWORD)0x822531C8, (DWORD)SV_ExecuteClientCommandHook, FALSE);
		Sleep(45);
		break;
	}
	return TRUE;
}