#include "BallClientPredictionModel.h"

void BallClientPredictionModel::Simulate(Chaos::FReal Dt, UPrimitiveComponent* Component, const FEmptyState& PrevState, FEmptyState& OutState, const FInputPacket& Input) {
	if (Input.bIsApplyingForce) {
		FBodyInstance* Body = Component->GetBodyInstance();
		Chaos::FRigidBodyHandle_Internal* Handle = Body->GetPhysicsActorHandle()->GetPhysicsThreadAPI();
		Handle->AddForce(Chaos::FVec3(0.0, 0.0, 1000000.0));
	}
}
