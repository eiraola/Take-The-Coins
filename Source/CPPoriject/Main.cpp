// Fill out your copyright notice in the Description page of Project Settings.


#include "Main.h"
#include "Enemy.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Sound/SoundCue.h"
#include "MainController.h"

// Sets default values
AMain::AMain()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//CreateCamera Boom(pulls towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	FollowCamera->bUsePawnControlRotation = false;

	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	GetCapsuleComponent()->SetCapsuleSize(35.f,108.f);

	
	MaxHealth = 100.f;
	MaxStamina = 150.f;
	health = 100.f;
	stamina= 150.f;
	Coins = 0;
	runningSpeed = 650.f;
	sprintSpeed = 800.f;
	bShiftKeyDown = false;
	InterSpeed = 15.f;
	bInterpToEnemy = false;
	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::EMS_Normal;
	staminaDrainRate = 25.f;
	minSprintStamina = 50.f;
	bMovingForward = false;
	bMovingRight = false;
	bLMBDown = false;
	bHasCombatTarget = false;
}

// Called when the game starts or when spawned
void AMain::BeginPlay()
{
	Super::BeginPlay();
	mainController = Cast<AMainController>(GetController());
	
}

// Called every frame
void AMain::Tick(float DeltaTime)
{
	if (bShiftKeyDown) {
		UE_LOG(LogTemp, Warning, TEXT("StaminaLevel %f"), stamina);
	}
	Super::Tick(DeltaTime);
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	float DeltaStamina = staminaDrainRate * DeltaTime;
	switch (StaminaStatus)
	{
	case EStaminaStatus::EMS_Normal:
		if (bShiftKeyDown) {
			if ((stamina - DeltaStamina) <= minSprintStamina) {
				SetStaminaStatus(EStaminaStatus::EMS_BellowMinimum);
				stamina -= DeltaStamina;
			}
			else {
				stamina -= DeltaStamina;
			}
			if (bMovingRight || bMovingForward) {
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		} else {
			if ((stamina + DeltaStamina) <= MaxStamina) {
				stamina += DeltaStamina;
			}
			else {
				stamina = MaxStamina;
			}
			
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::EMS_BellowMinimum:
		if (bShiftKeyDown) {
			if (stamina - DeltaStamina <= 0.f) {
				SetStaminaStatus(EStaminaStatus::EMS_Exhausted);
				stamina = 0;
				SetMovementStatus(EMovementStatus::EMS_Normal);
			} else {
				stamina -= DeltaStamina;
				if (bMovingRight || bMovingForward) {
					SetMovementStatus(EMovementStatus::EMS_Sprinting);
				}
			}
		} else {

			if (stamina + DeltaStamina >= minSprintStamina) {
				SetStaminaStatus(EStaminaStatus::EMS_Normal);
				stamina += DeltaStamina;
			}
			else {
				stamina += DeltaStamina;
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;
	case EStaminaStatus::EMS_Exhausted:
		if (bShiftKeyDown) {
			stamina = 0.f;
		
		} else {
			SetStaminaStatus(EStaminaStatus::EMS_Recovering);
			stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	case EStaminaStatus::EMS_Recovering:
		if (stamina + DeltaStamina >= minSprintStamina) {
			SetStaminaStatus(EStaminaStatus::EMS_Normal);
			stamina += DeltaStamina;
		} else {
			stamina += DeltaStamina;
		}
		SetMovementStatus(EMovementStatus::EMS_Normal);
		break;
	default:
		break;
	}
	if (bInterpToEnemy && combatTarget) {
		FRotator LookAtYaw = GetLookAtRotationYaw(combatTarget->GetActorLocation());
		FRotator interpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterSpeed);
		SetActorRotation(interpRotation);
	}
	if (combatTarget) {
		combatTargetLocation = combatTarget->GetActorLocation();
		if (mainController) {
			mainController->EnemyLocation = combatTargetLocation;
		}
	}
}
FRotator  AMain::GetLookAtRotationYaw(FVector target) {
	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), target);
	FRotator LookAtRotationYaw(0.f, LookAtRotator.Yaw, 0.f);
	return LookAtRotationYaw;

}
// Called to bind functionality to input
void AMain::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMain::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMain::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMain::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMain::ShiftKeyUp);
	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMain::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMain::LMBUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMain::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMain::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMain::TurnRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMain::LookUpRate);

}

void AMain::MoveForward(float value) {
	bMovingForward = false;
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	if ((Controller != nullptr) && (value != 0.0f) && (!bAttacking)) {
		bMovingForward = true;
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation = FRotator(0.f, rotation.Yaw, 0.f);

		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(direction, value);
	}
}
/*Called for forwards/backwards input*/
void AMain::MoveRight(float value) {
	bMovingRight = false;
	if (MovementStatus == EMovementStatus::EMS_Dead) return;
	if ((Controller != nullptr) && (value != 0.0f) && (!bAttacking)) {
		bMovingRight = true;
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRotation = FRotator(0.f, rotation.Yaw, 0.f);

		const FVector direction = FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}
}

