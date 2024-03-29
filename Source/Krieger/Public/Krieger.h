// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "Net/UnrealNetwork.h"

#include "ParticleDefinitions.h"
#include "Particles/ParticleSystemComponent.h"

#include "KriegerClasses.h"

/** When you modify this, please note that this information can be saved with instances
 * also DefaultEngine.ini [/Script/Engine.CollisionProfile] should match with this list **/
#define COLLISION_WEAPON		ECC_GameTraceChannel1
#define COLLISION_PROJECTILE	ECC_GameTraceChannel2
#define COLLISION_PICKUP		ECC_GameTraceChannel3

DECLARE_LOG_CATEGORY_EXTERN(LogKrieger, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogWeapon, Log, All);
