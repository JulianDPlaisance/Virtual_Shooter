// Fill out your copyright notice in the Description page of Project Settings.


#include "SteeringActor_CPP.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/ActorComponent.h"

// Sets default values
ASteeringActor_CPP::ASteeringActor_CPP()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Skeletal Mesh"));
	StaticMeshComponent->SetupAttachment(SceneComponent);
	//StaticMeshComponent->GetMass();
	SteeringBehavior = CreateDefaultSubobject<USteeringBehavior_CPP>(TEXT("Steering Behavior"));

}

// Called when the game starts or when spawned
void ASteeringActor_CPP::BeginPlay()
{
	Super::BeginPlay();
	
}
//Construction Script
void ASteeringActor_CPP::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
}

// Called every frame
void ASteeringActor_CPP::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

