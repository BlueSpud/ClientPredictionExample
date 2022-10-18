#include "BallClientPredictionModel.h"

void BallClientPredictionModel::Simulate(Chaos::FReal Dt, UPrimitiveComponent* Component, const FPhysicsContext& Context, const FBallState& PrevState, SimOutput& OutState, const FInputPacket& Input) {
	if (Input.bIsApplyingForce) {
		Context.AddForce(Chaos::FVec3(0.0, 0.0, 1000000.0));
	}
	
	if (Input.ForceVector.Size() > 0.0) {
		Context.AddForce(Input.ForceVector.GetSafeNormal() * 1000000.0);
	}

	FTransform BallTransform = Context.GetTransform();
	FVector Start = BallTransform.GetLocation();
	FVector End   = Start + FVector(0.0, 0.0, -100.0);

	FHitResult Result;
	OutState.State().bIsOnGround = Context.LineTraceSingle(Result, Start, End);
}
