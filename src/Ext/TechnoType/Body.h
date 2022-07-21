#pragma once
#include <TechnoTypeClass.h>

#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>
#include <Utilities/Enum.h>

#include <New/Type/ShieldTypeClass.h>
#include <New/Type/LaserTrailTypeClass.h>
#include <New/Type/AttachmentTypeClass.h>
#include <New/Type/DigitalDisplayTypeClass.h>
#include <New/Type/IonCannonTypeClass.h>
#include <New/Type/GScreenAnimTypeClass.h>
#include <New/Type/AttachEffectTypeClass.h>

class Matrix3D;
class ParticleSystemTypeClass;
class TechnoTypeExt
{
public:
	using base_type = TechnoTypeClass;

	class ExtData final : public Extension<TechnoTypeClass>
	{
	public:
		Valueable<bool> HealthBar_Hide;
		Valueable<CSFText> UIDescription;
		Valueable<bool> LowSelectionPriority;
		PhobosFixedString<0x20> GroupAs;
		Valueable<int> RadarJamRadius;
		Nullable<int> InhibitorRange;
		Valueable<Leptons> MindControlRangeLimit;
		Valueable<bool> Interceptor;
		Nullable<bool> Interceptor_Rookie;
		Nullable<bool> Interceptor_Veteran;
		Nullable<bool> Interceptor_Elite;
		Valueable<AffectedHouse> Interceptor_CanTargetHouses;
		Promotable<Leptons> Interceptor_GuardRange;
		Promotable<Leptons> Interceptor_MinimumGuardRange;
		Valueable<int> Interceptor_Weapon;
		Nullable<bool> Interceptor_DeleteOnIntercept;
		Nullable<WeaponTypeClass*> Interceptor_WeaponOverride;
		Valueable<bool> Interceptor_WeaponReplaceProjectile;
		Valueable<bool> Interceptor_WeaponCumulativeDamage;
		Valueable<bool> Interceptor_KeepIntact;
		Valueable<int> Interceptor_Success;
		Valueable<int> Interceptor_RookieSuccess;
		Valueable<int> Interceptor_VeteranSuccess;
		Valueable<int> Interceptor_EliteSuccess;
		Valueable<CoordStruct> TurretOffset;
		Valueable<bool> Powered_KillSpawns;
		Valueable<bool> Spawner_LimitRange;
		Valueable<int> Spawner_ExtraLimitRange;
		Nullable<int> Spawner_DelayFrames;
		Nullable<bool> Harvester_Counted;
		Valueable<bool> Promote_IncludeSpawns;
		Valueable<bool> ImmuneToCrit;
		Valueable<bool> MultiMindControl_ReleaseVictim;
		Valueable<int> CameoPriority;
		Valueable<bool> NoManualMove;
		Nullable<int> InitialStrength;
		Valueable<int> PassengerDeletion_Rate;
		Valueable<bool> PassengerDeletion_Rate_SizeMultiply;
		Valueable<bool> PassengerDeletion_UseCostAsRate;
		Valueable<double> PassengerDeletion_CostMultiplier;
		Valueable<bool> PassengerDeletion_Soylent;
		Valueable<bool> PassengerDeletion_SoylentFriendlies;
		Valueable<bool> PassengerDeletion_DisplaySoylent;
		Valueable<double> PassengerDeletion_SoylentMultiplier;
		Valueable<AffectedHouse> PassengerDeletion_DisplaySoylentToHouses;
		Valueable<Point2D> PassengerDeletion_DisplaySoylentOffset;
		NullableIdx<VocClass> PassengerDeletion_ReportSound;
		Nullable<AnimTypeClass*> PassengerDeletion_Anim;

		Valueable<bool> AutoDeath_OnAmmoDepletion;
		Valueable<int> AutoDeath_AfterDelay;
		Nullable<AutoDeathBehavior> AutoDeath_Behavior;
		Valueable<SlaveChangeOwnerType> Slaved_OwnerWhenMasterKilled;
		NullableIdx<VocClass> SellSound;
		NullableIdx<VoxClass> EVA_Sold;

		Valueable<ShieldTypeClass*> ShieldType;

		ValueableVector<AnimTypeClass*> WarpOut;
		ValueableVector<AnimTypeClass*> WarpIn;
		ValueableVector<AnimTypeClass*> WarpAway;
		Nullable<bool> ChronoTrigger;
		Nullable<int> ChronoDistanceFactor;
		Nullable<int> ChronoMinimumDelay;
		Nullable<int> ChronoRangeMinimum;
		Nullable<int> ChronoDelay;

