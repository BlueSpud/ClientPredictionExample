#include "BallPawn.h"

#include "PhysicsProxy/SingleParticlePhysicsProxy.h"

ABallPawn::ABallPawn() {
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);

	ClientPredictionComponent = CreateDefaultSubobject<UClientPredictionV2Component>(TEXT("ClientPredictionComponent"));
}

void ABallPawn::PostInitProperties() {
	Super::PostInitProperties();

	SetReplicatingMovement(false);
}

void ABallPawn::PostRegisterAllComponents() {
	Super::PostRegisterAllComponents();

	auto Delegates = ClientPredictionComponent->CreateSimulation<FBallPawnTraits>();
	Delegates->ProduceInputGTDelegate.AddUObject(this, &ABallPawn::ProduceInput);

	Delegates->SimTickPrePhysicsDelegate.AddUObject(this, &ABallPawn::SimTickPrePhysicsDelegate);
	Delegates->SimTickPostPhysicsDelegate.AddUObject(this, &ABallPawn::SimTickPostPhysicsDelegate);

	Delegates->FinalizeDelegate.AddUObject(this, &ABallPawn::FinalizeSim);

	Delegates->RegisterEvent<FBallPawnHitGroundEvent>().AddUFunction(this, TEXT("OnHitGround"));
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

void ABallPawn::SimTickPrePhysicsDelegate(const ClientPrediction::FSimTickInfo& TickInfo, const FBallPawnInputPacket& Input, const FBallPawnState& PrevState,
                                          ClientPrediction::FTickOutput<FBallPawnState>& Output) {
	Chaos::FRigidBodyHandle_Internal* Handle = GetPhysHandle(TickInfo);
	if (Handle == nullptr) { return; }

	if (Input.bIsApplyingForce) {
		Handle->AddForce(Chaos::FVec3(0.0, 0.0, 1000000.0));
	}

	if (Input.ForceVector.Size() > 0.0) {
		Handle->AddForce(Input.ForceVector.GetSafeNormal() * 1000000.0);
	}
}

void ABallPawn::SimTickPostPhysicsDelegate(const ClientPrediction::FSimTickInfo& TickInfo, const FBallPawnInputPacket& Input, const FBallPawnState& PrevState,
                                           ClientPrediction::FTickOutput<FBallPawnState>& Output) {
	Chaos::FRigidBodyHandle_Internal* Handle = GetPhysHandle(TickInfo);
	if (Handle == nullptr) { return; }

	const UWorld* World = TickInfo.UpdatedComponent->GetWorld();
	if (World == nullptr) { return; }

	FTransform BallTransform = {Handle->R(), Handle->X()};
	FVector Start = BallTransform.GetLocation();
	FVector End = Start + FVector(0.0, 0.0, -100.0);

	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	QueryParams.AddIgnoredActor(TickInfo.UpdatedComponent->GetOwner());
	QueryParams.bTraceComplex = false;

	FHitResult Result;
	Output.State.bIsOnGround = World->LineTraceSingleByChannel(Result, Start, End, ECC_WorldStatic, QueryParams);
	Output.State.Brightness = FMath::Clamp(Handle->V().Size() / 5000.0, 0.0, 1.0);

	if (!PrevState.bIsOnGround && Output.State.bIsOnGround) {
		Output.DispatchEvent<FBallPawnHitGroundEvent>({});
	}
}

Chaos::FRigidBodyHandle_Internal* ABallPawn::GetPhysHandle(const ClientPrediction::FSimTickInfo& TickInfo) {
	FBodyInstance* BodyInstance = TickInfo.UpdatedComponent->GetBodyInstance();
	if (BodyInstance == nullptr) { return nullptr; }

	FPhysicsActorHandle Actor = BodyInstance->GetPhysicsActorHandle();
	if (Actor == nullptr) { return nullptr; }

	return Actor->GetPhysicsThreadAPI();
}

void ABallPawn::FinalizeSim(const FBallPawnState& State, Chaos::FReal Dt) {
	OnGroundChanged(State.bIsOnGround, State.bIsFullState, State.Brightness);
}
