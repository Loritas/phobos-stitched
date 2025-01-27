#include "Body.h"

#include <Ext/TAction/Body.h>

#include <HouseClass.h>
#include <UI.h>

DEFINE_HOOK(0x6851AC, LoadGame_Initialize_IonStormClass, 0x5)
{
	auto swap_data = []()
	{
		std::swap(ScenarioExt::Global()->DefaultAmbientOriginal, ScenarioClass::Instance->AmbientOriginal);
		std::swap(ScenarioExt::Global()->DefaultAmbientCurrent, ScenarioClass::Instance->AmbientCurrent);
		std::swap(ScenarioExt::Global()->DefaultAmbientTarget, ScenarioClass::Instance->AmbientTarget);
		std::swap(ScenarioExt::Global()->DefaultNormalLighting, ScenarioClass::Instance->NormalLighting);
	};

	swap_data();

	MapClass::Instance->CellIteratorReset();
	for (auto pCell = MapClass::Instance->CellIteratorNext(); pCell; pCell = MapClass::Instance->CellIteratorNext())
	{
		if (pCell->LightConvert)
			GameDelete(pCell->LightConvert);

		pCell->InitLightConvert();
	}

	swap_data();


	for (auto& pLightConvert : *LightConvertClass::Array)
		pLightConvert->UpdateColors(
			ScenarioExt::Global()->CurrentTint_Tiles.Red * 10,
			ScenarioExt::Global()->CurrentTint_Tiles.Green * 10,
			ScenarioExt::Global()->CurrentTint_Tiles.Blue * 10,
			false);

	if (ScenarioExt::Global()->CurrentTint_Schemes != TintStruct { -1,-1,-1 })
	{
		for (auto& pScheme : *ColorScheme::Array)
			pScheme->LightConvert->UpdateColors(
				ScenarioExt::Global()->CurrentTint_Schemes.Red * 10,
				ScenarioExt::Global()->CurrentTint_Schemes.Green * 10,
				ScenarioExt::Global()->CurrentTint_Schemes.Blue * 10,
				false);
	}

	if (ScenarioExt::Global()->CurrentTint_Hashes != TintStruct { -1,-1,-1 })
	{
		ScenarioClass::UpdateHashPalLighting(
			ScenarioExt::Global()->CurrentTint_Hashes.Red * 10,
			ScenarioExt::Global()->CurrentTint_Hashes.Green * 10,
			ScenarioExt::Global()->CurrentTint_Hashes.Blue * 10,
			false);
	}

	TActionExt::RecreateLightSources();
	ScenarioClass::UpdateCellLighting();

	HouseClass::CurrentPlayer->RecheckRadar = true;

	return 0x6851B1;
}

// score options
// score music for single player missions
DEFINE_HOOK(0x6C924F, ScoreDialog_Handle_ScoreThemeA, 0x5)
{
	GET(char*, pTitle, ECX);
	GET(char*, pMessage, ESI);
	CSFText& Title = ScenarioExt::Global()->ParTitle;
	CSFText& Message = ScenarioExt::Global()->ParMessage;

	strcpy(pTitle, Title.Label);
	strcpy(pMessage, Message.Label);

	return 0;
}

DEFINE_HOOK(0x6C935C, ScoreDialog_Handle_ScoreThemeB, 0x5)
{
	REF_STACK(char*, pTheme, 0x0);

	auto& Theme = ScenarioExt::Global()->ScoreCampaignTheme;

	if (Theme.isset())
		strcpy(pTheme, Theme.Get().data());

	return 0;
}

DEFINE_HOOK(0x5AE192, SelectNextMission, 0x6)
{
	if (ScenarioExt::Global()->NextMission.isset())
		R->EAX(ScenarioExt::Global()->NextMission.Get().data());

	return 0;
}

DEFINE_HOOK(0x65F633, ScenarioClass_SetBriefing, 0x6)
{
	//	GET(wchar_t*, pText, EAX);

	R->EAX(ScenarioClass::Instance->Briefing);

	return 0;
}

DEFINE_HOOK(0x559E40, ScenarioClass_SaveGame_UI, 0x4)
{
	if (!ScenarioExt::Global()->CanSaveGame)
	{
		R->AL(false);
	}
	else
	{
		REF_STACK(LPCCH, pFileName, 0x4);
		REF_STACK(wchar_t*, pName, 0x8);

		CSFText pText;
		pText = "TXT_SAVING_GAME";

		const auto pHwnd = (HWND)UI::sub_623230((LPARAM)pText.Text, 0, 0);

		if (pHwnd)
			UI::FocusOnWindow(pHwnd);

		if (const auto savegame = ScenarioClass::Instance->SaveGame(pFileName, pName, 0))
			R->AL(savegame);

		if (pHwnd)
			UI::EndDialog(pHwnd);
	}
	
	return 0x559EA6;
}