		Nullable<WeaponTypeClass*> WarpInWeapon;
		Nullable<WeaponTypeClass*> WarpInMinRangeWeapon;
		Nullable<WeaponTypeClass*> WarpOutWeapon;
		Valueable<bool> WarpInWeapon_UseDistanceAsDamage;

		ValueableVector<AnimTypeClass*> OreGathering_Anims;
		ValueableVector<int> OreGathering_Tiberiums;
		ValueableVector<int> OreGathering_FramesPerDir;

		std::vector<DynamicVectorClass<CoordStruct>> WeaponBurstFLHs;
		std::vector<DynamicVectorClass<CoordStruct>> VeteranWeaponBurstFLHs;
		std::vector<DynamicVectorClass<CoordStruct>> EliteWeaponBurstFLHs;

		Valueable<bool> DestroyAnim_Random;
		Valueable<bool> NotHuman_RandomDeathSequence;

		Nullable<InfantryTypeClass*> DefaultDisguise;
		Valueable<bool> UseDisguiseMovementSpeed;

		Nullable<int> OpenTopped_RangeBonus;
		Nullable<float> OpenTopped_DamageMultiplier;
		Nullable<int> OpenTopped_WarpDistance;
		Valueable<bool> OpenTopped_IgnoreRangefinding;
		Valueable<bool> OpenTopped_AllowFiringIfDeactivated;

		struct AttachmentDataEntry
		{
			AttachmentTypeClass* Type;
			TechnoTypeClass* TechnoType;
			CoordStruct FLH;
			bool IsOnTurret;

			AttachmentDataEntry() = default;

			AttachmentDataEntry(AttachmentTypeClass* pAttachmentType, TechnoTypeClass* pTechnoType, CoordStruct crdFLH, bool isOnTurret)
				:Type(pAttachmentType), TechnoType(pTechnoType), FLH(crdFLH), IsOnTurret(isOnTurret)
			{ }

			~AttachmentDataEntry() = default;

			bool Load(PhobosStreamReader& stm, bool registerForChange);
			bool Save(PhobosStreamWriter& stm) const;

		private:
			template <typename T>
			bool Serialize(T& stm);
		};

		std::vector<std::unique_ptr<AttachmentDataEntry>> AttachmentData;

		Valueable<bool> AutoFire;
		Valueable<bool> AutoFire_TargetSelf;

		Valueable<bool> NoSecondaryWeaponFallback;

		Valueable<int> NoAmmoWeapon;
		Valueable<int> NoAmmoAmount;

		Nullable<bool> JumpjetAllowLayerDeviation;
		Nullable<bool> JumpjetTurnToTarget;

		Valueable<bool> DeployingAnim_AllowAnyDirection;
		Valueable<bool> DeployingAnim_KeepUnitVisible;
		Valueable<bool> DeployingAnim_ReverseForUndeploy;
		Valueable<bool> DeployingAnim_UseUnitDrawer;

		Valueable<CSFText> EnemyUIName;
		Valueable<int> ForceWeapon_Naval_Decloaked;

		Valueable<bool> Ammo_Shared;
		Valueable<int> Ammo_Shared_Group;

		Valueable<bool> Passengers_ChangeOwnerWithTransport;

		Nullable<SelfHealGainType> SelfHealGainType;
		Valueable<bool> Passengers_SyncOwner;
		Valueable<bool> Passengers_SyncOwner_RevertOnExit;

		Nullable<bool> IronCurtain_KeptOnDeploy;

		Promotable<SHPStruct*> Insignia;
		Valueable<Vector3D<int>> InsigniaFrames;
		Promotable<int> InsigniaFrame;
		Nullable<bool> Insignia_ShowEnemy;

		Valueable<Vector2D<double>> InitialStrength_Cloning;

		struct LaserTrailDataEntry
		{
			ValueableIdx<LaserTrailTypeClass> Type;
			Valueable<CoordStruct> FLH;
			Valueable<bool> IsOnTurret;

			bool Load(PhobosStreamReader& stm, bool registerForChange);
			bool Save(PhobosStreamWriter& stm) const;

		private:
			template <typename T>
			bool Serialize(T& stm);
		};

		ValueableVector<LaserTrailDataEntry> LaserTrailData;

		struct GiftBoxDataEntry
		{
			ValueableVector<TechnoTypeClass*> GiftBox_Types;
			ValueableVector<int> GiftBox_Nums;
			Valueable<bool> GiftBox_Remove;
			Valueable<bool> GiftBox_Destroy;
			Valueable<int> GiftBox_Delay;
			Valueable<Point2D> GiftBox_DelayMinMax;
			Valueable<int> GiftBox_CellRandomRange;
			Valueable<bool> GiftBox_EmptyCell;
			Valueable<bool> GiftBox_RandomType;

