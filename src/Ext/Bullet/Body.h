#pragma once
#include <BulletClass.h>

#include <Ext/BulletType/Body.h>
#include <Helpers/Macro.h>
#include <Utilities/Container.h>
#include <Utilities/TemplateDef.h>
#include <New/Entity/LaserTrailClass.h>

#include "Trajectories/PhobosTrajectory.h"

class BulletExt
{
public:
	using base_type = BulletClass;

	class ExtData final : public Extension<BulletClass>
	{
	public:
		BulletTypeExt::ExtData* TypeExtData;
		HouseClass* FirerHouse;
		int CurrentStrength;
		bool IsInterceptor;
		InterceptedStatus InterceptedStatus;
		bool DetonateOnInterception;
		bool SnappedToTarget;
		std::vector<std::unique_ptr<LaserTrailClass>> LaserTrails;

		PhobosTrajectory* Trajectory;

		bool ShouldDirectional;
		DirStruct BulletDir;

		ExtData(BulletClass* OwnerObject) : Extension<BulletClass>(OwnerObject)
			, TypeExtData { nullptr }
			, FirerHouse { nullptr }
			, CurrentStrength { 0 }
			, IsInterceptor { false }
			, InterceptedStatus { InterceptedStatus::None }
			, DetonateOnInterception { true }
			, SnappedToTarget { false }
			, LaserTrails {}
			, Trajectory { nullptr }
			, ShouldDirectional { false }
			, BulletDir {}
		{ }

		virtual ~ExtData() = default;

		virtual void InvalidatePointer(void* ptr, bool bRemoved) override { }

		virtual void LoadFromStream(PhobosStreamReader& Stm) override;
		virtual void SaveToStream(PhobosStreamWriter& Stm) override;

		void InterceptBullet(TechnoClass* pSource, WeaponTypeClass* pWeapon);
		void ApplyRadiationToCell(CellStruct Cell, int Spread, int RadLevel);
		void InitializeLaserTrails();

	private:
		template <typename T>
		void Serialize(T& Stm);
	};

	class ExtContainer final : public Container<BulletExt>
	{
	public:
		ExtContainer();
		~ExtContainer();
	};

	static void DrawElectricLaserWeapon(BulletClass* pThis, WeaponTypeClass* pWeaponType);
	static void DrawElectricLaser(CoordStruct PosFire, CoordStruct PosEnd, int Length, ColorStruct Color, int Duration, int Thickness, bool IsSupported);

	static ExtContainer ExtMap;
};
