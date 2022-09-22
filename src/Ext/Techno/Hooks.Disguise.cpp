#include "Body.h"


DEFINE_HOOK_AGAIN(0x522790, TechnoClass_DefaultDisguise, 0x6) // InfantryClass_SetDisguise_DefaultDisguise
DEFINE_HOOK(0x6F421C, TechnoClass_DefaultDisguise, 0x6) // TechnoClass_DefaultDisguise
{
	GET(TechnoClass*, pThis, ESI);

	enum { SetDisguise = 0x5227BF, DefaultDisguise = 0x6F4277 };

	if (auto const pExt = TechnoTypeExt::ExtMap.Find(pThis->GetTechnoType()))
	{
		if (pExt->DefaultDisguise.isset())
		{
			pThis->Disguise = pExt->DefaultDisguise;
			pThis->Disguised = true;
			return R->Origin() == 0x522790 ? SetDisguise : DefaultDisguise;
		}
	}

	pThis->Disguised = false;

	return 0;
}

#define CAN_BLINK_DISGUISE(pTechno) \
RulesExt::Global()->ShowAllyDisguiseBlinking && (HouseClass::IsCurrentPlayerObserver() || pTechno->Owner->IsAlliedWith(HouseClass::CurrentPlayer))

DEFINE_HOOK(0x70EE53, TechnoClass_IsClearlyVisibleTo_BlinkAllyDisguise1, 0xA)
{
	enum { SkipGameCode = 0x70EE6A, Return = 0x70EEEC };

	GET(TechnoClass*, pThis, ESI);
	GET(int, accum, EAX);

	if (CAN_BLINK_DISGUISE(pThis))
		return SkipGameCode;
	else if (accum && !pThis->Owner->IsControlledByCurrentPlayer())
		return Return;

	return SkipGameCode;
}

DEFINE_HOOK(0x70EE6A, TechnoClass_IsClearlyVisibleTo_BlinkAllyDisguise2, 0x6)
{
	enum { SkipCheck = 0x70EE79 };

	GET(TechnoClass*, pThis, ESI);

	if (CAN_BLINK_DISGUISE(pThis))
		return SkipCheck;

	return 0;
}

DEFINE_HOOK(0x7062F5, TechnoClass_DrawObject_BlinkAllyDisguise, 0x6)
{
	enum { SkipCheck = 0x706304 };

	GET(TechnoClass*, pThis, ESI);

	if (CAN_BLINK_DISGUISE(pThis))
		return SkipCheck;

	return 0;
}

DEFINE_HOOK(0x70EDAD, TechnoClass_DisguiseBlitFlags_BlinkAllyDisguise, 0x6)
{
	enum { SkipCheck = 0x70EDBC };

	GET(TechnoClass*, pThis, EDI);

	if (CAN_BLINK_DISGUISE(pThis))
		return SkipCheck;

	return 0;
}

DEFINE_HOOK(0x7060A9, TechnoClass_DrawObject_DisguisePalette, 0x6)
{
	enum { SkipGameCode = 0x7060CA };

	GET(TechnoClass*, pThis, ESI);

	LightConvertClass* convert = nullptr;

	auto const pType = pThis->IsDisguised() ? TechnoTypeExt::GetTechnoType(pThis->Disguise) : nullptr;
	int colorIndex = pThis->GetDisguiseHouse(true)->ColorSchemeIndex;

	if (pType && pType->Palette && pType->Palette->Count > 0)
		convert = pType->Palette->GetItem(colorIndex)->LightConvert;
	else
		convert = ColorScheme::Array->GetItem(colorIndex)->LightConvert;

	R->EBX(convert);

	return SkipGameCode;
}
