// Fill out your copyright notice in the Description page of Project Settings.


#include "BallPawn.h"

// Sets default values
ABallPawn::ABallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsComponent = CreateDefaultSubobject<UClientPredictionComponent>(TEXT("PhysicsComponent"));

	BallClientPredictionModel* Model = PhysicsComponent->CreateModel<BallClientPredictionModel>();
	Model->InputDelegate.BindUObject(this, &ABallPawn::ProduceInput);
	Model->OnFinalized.BindUObject(this, &ABallPawn::FinalizeSim);
	
	bReplicates = true;
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABallPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	InputComponent->BindAxis(TEXT("Jump"));
	InputComponent->BindAxis(TEXT("Move"));
	InputComponent->BindAxis(TEXT("Strafe"));
}

void ABallPawn::OnGroundChanged_Implementation(bool bIsOnGround) { /* No-op */ }

void ABallPawn::ProduceInput(FInputPacket& Packet) {
	if (InputComponent == nullptr) {
		return;
	}

	FRotator ControlRotation = GetControlRotation();
	FVector Unprojected = ControlRotation.Vector();
	
	FVector Forward = FVector(Unprojected.X, Unprojected.Y, 0.0);
	FVector Right = FVector::CrossProduct(FVector(0.0, 0.0, 1.0), Forward);
	
	Packet.ForceVector = Forward * GetInputAxisValue(TEXT("Move")) + Right * GetInputAxisValue(TEXT("Strafe"));
	Packet.bIsApplyingForce = GetInputAxisValue(TEXT("Jump")) == 1.0;
}

void ABallPawn::FinalizeSim(const FBallState& State) {
	OnGroundChanged(State.bIsOnGround);
}

