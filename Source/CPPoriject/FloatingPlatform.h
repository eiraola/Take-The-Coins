// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class CPPORIJECT_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Floating platform")
	class UStaticMeshComponent* floor;
	UPROPERTY(EditAnywhere,  Category = "Floating platform")
	FVector startPoint;
	UPROPERTY(EditAnywhere, meta=(MakeEditWidget="true"), Category = "Floating platform")
	FVector endPoint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floating platform")

    float interpTime;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floating platform")
	float speed;

	FTimerHandle InterpTimer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Floating platform")
	bool bInterping;
	float distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void ToggleInterping();
	void SwapVector(FVector& vec1, FVector& vec2);

};
