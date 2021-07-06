// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Main.generated.h"

UENUM(BlueprintType)
enum class EMovementStatus : uint8 {
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_Sprinting UMETA(DisplayName = "Sprinting"),
	EMS_Dead UMETA(DisplayName = "Dead"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UENUM(BlueprintType)
enum class EStaminaStatus : uint8 {
	EMS_Normal UMETA(DisplayName = "Normal"),
	EMS_BellowMinimum UMETA(DisplayName = "BellowMinimum"),
	EMS_Exhausted UMETA(DisplayName = "Exhausted"),
	EMS_Recovering UMETA(DisplayName = "ExhaustedRecovering"),
	EMS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class CPPORIJECT_API AMain : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMain();
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Combat")
	bool bHasCombatTarget;

	FORCEINLINE void SetHasCombatTarget(bool hasTarget) { bHasCombatTarget = hasTarget; };
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Combat")
	FVector combatTargetLocation;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Controller")
	class AMainController* mainController;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Items")
	class AWeapon* EquippedWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Items")
	class AItem* ActiveOverlapingItem;

	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "speed")
	float runningSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "speed")
	float sprintSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EMovementStatus MovementStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
	EStaminaStatus StaminaStatus;

	void SetMovementStatus(EMovementStatus Status);
	/** Camera boom positioning the camera behind the player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess= "true"))
	class USpringArmComponent* CameraBoom;
	/**Follow Camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	bool bShiftKeyDown;

	void ShiftKeyDown();

	void ShiftKeyUp();

	FORCEINLINE void SetStaminaStatus(EStaminaStatus status) { StaminaStatus = status; }

	//STATS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		class UParticleSystem* HitParticles;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float health;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats")
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	float stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
	int32 Coins;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Main | Sound")
	class USoundCue* HitSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats")
	float staminaDrainRate;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "PlayerStats")
	float minSprintStamina;
	float InterSpeed;
	bool bInterpToEnemy;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
	class AEnemy* combatTarget;

	FORCEINLINE void SetCombatTarget(AEnemy* target) { combatTarget = target; }
	FRotator GetLookAtRotationYaw(FVector target);
	void SetInterpToEnemy(bool Interp);
	void DecrementHealth(float amount);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void addCoins(float amount);
	FORCEINLINE void SetEquippedWeapon(AWeapon* WeaponToSet) { EquippedWeapon = WeaponToSet; }
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
	FORCEINLINE void SetActiveOverlapedItem(AItem* OverlapingItem) { ActiveOverlapingItem = OverlapingItem; }
	void Die();
	virtual void Jump() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	bool bLMBDown;
	void LMBDown();
	void LMBUp();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*Called for forwards/backwards input*/
	void MoveForward(float value);
	bool bMovingForward;
	
	void MoveRight(float value);
	bool bMovingRight;
	void TurnRate(float value);
	
	void LookUpRate(float value);

	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UPROPERTY(VisibleAnywhere, BlueprintreadOnly, Category = "Anims")
	bool bAttacking;

	void Attack();
	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	class UAnimMontage* CombatMontage;
	void UpdateTarget();
	UPROPERTY(EditAnywhere, BlueprintreadWrite, Category = "Combat")
	TSubclassOf<AEnemy> EnemyFilter;
	void SwitchLevel(FName LevelName);
	

};