void AMain::TurnRate(float value) {
	AddControllerYawInput(value * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMain::LookUpRate(float value) {
	AddControllerPitchInput(value * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMain::DecrementHealth(float amount) {
	health = health - amount;
	if ((health ) <= 0) {
		Die();
	}
}
void AMain::Jump(){
	if (MovementStatus == EMovementStatus::EMS_Dead) { return; }
	ACharacter::Jump();
}
void AMain::Die() {
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && CombatMontage) {
		AnimInstance->Montage_Play(CombatMontage, 1.0);
		AnimInstance->Montage_JumpToSection(FName("Death"));
	}
	SetMovementStatus(EMovementStatus::EMS_Dead);
	return;
}
void  AMain::addCoins(float amount) {
	Coins++;
}

void AMain::SetMovementStatus(EMovementStatus Status) {

	MovementStatus = Status;
	if (MovementStatus == EMovementStatus::EMS_Sprinting) {
		GetCharacterMovement()->MaxWalkSpeed = sprintSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = runningSpeed;
	}
}


void AMain::ShiftKeyDown() {
	bShiftKeyDown = true;
	UE_LOG(LogTemp, Warning, TEXT("ShiftDown"));
}

void AMain::ShiftKeyUp() {
	bShiftKeyDown = false;
UE_LOG(LogTemp, Warning, TEXT("ShiftUP"));
}

void AMain::LMBDown() {


	bLMBDown = true;
	if (MovementStatus == EMovementStatus::EMS_Dead) { return; }

	if (ActiveOverlapingItem) {
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlapingItem);
		if (Weapon) {
			UE_LOG(LogTemp, Warning, TEXT("EsWeapon"));
			Weapon->Equip(this);
			SetActiveOverlapedItem(nullptr);
		}

	}
	else if (EquippedWeapon) {
		Attack();
	}
}
void AMain::LMBUp() {
	bLMBDown = false;
}
void AMain::Attack() {
	if (MovementStatus == EMovementStatus::EMS_Dead) { return; }
	if (!bAttacking) {
		SetInterpToEnemy(true);
		bAttacking = true;

		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		if (AnimInstance && CombatMontage) {
			char name[10] = "";
			int32 Section = FMath::RandRange(0, 1);
			switch (Section) {

			case 0:
				AnimInstance->Montage_Play(CombatMontage, 2.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);

				break;
			case 1:
				AnimInstance->Montage_Play(CombatMontage, 2.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;
			default:
				AnimInstance->Montage_Play(CombatMontage, 2.35f);
				AnimInstance->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
				break;

			}

		}
		if (EquippedWeapon->SwingSound) {
			UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
		}
	}

}
void AMain::AttackEnd() {

	bAttacking = false;
	SetInterpToEnemy(false);
	if (bLMBDown) {
		Attack();
	}
}
void AMain::SetInterpToEnemy(bool Interp) {
	bInterpToEnemy = Interp;
}
float AMain::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	if (MovementStatus == EMovementStatus::EMS_Dead) {
		AEnemy* damager = Cast<AEnemy>(DamageCauser);
		if (damager) {
			damager->SetEnemyCanAttack(false);
		}
		return DamageAmount;
	}
	DecrementHealth(DamageAmount);
	return DamageAmount;
}
void AMain::DeathEnd() {
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

}
void AMain::UpdateTarget() {
	TArray<AActor*> overlapingActors;
	GetOverlappingActors(overlapingActors, EnemyFilter);
	if (overlapingActors.Num() == 0) {
		if(mainController){
			mainController->RemoveEnemyHealthBar();
		}
		return;
	}
	AEnemy* closestEnemy = Cast<AEnemy>(overlapingActors[0]);
	if (closestEnemy) {
		float minDistance = (closestEnemy->GetActorLocation() - GetActorLocation()).Size();

		for (auto actor : overlapingActors)
		{
			AEnemy* enemy = Cast<AEnemy>(actor);
			if (enemy) {
				float distanceToActor = (enemy->GetActorLocation() - GetActorLocation()).Size();
				if (minDistance > distanceToActor) {
					minDistance = distanceToActor;
					closestEnemy = enemy;
				}
			}
		}
		if (mainController) {
			mainController->DisplayEnemyHealthBar();
		}
		SetCombatTarget(closestEnemy);
		bHasCombatTarget = true;
	}
}

void AMain::SwitchLevel(FName LevelName){
	UWorld* world = GetWorld();
	if (world) {
		FString CurrentLevel = world->GetMapName();
		FName CurrentLevelName(*CurrentLevel);
		if(CurrentLevelName != LevelName){
			if(Coins>=10){
				UGameplayStatics::OpenLevel(world, LevelName);
			}
		}

	}
}