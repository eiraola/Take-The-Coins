// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	floor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	RootComponent = floor;
	startPoint = FVector(0.f);
	endPoint = FVector(0.f);
	speed = 4.0f;
	
	interpTime = 1.0f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{

	Super::BeginPlay();
	
	startPoint = GetActorLocation();
	endPoint += startPoint;

	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, interpTime);
	distance = (endPoint - startPoint).Size();
	
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bInterping) {
	FVector CurrentLocation = GetActorLocation(); 
	FVector Interp = FMath::VInterpConstantTo(CurrentLocation,endPoint, DeltaTime, speed);
	SetActorLocation(Interp);
	if (FMath::Abs((GetActorLocation() - startPoint).Size() - distance) < 0.5f){
			ToggleInterping();
			GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, interpTime);
			SwapVector(startPoint,endPoint);
		}
	//UE_LOG(LogTemp, Error, TEXT("PhysicsHandle working %s"), *Interp.ToString());
	}
	
	
}
void AFloatingPlatform::ToggleInterping() {
	bInterping = !bInterping;

}
void AFloatingPlatform::SwapVector(FVector& vec1, FVector& vec2) {
	FVector aux = vec1;
	vec1 = vec2;
	vec2 = aux;

}

