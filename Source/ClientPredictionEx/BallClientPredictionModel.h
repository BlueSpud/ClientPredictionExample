#pragma once

#include "Physics/ClientPredictionPhysicsModel.h"

enum EBallCueSet: uint8 {};

struct FInputPacket {

	/** Temporary test input */
	bool bIsApplyingForce = false;

	FVector ForceVector = FVector::ZeroVector;

	void NetSerialize(FArchive& Ar) {
		Ar << bIsApplyingForce;
		Ar << ForceVector;
	}
};

struct FBallState {

	/** true if the ball is on the ground, false otherwise. */
	bool bIsOnGround = false;

    /** Boolean to demonstrate how bSerializeFullState is used */
	bool bIsFullState = true;

	/**
	 * bSerializeFullState can be used to avoid sending extra data to simulated proxies.
	 * If it is false, that data shouldn't be serialized. You should ensure that if bSerializeFullState,
	 * the other values on the state are default initialized / set to reasonable values.
	 */
	void NetSerialize(FArchive& Ar, bool bSerializeFullState) {
		Ar << bIsOnGround;
		bIsFullState = bSerializeFullState;
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
	virtual void SimulatePhysics(Chaos::FReal Dt, UPrimitiveComponent* Component, FPhysicsContext& Context, const FBallState& PrevState, SimOutput& Output, const FInputPacket& Input) override;
	virtual void PostSimulatePhysics(Chaos::FReal Dt, UPrimitiveComponent* Component, const FPhysicsContext& Context, const FBallState& PrevState, SimOutput& Output, const FInputPacket& Input) override;
};