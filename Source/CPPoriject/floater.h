// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "floater.generated.h"

UCLASS()
class CPPORIJECT_API Afloater : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	Afloater();
	//UPROPERTY(VisibleAnywhere, Category = "ActorMeshComponents")
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "ActorMeshComponents")
	UStaticMeshComponent* staticMesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FloaterVectors")
	FVector InitialLocation;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "FloaterVectors")
	FVector PlacedLocation;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "FloaterVectors")
	FVector WorldOrigin;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVectors")
	FVector InitialDirection;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "FloaterVectors")
	FVector InitialForce;
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, Category = "FloaterVectors")
	FVector InitialTorque;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVectors")
	bool bShouldFloat;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FloaterVectors")
	bool bInitializeFloaterLocations;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVectors")
	float amplitude;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVectors")
	float period;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVectors")
	float shift;
private:
	float RunningTime;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
