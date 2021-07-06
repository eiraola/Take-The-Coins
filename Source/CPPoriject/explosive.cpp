// Fill out your copyright notice in the Description page of Project Settings.


#include "explosive.h"
#include "Main.h"

Aexplosive::Aexplosive() {
	damage = 15.f;
}

void Aexplosive::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	Super::OnOverlapBegin( OverlappedComponent,  OtherActor, OtherComp, OtherBodyIndex, bFromSweep,  SweepResult);
	UE_LOG(LogTemp, Warning, TEXT("StartExplosive"));
	if (OtherActor) {
		AMain* Main = Cast<AMain>(OtherActor);
		if (Main) {
			Main->DecrementHealth(damage);
			Destroy();
		}
	
	}


}

void Aexplosive::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	Super::OnOverlapEnd(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
	UE_LOG(LogTemp, Warning, TEXT("EndExplosive"));
}
