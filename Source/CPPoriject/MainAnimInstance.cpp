// Fill out your copyright notice in the Description page of Project Settings.


#include "MainAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "Main.h"
void UMainAnimInstance::NativeInitializeAnimation() {
	if (pawn == nullptr) {
		pawn = TryGetPawnOwner();
		if (pawn) {
			main = Cast<AMain>(pawn);
		}
	}
}
void UMainAnimInstance::UpdateAnimationProperties() {

	if (pawn == nullptr) {
		pawn = TryGetPawnOwner();
	}
	if (pawn) {
		FVector speed = pawn->GetVelocity();
		FVector lateralSpeed = FVector(speed.X, speed.Y, 0.f);
		MovementSpeed = lateralSpeed.Size();

		bIsInAir = pawn->GetMovementComponent()->IsFalling();
		if (main == nullptr) {
			main = Cast<AMain>(pawn);
		}
	}
}