			GiftBoxDataEntry() :
				GiftBox_Types {}
				, GiftBox_Nums {}
				, GiftBox_Remove { true }
				, GiftBox_Destroy { false }
				, GiftBox_Delay { 0 }
				, GiftBox_DelayMinMax { { 0,0 } }
				, GiftBox_CellRandomRange { 0 }
				, GiftBox_EmptyCell { false }
				, GiftBox_RandomType { true }
			{ }

			operator bool() const
			{
				return !GiftBox_Types.empty() && !GiftBox_Nums.empty();
			}

			bool Load(PhobosStreamReader& stm, bool registerForChange);
			bool Save(PhobosStreamWriter& stm) const;

		private:
			template <typename T>
			bool Serialize(T& stm);
		};

		GiftBoxDataEntry GiftBoxData;

		SHPStruct* Shape_SelectBox;
		ConvertClass* Palette_SelectBox;

		Nullable<bool> UseSelectBox;
		PhobosFixedString<32U> SelectBox_Shape;
		PhobosFixedString<32U> SelectBox_Palette;
		Nullable<Vector3D<int>> SelectBox_Frame;
		Nullable<Vector2D<int>> SelectBox_DrawOffset;
		Nullable<int> SelectBox_TranslucentLevel;
		Nullable<AffectedHouse> SelectBox_CanSee;
		Nullable<bool> SelectBox_CanObserverSee;

		Valueable<bool> CanRepairCyborgLegs;

		Nullable<CoordStruct> PronePrimaryFireFLH;
		Nullable<CoordStruct> ProneSecondaryFireFLH;
		Nullable<CoordStruct> DeployedPrimaryFireFLH;
		Nullable<CoordStruct> DeployedSecondaryFireFLH;
		std::vector<DynamicVectorClass<CoordStruct>> CrouchedWeaponBurstFLHs;
		std::vector<DynamicVectorClass<CoordStruct>> VeteranCrouchedWeaponBurstFLHs;
		std::vector<DynamicVectorClass<CoordStruct>> EliteCrouchedWeaponBurstFLHs;
		std::vector<DynamicVectorClass<CoordStruct>> DeployedWeaponBurstFLHs;
		std::vector<DynamicVectorClass<CoordStruct>> VeteranDeployedWeaponBurstFLHs;
		std::vector<DynamicVectorClass<CoordStruct>> EliteDeployedWeaponBurstFLHs;

		NullableVector<int> Overload_Count;
		NullableVector<int> Overload_Damage;
		NullableVector<int> Overload_Frames;
		NullableIdx<VocClass> Overload_DeathSound;
		Nullable<ParticleSystemTypeClass*> Overload_ParticleSys;
		Nullable<int> Overload_ParticleSysCount;
		Valueable<bool> Draw_MindControlLink;

		ValueableVector<DigitalDisplayTypeClass*> DigitalDisplayTypes;
		Valueable<bool> DigitalDisplay_Disable;

		ValueableVector<TechnoTypeClass*> RandomProduct;

		Valueable<bool> HugeBar;
		Valueable<int> HugeBar_Priority;

		ValueableVector<WeaponTypeClass*> FireSelf_Weapon;
		ValueableVector<int> FireSelf_ROF;
		ValueableVector<WeaponTypeClass*> FireSelf_Weapon_MaxHealth;
		ValueableVector<int> FireSelf_ROF_MaxHealth;
		ValueableVector<WeaponTypeClass*> FireSelf_Weapon_GreenHealth;
		ValueableVector<int> FireSelf_ROF_GreenHealth;
		ValueableVector<WeaponTypeClass*> FireSelf_Weapon_YellowHealth;
		ValueableVector<int> FireSelf_ROF_YellowHealth;
		ValueableVector<WeaponTypeClass*> FireSelf_Weapon_RedHealth;
		ValueableVector<int> FireSelf_ROF_RedHealth;

		Nullable<IonCannonTypeClass*> IonCannonType;

		ConvertClass* SHP_PipsPAL;
		SHPStruct* SHP_PipsSHP;
		ConvertClass* SHP_PipBrdPAL;
		SHPStruct* SHP_PipBrdSHP;
		Nullable<Vector3D<int>> HealthBar_Pips;
		Nullable<Vector2D<int>> HealthBar_Pips_DrawOffset;
		Nullable<int> HealthBar_PipsLength;
		PhobosFixedString<32U> HealthBar_PipsSHP;
		PhobosFixedString<32U> HealthBar_PipsPAL;
		Nullable<int> HealthBar_PipBrd;
		PhobosFixedString<32U> HealthBar_PipBrdSHP;
		PhobosFixedString<32U> HealthBar_PipBrdPAL;
		Nullable<Vector2D<int>> HealthBar_PipBrdOffset;
		Nullable<int> HealthBar_XOffset;
		Valueable<bool> UseNewHealthBar;
		PhobosFixedString<32U> HealthBar_PictureSHP;
		PhobosFixedString<32U> HealthBar_PicturePAL;
		Valueable<int> HealthBar_PictureTransparency;
		SHPStruct* SHP_PictureSHP;
		ConvertClass* SHP_PicturePAL;

