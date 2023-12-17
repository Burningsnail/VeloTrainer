#include "BikeWheeledPawn.h"
#include "BikeChaosMovementComponent.h"
ABikeWheeledPawn::ABikeWheeledPawn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UBikeChaosMovementComponent>(VehicleMovementComponentName))
{
}