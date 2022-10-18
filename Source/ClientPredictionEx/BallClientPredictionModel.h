#pragma once

#include "Physics/ClientPredictionPhysicsModel.h"

enum EBallCueSet: uint8 {};

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

	void Print(FAnsiStringBuilderBase& Builder) const {
		// TODO implement
	}
	
	bool operator ==(const FBallState& Other) const {
		return Other.bIsOnGround == bIsOnGround;
	}
	
};

class BallClientPredictionModel : public BaseClientPredictionPhysicsModel<FInputPacket, FBallState, EBallCueSet> {
public:
	using SimOutput = FPhysicsSimulationOutput<FBallState, EBallCueSet>;
	using StateType = FPhysicsStateWrapper<FBallState>;

protected:
	virtual void Simulate(Chaos::FReal Dt, UPrimitiveComponent* Component, const FPhysicsContext& Context, const FBallState& PrevState, SimOutput& OutState, const FInputPacket& Input) override;
};