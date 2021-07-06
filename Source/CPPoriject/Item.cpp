// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Sound/SoundCue.h"
#include "Main.h"
// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ColisionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionVolume"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = ColisionVolume;
	Mesh->SetupAttachment(GetRootComponent());
	
	idleParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("IdleParticlesComponent"));
	idleParticles->SetupAttachment(GetRootComponent());
	rotate = false;
	rotationRate = 0.5;

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	ColisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnOverlapBegin);
	ColisionVolume->OnComponentEndOverlap.AddDynamic(this, &AItem::OnOverlapEnd);

}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (rotate) {
		FRotator rotation = GetActorRotation();
		rotation.Yaw += rotationRate;
		SetActorRotation(rotation);

	}

}

void AItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UE_LOG(LogTemp, Warning, TEXT("Start"));
	AMain* player = Cast<AMain>(OtherActor);
	if (!player) return;
	if (OverlapParticles) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), OverlapParticles, GetActorLocation(), FRotator(0.f),true);
		
		if (sound) {
			UGameplayStatics::PlaySound2D(this,sound);
		}
	}
}

void AItem::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	UE_LOG(LogTemp, Warning, TEXT("End"));
}


