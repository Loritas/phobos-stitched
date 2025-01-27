#include "PhobosGlobal.h"
#include <Utilities/TemplateDef.h>

#include <JumpjetLocomotionClass.h>

#include <Ext/SWType/Body.h>
#include <Ext/House/Body.h>

//GlobalObject initial
PhobosGlobal PhobosGlobal::GlobalObject;

void PhobosGlobal::Clear()
{
	GlobalObject.Reset();
}

void PhobosGlobal::Reset()
{
	Techno_HugeBar.clear();
	RandomTriggerPool.clear();
	GenericStand = nullptr;
	MultipleSWFirer_Queued.clear();
	TriggerType_HouseMultiplayer.clear();
	FallUnit_Queued.clear();
}

void PhobosGlobal::PointerGotInvalid(void* ptr, bool removed)
{
	GlobalObject.InvalidatePointer(ptr, removed);
}

void PhobosGlobal::InvalidatePointer(void* ptr, bool removed)
{
	if (removed)
	{
		for (auto it = Techno_HugeBar.begin(); it != Techno_HugeBar.end(); ++it)
		{
			if (it->second == ptr)
			{
				Techno_HugeBar.erase(it);

				break;
			}
		}
	}
}

void PhobosGlobal::CheckSuperQueued()
{
	for (auto& item : MultipleSWFirer_Queued)
	{
		if (item.Timer.Completed())
		{
			SuperClass* pSuper = item.Super;
			const auto pSWTypeExt = SWTypeExt::ExtMap.Find(item.Super->Type);

			if (!item.RealLaunch || pSuper->Granted && pSuper->IsCharged && pSuper->Owner->CanTransactMoney(pSWTypeExt->Money_Amount))
			{
				if (!pSWTypeExt->HasInhibitor(pSuper->Owner, item.MapCoords))
				{
					pSuper->SetReadiness(true);
					pSuper->Launch(item.MapCoords, item.IsPlayer);
					pSuper->Reset();
				}
			}
		}
	}

	MultipleSWFirer_Queued.erase
	(
		std::remove_if(MultipleSWFirer_Queued.begin(), MultipleSWFirer_Queued.end(),
			[](QueuedSW& item)
			{
				return item.Timer.Expired();
			}),
		MultipleSWFirer_Queued.end()
	);
}

