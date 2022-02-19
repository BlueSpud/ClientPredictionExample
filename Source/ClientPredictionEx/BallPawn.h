// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "ClientPredictionPhysicsComponent.h"

#include "BallPawn.generated.h"

UCLASS()
class CLIENTPREDICTIONEX_API ABallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABallPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UFUNCTION()
	void ProduceInput(FInputPacket& Packet);

	UPROPERTY(EditDefaultsOnly)
	UClientPredictionPhysicsComponent* PhysicsComponent;
	
	
};
