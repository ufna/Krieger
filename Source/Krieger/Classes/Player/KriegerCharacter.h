// Copyright 2014 Vladimir Alyamkin. All Rights Reserved.

#include "KriegerCharacter.generated.h"

class AKriegerWeapon;

UCLASS(Abstract, Blueprintable)
class AKriegerCharacter : public ACharacter
{
	GENERATED_UCLASS_BODY()

	/** Spawn inventory, setup initial variables */
	virtual void PostInitializeComponents() override;

	/** Cleanup inventory */
	virtual void Destroyed() override;

	/** [server] Perform PlayerState related setup */
	virtual void PossessedBy(class AController* C) override;

	/** [client] Perform PlayerState related setup */
	virtual void OnRep_PlayerState() override;


	//////////////////////////////////////////////////////////////////////////
	// Health

	/** Check if pawn is still alive */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	bool IsAlive() const;

	/** Get current health */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	float GetHealth() const;

	/** [server] Set current health */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	void SetHealth(float NewHealth);

	/** Get max health */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	float GetMaxHealth() const;

	/** [server] Set max health */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	void SetMaxHealth(float NewMaxHealth);

protected:
	/** Current health of the Pawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category=Health)
	float Health;

	/** Maximum health of the Pawn */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category=Health)
	float MaxHealth;


	//////////////////////////////////////////////////////////////////////////
	// Inventory
protected:

	/** [server] Spawns default inventory */
	virtual void SpawnDefaultInventory();

	/** [server] Remove all weapons from inventory and destroy them */
	virtual void DestroyInventory();

public:
	/** Get weapon attach point */
	virtual FName GetWeaponAttachPoint(AKriegerWeapon* Weapon) const PURE_VIRTUAL(AKriegerCharacter::GetWeaponAttachPoint, return TEXT(""););


	//////////////////////////////////////////////////////////////////////////
	// Weapon usage

public:
	/** [local] starts weapon fire */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	void StartFire(int32 WeaponIdx = 0, int32 WeaponMode = 0);

	/** [local] stops weapon fire */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	void StopFire(int32 WeaponIdx = 0, int32 WeaponMode = 0);

protected:
	/** [local] Character specific fire implementation */
	virtual void StartWeaponFire(int32 WeaponIdx, int32 WeaponMode) PURE_VIRTUAL(AKriegerCharacter::StartWeaponFire, );
	virtual void StopWeaponFire(int32 WeaponIdx, int32 WeaponMode) PURE_VIRTUAL(AKriegerCharacter::StopWeaponFire, );

public:
	/** Check if pawn can fire weapon */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	bool CanFire(int32 WeaponIdx = 0) const;

	/** Check if pawn can reload weapon */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	bool CanReload(int32 WeaponIdx = 0) const;

	/** Get firing state */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	bool IsFiring(int32 WeaponIdx = 0) const;

protected:
	/** Current firing state */
	uint8 bWantsToFire : 1;


	//////////////////////////////////////////////////////////////////////////
	// Targeting (Aiming)

public:
	/** Get the point we're targeting too */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	FVector GetTargetPoint() const;

	/** [local] Set the point we're targeting too */
	UFUNCTION(BlueprintCallable, Category = "Krieger|Character")
	void SetTargetPoint(const FVector& TargetLocation);

	/** [server] Set the point we're targeting too */
	UFUNCTION(reliable, server, WithValidation)
	void ServerSetTargetPoint(const FVector TargetLocation);


protected:
	/** Weapon target point set by player */
	UPROPERTY(Transient, Replicated)
	FVector TargetPoint;


	//////////////////////////////////////////////////////////////////////////
	// Meshes

public:
	/** Get mesh component */
	USkeletalMeshComponent* GetPawnMesh() const;

	/** Update the team color of all player meshes. */
	void UpdateTeamColorsAllMIDs();

protected:
	/** Handle mesh colors on specified material instance */
	void UpdateTeamColors(UMaterialInstanceDynamic* UseMID);

	/** Material instances for setting team color in mesh (3rd person view) */
	UPROPERTY(Transient)
	TArray<UMaterialInstanceDynamic*> MeshMIDs;


	//////////////////////////////////////////////////////////////////////////
	// Effects

protected:
	/** Effect played on respawn */
	UPROPERTY(EditDefaultsOnly, Category=Effects)
	UParticleSystem* RespawnFX;


	//////////////////////////////////////////////////////////////////////////
	// Sound

protected:
	/** Sound played on death, local player only */
	UPROPERTY(EditDefaultsOnly, Category=Pawn)
	USoundCue* DeathSound;

	/** Sound played on respawn */
	UPROPERTY(EditDefaultsOnly, Category=Pawn)
	USoundCue* RespawnSound;

};

