#pragma once

#include "ClientPredictionPhysicsModel.h"

struct FInputPacket {

	/** Temporary test input */
	bool bIsApplyingForce = false;

	FVector ForceVector;

	void NetSerialize(FArchive& Ar) {
		Ar << bIsApplyingForce;
		Ar << ForceVector;
	}
};

class BallClientPredictionModel : public BaseClientPredictionPhysicsModel<FInputPacket, FEmptyState> {
protected:
	virtual void Simulate(Chaos::FReal Dt, UPrimitiveComponent* Component, ImmediatePhysics::FActorHandle* Handle, const FEmptyState& PrevState, FEmptyState& OutState, const FInputPacket& Input) override;
};