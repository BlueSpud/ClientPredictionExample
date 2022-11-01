#include "BallClientPredictionModel.h"

void BallClientPredictionModel::SimulatePhysics(Chaos::FReal Dt, UPrimitiveComponent* Component, FPhysicsContext& Context, const FBallState& PrevState, SimOutput& Output, const FInputPacket& Input) {
	if (Input.bIsApplyingForce) {
		Context.AddForce(Chaos::FVec3(0.0, 0.0, 1000000.0));
	}
	
	if (Input.ForceVector.Size() > 0.0) {
		Context.AddForce(Input.ForceVector.GetSafeNormal() * 1000000.0);
	}

	
}

void BallClientPredictionModel::PostSimulatePhysics(Chaos::FReal Dt, UPrimitiveComponent* Component, const FPhysicsContext& Context, const FBallState& PrevState, SimOutput& Output, const FInputPacket& Input) {
	FTransform BallTransform = Context.GetTransform();
	FVector Start = BallTransform.GetLocation();
	FVector End   = Start + FVector(0.0, 0.0, -100.0);

	FHitResult Result;
	Output.State().bIsOnGround = Context.LineTraceSingle(Result, Start, End);
}
