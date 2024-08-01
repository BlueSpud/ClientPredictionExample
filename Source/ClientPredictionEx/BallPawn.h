#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "ClientPredictionV2Component.h"

#include "BallPawn.generated.h"

struct FBallPawnInputPacket {
	FVector ForceVector = FVector::ZeroVector;
	bool bIsApplyingForce = false;

	void NetSerialize(FArchive& Ar) {
		Ar << ForceVector;
		Ar << bIsApplyingForce;
	}
};

struct FBallPawnState {
	/** true if the ball is on the ground, false otherwise. */
	bool bIsOnGround = false;

	/** Boolean to demonstrate how bSerializeFullState is used */
	bool bIsFullState = true;

	float Brightness = 1.0;

	void NetSerialize(FArchive& Ar, const ClientPrediction::EDataCompleteness Completeness) {
		Ar << bIsOnGround;
		Ar << Brightness;

		bIsFullState = Completeness == ClientPrediction::EDataCompleteness::kFull;
	}

	bool ShouldReconcile(const FBallPawnState& Other) const {
		return bIsOnGround != Other.bIsOnGround
			&& FMath::Abs(Brightness - Other.Brightness) < 0.1;
	}

	void Interpolate(const FBallPawnState& Other, Chaos::FReal Alpha) {
		bIsOnGround = Other.bIsOnGround;
		Brightness = FMath::Lerp(Brightness, Other.Brightness, Alpha);
	}
};

struct FBallPawnHitGroundEvent {
	void NetSerialize(FArchive& Ar) {}
	bool NetIdentical(const FBallPawnHitGroundEvent& Other) { return true; }
};

struct FBallPawnTraits {
	using InputType = FBallPawnInputPacket;
	using StateType = FBallPawnState;
};

UCLASS()
class CLIENTPREDICTIONEX_API ABallPawn : public APawn {
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();

	virtual void PostInitProperties() override;
	virtual void PostRegisterAllComponents() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintNativeEvent)
	void OnGroundChanged(bool bIsOnGround, bool bIsFullState, float Brightness);

	UFUNCTION(BlueprintNativeEvent)
	void OnHitGround();

private:
	void ProduceInput(FBallPawnInputPacket& Packet);

	void SimTickPrePhysicsDelegate(const ClientPrediction::FSimTickInfo& TickInfo, const FBallPawnInputPacket& Input, const FBallPawnState& PrevState,
	                               ClientPrediction::FTickOutput<FBallPawnState>& Output);
	void SimTickPostPhysicsDelegate(const ClientPrediction::FSimTickInfo& TickInfo, const FBallPawnInputPacket& Input, const FBallPawnState& PrevState,
	                                ClientPrediction::FTickOutput<FBallPawnState>& Output);

	Chaos::FRigidBodyHandle_Internal* GetPhysHandle(const ClientPrediction::FSimTickInfo& TickInfo);

	void FinalizeSim(const FBallPawnState& State, Chaos::FReal Dt);

	UPROPERTY(EditDefaultsOnly)
	UClientPredictionV2Component* ClientPredictionComponent;
};
