// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"
void UEnemyAnimInstance::NativeInitializeAnimation() {
	if (pawn == nullptr) {
		pawn = TryGetPawnOwner();
		if (pawn) {
			enemy = Cast<AEnemy>(pawn);
		}
	}
}

void UEnemyAnimInstance::UpdateAnimationProperties() {

	if (pawn == nullptr) {
		pawn = TryGetPawnOwner();
	}
	if (pawn) {
		FVector speed = pawn->GetVelocity();
		FVector lateralSpeed = FVector(speed.X, speed.Y, 0.f);
		MovementSpeed = lateralSpeed.Size();

		
		if (enemy == nullptr) {
			enemy = Cast<AEnemy>(pawn);
		}
	}
}

