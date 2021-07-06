// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

UCLASS()
class CPPORIJECT_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Category = "item | Collision")
	class USphereComponent* ColisionVolume;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "item | Mesh")
    UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "item | Particle")
	class UParticleSystemComponent* idleParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | Particle")
	class UParticleSystem* OverlapParticles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | sounds")
	class USoundCue* sound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | sounds")
	bool rotate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "item | sounds")
	float rotationRate;
protected:
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
