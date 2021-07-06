// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"
UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	
	EMS_Idle			UMETA(DeplayName = "Idle"),
	EMS_MoveToTarget	UMETA(DeplayName = "MoveToTarget"),
	EMS_Attack			UMETA(DeplayName = "Attack"),
	EMS_MAX				UMETA(DeplayName = "DefaultMax"),
	EMS_Dead			UMETA(DeplayName = "Dead")
};
UCLASS()
class CPPORIJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class USphereComponent* agroSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	USphereComponent* combatSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	EEnemyMovementStatus enemyStatus;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class AAIController* AIController;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Enemy | Sound")
	class USoundCue* HitSound;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Enemy | Sound")
	USoundCue* SwingSound;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CombatCollision")
	class UBoxComponent* combatColision;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "Combat" )
	class UAnimMontage* CombatMontage;
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category =  "Combat")
	float AttackMinTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float AttackMaxTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TSubclassOf<UDamageType> DamageTypeClass;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	FORCEINLINE void SetEnemyMovementStatus(EEnemyMovementStatus status) { enemyStatus = status; }
	FTimerHandle DeathTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	float DeathDelay;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	bool bCanAttack;
	FORCEINLINE void SetEnemyCanAttack(bool canAttack) { bCanAttack = canAttack; }
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void MoveToTarget(class AMain* Target);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
		virtual void AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		virtual void CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		virtual void CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
		void OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(BlueprintCallable)
		void ActivateCollision();
	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	bool bOverlapingCombatSphere;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	AMain* CombatTarget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float maxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	float damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Particle")
		class UParticleSystem* HitParticles;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Combat")
	bool bAttacking;
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void Attack();
	UFUNCTION()
	void Die(AActor* Causer);
	UFUNCTION(BlueprintCallable)
	void DeathEnd();
	UFUNCTION(BlueprintCallable)
	bool Alive();
	UFUNCTION(BlueprintCallable)
	void Disappear();

};