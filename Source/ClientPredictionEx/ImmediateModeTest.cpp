// Fill out your copyright notice in the Description page of Project Settings.


#include "ImmediateModeTest.h"

#include "Chaos/DebugDrawQueue.h"
#include "Components/SphereComponent.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsChaos/ImmediatePhysicsActorHandle_Chaos.h"
#include "Physics/ImmediatePhysics/ImmediatePhysicsChaos/ImmediatePhysicsSimulation_Chaos.h"


// Sets default values
AImmediateModeTest::AImmediateModeTest() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PhysicsSimulation = new ImmediatePhysics::FSimulation();

	RootComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ROOT"));
	Sphere = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SPHERE"));
	Sphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

AImmediateModeTest::~AImmediateModeTest() {
	delete PhysicsSimulation;
}

// Called when the game starts or when spawned
void AImmediateModeTest::BeginPlay() {
	Super::BeginPlay();
	
	SphereActorHandle = PhysicsSimulation->CreateActor(ImmediatePhysics::EActorType::DynamicActor, &Sphere->BodyInstance, Sphere->GetComponentTransform());
	check(SphereActorHandle);
	SphereActorHandle->SetEnabled(true);
	PhysicsSimulation->SetNumActiveBodies(1, {0});

	CollectObjects();
	
	float Mass = SphereActorHandle->GetMass();
	UE_LOG(LogTemp, Warning, TEXT("Mass %f"), Mass);

	PhysicsSimulation->SetSolverIterations(0.016f, 5, 5, 5, 5, 5, 5);
}

void AImmediateModeTest::CollectObjects() {
	UWorld* UnsafeWorld = GetWorld();
	FPhysScene* PhysScene = UnsafeWorld->GetPhysicsScene();
	
	if ((UnsafeWorld != nullptr) && (PhysScene != nullptr))
	{
		TArray<FOverlapResult> Overlaps;
		UnsafeWorld->OverlapMultiByChannel(Overlaps, GetActorLocation(), FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(10000.0), FCollisionQueryParams::DefaultQueryParam, FCollisionResponseParams(ECR_Overlap));

		for (const FOverlapResult& Overlap : Overlaps)
		{
			if (UPrimitiveComponent* OverlapComp = Overlap.GetComponent())
			{
				// FWorldObject* WorldObject = ComponentsInSim.Find(OverlapComp);
				// if (WorldObject != nullptr)
				// {
				// 	// Existing object - reset its age
				// 	WorldObject->LastSeenTick = ComponentsInSimTick;
				// }
				// else
				{
					// New object - add it to the sim
					const bool bIsSelf = (this == OverlapComp->GetOwner());
					if (!bIsSelf)
					{
						// Create a kinematic actor. Not using Static as world-static objects may move in the simulation's frame of reference
						ImmediatePhysics::FActorHandle* ActorHandle = PhysicsSimulation->CreateActor(ImmediatePhysics::EActorType::KinematicActor, &OverlapComp->BodyInstance, OverlapComp->GetComponentTransform());
						PhysicsSimulation->AddToCollidingPairs(ActorHandle);
					}
				}
			}
		}
	}
}

// Called every frame
void AImmediateModeTest::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	PhysicsSimulation->Simulate_AssumesLocked(0.016, 1.0, 5, FVector(0.0, 0.0, -980.0));
	Sphere->SetWorldTransform(SphereActorHandle->GetWorldTransform());
}

