// Fill out your copyright notice in the Description page of Project Settings.


#include "MainController.h"
#include "Blueprint/UserWidget.h"
void AMainController::BeginPlay() {
	Super::BeginPlay();
	if (HUDOverlayAsset) {
		HUDOverlay = CreateWidget<UUserWidget>(this, HUDOverlayAsset);
	}
	HUDOverlay->AddToViewport();
	HUDOverlay->SetVisibility(ESlateVisibility::Visible);

	if (WEnemyHealthBar) {
		EnemyHealthBar = CreateWidget<UUserWidget>(this, WEnemyHealthBar);
		if (EnemyHealthBar) {
			EnemyHealthBar->AddToViewport();
			EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
			bEnemyHealthbarVisible = false;
		}
	}
}
void AMainController::DisplayEnemyHealthBar() {
	bEnemyHealthbarVisible = true;
	EnemyHealthBar->SetVisibility(ESlateVisibility::Visible);
}
void AMainController::RemoveEnemyHealthBar() {
	bEnemyHealthbarVisible = false;
	EnemyHealthBar->SetVisibility(ESlateVisibility::Hidden);
}
void AMainController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	if (EnemyHealthBar) {
		FVector2D PositionInViewport;
		ProjectWorldLocationToScreen(EnemyLocation, PositionInViewport);
		FVector2D SizeInViewPort = FVector2D(200.f, 25.f);
		PositionInViewport.Y -= 85;

		EnemyHealthBar->SetPositionInViewport(PositionInViewport);
		EnemyHealthBar->SetDesiredSizeInViewport(SizeInViewPort);
	}
	FVector2D Alignment(0.f, 0.f);
	EnemyHealthBar->SetAlignmentInViewport(Alignment);
}