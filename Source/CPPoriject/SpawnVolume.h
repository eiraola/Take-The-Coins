// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class CPPORIJECT_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn Volume")
	class UBoxComponent* SpawningBox;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Volume")
	TSubclassOf<class ACritter> PawnToSpawn;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintPure, Category= "Spawn Volume")
	FVector GetSpawnPoint();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Spawn Volume")
	void SpawnOurPawn(UClass* ToSpawn, const FVector& location);

};
