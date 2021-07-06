// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "ColliderMovementComponent.h"
// Sets default values
ACollider::ACollider()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	//RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(GetRootComponent());
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshComponentAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule'"));
	if (MeshComponentAsset.Succeeded())
	{
		MeshComponent->SetStaticMesh(MeshComponentAsset.Object);
		MeshComponent->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
		MeshComponent->SetWorldScale3D(FVector(0.8f, 0.8f, 0.8f));
	}
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));

	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;

	Camera =  CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("Movement"));
	OurMovementComponent->UpdatedComponent = RootComponent;

	CameraInput = FVector2D(0.f,0.f);
}
// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator NewRotation = GetActorRotation();
	

	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);
	FRotator NewArmRotation = SpringArm->GetComponentRotation();
	NewArmRotation.Pitch = FMath::Clamp(NewArmRotation.Pitch += CameraInput.Y, -80.f, -15.f);
	SpringArm->SetWorldRotation(NewArmRotation);
}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRigth"), this, &ACollider::MoveRigth);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::PitchCamera);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::YawCamera);

}

void ACollider::MoveForward(float value) {
	FVector Forward= GetActorForwardVector();
	AddMovementInput(value * Forward);
	if (OurMovementComponent) {
		OurMovementComponent->AddInputVector(Forward * value);
	}
}
void ACollider::MoveRigth(float value) {
	FVector Rigth = GetActorRightVector();
	AddMovementInput(value * Rigth);
	if (OurMovementComponent) {
		OurMovementComponent->AddInputVector(Rigth * value);
	}
}

UPawnMovementComponent* ACollider::GetMovementComponent() const {
	return OurMovementComponent;
}

void ACollider::PitchCamera(float Axisvalue) { CameraInput.Y = Axisvalue; }
void ACollider::YawCamera(float Axisvalue) { CameraInput.X = Axisvalue; }