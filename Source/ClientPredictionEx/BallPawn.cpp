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
}

void ABallPawn::ProduceInput(FInputPacket& Packet) {
	if (InputComponent == nullptr) {
		return;
	}

	Packet.bIsApplyingForce = GetInputAxisValue(TEXT("Jump")) == 1.0;
}

