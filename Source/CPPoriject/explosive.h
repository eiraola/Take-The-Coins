// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "explosive.generated.h"

/**
 *
 */
UCLASS()
class CPPORIJECT_API Aexplosive : public AItem
{
	GENERATED_BODY()
		Aexplosive();

public:
		
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float damage;
};
