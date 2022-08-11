#include <Ext/Techno/Body.h>

// ROF
DEFINE_HOOK(0x6FD1F1, TechnoClass_GetROF, 0x5)
{
	GET(TechnoClass*, pThis, ESI);
	GET(int, iROF, EBP);

	TechnoExt::ExtData* pExt = TechnoExt::ExtMap.Find(pThis);
	int iROFBuff = 0;

	if (pExt->BuildingROFFix > 0)
	{
		iROF = pExt->BuildingROFFix;
		pExt->BuildingROFFix = -1;
	}

	double dblMultiplier = 1.0;

	for (auto& pAE: pExt->AttachEffects)
	{
		dblMultiplier *= pAE->Type->ROF_Multiplier;
		iROFBuff += pAE->Type->ROF;
	}

	iROF = Game::F2I(iROF * dblMultiplier);
	iROF += iROFBuff;
	iROF = std::max(iROF, 1);

	R->EBP(iROF);

	return 0;
}

// FirePower
DEFINE_HOOK(0x46B050, BulletTypeClass_CreateBullet, 0x6)
{
	GET_STACK(TechnoClass*, pOwner, 0x4);
	REF_STACK(int, iDamage, 0x8);

	TechnoExt::ExtData* pOwnerExt = TechnoExt::ExtMap.Find(pOwner);

	if (pOwner == nullptr || pOwnerExt == nullptr)
		return 0;

	double dblMultiplier = 1.0;
	int iDamageBuff = 0;
	
	for (auto& pAE : pOwnerExt->AttachEffects)
	{
		dblMultiplier *= pAE->Type->FirePower_Multiplier;
		iDamageBuff += pAE->Type->FirePower;
	}

	iDamage = Game::F2I(iDamage * dblMultiplier);
	iDamage += iDamageBuff;

	return 0;
}

// Speed
DEFINE_HOOK(0x4DB221, FootClass_GetCurrentSpeed, 0x5)
{
	GET(FootClass*, pThis, ESI);
	GET(int, iSpeed, EDI);

	TechnoExt::ExtData* pExt = TechnoExt::ExtMap.Find(pThis);

	double dblMultiplier = 1.0;
	int iSpeedBuff = 0;

	for (auto& pAE : pExt->AttachEffects)
	{
		dblMultiplier *= pAE->Type->Speed_Multiplier;
		iSpeedBuff += pAE->Type->Speed;
	}

	iSpeedBuff = iSpeedBuff * 256 / 100;
	iSpeed = Game::F2I(iSpeed * dblMultiplier);
	iSpeed += iSpeedBuff;
	iSpeed = std::max(0, iSpeed);
	iSpeed = std::min(256, iSpeed);
	R->EDI(iSpeed);

	if (pThis->WhatAmI() != AbstractType::Unit)
		return 0x4DB23E;

	return 0x4DB226;
}

// DisableWeapon
DEFINE_HOOK(0x6FC0B0, TechnoClass_GetFireError, 0x8)
{
	GET(TechnoClass*, pThis, ECX);

	TechnoExt::ExtData* pExt = TechnoExt::ExtMap.Find(pThis);

	for (auto& pAE : pExt->AttachEffects)
	{
		if (pAE->Type->DisableWeapon)
		{
			R->EAX(FireError::CANT);
			return 0x6FC0EB;
		}
	}

	return 0;
}