		Nullable<Vector2D<int>> GroupID_Offset;
		Nullable<Vector2D<int>> SelfHealPips_Offset;
		Valueable<bool> UseCustomHealthBar;
		Valueable<bool> UseUnitHealthBar;

		Nullable<GScreenAnimTypeClass*> GScreenAnimType;

		Valueable<bool> MovePassengerToSpawn;
		Valueable<bool> SilentPassenger;
		Valueable<bool> Spawner_SameLoseTarget;

		Valueable<bool> DeterminedByRange;
		Valueable<int> DeterminedByRange_ExtraRange;
		Valueable<int> DeterminedByRange_MainWeapon;

		ValueableVector<TechnoTypeClass*> BuildLimit_Group_Types;
		Valueable<bool> BuildLimit_Group_Any;
		ValueableVector<int> BuildLimit_Group_Limits;

		ValueableVector<TechnoTypeClass*> BuildLimit_As;

		Valueable<bool> VehicleImmuneToMindControl;

		ValueableVector<TechnoTypeClass*> Convert_Deploy;
		Nullable<AnimTypeClass*> Convert_DeployAnim;

		Valueable<bool> IsExtendGattling;
		Valueable<bool> Gattling_Cycle;
		Valueable<bool> Gattling_Charge;
		std::vector<DynamicVectorClass<WeaponTypeClass*>> Weapons;
		std::vector<DynamicVectorClass<WeaponTypeClass*>> VeteranWeapons;
		std::vector<DynamicVectorClass<WeaponTypeClass*>> EliteWeapons;
		std::vector<DynamicVectorClass<int>> Stages;
		std::vector<DynamicVectorClass<int>> VeteranStages;
		std::vector<DynamicVectorClass<int>> EliteStages;
		std::vector<DynamicVectorClass<CoordStruct>> WeaponFLHs;
		std::vector<DynamicVectorClass<CoordStruct>> VeteranWeaponFLHs;
		std::vector<DynamicVectorClass<CoordStruct>> EliteWeaponFLHs;

		Valueable<WeaponTypeClass*> Primary;
		Valueable<WeaponTypeClass*> Secondary;
		Valueable<WeaponTypeClass*> OccupyWeapon;
		Valueable<WeaponTypeClass*> VeteranPrimary;
		Valueable<WeaponTypeClass*> VeteranSecondary;
		Valueable<WeaponTypeClass*> VeteranOccupyWeapon;
		Valueable<WeaponTypeClass*> ElitePrimary;
		Valueable<WeaponTypeClass*> EliteSecondary;
		Valueable<WeaponTypeClass*> EliteOccupyWeapon;

		Valueable<UnitTypeClass*> JJConvert_Unload;
		Nullable<IronCurtainAffects> IronCurtain_Affect;

		ValueableVector<WeaponTypeClass*> AttackedWeapon;
		ValueableVector<WeaponTypeClass*> AttackedWeapon_Veteran;
		ValueableVector<WeaponTypeClass*> AttackedWeapon_Elite;
		ValueableVector<int> AttackedWeapon_ROF;
		ValueableVector<bool> AttackedWeapon_FireToAttacker;
		ValueableVector<bool> AttackedWeapon_IgnoreROF;
		ValueableVector<bool> AttackedWeapon_IgnoreRange;
		ValueableVector<int> AttackedWeapon_Range;
		ValueableVector<WarheadTypeClass*> AttackedWeapon_ResponseWarhead;
		ValueableVector<WarheadTypeClass*> AttackedWeapon_NoResponseWarhead;
		ValueableVector<bool> AttackedWeapon_ResponseZeroDamage;
		std::vector<AffectedHouse> AttackedWeapon_ResponseHouse;
		ValueableVector<int> AttackedWeapon_ActiveMaxHealth;
		ValueableVector<int> AttackedWeapon_ActiveMinHealth;
		std::vector<CoordStruct> AttackedWeapon_FLHs;

		Promotable<WeaponTypeClass*> WeaponInTransport;

