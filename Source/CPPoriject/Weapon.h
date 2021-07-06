// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

UENUM(BlueprintType)
enum class EWeaponState :uint8 {
	EWS_PickUp UMETA(DisplayName = "Pickup"),
	EWS_Equipped UMETA(DisplayName = "Equipped"),
	EWS_MAX UMETA(DisplayName = "DefaultMax")
};
/**
 * 
 */
UCLASS()
class CPPORIJECT_API AWeapon : public AItem
{
	GENERATED_BODY()
	

public:
	AWeapon();
	EWeaponState weaponState;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item | Sound")
	class USoundCue* OnEquipSound;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item | Sound")
	USoundCue* SwingSound;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Item | Combat")
	class UBoxComponent* CombatCollision;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)override;
	UFUNCTION()
	void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Combat")
	float damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Particle")
	class UParticleSystem* HitParticles;
	void Equip(class AMain* character);
	FORCEINLINE void SetWeaponState(EWeaponState state) { weaponState = state; }
	FORCEINLINE EWeaponState GetWeaponState() { return weaponState; }
	UFUNCTION(BlueprintCallable)
	void ActivateCollision();
	UFUNCTION(BlueprintCallable)
	void DeactivateCollision();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	AController* WeaponInstigator;

	FORCEINLINE void SetInstigator(AController* inst) { WeaponInstigator = inst; }
protected:
	virtual void BeginPlay()override;
};
