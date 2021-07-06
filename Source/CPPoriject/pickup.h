// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "pickup.generated.h"

/**
 * 
 */
UCLASS()
class CPPORIJECT_API Apickup : public AItem
{
	GENERATED_BODY()
		Apickup();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Amount")
	float amount;
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	
};
