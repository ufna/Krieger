// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "Krieger.h"

AKriegerMech::AKriegerMech(const class FPostConstructInitializeProperties& PCIP) 
	: Super(PCIP)
{
	Health = 100;
	MaxHealth = 100;
}

//////////////////////////////////////////////////////////////////////////
// Inventory

void AKriegerMech::SpawnDefaultInventory()
{
	Super::SpawnDefaultInventory();

	if (Role < ROLE_Authority)
	{
		return;
	}

	// Weapons can collide mech on spawn, it's okay
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoCollisionFail = true;

	// Spawn and equip default weapons
	if (DefaultWeaponRight != nullptr)
	{
		AKriegerWeapon* NewWeapon = GetWorld()->SpawnActor<AKriegerWeapon>(DefaultWeaponRight, SpawnInfo);
		AddWeapon(NewWeapon);
		EquipWeapon(&WeaponRight, NewWeapon);
	}

	if (DefaultWeaponLeft != nullptr)
	{
		AKriegerWeapon* NewWeapon = GetWorld()->SpawnActor<AKriegerWeapon>(DefaultWeaponLeft, SpawnInfo);
		AddWeapon(NewWeapon);
		EquipWeapon(&WeaponLeft, NewWeapon);
	}

	if (DefaultWeaponBody != nullptr)
	{
		AKriegerWeapon* NewWeapon = GetWorld()->SpawnActor<AKriegerWeapon>(DefaultWeaponBody, SpawnInfo);
		AddWeapon(NewWeapon);
		EquipWeapon(&WeaponBody, NewWeapon);
	}

	if (DefaultWeaponBack != nullptr)
	{
		AKriegerWeapon* NewWeapon = GetWorld()->SpawnActor<AKriegerWeapon>(DefaultWeaponBack, SpawnInfo);
		AddWeapon(NewWeapon);
		EquipWeapon(&WeaponBack, NewWeapon);
	}
}

void AKriegerMech::DestroyInventory()
{
	Super::DestroyInventory();

	if (Role < ROLE_Authority)
	{
		return;
	}

	// Remove all weapons from inventory and destroy them
	for (int32 i = WeaponInventory.Num() - 1; i >= 0; i--)
	{
		AKriegerWeapon* Weapon = WeaponInventory[i];
		if (Weapon)
		{
			RemoveWeapon(Weapon);
			Weapon->Destroy();
		}
	}
}

void AKriegerMech::AddWeapon(AKriegerWeapon* Weapon)
{
	if (Weapon && Role == ROLE_Authority)
	{
		Weapon->OnEnterInventory(this);
		WeaponInventory.AddUnique(Weapon);
	}
}

void AKriegerMech::RemoveWeapon(AKriegerWeapon* Weapon)
{
	if (Weapon && Role == ROLE_Authority)
	{
		Weapon->OnLeaveInventory();
		WeaponInventory.RemoveSingle(Weapon);
	}
}

FName AKriegerMech::GetWeaponAttachPoint(AKriegerWeapon* Weapon) const
{
	if (Weapon == WeaponRight)
	{
		return WeaponSocketRight;
	}
	else if (Weapon == WeaponLeft)
	{
		return WeaponSocketLeft;
	}
	else if (Weapon == WeaponBody)
	{
		return WeaponSocketBody;
	}
	else if (Weapon == WeaponBack)
	{
		return WeaponSocketBack;
	}

	return TEXT("Invalid");
}


//////////////////////////////////////////////////////////////////////////
// Weapons (Inventory)

int32 AKriegerMech::GetInventoryCount() const
{
	return WeaponInventory.Num();
}

AKriegerWeapon* AKriegerMech::GetInventoryWeapon(int32 index) const
{
	return WeaponInventory[index];
}

void AKriegerMech::EquipWeapon(AKriegerWeapon** WeaponSlot, AKriegerWeapon* Weapon)
{
	if (WeaponSlot && Weapon)
	{
		if (Role == ROLE_Authority)
		{
			SetCurrentWeapon(WeaponSlot, Weapon);
		}
		else
		{
			// @TODO RPC from client to server for weapon equip
		}
	}
}

void AKriegerMech::SetCurrentWeapon(AKriegerWeapon** WeaponSlot, AKriegerWeapon* NewWeapon, AKriegerWeapon* LastWeapon)
{
	AKriegerWeapon* LocalLastWeapon = NULL;

	if (LastWeapon != NULL)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != *WeaponSlot)
	{
		LocalLastWeapon = *WeaponSlot;
	}

	// Unequip previous
	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
	}

	*WeaponSlot = NewWeapon;

	// Equip new one
	if (NewWeapon)
	{
		// Make sure weapon's MyPawn is pointing back to us. During replication, 
		// we can't guarantee APawn::CurrentWeapon will rep after AWeapon::MyPawn!
		NewWeapon->SetOwningPawn(this);
		NewWeapon->OnEquip();
	}
}


//////////////////////////////////////////////////////////////////////////
// Weapon usage

void AKriegerMech::StartWeaponFire(int32 WeaponIdx)
{
	switch (WeaponIdx)
	{
	case 0:
		WeaponRight->StartFire();
		break;

	case 1:
		WeaponLeft->StartFire();
		break;

	case 2:
		WeaponBody->StartFire();
		break;

	case 3:
		WeaponBack->StartFire();
		break;

	default:
		break;
	}
}

void AKriegerMech::StopWeaponFire(int32 WeaponIdx)
{
	switch (WeaponIdx)
	{
	case 0:
		WeaponRight->StopFire();
		break;

	case 1:
		WeaponLeft->StopFire();
		break;

	case 2:
		WeaponBody->StopFire();
		break;

	case 3:
		WeaponBack->StopFire();
		break;

	default:
		break;
	}
}


//////////////////////////////////////////////////////////////////////////
// Replication

void AKriegerMech::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Only to local owner: weapon change requests are locally instigated, other clients don't need it
	DOREPLIFETIME_CONDITION(AKriegerMech, WeaponInventory, COND_OwnerOnly);

	// Everyone
	DOREPLIFETIME(AKriegerMech, WeaponRight);
	DOREPLIFETIME(AKriegerMech, WeaponLeft);
	DOREPLIFETIME(AKriegerMech, WeaponBody);
	DOREPLIFETIME(AKriegerMech, WeaponBack);
}

void AKriegerMech::OnRep_WeaponRight(AKriegerWeapon* LastWeapon)
{
	SetCurrentWeapon(&WeaponRight, WeaponRight, LastWeapon);
}

void AKriegerMech::OnRep_WeaponLeft(AKriegerWeapon* LastWeapon)
{
	SetCurrentWeapon(&WeaponLeft, WeaponLeft, LastWeapon);
}

void AKriegerMech::OnRep_WeaponBody(AKriegerWeapon* LastWeapon)
{
	SetCurrentWeapon(&WeaponBody, WeaponBody, LastWeapon);
}

void AKriegerMech::OnRep_WeaponBack(AKriegerWeapon* LastWeapon)
{
	SetCurrentWeapon(&WeaponBack, WeaponBack, LastWeapon);
}