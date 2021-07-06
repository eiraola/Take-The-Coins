// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelTransitionVolume.h"
#include "Components/BoxComponent.h"
#include "Components/BillboardComponent.h"
#include "Main.h"

// Sets default values
ALevelTransitionVolume::ALevelTransitionVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	transitionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("Transition"));
	RootComponent = transitionVolume;
	billBoard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	billBoard->SetupAttachment(transitionVolume);
	LevelTransitionName = "Demo_main_map";
}

// Called when the game starts or when spawned
void ALevelTransitionVolume::BeginPlay()
{
	Super::BeginPlay();
	transitionVolume->OnComponentBeginOverlap.AddDynamic(this, &ALevelTransitionVolume::OnOverlapBegin);
	
}

// Called every frame
void ALevelTransitionVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ALevelTransitionVolume::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	AMain* main = Cast<AMain>(OtherActor);
	UE_LOG(LogTemp, Warning, TEXT("Overlapeo"));
	if(main){
		UE_LOG(LogTemp, Warning, TEXT("Overlapeo"));
			main->SwitchLevel(LevelTransitionName);
		
	}

 }


