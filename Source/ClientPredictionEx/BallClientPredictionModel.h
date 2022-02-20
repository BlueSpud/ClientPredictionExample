#pragma once

#include "ClientPredictionPhysicsModel.h"

class BallClientPredictionModel : public BaseClientPredictionPhysicsModel<FInputPacket, FEmptyState> {

protected:
	
	virtual void Simulate(Chaos::FReal Dt, UPrimitiveComponent* Component, const FEmptyState& PrevState, FEmptyState& OutState, const FInputPacket& Input) override;
};