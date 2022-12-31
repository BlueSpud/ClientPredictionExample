// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "ClientPredictionComponent.h"

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

    void NetSerialize(FArchive& Ar, bool bSerializeFullState) {
        Ar << bIsOnGround;
        Ar << Brightness;

        bIsFullState = bSerializeFullState;
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

enum ETestEvents : uint8 {
    kLanding = 0
};

struct FBallPawnModel : public ClientPrediction::FPhysicsModel<FBallPawnInputPacket, FBallPawnState, ETestEvents> {
    virtual void SimulatePrePhysics(Chaos::FReal Dt, ClientPrediction::FPhysicsContext& Context, const FBallPawnInputPacket& Input, const FBallPawnState& PrevState,
                                    SimOutput& OutState) override;
    virtual void SimulatePostPhysics(Chaos::FReal Dt, const ClientPrediction::FPhysicsContext& Context, const FBallPawnInputPacket& Input,
                                     const FBallPawnState& PrevState, SimOutput& OutState) override;
};

UCLASS()
class CLIENTPREDICTIONEX_API ABallPawn : public APawn {
    GENERATED_BODY()

public:
    // Sets default values for this pawn's properties
    ABallPawn();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called to bind functionality to input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION(BlueprintNativeEvent)
    void OnGroundChanged(bool bIsOnGround, bool bIsFullState, float Brightness);

    UFUNCTION(BlueprintNativeEvent)
    void OnHitGround();

private:
    void ProduceInput(FBallPawnInputPacket& Packet);
    void FinalizeSim(const FBallPawnState& State, Chaos::FReal Dt);
    void HandleEvent(ETestEvents Event, const FBallPawnState& State, const ClientPrediction::FPhysicsState& PhysState);

    UPROPERTY(EditDefaultsOnly)
    UClientPredictionComponent* PhysicsComponent;
};