		Valueable<bool> ProtectPassengers;
		Valueable<bool> ProtectPassengers_Clear;
		Valueable<bool> ProtectPassengers_Release;
		Valueable<bool> ProtectPassengers_Damage;

		Valueable<AffectedHouse> Dodge_Houses;
		Valueable<double> Dodge_MaxHealthPercent;
		Valueable<double> Dodge_MinHealthPercent;
		Valueable<double> Dodge_Chance;
		Nullable<AnimTypeClass*> Dodge_Anim;
		Valueable<bool> Dodge_OnlyDodgePositiveDamage;

		Valueable<int> MoveDamage;
		Valueable<int> MoveDamage_Delay;
		Nullable<WarheadTypeClass*> MoveDamage_Warhead;
		Valueable<int> StopDamage;
		Valueable<int> StopDamage_Delay;
		Nullable<WarheadTypeClass*> StopDamage_Warhead;

		ValueableVector<TechnoTypeClass*> WeaponRangeShare_Technos;
		Valueable<double> WeaponRangeShare_Range;
		Valueable<bool> WeaponRangeShare_ForceAttack;
		Valueable<int> WeaponRangeShare_UseWeapon;

		Nullable<int> AllowMinHealth;

		Valueable<Vector2D<int>> AllowMaxDamage;
		Valueable<Vector2D<int>> AllowMinDamage;

		Valueable<bool> ImmuneToAbsorb;

		ValueableVector<AttachEffectTypeClass*> AttachEffects;
		ValueableVector<int> AttachEffects_Duration;
		ValueableVector<int> AttachEffects_Delay;

		Valueable<bool> TeamAffect;
		Valueable<double> TeamAffect_Range;
		ValueableVector<TechnoTypeClass*> TeamAffect_Technos;
		Valueable<AffectedHouse> TeamAffect_Houses;
		Valueable<int> TeamAffect_Number;
		Valueable<WeaponTypeClass*> TeamAffect_Weapon;
		Nullable<int> TeamAffect_ROF;
		Valueable<WeaponTypeClass*> TeamAffect_LoseEfficacyWeapon;
		Nullable<int> TeamAffect_LoseEfficacyROF;

		ValueableVector<TechnoTypeClass*> PoweredTechnos;
		Valueable<bool> PoweredTechnos_Any;
		Nullable<AnimTypeClass*> PoweredTechnos_Sparkles;

		Nullable<int> Temperature;
		Nullable<double> Temperature_HeatUpRate;
		Nullable<int> Temperature_HeatUpFrame;
		Nullable<int> Temperature_HeatUpAmount;

		ValueableVector<TechnoTypeClass*> Death_Types;

		std::vector<DynamicVectorClass<int>> Turrets;

		//Ares
		ValueableVector<TechnoTypeClass*> InitialPayload_Types;
		ValueableVector<int> InitialPayload_Nums;
		ValueableVector<BuildingTypeClass*> BuiltAt;
		Nullable<int> TurretROT;

		/*
		Interceptor
		EatPassengers
		MovePassengerToSpawn
		IonConnan
		*/
		bool LV_5_1_Used() const;
		/*
		Interceptor
		EatPassengers
		MovePassengerToSpawn
		JJConvert_Unload
		*/
		bool LV5_1 = false;

		/*
		SilentPassenger
		Spawner_SameLoseTarget
		Powered_KillSpawns
		Spawn_LimitRange
		MindControlRange
		*/
		bool LV4_1_Used() const;
		/*
		SilentPassenger
		Spawner_SameLoseTarget
		Powered_KillSpawns
		Spawn_LimitRange
		MindControlRange
		*/
		bool LV4_1 = false;

		/*
		LaserTrails
		DeathConditions
		ExtendGattling
		FireSelf
		*/
		bool LV4_2_Used() const;
		/*
		LaserTrails
		DeathConditions
		ExtendGattling
		FireSelf
		*/
		bool LV4_2 = false;

		bool CanBeBuiltAt_Ares(BuildingTypeClass* pFactoryType);

		Valueable<bool> UseConvert;
		ValueableIdxVector<TechnoTypeClass> Convert_Passangers;
		ValueableIdxVector<TechnoTypeClass> Convert_Types;