void PhobosGlobal::CheckFallUnitQueued()
{
	for (auto& item : FallUnit_Queued)
	{
		if (item.Timer.Completed())
		{
			SWTypeExt::ExtData* pSWTypeExt = SWTypeExt::ExtMap.Find(item.pSW->Type);
			HouseClass* pHouse = item.pSW->Owner;
			TechnoTypeClass* pTechnoType = pSWTypeExt->UnitFall_Types[item.I];

			auto newOwner = HouseExt::GetHouseKind(pSWTypeExt->UnitFall_Owners[item.I], true, pHouse, pHouse, pHouse);

			HouseClass* decidedOwner = newOwner && !newOwner->Defeated
				? newOwner : HouseClass::FindCivilianSide();

			bool allowBridges = pTechnoType->SpeedType != SpeedType::Float;
			auto pCell = MapClass::Instance->TryGetCellAt(item.Cell);
			CoordStruct location = pCell->GetCoords();

			if (pSWTypeExt->UnitFall_RandomInRange)
			{
				int range = static_cast<int>(item.pSW->Type->Range * 256);
				double random = ScenarioClass::Instance()->Random.RandomRanged(0, range);
				double theta = ScenarioClass::Instance()->Random.RandomDouble() * Math::TwoPi;

				CoordStruct offset
				{
					static_cast<int>(random * Math::cos(theta)),
					static_cast<int>(random * Math::sin(theta)),
					0
				};
				location += offset;

				auto NewCell = CellClass::Coord2Cell(location);
				pCell = MapClass::Instance->TryGetCellAt(NewCell);
			}

			location.Z = pSWTypeExt->UnitFall_Heights[item.I] > 0 ? pSWTypeExt->UnitFall_Heights[item.I] : 0;

			if (pCell && allowBridges)
				location = pCell->GetCoordsWithBridge();

			auto nCell = MapClass::Instance->NearByLocation(CellClass::Coord2Cell(location),
					pTechnoType->SpeedType, -1, pTechnoType->MovementZone, false, 1, 1, false,
					false, false, allowBridges, CellStruct::Empty, false, false);

			pCell = MapClass::Instance->TryGetCellAt(nCell);

			if (pCell && allowBridges)
				location = pCell->GetCoordsWithBridge();

			location.Z = pSWTypeExt->UnitFall_Heights[item.I];

			if (auto pTechno = static_cast<FootClass*>(pTechnoType->CreateObject(decidedOwner)))
			{
				bool success = false;

				auto aFacing = pSWTypeExt->UnitFall_RandomFacings[item.I]
					? static_cast<unsigned short>(ScenarioClass::Instance->Random.RandomRanged(0, 255)) : pSWTypeExt->UnitFall_Facings[item.I];

				if (pCell && allowBridges)
					pTechno->OnBridge = pCell->ContainsBridge();

				BuildingClass* pBuilding = pCell ? pCell->GetBuilding() : MapClass::Instance->TryGetCellAt(location)->GetBuilding();

				if (!pBuilding)
				{
					++Unsorted::IKnowWhatImDoing;
					success = pTechno->Unlimbo(location, static_cast<DirType>(aFacing));
					--Unsorted::IKnowWhatImDoing;
				}
				else
				{
					success = pTechno->Unlimbo(location, static_cast<DirType>(aFacing));
				}

				if (success)
				{
					if (!pTechno->InLimbo)
					{
						if (pTechno->IsInAir())
						{
							const auto pTechnoExt = TechnoExt::ExtMap.Find(pTechno);
							if (auto const pJJLoco = locomotion_cast<JumpjetLocomotionClass*>(pTechno->Locomotor))
							{
								if (!pSWTypeExt->UnitFall_AlwaysFalls[item.I])
								{
									pTechno->IsFallingDown = false;
									pTechnoExt->WasFallenDown = false;
									pTechnoExt->CurrtenFallRate = 0;
									pTechno->FallRate = pTechnoExt->CurrtenFallRate;

									pJJLoco->LocomotionFacing.SetCurrent(DirStruct(static_cast<DirType>(aFacing)));

									if (pTechnoType->BalloonHover)
									{
										// Makes the jumpjet think it is hovering without actually moving.
										pJJLoco->State = JumpjetLocomotionClass::State::Hovering;
										pJJLoco->IsMoving = true;
										pJJLoco->DestinationCoords = location;
										pJJLoco->CurrentHeight = pTechnoType->JumpjetHeight;
									}
									else
									{
										// Order non-BalloonHover jumpjets to land.
										pJJLoco->Move_To(location);
									}

									pTechnoExt->UnitFallWeapon = nullptr;
									pTechnoExt->UnitFallDestory = false;
									pTechnoExt->UnitFallDestoryHeight = -1;
								}
								else
								{
									if (pSWTypeExt->UnitFall_UseParachutes[item.I])
										TechnoExt::FallenDown(pTechno);
									else
									{
										pTechno->IsFallingDown = true;
										pTechnoExt->WasFallenDown = true;
									}

									pTechnoExt->UnitFallWeapon = pSWTypeExt->UnitFall_Weapons[item.I];
									pTechnoExt->UnitFallDestory = pSWTypeExt->UnitFall_Destorys[item.I];
									pTechnoExt->UnitFallDestoryHeight = pSWTypeExt->UnitFall_DestoryHeights[item.I];
								}
							}
							else
							{
								if (pSWTypeExt->UnitFall_UseParachutes[item.I])
									TechnoExt::FallenDown(pTechno);
								else
								{
									pTechno->IsFallingDown = true;
									pTechnoExt->WasFallenDown = true;
								}

								pTechnoExt->UnitFallWeapon = pSWTypeExt->UnitFall_Weapons[item.I];
								pTechnoExt->UnitFallDestory = pSWTypeExt->UnitFall_Destorys[item.I];
								pTechnoExt->UnitFallDestoryHeight = pSWTypeExt->UnitFall_DestoryHeights[item.I];
							}
						}
						pTechno->QueueMission(pSWTypeExt->UnitFall_Missions[item.I], false);

						if (pSWTypeExt->UnitFall_Healths[item.I] > 0)
							pTechno->Health = pSWTypeExt->UnitFall_Healths[item.I];

						if (pSWTypeExt->UnitFall_Veterancys[item.I] > 0)
						{
							VeterancyStruct* vstruct = &pTechno->Veterancy;
							vstruct->Add(pSWTypeExt->UnitFall_Veterancys[item.I]);
							if (vstruct->IsElite())
								vstruct->SetElite();
						}

						if (pSWTypeExt->UnitFall_Anims[item.I] != nullptr)
						{
							auto pAnim = GameCreate<AnimClass>(pSWTypeExt->UnitFall_Anims[item.I], pTechno->Location);
							pAnim->Owner = pTechno->Owner;
						}
					}

					if (!decidedOwner->Type->MultiplayPassive)
						decidedOwner->RecheckTechTree = true;
				}
				else
				{
					if (pTechno)
						pTechno->UnInit();
				}
			}
		}
	}

	FallUnit_Queued.erase
	(
		std::remove_if(FallUnit_Queued.begin(), FallUnit_Queued.end(),
			[](QueuedFall& item)
			{
				return item.Timer.Expired();
			}),
		FallUnit_Queued.end()
	);
}

