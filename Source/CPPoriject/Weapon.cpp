// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Main.h"
#include "Enemy.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include"Components/BoxComponent.h"
#include <CPPoriject\Enemy.h>
#include "Engine/SkeletalMeshSocket.h"

AWeapon::AWeapon() {


	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(GetRootComponent());

	weaponState = EWeaponState::EWS_PickUp;

	CombatCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("CombatCollision"));
	CombatCollision->SetupAttachment(GetRootComponent());
	damage = 25.f;
	
	
}
void AWeapon::BeginPlay() {
	Super::BeginPlay();
	CombatCollision->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnCombatOverlapBegin);
	CombatCollision->OnComponentEndOverlap.AddDynamic(this, &AWeapon::OnCombatOverlapEnd);
	CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CombatCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CombatCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CombatCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	
}
void AWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if ((weaponState == EWeaponState::EWS_PickUp) && OtherActor) {
		UE_LOG(LogTemp, Warning, TEXT("Hace Overlap"));
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			UE_LOG(LogTemp, Warning, TEXT("Es Main"));
			//Equip(Main);
			Main->SetActiveOverlapedItem(this); 
		}
		
	}
}

void AWeapon::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {

		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			//Equip(Main);
			Main->SetActiveOverlapedItem(nullptr);
		}

	}
}

void AWeapon::OnCombatOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
	if (OtherActor) {
		UE_LOG(LogTemp, Warning, TEXT("Overlapeo"));
		AEnemy* enemy = Cast<AEnemy>(OtherActor);
		if (enemy) {
			UE_LOG(LogTemp, Warning, TEXT("Es enemigo"));
			if (enemy->HitParticles) {
				const USkeletalMeshSocket* WeaponSocket = SkeletalMesh->GetSocketByName("ParticleSocket");
				if (WeaponSocket) {
					FVector socketLocation = WeaponSocket->GetSocketLocation(SkeletalMesh);
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),enemy->HitParticles, socketLocation,FRotator(0.,0.,0.),false);
				}

			}
			if (enemy->HitSound) {
				UGameplayStatics::PlaySound2D(this, enemy->HitSound);
			}
			if (DamageTypeClass) {
				UGameplayStatics::ApplyDamage(enemy, damage, WeaponInstigator, this, DamageTypeClass);
			}
		
		}
	}
	
}

void AWeapon::OnCombatOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	if (OtherActor) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			//Equip(Main);
			Main->SetActiveOverlapedItem(nullptr);
		}

	}
}

void AWeapon::Equip(AMain* character) {
	if (character) {
		
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera,ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		SkeletalMesh->SetSimulatePhysics(false);

		const USkeletalMeshSocket* RightHandSock = character->GetMesh()->GetSocketByName("RightHandSocket");
		if (RightHandSock) {
			RightHandSock->AttachActor(this,character->GetMesh());
			if (character->GetEquippedWeapon())
				character->GetEquippedWeapon()->Destroy();
			character->SetEquippedWeapon(this);
			character->SetActiveOverlapedItem(nullptr);
			weaponState = EWeaponState::EWS_Equipped;
			if (OnEquipSound) {
				UGameplayStatics::PlaySound2D(this, OnEquipSound);
			}
		}

	}
}
void  AWeapon::ActivateCollision() { CombatCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly); }

void  AWeapon::DeactivateCollision() { CombatCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision); }