		ExtData(TechnoTypeClass* OwnerObject) : Extension<TechnoTypeClass>(OwnerObject)
			, HealthBar_Hide { false }
			, UIDescription {}
			, LowSelectionPriority { false }
			, GroupAs { NONE_STR }
			, RadarJamRadius { 0 }
			, InhibitorRange { }
			, MindControlRangeLimit {}
			, Interceptor { false }
			, Interceptor_CanTargetHouses { AffectedHouse::Enemies }
			, Interceptor_Rookie {}
			, Interceptor_Veteran {}
			, Interceptor_Elite {}
			, Interceptor_GuardRange {}
			, Interceptor_MinimumGuardRange {}
			, Interceptor_Weapon { 0 }
			, Interceptor_DeleteOnIntercept {}
			, Interceptor_WeaponOverride {}
			, Interceptor_WeaponReplaceProjectile { false }
			, Interceptor_WeaponCumulativeDamage { false }
			, Interceptor_KeepIntact { false }
			, Interceptor_Success { 100 }
			, Interceptor_RookieSuccess { -1 }
			, Interceptor_VeteranSuccess { -1 }
			, Interceptor_EliteSuccess { -1 }
			, TurretOffset { { 0, 0, 0 } }
			, Powered_KillSpawns { false }
			, Spawner_LimitRange { false }
			, Spawner_ExtraLimitRange { 0 }
			, Spawner_DelayFrames {}
			, Harvester_Counted {}
			, Promote_IncludeSpawns { false }
			, ImmuneToCrit { false }
			, MultiMindControl_ReleaseVictim { false }
			, CameoPriority { 0 }
			, NoManualMove { false }
			, InitialStrength {}
			, ShieldType {}
			, WarpOut {}
			, WarpIn {}
			, WarpAway {}
			, ChronoTrigger {}
			, ChronoDistanceFactor {}
			, ChronoMinimumDelay {}
			, ChronoRangeMinimum {}
			, ChronoDelay {}
			, WarpInWeapon {}
			, WarpInMinRangeWeapon {}
			, WarpOutWeapon {}
			, WarpInWeapon_UseDistanceAsDamage { false }
			, OreGathering_Anims {}
			, OreGathering_Tiberiums {}
			, OreGathering_FramesPerDir {}
			, LaserTrailData {}
			, DestroyAnim_Random { true }
			, NotHuman_RandomDeathSequence { false }
			, GiftBoxData {}
			, PassengerDeletion_Rate { 0 }
			, PassengerDeletion_Rate_SizeMultiply { true }
			, PassengerDeletion_UseCostAsRate { false }
			, PassengerDeletion_CostMultiplier { 1.0 }
			, PassengerDeletion_Soylent { false }
			, PassengerDeletion_SoylentMultiplier { 1.0 }
			, PassengerDeletion_SoylentFriendlies { false }
			, PassengerDeletion_DisplaySoylent { false }
			, PassengerDeletion_DisplaySoylentToHouses { AffectedHouse::All }
			, PassengerDeletion_DisplaySoylentOffset { { 0, 0 } }
			, PassengerDeletion_ReportSound {}
			, PassengerDeletion_Anim {}
			, DefaultDisguise {}
			, UseDisguiseMovementSpeed {}
			, OpenTopped_RangeBonus {}
			, OpenTopped_DamageMultiplier {}
			, OpenTopped_WarpDistance {}
			, OpenTopped_IgnoreRangefinding { false }
			, OpenTopped_AllowFiringIfDeactivated { true }
			, AttachmentData {}
			, AutoFire { false }
			, AutoFire_TargetSelf { false }
			, NoSecondaryWeaponFallback { false }
			, NoAmmoWeapon { -1 }
			, NoAmmoAmount { 0 }
			, JumpjetAllowLayerDeviation {}
			, JumpjetTurnToTarget {}
			, DeployingAnim_AllowAnyDirection { false }
			, DeployingAnim_KeepUnitVisible { false }
			, DeployingAnim_ReverseForUndeploy { true }
			, DeployingAnim_UseUnitDrawer { true }
			, AutoDeath_Behavior { }
			, AutoDeath_OnAmmoDepletion { false }
			, AutoDeath_AfterDelay { 0 }
			, Slaved_OwnerWhenMasterKilled { SlaveChangeOwnerType::Killer }
			, SellSound { }
			, EVA_Sold { }
			, EnemyUIName {}
			, ForceWeapon_Naval_Decloaked { -1 }
			, Ammo_Shared { false }
			, Ammo_Shared_Group { -1 }
			, Passengers_ChangeOwnerWithTransport { false }
			, UseSelectBox {}
			, SelectBox_Shape {}
			, SelectBox_Palette {}
			, SelectBox_Frame { { -1,-1,-1 } }
			, SelectBox_DrawOffset {}
			, SelectBox_TranslucentLevel {}
			, SelectBox_CanSee {}
			, SelectBox_CanObserverSee {}
			, PronePrimaryFireFLH { }
			, ProneSecondaryFireFLH { }
			, DeployedPrimaryFireFLH { }
			, DeployedSecondaryFireFLH { }
			, CanRepairCyborgLegs { false }
			, Draw_MindControlLink { true }
			, Overload_Count {}
			, Overload_Damage {}
			, Overload_Frames {}
			, Overload_DeathSound {}
			, Overload_ParticleSys {}
			, Overload_ParticleSysCount {}
			, SelfHealGainType {}
			, Shape_SelectBox { nullptr }
			, Palette_SelectBox { nullptr }
			, Passengers_SyncOwner { false }
			, Passengers_SyncOwner_RevertOnExit { true }
			, Insignia {}
			, InsigniaFrame { -1 }
			, InsigniaFrames { { -1, -1, -1 } }
			, Insignia_ShowEnemy {}
			, InitialStrength_Cloning { { 1.0, 0.0 } }
			, IronCurtain_KeptOnDeploy { }
			, DigitalDisplayTypes {}
			, DigitalDisplay_Disable { false }
			, HugeBar { false }
			, HugeBar_Priority { -1 }
			, IonCannonType {}
			, FireSelf_Weapon {}
			, FireSelf_ROF {}
			, FireSelf_Weapon_MaxHealth {}
			, FireSelf_ROF_MaxHealth {}
			, FireSelf_Weapon_GreenHealth {}
			, FireSelf_ROF_GreenHealth {}
			, FireSelf_Weapon_YellowHealth {}
			, FireSelf_ROF_YellowHealth {}
			, FireSelf_Weapon_RedHealth {}
			, FireSelf_ROF_RedHealth {}
			, SHP_PipsPAL { nullptr }
			, SHP_PipsSHP { nullptr }
			, SHP_PipBrdPAL { nullptr }
			, SHP_PipBrdSHP { nullptr }
			, HealthBar_Pips {}
			, HealthBar_Pips_DrawOffset {}
			, HealthBar_PipsLength {}
			, HealthBar_PipsSHP { "" }
			, HealthBar_PipsPAL { "" }
			, HealthBar_PipBrd {}
			, HealthBar_PipBrdSHP { "" }
			, HealthBar_PipBrdPAL { "" }
			, HealthBar_PipBrdOffset { }
			, HealthBar_XOffset { 0 }
			, GroupID_Offset { {0,0} }
			, SelfHealPips_Offset { {0,0} }
			, UseCustomHealthBar { false }
			, UseUnitHealthBar { false }
			, UseNewHealthBar { false }
			, HealthBar_PictureSHP { "" }
			, HealthBar_PicturePAL { "" }
			, HealthBar_PictureTransparency { 0 }
			, SHP_PictureSHP {}
			, SHP_PicturePAL {}
			, GScreenAnimType {}
			, MovePassengerToSpawn { false }
			, SilentPassenger { false }
			, Spawner_SameLoseTarget { false }
			, DeterminedByRange { false }
			, DeterminedByRange_ExtraRange { 0 }
			, DeterminedByRange_MainWeapon { -1 }
			, BuildLimit_Group_Types {}
			, BuildLimit_Group_Any { false }
			, BuildLimit_Group_Limits {}
			, VehicleImmuneToMindControl { false }
			, Convert_Deploy {}
			, Convert_DeployAnim {}
			, IsExtendGattling { false }
			, Gattling_Cycle { false }
			, Gattling_Charge { false }
			, Weapons {}
			, VeteranWeapons {}
			, EliteWeapons {}
			, Stages {}
			, VeteranStages {}
			, EliteStages {}
			, WeaponFLHs {}
			, VeteranWeaponFLHs {}
			, EliteWeaponFLHs {}
			, Primary {}
			, Secondary {}
			, OccupyWeapon {}
			, VeteranPrimary {}
			, VeteranSecondary {}
			, VeteranOccupyWeapon {}
			, ElitePrimary {}
			, EliteSecondary {}
			, EliteOccupyWeapon {}
			, JJConvert_Unload {}
			, IronCurtain_Affect {}
			, BuildLimit_As {}