//Save/Load
#pragma region save/load

template <typename T>
bool PhobosGlobal::Serialize(T& stm)
{
	return stm
		.Process(this->Techno_HugeBar)
		.Process(this->RandomTriggerPool)
		.Process(this->GenericStand)
		.Process(this->MultipleSWFirer_Queued)
		.Process(this->TriggerType_HouseMultiplayer)
		.Process(this->FallUnit_Queued)
		.Success();
}

bool PhobosGlobal::Save(PhobosStreamWriter& stm)
{
	return Serialize(stm);
}

bool PhobosGlobal::Load(PhobosStreamReader& stm)
{
	return Serialize(stm);
}

bool PhobosGlobal::SaveGlobals(PhobosStreamWriter& stm)
{
	GlobalObject.Save(stm);
	return stm.Success();
}

bool PhobosGlobal::LoadGlobals(PhobosStreamReader& stm)
{
	GlobalObject.Load(stm);
	return stm.Success();
}

#pragma endregion save/load

TechnoClass* PhobosGlobal::GetGenericStand()
{
	if (GenericStand != nullptr && TechnoExt::IsReallyAlive(GenericStand))
		return GenericStand;

	HouseClass* pHouse = HouseClass::FindNeutral();

	if (pHouse == nullptr)
		pHouse = HouseClass::FindSpecial();

	if (pHouse == nullptr && HouseClass::Array->Count > 0)
		pHouse = HouseClass::Array->GetItem(0);

	if (pHouse == nullptr)
		Debug::FatalErrorAndExit("House is empty!\n");

	if (UnitTypeClass::Array->Count > 0)
	{
		GenericStand = abstract_cast<TechnoClass*>(UnitTypeClass::Array->GetItem(0)->CreateObject(pHouse));
	}
	else
	{
		Debug::Log("[Warning] UnitTypes Is Empty\n");
		if (InfantryTypeClass::Array->Count > 0)
		{
			GenericStand = abstract_cast<TechnoClass*>(InfantryTypeClass::Array->GetItem(0)->CreateObject(pHouse));
		}
		else
		{
			Debug::Log("[Warning] InfantryTypes Is Empty\n");
			if (BuildingTypeClass::Array->Count > 0)
			{
				GenericStand = abstract_cast<TechnoClass*>(BuildingTypeClass::Array->GetItem(0)->CreateObject(pHouse));
			}
			else
			{
				Debug::Log("[Warning] BuildingTypes Is Empty\n");
				Debug::FatalErrorAndExit("[Error] Generic Stand Create Fail\n");
			}
		}
	}

	GenericStand->Limbo();

	return GenericStand;
}
