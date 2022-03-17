// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsDeclares.h"
#include "ImmediateModeTest.generated.h"

UCLASS(Blueprintable)
class CLIENTPREDICTIONEX_API AImmediateModeTest : public AActor {
	GENERATED_BODY()

	public:
		// Sets default values for this actor's properties
		AImmediateModeTest();

		virtual ~AImmediateModeTest() override;

	protected:
		// Called when the game starts or when spawned
		virtual void BeginPlay() override;

		void CollectObjects();
	
	public:
		// Called every frame
		virtual void Tick(float DeltaTime) override;

	private:

		UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent* Sphere;
	
		ImmediatePhysics::FSimulation* PhysicsSimulation;
		ImmediatePhysics::FActorHandle* SphereActorHandle;

};
