// Fill out your copyright notice in the Description page of Project Settings.


#include "SteeringBehavior_CPP.h"

// Sets default values for this component's properties
USteeringBehavior_CPP::USteeringBehavior_CPP()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	/****************  Variable Initialization ******************************/
	SeekWeight = 1.0f;
	FleeWeight = 1.0f;
	WanderWeight = 1.0f;
	WanderRadius = 1.0f;
	WanderDistance = 1.0f;
	PursuitWeight = 1.0f;
	EvadeWeight = 1.0f;
	CollisionAvoidanceWeight = 1.0f;
	WallAvoidanceWeight = 1.0f;
	ArriveWeight = 1.0f;
	PathWeight = 1.0f;
	bPathLooping = false;
	PathIndex = 0;
	SlowingDistance = 1.0f;
	MaxSpeed = 300.0f;
	Mass = 1.0f;
	MyVelocity = FVector(0.f, 0.f, 0.f);
	WanderTarget = MyVelocity;

}


void USteeringBehavior_CPP::PostEditChangeProperty(FPropertyChangedEvent& e)
{
	Super::PostEditChangeProperty(e);
	////Get current Actor position
}

// Called when the game starts
void USteeringBehavior_CPP::BeginPlay()
{
	Super::BeginPlay();

	//Check if static target if assigned
	if (isSet(ENpcBehaviors1::Seek) || isSet(ENpcBehaviors1::Flee) || isSet(ENpcBehaviors1::Arrive))
	{
		if (!StaticTarget)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Static Target Assigned"));
		}
	}
	//Check if dynamic target if assigned
	if (isSet(ENpcBehaviors1::Evade) || isSet(ENpcBehaviors1::Pursuit))
	{
		if (!DynamicTarget)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Dynamic Target Assigned"));
		}
	}

	UStaticMeshComponent* StaticMesh = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	if (StaticMesh->IsSimulatingPhysics()) { Mass = StaticMesh->GetMass(); }

	//Prints bit value to Output Log (Testing purposes)
	FString Message = FString::FromInt(bActivateNPCBehavior1 & 0x01);
}


// Called every frame
void USteeringBehavior_CPP::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Stores current FVector Location of Agent
	MyLocation = GetOwner()->GetActorLocation();

	// Calculates weighted steering force based on selected forces
	SteeringForce = CalculateNewSteeringForce();
	//UE_LOG(LogTemp, Warning, TEXT("SF: %s"), *(SteeringForce.ToString()));

	// This is a ground based agent, so we ignore Z
	SteeringForce.Z = 0.0f;

	// This scales our final steering force to max speed
	SteeringForce.Normalize();
	SteeringForce *= MaxSpeed;


	////Formula: Acceleration = Force/Mass
	Acceleration = SteeringForce / Mass;

	//Update velocity
	MyVelocity += Acceleration;

	//Make sure vehicle does not exceed max velocity
	HelperFunctions::Clamp3DVector(MyVelocity, MaxSpeed);

	MyVelocity = UGameplayStatics::GetWorldDeltaSeconds(this) * MyVelocity;

	// get the actor location in Vec3
	// Add Velocity above 

	FVector CurrentLocation = MyLocation;
	// Set new location
	CurrentLocation += MyVelocity;
	

	//Update Actor position
	GetOwner()->SetActorLocation(CurrentLocation, true);

	GetOwner()->GetRootComponent()->ComponentVelocity = MyVelocity;

}

