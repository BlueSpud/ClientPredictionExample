#include "BallClientPredictionModel.h"

void BallClientPredictionModel::Simulate(Chaos::FReal Dt, UPrimitiveComponent* Component, ImmediatePhysics::FActorHandle* Handle, const FEmptyState& PrevState, FEmptyState& OutState, const FInputPacket& Input) {
	if (Input.bIsApplyingForce) {
		Handle->AddForce(Chaos::FVec3(0.0, 0.0, 1000000.0));
	}

	if (Input.ForceVector.Size() > 0.0) {
		Handle->AddForce(Input.ForceVector.GetSafeNormal() * 1000000.0);
	}
	
}
