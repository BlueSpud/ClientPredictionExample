// Fill out your copyright notice in the Description page of Project Settings.

#include "BallPawn.h"

void FBallPawnModel::SimulatePrePhysics(Chaos::FReal Dt, ClientPrediction::FPhysicsContext& Context, const FBallPawnInputPacket& Input, const FBallPawnState& PrevState,
                                        SimOutput& OutState) {
	if (Input.bIsApplyingForce) {
		Context.AddForce(Chaos::FVec3(0.0, 0.0, 1000000.0));
	}

	if (Input.ForceVector.Size() > 0.0) {
		Context.AddForce(Input.ForceVector.GetSafeNormal() * 1000000.0);
	}
}

void FBallPawnModel::SimulatePostPhysics(Chaos::FReal Dt, const ClientPrediction::FPhysicsContext& Context, const FBallPawnInputPacket& Input,
                                         const FBallPawnState& PrevState, SimOutput& OutState) {
	FTransform BallTransform = Context.GetTransform();
	FVector Start = BallTransform.GetLocation();
	FVector End = Start + FVector(0.0, 0.0, -100.0);

	FHitResult Result;
	OutState.State().bIsOnGround = Context.LineTraceSingle(Result, Start, End);
	OutState.State().Brightness = FMath::Clamp(Context.GetLinearVelocity().Size() / 5000.0, 0.0, 1.0);

	if (!PrevState.bIsOnGround && OutState.State().bIsOnGround) {
		OutState.DispatchEvent(kLanding);
	}
}

// Sets default values
ABallPawn::ABallPawn() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsComponent = CreateDefaultSubobject<UClientPredictionComponent>(TEXT("PhysicsComponent"));

	auto* Model = PhysicsComponent->CreateModel<FBallPawnModel>();
	Model->ProduceInputDelegate.BindUObject(this, &ABallPawn::ProduceInput);
	Model->FinalizeDelegate.BindUObject(this, &ABallPawn::FinalizeSim);
	Model->DispatchEventDelegate.BindUObject(this, &ABallPawn::HandleEvent);

	bReplicates = true;
}

// Called when the game starts or when spawned
void ABallPawn::BeginPlay() {
	Super::BeginPlay();
	SetReplicateMovement(false);
}

// Called to bind functionality to input
void ABallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis(TEXT("Jump"));
	InputComponent->BindAxis(TEXT("Move"));
	InputComponent->BindAxis(TEXT("Strafe"));
}

void ABallPawn::OnGroundChanged_Implementation(bool bIsOnGround, bool bIsFullState, float Brightness) {
	/* No-op */
}

void ABallPawn::OnHitGround_Implementation() {}

void ABallPawn::ProduceInput(FBallPawnInputPacket& Packet) {
	if (InputComponent == nullptr) {
		return;
	}

	const FRotator ControlRotation = GetControlRotation();
	const FVector Unprojected = ControlRotation.Vector();

	const FVector Forward = FVector(Unprojected.X, Unprojected.Y, 0.0);
	const FVector Right = FVector::CrossProduct(FVector(0.0, 0.0, 1.0), Forward);

	Packet.ForceVector = Forward * GetInputAxisValue(TEXT("Move")) + Right * GetInputAxisValue(TEXT("Strafe"));
	Packet.bIsApplyingForce = GetInputAxisValue(TEXT("Jump")) == 1.0;
}

void ABallPawn::FinalizeSim(const FBallPawnState& State, Chaos::FReal Dt) {
	OnGroundChanged(State.bIsOnGround, State.bIsFullState, State.Brightness);
}

void ABallPawn::HandleEvent(ETestEvents Event, const FBallPawnState& State, const ClientPrediction::FPhysicsState& PhysState, const ClientPrediction::FDelayInfo& DelayInfo) {
	switch (Event) {
	case ETestEvents::kLanding:
		OnHitGround();
		break;
	default:
		break;
	};
}
