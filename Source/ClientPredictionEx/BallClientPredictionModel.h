#pragma once

#include "ClientPredictionPhysicsModel.h"

struct FInputPacket {

	/** Temporary test input */
	bool bIsApplyingForce = false;

	void NetSerialize(FArchive& Ar) {
		Ar << bIsApplyingForce;
	}
};

class BallClientPredictionModel : public BaseClientPredictionPhysicsModel<FInputPacket, FEmptyState> {

protected:
	
	virtual void Simulate(Chaos::FReal Dt, UPrimitiveComponent* Component, const FEmptyState& PrevState, FEmptyState& OutState, const FInputPacket& Input) override;
};