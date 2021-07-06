// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "Main.h"
#include "Sound/SoundCue.h"
#include "DrawDebugHelpers.h"
#include "Math/Color.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "MainController.h"
// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	agroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
	combatSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
	combatColision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	combatColision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("FX_Trail_02_L"));
	
	//HitParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitParticles"));
	agroSphere->SetupAttachment(GetRootComponent());
	combatSphere->SetupAttachment(GetRootComponent());

	combatSphere->InitSphereRadius(75.f);
	agroSphere->InitSphereRadius(600.f);
	AttackMinTime=1.5f;
	AttackMaxTime=3.0f;
	DeathDelay = 3.f;
	bCanAttack = true;


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	combatColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	combatColision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	combatColision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	combatColision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	combatColision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnCombatOverlapBegin);
	combatColision->OnComponentEndOverlap.AddDynamic(this, &AEnemy::OnCombatOverlapEnd);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	combatSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapBegin);
	combatSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);
	agroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AggroSphereOnOverlapBegin);
	agroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereOnOverlapEnd);

	AIController = Cast<AAIController>(GetController());
	bOverlapingCombatSphere = false;
	maxHealth = 50.f;
	health = maxHealth;
	damage = 5.f;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::AggroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!Alive()) { return; }
	if (OtherActor) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			MoveToTarget(Main);
		}
	}
	
}

void AEnemy::AggroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!Alive()) { return; }
	if (OtherActor) {
		AMain* main = Cast<AMain>(OtherActor);
		if (main) {
			
			if (main->combatTarget == this) {
				main->SetCombatTarget(nullptr);
			}
			main->UpdateTarget();

			
			SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Idle);
			if (AIController) {
				AIController->StopMovement();
			}
			//MoveToTarget(main);
		}
	}
}
void AEnemy::CombatSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!Alive()) { return; }
	
	if (OtherActor) {
		AMain* main = Cast<AMain>(OtherActor);
		if (main) {
			main->SetCombatTarget(this);
			main->SetHasCombatTarget(true);
			if (main->mainController) {
				main->mainController->DisplayEnemyHealthBar();
			}
			bOverlapingCombatSphere = true;
			CombatTarget = main;
			Attack();
		}
	
	}

}

void AEnemy::CombatSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!Alive()) { return; }
	if (OtherActor) {
		AMain* main = Cast<AMain>(OtherActor);
		if (main) {
			if (main->combatTarget == this) {
				main->SetHasCombatTarget(false);
				main->SetCombatTarget(nullptr);
				main->UpdateTarget();
				if (main->mainController) {
					main->mainController->RemoveEnemyHealthBar();
				}

			}
			
			bOverlapingCombatSphere = false; 
			MoveToTarget(main);
			CombatTarget = nullptr;
			
			
			GetWorldTimerManager().ClearTimer(AttackTimer);
			//SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
			
		}
	}
}
void AEnemy::MoveToTarget(class AMain* Target) {
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);
	if (AIController){
		FAIMoveRequest moveRequest;
		moveRequest.SetGoalActor(Target);
		moveRequest.SetAcceptanceRadius(5.0f);

		FNavPathSharedPtr NavPath = nullptr;


		AIController->MoveTo(moveRequest,&NavPath);
		if (NavPath) {
		auto PathPoints = NavPath->GetPathPoints();
		for (auto Point : PathPoints) {
			FVector Location = Point.Location;
			//DrawDebugSphere(GetWorld(),Location,25.5f,8,FLinearColor::Green, false,10.f,3.f);
			UKismetSystemLibrary::DrawDebugSphere(GetWorld(), Location,30,8,FLinearColor::Green, 8.f,8.f);
		}
		}
		
	}
	
}
void AEnemy::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!Alive()) { return; }
	if (OtherActor) {
		AMain* player = Cast<AMain>(OtherActor);
		if (player) {
			if (player->HitParticles) {
				const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName("FX_Trail_01_L");
				if (WeaponSocket) {
					FVector socketLocation = WeaponSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), player->HitParticles, socketLocation, FRotator(0., 0., 0.), false);
				}

			}
			if (player->HitSound) {
				UGameplayStatics::PlaySound2D(this, player->HitSound);
			}
			if (DamageTypeClass) {
				UGameplayStatics::ApplyDamage(player, damage, AIController, this, DamageTypeClass);
			}

		}
	}

}

void AEnemy::OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (!Alive()) { return; }
	if (OtherActor) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			//Equip(Main);
			Main->SetActiveOverlapedItem(nullptr);
		}

	}
}
void  AEnemy::ActivateCollision() { combatColision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);  
}

void  AEnemy::DeactivateCollision() { combatColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::AttackEnd() {
	bAttacking = false;
	if (bOverlapingCombatSphere) {
		float AttackTime = FMath::RandRange(AttackMinTime, AttackMaxTime);
		GetWorldTimerManager().SetTimer(AttackTimer, this,&AEnemy::Attack, AttackTime);
		
	}
}
void AEnemy::Attack() {
	if (!bCanAttack) return;
	if (AIController) {
		AIController->StopMovement();
		SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Attack);
	}
	
	if (!bAttacking) {
		
		bAttacking = true;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance) {
			
			AnimInstance->Montage_Play(CombatMontage, 1.35f);
			AnimInstance->Montage_JumpToSection(FName("Attack"), CombatMontage);
		}
		if (SwingSound) {
			UGameplayStatics::PlaySound2D(this,SwingSound);
		}
	}
}
float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	if (health - DamageAmount <= 0.0f) {
		health = 0.0;
		Die(DamageCauser);
	}
	else {

		health = health - DamageAmount;
	}
	return DamageAmount;
}

void  AEnemy::Die(AActor* Causer) {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	GetWorldTimerManager().ClearTimer(AttackTimer);
	if (AnimInstance && CombatMontage) {
		AnimInstance->Montage_Play(CombatMontage, 1.0);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	SetEnemyMovementStatus(EEnemyMovementStatus::EMS_Dead);
	combatColision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    agroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	combatSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AMain* main = Cast<AMain>(Causer);
	if(main) {
		main->UpdateTarget();
	}
}
void AEnemy::DeathEnd() {
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
	GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}
bool AEnemy::Alive() { 
	return EEnemyMovementStatus::EMS_Dead != enemyStatus;
}
void AEnemy::Disappear() {
	Destroy();
}