			, AttackedWeapon {}
			, AttackedWeapon_FireToAttacker {}
			, AttackedWeapon_ROF {}
			, AttackedWeapon_IgnoreROF {}
			, AttackedWeapon_IgnoreRange {}
			, AttackedWeapon_Range {}
			, AttackedWeapon_ResponseWarhead {}
			, AttackedWeapon_NoResponseWarhead {}
			, AttackedWeapon_ResponseZeroDamage {}
			, AttackedWeapon_ResponseHouse {}
			, AttackedWeapon_ActiveMaxHealth {}
			, AttackedWeapon_ActiveMinHealth {}
			, AttackedWeapon_FLHs {}

			, WeaponInTransport {}
			
			, ProtectPassengers { false }
			, ProtectPassengers_Clear { false }
			, ProtectPassengers_Release { false }
			, ProtectPassengers_Damage { false }

			, Dodge_Houses { AffectedHouse::All }
			, Dodge_MaxHealthPercent { 1.0 }
			, Dodge_MinHealthPercent { 0.0 }
			, Dodge_Chance { 0.0 }
			, Dodge_Anim {}
			, Dodge_OnlyDodgePositiveDamage { true }

			, MoveDamage { 0 }
			, MoveDamage_Delay { 0 }
			, MoveDamage_Warhead {}
			, StopDamage { 0 }
			, StopDamage_Delay { 0 }
			, StopDamage_Warhead {}

