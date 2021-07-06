// Fill out your copyright notice in the Description page of Project Settings.


#include "floater.h"

// Sets default values
Afloater::Afloater()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	staticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));
	InitialLocation = FVector(0.0f, 0.0f, 0.0f);
	PlacedLocation = FVector(0.0f, 0.0f, 0.0f);
	WorldOrigin = FVector(0.0f, 0.0f, 0.0f);
	InitialDirection = FVector(0.0f, 0.0f, 0.0f);
	InitialForce = FVector(200000000.0f, 0.0f, 200000000.0f);
	InitialTorque = FVector(200000000.0f, 0.0f, 200000000.0f);
	bShouldFloat = false;

	RunningTime = 0.0f;
	amplitude = 1;
	period = 1;
	shift = 0;
}

// Called when the game starts or when spawned
void Afloater::BeginPlay()
{
	Super::BeginPlay();
	float InitialX = FMath::FRand();
	float InitialY = FMath::FRand();
	float InitialZ = FMath::FRand();
	PlacedLocation = GetActorLocation();
	if (bInitializeFloaterLocations) {
		SetActorLocation(InitialLocation);
	}
	FHitResult Hitresult;
	FVector LocalOffset = FVector(200.0f, 0.0f, .0f);
	InitialDirection.X = InitialX;
	InitialDirection.Y = InitialY;
	InitialDirection.Z = InitialZ;
	AddActorLocalOffset(InitialDirection*200, true, &Hitresult);

	FRotator Rotation = FRotator(30.0f, 0.0f, 0.0f);
	AddActorLocalRotation(Rotation);

	staticMesh->AddTorque(InitialTorque);
	//staticMesh->AddForce(InitialForce);
	
}

// Called every frame
void Afloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bShouldFloat) {
		FVector NewLocation = GetActorLocation();
		NewLocation.Z = NewLocation.Z + amplitude * FMath::Sin(RunningTime * period) + shift;
		SetActorLocation(NewLocation);
		RunningTime += DeltaTime;

		/*
		FHitResult Hitresult;
		AddActorLocalOffset(InitialDirection,true, &Hitresult);

		FVector HitLocation = Hitresult.Location;
		UE_LOG(LogTemp, Warning, TEXT("Hit location: X = %f, Y = %f, Z = %f"), HitLocation.X, HitLocation.Y, HitLocation.Z)
		*/
	}

}