#if WITH_EDITOR
bool USteeringBehavior_CPP::CanEditChange(const UProperty* InProperty) const
{

	const bool ParentVal = Super::CanEditChange(InProperty);

	// Can we edit seek weight?
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, SeekWeight))
	{
		return isSet(ENpcBehaviors1::Seek);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, FleeWeight))
	{
		return isSet(ENpcBehaviors1::Flee);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, FleeDistance))
	{
		return isSet(ENpcBehaviors1::Flee);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, bFleeIndefinitely))
	{
		return isSet(ENpcBehaviors1::Flee);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, EvadeWeight))
	{
		return isSet(ENpcBehaviors1::Evade);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, EvadeDistance))
	{
		return isSet(ENpcBehaviors1::Evade);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, bEvadeIndefinitely))
	{
		return isSet(ENpcBehaviors1::Evade);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, PursuitWeight))
	{
		return isSet(ENpcBehaviors1::Pursuit);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, WanderWeight))
	{
		return isSet(ENpcBehaviors1::Wander);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, WanderRadius))
	{
		return isSet(ENpcBehaviors1::Wander);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, WanderDistance))
	{
		return isSet(ENpcBehaviors1::Wander);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, WanderJitter))
	{
		return isSet(ENpcBehaviors1::Wander);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, CollisionAvoidanceWeight))
	{
		return isSet(ENpcBehaviors1::CollisionAvoidance);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, WallAvoidanceWeight))
	{
		return isSet(ENpcBehaviors1::WallAvoidance);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, ArriveWeight))
	{
		return isSet(ENpcBehaviors1::Arrive);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, SlowingDistance))
	{
		return isSet(ENpcBehaviors1::Arrive);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, PathWeight))
	{
		return isSet(ENpcBehaviors2::Path);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, PathTargets))
	{
		return isSet(ENpcBehaviors2::Path);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, WaypointDistance))
	{
		return isSet(ENpcBehaviors2::Path);
	}
	if (InProperty->GetFName() == GET_MEMBER_NAME_CHECKED(USteeringBehavior_CPP, bPathLooping))
	{
		return isSet(ENpcBehaviors2::Path);
	}

	return ParentVal;
}

#endif

