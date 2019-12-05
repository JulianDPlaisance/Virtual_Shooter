// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/Classes/Engine/TargetPoint.h"
#include "Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Public/DrawDebugHelpers.h"
#include "HelperFunctions.h"
#include "GameFramework/Character.h"
#include "SteeringBehavior_CPP.generated.h"

#define TEST_BIT(Bitmask, Bit) (((Bitmask) & (1 << static_cast<uint8>(Bit))) > 0)
#define SET_BIT(Bitmask, Bit) (Bitmask |= 1 << static_cast<uint8>(Bit))
#define CLEAR_BIT(Bitmask, Bit) (Bitmask &= ~(1 << static_cast<uint8>(Bit)))

UENUM (BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ENpcBehaviors1 : uint8
{
	//None					= 0x00	UMETA(DisplayName = "None"),
	Seek					= 0x01	UMETA(DisplayName = "Seek"),
	Flee					= 0x02	UMETA(DisplayName = "Flee"),
	Wander					= 0x04	UMETA(DisplayName = "Wander"),
	Pursuit					= 0x08	UMETA(DisplayName = "Pursuit"),
	Evade					= 0x10	UMETA(DisplayName = "Evade"),
	CollisionAvoidance		= 0x20	UMETA(DisplayName = "Collision Avoidance"),
	WallAvoidance			= 0x40	UMETA(DisplayName = "Wall Avoidance"),
	Arrive					= 0x80	UMETA(DisplayName = "Arrive")
};
ENUM_CLASS_FLAGS(ENpcBehaviors1);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class ENpcBehaviors2 : uint8
{
	//None					= 0x00	UMETA(DisplayName = "None"),
	Path					= 0x01	UMETA(DisplayName = "Path")

};
ENUM_CLASS_FLAGS(ENpcBehaviors2);

UENUM(BlueprintType, meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EColor : uint8
{
	Black			= 0x01 UMETA(DisplayName = "Black"),
	Red				= 0x02 UMETA(DisplayName = "Red"),
	Green			= 0x04 UMETA(DisplayName = "Green"),
	Blue			= 0x08 UMETA(DisplayName = "Blue"),
	Yellow			= 0x10 UMETA(DisplayName = "Yellow"),
	Purple			= 0x20 UMETA(DisplayName = "Purple"),
	Cyan			= 0x40 UMETA(DisplayName = "Cyan"),
	White			= 0x80 UMETA(DisplayName = "White")
};
ENUM_CLASS_FLAGS(EColor);

UCLASS( ClassGroup=(Custom), Blueprintable, meta=(BlueprintSpawnableComponent) )
class MVPSHOOTER_API USteeringBehavior_CPP : public UActorComponent
{
	GENERATED_BODY()

public:	


	// Sets default values for this component's properties
	USteeringBehavior_CPP();

	// Updates when values change in the editor, used for the Enum Bitmask
	void PostEditChangeProperty(FPropertyChangedEvent& e);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Avoids all actors with actor Tag of "Wall". If actor is not tagged behavior will ignore that game object.
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "NPC Behavior")
		FVector WallAvoidance();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


	// Only runs in editor, not when game is played
#if WITH_EDITOR

	// Preventing users from editing variables when bitmask flag is false
	virtual bool CanEditChange(const UProperty* InProperty) const override;
#endif

	// Flags which behaviors are being used on actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Behavior", meta = (DisplayName="Activate NPC Behavior Flag(s)", Bitmask, BitmaskEnum = "ENpcBehaviors1"))
	uint8 bActivateNPCBehavior1;

	// Flags which behaviors are being used on actor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "NPC Behavior", meta = (DisplayName = "Activate NPC Behavior Flag(s)", Bitmask, BitmaskEnum = "ENpcBehaviors2"))
	uint8 bActivateNPCBehavior2;


private:

		/**************** Target Points ****************/
		//Static target (doesn't move)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Behavior", meta = (AllowPrivateAccess = "true"))
			AActor* StaticTarget;
		//Dynamic target (Moves continuously)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC Behavior", meta = (AllowPrivateAccess = "true"))
			AActor* DynamicTarget;

		/**************** Steering Behavior Functions ****************/
		// Returns whether a flag is set to true on our NPC Enum Bitmask
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			bool isSet(ENpcBehaviors1 Behavior = ENpcBehaviors1::Seek) const;

		bool isSet(ENpcBehaviors2 Behavior) const;

		// Flee behavior where actor moves away from static target
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			FVector Flee(FVector Location);

		// Evade behavior where actor moves away from dynamic target prediciting its future position
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			FVector Evade(FVector Location, FVector Veloc);

		// Pursue dynamic target predicting its future position
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			FVector Pursuit(FVector Location, FVector Veloc);

		// Go towards target in non-linear fashion with some random jitter
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			FVector Wander();

		// Move away from solid objects that you can collide with
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			FVector CollisionAvoidance();

		// Adds up all steering forces to come to final steering force
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			FVector CalculateNewSteeringForce();

		// Seek towards static target
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			FVector Seek(FVector ActorLocation);

		// Seek towards static target and slow down when approaching end (ease out)
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			FVector Arrive(FVector TargetLocation);

		// Seek towards static target and slow down when approaching end (ease out)
		UFUNCTION(BlueprintCallable, Category = "NPC Behavior")
			FVector Path(TArray<AActor*> Targets);


		/**************** Variable Declaration for each steering behavior ******************************/

		//Show Debug Lines
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (DisplayName = "Debug Draw Lines", AllowPrivateAccess = "true"))
			bool bDebugLinesActive = false;
		//Show Debug Circles
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debug", meta = (DisplayName = "Debug Draw Circles", AllowPrivateAccess = "true"))
			bool bDebugCirclesActive = false;

		/****************  Weight set to each steering behavior ******************************/

		//Actor's weight while seeking
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seek", meta = (DisplayName = "Seek Weight", AllowPrivateAccess = "true"))
			float SeekWeight;

		/******* Flee *******/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee", meta = (DisplayName = "Flee Weight", AllowPrivateAccess = "true"))
			float FleeWeight;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee", meta = (DisplayName = "Flee Range", AllowPrivateAccess = "true"))
			float FleeDistance;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flee", meta = (DisplayName = "Flee Indefinitely?", AllowPrivateAccess = "true"))
			bool bFleeIndefinitely = false;


		/******* Wander *******/
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wander", meta = (DisplayName = "Wander Target", AllowPrivateAccess = "true"))
			FVector WanderTarget;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander", meta = (DisplayName = "Wander Weight", AllowPrivateAccess = "true"))
			float WanderWeight;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander", meta = (DisplayName = "Wander Radius", AllowPrivateAccess = "true"))
			float WanderRadius;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander", meta = (DisplayName = "Wander Distance", AllowPrivateAccess = "true"))
			float WanderDistance;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Wander", meta = (DisplayName = "Wander Jitter", AllowPrivateAccess = "true"))
			float WanderJitter;

		//Actor's weight while pursuing
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pursuit", meta = (DisplayName = "Pursuit Weight", AllowPrivateAccess = "true"))
			float PursuitWeight;

		/******* Evade *******/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade", meta = (DisplayName = "Evade Weight", AllowPrivateAccess = "true"))
			float EvadeWeight;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade", meta = (DisplayName = "Evade Range", AllowPrivateAccess = "true"))
			float EvadeDistance;
		
		//Allow actor to evade indefinitely
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Evade", meta = (DisplayName = "Evade Indefinitely?", AllowPrivateAccess = "true"))
			bool bEvadeIndefinitely = false;

		
		//Actor's weight while avoiding collisions
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionAvoidance", meta = (DisplayName = "Collision Avoidance Weight", AllowPrivateAccess = "true"))
			float CollisionAvoidanceWeight;


		//Actor's weight while avoiding walls
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallAvoidance", meta = (DisplayName = "Wall Avoidance Weight", AllowPrivateAccess = "true"))
			float WallAvoidanceWeight;
		//Length to feel out ahead to check for Wall Avoidance in Meters
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WallAvoidance", meta = (DisplayName = "Wall Avoidance Trace Length", AllowPrivateAccess = "true"))
			float Length;


		/******* Arrive *******/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrive", meta = (DisplayName = "Arrive Weight", AllowPrivateAccess = "true"))
			float ArriveWeight;

//Amount of distance the actor needs to be in cm/s (UE4 units/s) from target in order to start slowing down.  Value should be larger than MaxSpeed
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Arrive", meta = (DisplayName = "Slowing Distance", AllowPrivateAccess = "true"))
			float SlowingDistance;


		//Array of path targets
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (DisplayName = "Path Targets", AllowPrivateAccess = "true"))
			TArray<AActor*> PathTargets;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (DisplayName = "Path Weight", AllowPrivateAccess = "true"))
			float PathWeight;

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (DisplayName = "Waypoint Distance", AllowPrivateAccess = "true"))
			float WaypointDistance;

		//Allow actor to loop indefinitely around a path of targets
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Path", meta = (DisplayName = "Loop Path?", AllowPrivateAccess = "true"))
			bool bPathLooping;

		int32 PathIndex;
		

		/******* Physics *******/

		//Acceleration in cm/second^2
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics", meta = (DisplayName = "Acceleration", AllowPrivateAccess = "true"))
			FVector Acceleration;
		//Velocity measured in cm/second
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics", meta = (DisplayName = "Velocity", AllowPrivateAccess = "true"))
			FVector MyVelocity;

		//Measured in kilograms (kg)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (DisplayName = "Mass", AllowPrivateAccess = "true"))
			float Mass = 1.0f;

		//Maximum speed that an entity may travel (km/hour)
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (DisplayName = "Max Speed", AllowPrivateAccess = "true"))
			float MaxSpeed = 300.0f;

		//Maximum force this entity can produce to power itself
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (DisplayName = "Max Force", AllowPrivateAccess = "true"))
			float MaxForce;

		//Maximum rate (radians/second) at which the vehicle can rotate
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Physics", meta = (DisplayName = "Max Turn Rate", AllowPrivateAccess = "true"))
			float MaxTurnRate;
		
		//Keeps the final Steering Force to apply to actor
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics", meta = (DisplayName = "Steering Force", AllowPrivateAccess = "true"))
			FVector SteeringForce;

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Physics", meta = (DisplayName = "Steering Force", AllowPrivateAccess = "true"))
			FVector MyLocation;

		FColor GetColor(EColor Col);
		void DebugLines(const FVector Direction, FColor Col);
		void DebugCircles(FVector Center, float Radius, FColor Col);
};


