#pragma once

#include "Physics/ClientPredictionPhysicsModel.h"

struct FInputPacket {

	/** Temporary test input */
	bool bIsApplyingForce = false;

	FVector ForceVector;

	void NetSerialize(FArchive& Ar) {
		Ar << bIsApplyingForce;
		Ar << ForceVector;
	}
};

struct FBallState {

	// true if the ball is on the ground, false otherwise.
	bool bIsOnGround = false;
	
	void NetSerialize(FArchive& Ar) {
		Ar << bIsOnGround;
	}
	
	void Rewind(class UPrimitiveComponent* Component) const {
		// No-op, no special logic
	}
	
	void Interpolate(float Alpha, const FBallState& Other) {
		// No-op, always use this state's value
	}
	
	bool operator ==(const FBallState& Other) const {
		return Other.bIsOnGround == bIsOnGround;
	}
	
};

class BallClientPredictionModel : public BaseClientPredictionPhysicsModel<FInputPacket, FBallState> {
protected:
	virtual void Simulate(Chaos::FReal Dt, UPrimitiveComponent* Component, const FPhysicsContext& Context, const FBallState& PrevState, FBallState& OutState, const FInputPacket& Input) override;
};