/*
Author: Julian D.P.
Purpose: isSet sees if a passed behavior is set to true on bActivateNPCBehavior
Elaboration: checks if the passed Behavior binary and with bActivateNPCBehavior
is 1
*/
bool USteeringBehavior_CPP::isSet(ENpcBehaviors1 Behavior) const
{
	if (static_cast<uint8>(Behavior) & bActivateNPCBehavior1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool USteeringBehavior_CPP::isSet(ENpcBehaviors2 Behavior) const
{
	if (static_cast<uint8>(Behavior) & bActivateNPCBehavior2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*
Author: Diamonick D
Purpose: Sends agent directly towards a static FVector target and doesn't stop at the end
*/
FVector USteeringBehavior_CPP::Seek(FVector TargetLocation)
{
	// New velocity based on target location
	FVector DesiredVelocity;

	// Calculates new vector between target and agent
	DesiredVelocity = TargetLocation - MyLocation;

	// Clamps desired velocity to max speed
	DesiredVelocity.Normalize();
	DesiredVelocity *= MaxSpeed;

	//Return steering force
	return (DesiredVelocity - MyVelocity);
}

//*********************************************************************************************************************************
//Currently working on implementing Arrive() Behavior. From: Diamonick

FVector USteeringBehavior_CPP::Arrive(FVector TargetLocation)
{
	FVector DesiredVelocity;

	float Distance;
	float RampedSpeed;
	float ClippedSpeed;

	DesiredVelocity = (FVector(TargetLocation.X, TargetLocation.Y, 0.0f) - FVector(MyLocation.X, MyLocation.Y, 0.0f));
	Distance = DesiredVelocity.Size();
	if (Distance / SlowingDistance > .01f)
	{
		RampedSpeed = MaxSpeed * (Distance / SlowingDistance);
		ClippedSpeed = FMath::Min(RampedSpeed, MaxSpeed);
		UE_LOG(LogTemp, Warning, TEXT("D/SD Speed: %f"), Distance/SlowingDistance);
		DesiredVelocity *= (ClippedSpeed/Distance);

		//Return velocity
		return (DesiredVelocity - MyVelocity);
	}
	else { return FVector(0.0f, 0.0f, 0.0f); }
}

/*
Author: Julian D.P.
Purpose: GetColor is to return a FColor based on bDebugLineColor
Elaboration: if the current color is black, return FColor(0,0,0)
and so on, if there are multiple colors selected then return white
*/
FColor USteeringBehavior_CPP::GetColor(EColor Col)
{
	switch (Col)
	{
	case EColor::Black:
		return FColor(0, 0, 0);
		break;
	case EColor::Red:
		return FColor(255, 0, 0);
		break;
	case EColor::Green:
		return FColor(0, 255, 0);
		break;
	case EColor::Blue:
		return FColor(0, 0, 255);
		break;
	case EColor::Yellow:
		return FColor(255, 255, 0);
		break;
	case EColor::Purple:
		return FColor(255, 0, 255);
		break;
	case EColor::Cyan:
		return FColor(0, 255, 255);
		break;
	case EColor::White:
	default:
		return FColor(255, 255, 255);
		break;

	}
}

/*
Author: Julian D.P.
Purpose: DebugLines draws a line from MyLocation out to the direction I wish to go
Elaboration: The Z is replaced with MyLocation.Z due to movement being 2 Dimensional,
also is only active when bDebugLinesActive is set to true
*/
void USteeringBehavior_CPP::DebugLines(const FVector Direction, FColor Col)
{
	if (bDebugLinesActive)
	{
		FVector ZZeroed = Direction;
		ZZeroed.Z = MyLocation.Z;
		DrawDebugLine(GetWorld(), MyLocation, ZZeroed, Col, false, -1, 0, 3);
	}
}

/*
Author: Diamonick D.
Purpose: DebugCircle draws a circle from a center
*/
void USteeringBehavior_CPP::DebugCircles(FVector Center, float Radius, FColor Col)
{
	if (bDebugCirclesActive)
	{
		DrawDebugCircle(GetWorld(), Center, Radius, 32, Col, false, -1, 0, 4, FVector(1.0f, 0.0f, 0.0f), FVector(0.0f, 1.0f, 0.0f), true);
	}
}
//*********************************************************************************************************************************

/*
Author: Julian D.P.
Purpose: Flee is to try to escape a static target
Elaboration: Flee is the opposite of Seek or Seek * -1
*/
FVector USteeringBehavior_CPP::Flee(FVector Location)
{
	float Distance = FVector::DistSquared(Location, MyLocation);
	float FleeDist = FMath::Square(FleeDistance);

	if (bFleeIndefinitely) { return -1 * Seek(Location); }
	else if (Distance < FleeDist) { return -1 * Seek(Location); }
	else { return FVector(0.0f, 0.0f, 0.0f); }
}

/*
Author: Julian D.P.
Purpose: Evade is to try to escape the pursuing dynamic target
Elaboration:   Evade is the opposite of Pursuit or Pusuit * -1
*/
FVector USteeringBehavior_CPP::Evade(FVector Location, FVector Veloc)
{
	float Distance = FVector::DistSquared(Location, MyLocation);
	float EvadeDist = FMath::Square(EvadeDistance);

	if (bEvadeIndefinitely) { return -1 * Pursuit(Location, Veloc); }
	else if (Distance < EvadeDist) { return -1 * Pursuit(Location, Veloc); }
	else { return FVector(0.0f, 0.0f, 0.0f); }
}

/*
Author: Julian D.P.
Purpose: Pursuit is to try and cut off fleeing dynamic targets
Elaboration:  Pursuit tries to keep track of target's location
and their current velocity projecting where the target will be
in the future and going in that direction.  This will act like
'seek' when the target isn't moving or moving directly towards
the Pursuer's location
*/
FVector USteeringBehavior_CPP::Pursuit(FVector Location, FVector Veloc)
{
	//Find the vector between the target and myself
	FVector ToEvader = Location - MyLocation;
	//My movement vector (so I don't change Velocity variable)
	FVector MyHeading = MyVelocity;
	//Their movement vector (so I don't change their Velocity variable)
	FVector TargetHeading = Veloc;
	//Make them equal to 1
	MyHeading.Normalize();
	TargetHeading.Normalize();
	//Get the dot product of our normalized velocities
	float RelativeHeading = FVector::DotProduct(MyHeading, TargetHeading);
	//This is to get a normalized ToEvader without losing ToEvader's value
	FVector ToEvaderNormalize = ToEvader;
	ToEvaderNormalize.Normalize();
	//If the dot product of our distance + my direction and the dot product of our normalized velocities are below 18 degrees, then just seek
	if (FVector::DotProduct(ToEvaderNormalize, MyHeading) > 0 && RelativeHeading < -.95) // acos(0.95)=18 degrees
	{
		return Seek(Location);
	}
	//guess the time it will take to get to target
	float LookAheadTime = ToEvader.Size() / (MaxSpeed + Veloc.Size());
	//project a future path and seek that path
	FVector SeekPosition = Seek(Location + (Veloc * LookAheadTime));
	
	return SeekPosition;
}

/*
Author: Diamonick D.
// Character walks non-linearly towards its current set velocity

*/
FVector USteeringBehavior_CPP::Wander()
{
	FVector LocalTarget;
	FVector WorldTarget;

	// Adds jitter to our existing velocity
	WanderTarget += FVector(FMath::FRandRange(-1.0f, 1.0f) * WanderJitter, FMath::FRandRange(-1.0f, 1.0f) * WanderJitter, 0.0f);

	// Projects jitter by the radius amount in the jitter cirlce
	WanderTarget.Normalize();
	WanderTarget *= WanderRadius;
	// Projects WanderTarget in WanderDistance away from agent
	LocalTarget = WanderTarget + GetOwner()->GetActorForwardVector() * WanderDistance;

	// Local to world space
	WorldTarget = MyLocation + LocalTarget;
	// Geta a vector from the agent to the outer radius of the circle


	FVector SteeringVector = FVector(WorldTarget.X, WorldTarget.Y, MyLocation.Z);

	//UE_LOG(LogTemp, Warning, TEXT("Local Target %s - WorldTarget %s"), *(LocalTarget.ToString()), *(WorldTarget.ToString()));

	DebugCircles(MyLocation + (GetOwner()->GetActorForwardVector() * WanderDistance), WanderRadius, GetColor(EColor::Yellow));
	DebugLines(WorldTarget, GetColor(EColor::White));

	//Return velocity
	return LocalTarget - MyVelocity;
}

FVector USteeringBehavior_CPP::WallAvoidance_Implementation()
{
	int index;
	float length = Length * 100.0f;
	FVector BounceOffWall;
	FCollisionQueryParams CollisionParams;
	FVector MyNormalVelocity = MyVelocity;
	MyNormalVelocity.Normalize();
	CollisionParams.AddIgnoredActor(GetOwner());

	TArray<FHitResult> Hits{ FHitResult(ForceInit), FHitResult(ForceInit), FHitResult(ForceInit) };
	TArray<FVector> Distances{ FVector(100000.0f), FVector(100000.0f), FVector(100000.0f) };
	TArray<FVector> ImpactNormals{ FVector(0.0f), FVector(0.0f), FVector(0.0f) };
	TArray<FVector> End
	{
		MyLocation + MyNormalVelocity * length,
		MyLocation + MyNormalVelocity.RotateAngleAxis(-45.0f, MyLocation.UpVector) * length,
		MyLocation + MyNormalVelocity.RotateAngleAxis(45.0f, MyLocation.UpVector) * length
	};


	for (int i = 0; i <= 2; i++)
	{
		DrawDebugLine(GetWorld(), MyLocation, End[i], GetColor(EColor::Blue), false, -1, 0, 3);
		GetWorld()->LineTraceSingleByChannel(Hits[i], MyLocation, End[i], ECC_WorldDynamic, CollisionParams);
		if (Hits[i].GetActor() && Hits[i].GetActor()->ActorHasTag(FName("Wall")))
		{
			Distances[i] = (Hits[i].Location - Hits[i].TraceStart);
			ImpactNormals[i] = Hits[i].ImpactNormal;
		}
	}
	for (int i = 0; i <= 2; i++)
	{
		if (Distances[i].Size() <= Distances[(i + 1) % 3].Size() && Distances[i].Size() <= Distances[(i + 2) % 3].Size())
		{
			index = i;
		}
	}
	BounceOffWall = { (length - Distances[index].Size()) * ImpactNormals[index] };
	return BounceOffWall;
}


FVector USteeringBehavior_CPP::CollisionAvoidance()
{
	return FVector();
}

/*
Author: Diamonick D.
// Character follows a path of targets (either once or indefinitely)

*/
FVector USteeringBehavior_CPP::Path(TArray<AActor*> Targets)
{
	FVector ResultVelocity;
	FVector CurrentVelocity;
	float Distance;
	FVector CurrentTargetLocation = Targets[PathIndex]->GetActorLocation();

	//Have actor arrive at target if it's the last target and Loop Path? is false
	if (PathIndex == Targets.Max() - 1 && !bPathLooping)
	{
		ResultVelocity = Arrive(CurrentTargetLocation);
	}
	//Have actor seek at target if it's NOT the last target
	else
	{
		ResultVelocity = Seek(CurrentTargetLocation);
	}

	CurrentVelocity = (CurrentTargetLocation - MyLocation);
	Distance = CurrentVelocity.Size();

	//Have actor move to the next target if it's near a specific distance from the target
	if (Distance < WaypointDistance)
	{
		if (!bPathLooping && PathIndex == Targets.Max() - 1) { return FVector(0.0f, 0.0f, 0.0f); }
		PathIndex = ++PathIndex % Targets.Max();
	}

	return ResultVelocity;
}

FVector USteeringBehavior_CPP::CalculateNewSteeringForce()
{
	FVector SteerForce;

	FVector SteeringForceTotal(0.0f, 0.0f, 0.0f);
	if (isSet(ENpcBehaviors1::Seek))
	{
		if (StaticTarget)
		{
			SteerForce = Seek(StaticTarget->GetActorLocation()) * SeekWeight;
			SteeringForceTotal += SteerForce;
			DebugLines(SteerForce + MyVelocity + MyLocation, GetColor(EColor::Red));
		}
	}
	if (isSet(ENpcBehaviors1::Flee))
	{
		if (StaticTarget)
		{
			SteerForce = Flee(StaticTarget->GetActorLocation()) * FleeWeight;
			SteeringForceTotal += SteerForce;
			DebugLines(SteerForce + MyVelocity + MyLocation, GetColor(EColor::Blue));
			DebugCircles(MyLocation, FleeDistance, GetColor(EColor::Blue));
		}
	}
	if (isSet(ENpcBehaviors1::Evade))
	{
		if (DynamicTarget)
		{
			SteerForce = Evade(DynamicTarget->GetActorLocation(), DynamicTarget->GetVelocity()) * EvadeWeight;
			SteeringForceTotal += SteerForce;
			DebugLines(SteerForce + MyVelocity + MyLocation, GetColor(EColor::Purple));
			DebugCircles(MyLocation, EvadeDistance, GetColor(EColor::Purple));
		}
	}
	if (isSet(ENpcBehaviors1::Pursuit))
	{
		if (DynamicTarget)
		{
			SteerForce = Pursuit(DynamicTarget->GetActorLocation(), DynamicTarget->GetVelocity()) * PursuitWeight;
			SteeringForceTotal += SteerForce;
			DebugLines(SteerForce + MyVelocity + MyLocation, GetColor(EColor::Green));
		}
	}
	if (isSet(ENpcBehaviors1::Wander))
	{
		SteerForce = Wander() * WanderWeight;
		SteeringForceTotal += SteerForce;
	}
	if (isSet(ENpcBehaviors1::CollisionAvoidance))
	{
		SteerForce = CollisionAvoidance() * CollisionAvoidanceWeight;
		SteeringForceTotal += SteerForce;
		DebugLines(SteerForce + MyVelocity + MyLocation, GetColor(EColor::Black));
	}
	if (isSet(ENpcBehaviors1::Arrive))
	{
		if (StaticTarget)
		{
			SteerForce = Arrive(StaticTarget->GetActorLocation()) * ArriveWeight;
			SteeringForceTotal += SteerForce;
			DebugLines(SteerForce + MyVelocity + MyLocation, GetColor(EColor::Cyan));
		}
	}
	if (isSet(ENpcBehaviors1::WallAvoidance))
	{
		SteerForce = WallAvoidance() * WallAvoidanceWeight;
		SteeringForceTotal += SteerForce;
		UE_LOG(LogTemp, Warning, TEXT("SteerForce Vector: %s"), *(SteerForce.ToString()));
		UE_LOG(LogTemp, Warning, TEXT("SteeringForceTotal Vector: %s"), *(SteeringForceTotal.ToString()));
		DebugLines(SteerForce + MyVelocity + MyLocation, GetColor(EColor::Cyan));
	}
	if (isSet(ENpcBehaviors2::Path))
	{
		if (PathTargets[PathIndex])
		{
			SteerForce = Path(PathTargets) * PathWeight;
		}
		SteeringForceTotal += SteerForce;
		DebugLines(SteerForce + MyVelocity + MyLocation, GetColor(EColor::Cyan));
	}
	return SteeringForceTotal;
}

