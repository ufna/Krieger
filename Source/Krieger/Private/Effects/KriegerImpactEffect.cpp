// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "Krieger.h"

AKriegerImpactEffect::AKriegerImpactEffect(const class FObjectInitializer& PCIP) : Super(PCIP)
{
	bAutoDestroyWhenFinished = true;
}

void AKriegerImpactEffect::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPhysicalMaterial* HitPhysMat = SurfaceHit.PhysMaterial.Get();
	EPhysicalSurface HitSurfaceType = UPhysicalMaterial::DetermineSurfaceType(HitPhysMat);

	// Show particles
	UParticleSystem* ImpactFX = GetImpactFX(HitSurfaceType);
	if (ImpactFX)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactFX, GetActorLocation(), GetActorRotation());
	}

	// Play sound
	USoundCue* ImpactSound = GetImpactSound(HitSurfaceType);
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}

	if (DefaultDecal.DecalMaterial)
	{
		FRotator RandomDecalRotation = SurfaceHit.ImpactNormal.Rotation();
		RandomDecalRotation.Roll = FMath::FRandRange(-180.0f, 180.0f);

		UGameplayStatics::SpawnDecalAttached(DefaultDecal.DecalMaterial, FVector(DefaultDecal.DecalSize, DefaultDecal.DecalSize, 1.0f),
			SurfaceHit.Component.Get(), SurfaceHit.BoneName,
			SurfaceHit.ImpactPoint, RandomDecalRotation, EAttachLocation::KeepWorldPosition,
			DefaultDecal.LifeSpan);
	}
}

UParticleSystem* AKriegerImpactEffect::GetImpactFX(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	UParticleSystem* ImpactFX = NULL;

	switch (SurfaceType)
	{
		case KRIEGER_SURFACE_Concrete:	ImpactFX = ConcreteFX; break;
		case KRIEGER_SURFACE_Dirt:		ImpactFX = DirtFX; break;
		case KRIEGER_SURFACE_Water:		ImpactFX = WaterFX; break;
		case KRIEGER_SURFACE_Metal:		ImpactFX = MetalFX; break;
		case KRIEGER_SURFACE_Wood:		ImpactFX = WoodFX; break;
		case KRIEGER_SURFACE_Grass:		ImpactFX = GrassFX; break;
		case KRIEGER_SURFACE_Glass:		ImpactFX = GlassFX; break;
		case KRIEGER_SURFACE_Flesh:		ImpactFX = FleshFX; break;
		default:						ImpactFX = DefaultFX; break;
	}

	return ImpactFX;
}

USoundCue* AKriegerImpactEffect::GetImpactSound(TEnumAsByte<EPhysicalSurface> SurfaceType) const
{
	USoundCue* ImpactSound = NULL;

	switch (SurfaceType)
	{
		case KRIEGER_SURFACE_Concrete:	ImpactSound = ConcreteSound; break;
		case KRIEGER_SURFACE_Dirt:		ImpactSound = DirtSound; break;
		case KRIEGER_SURFACE_Water:		ImpactSound = WaterSound; break;
		case KRIEGER_SURFACE_Metal:		ImpactSound = MetalSound; break;
		case KRIEGER_SURFACE_Wood:		ImpactSound = WoodSound; break;
		case KRIEGER_SURFACE_Grass:		ImpactSound = GrassSound; break;
		case KRIEGER_SURFACE_Glass:		ImpactSound = GlassSound; break;
		case KRIEGER_SURFACE_Flesh:		ImpactSound = FleshSound; break;
		default:						ImpactSound = DefaultSound; break;
	}

	return ImpactSound;
}