			, InitialPayload_Types {}
			, InitialPayload_Nums {}

			, WeaponRangeShare_Technos {}
			, WeaponRangeShare_Range { 0.0 }
			, WeaponRangeShare_ForceAttack { false }
			, WeaponRangeShare_UseWeapon { 0 }

			, AllowMinHealth {}
			, Death_Types {}
			, Turrets {}
			, AllowMaxDamage { { INT_MAX, -INT_MAX } }
			, AllowMinDamage { { -INT_MAX, INT_MAX } }
			, ImmuneToAbsorb { false }

			, TeamAffect { false }
			, TeamAffect_Range { 0.0 }
			, TeamAffect_Technos {}
			, TeamAffect_Houses { AffectedHouse::Owner }
			, TeamAffect_Number { 0 }
			, TeamAffect_Weapon {}
			, TeamAffect_ROF {}
			, TeamAffect_LoseEfficacyWeapon {}
			, TeamAffect_LoseEfficacyROF {}

			, AttachEffects {}
			, AttachEffects_Duration {}
			, AttachEffects_Delay {}

			, PoweredTechnos {}
			, PoweredTechnos_Any { true }
			, PoweredTechnos_Sparkles {}

			, Temperature {}
			, Temperature_HeatUpRate {}
			, Temperature_HeatUpFrame {}
			, Temperature_HeatUpAmount {}

			, TurretROT {}

			, UseConvert { false }
			, Convert_Passangers {}
			, Convert_Types {}
		{ }

		virtual ~ExtData() = default;
		virtual void LoadFromINIFile(CCINIClass* pINI) override;
		virtual void Initialize() override;

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override { }

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;

		void ApplyTurretOffset(Matrix3D* mtx, double factor = 1.0);
		bool IsCountedAsHarvester();

		// Ares 0.A
		const char* GetSelectionGroupID() const;

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<TechnoTypeExt>
	{
	public:
		ExtContainer();
		~ExtContainer();
	};

	static ExtContainer ExtMap;

	static void ApplyTurretOffset(TechnoTypeClass* pType, Matrix3D* mtx, double factor = 1.0);
	static void GetBurstFLHs(TechnoTypeClass* pThis, INI_EX& exArtINI, const char* pArtSection, std::vector<DynamicVectorClass<CoordStruct>>& nFLH, std::vector<DynamicVectorClass<CoordStruct>>& nVFlh, std::vector<DynamicVectorClass<CoordStruct>>& nEFlh, const char* pPrefixTag);
	static TechnoTypeClass* GetTechnoType(ObjectTypeClass* pType);
	static void GetWeaponCounts(TechnoTypeClass* pThis, INI_EX& exINI, const char* pSection, std::vector<DynamicVectorClass<WeaponTypeClass*>>& n, std::vector<DynamicVectorClass<WeaponTypeClass*>>& nV, std::vector<DynamicVectorClass<WeaponTypeClass*>>& nE);
	static void GetWeaponStages(TechnoTypeClass* pThis, INI_EX& exINI, const char* pSection, std::vector<DynamicVectorClass<int>>& nStage, std::vector<DynamicVectorClass<int>>& nStageV, std::vector<DynamicVectorClass<int>>& nStageE);
	static void GetWeaponFLHs(TechnoTypeClass* pThis, INI_EX& exArtINI, const char* pArtSection, std::vector<DynamicVectorClass<CoordStruct>>& nFLH, std::vector<DynamicVectorClass<CoordStruct>>& nVFlh, std::vector<DynamicVectorClass<CoordStruct>>& nEFlh);
	static void GetIFVTurrets(TechnoTypeClass* pThis, INI_EX& exINI, const char* pSection, std::vector<DynamicVectorClass<int>>& nturret);
	static std::vector<WeaponTypeClass*> GetAllWeapons(TechnoTypeClass* pThis);

	// Ares 0.A
	static const char* GetSelectionGroupID(ObjectTypeClass* pType);
	static bool HasSelectionGroupID(ObjectTypeClass* pType, const char* pID);
};
