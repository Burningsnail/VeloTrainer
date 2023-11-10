// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SimpleWheeledVehicleMovementComponent.h"
#include "BikeMovementComponent.generated.h"

/**
 * 
 */
UCLASS( meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable )
class VELOTRAINER3_API UBikeMovementComponent : public UActorComponent
{
	GENERATED_BODY()
	
	public:
		UFUNCTION(BlueprintCallable, Category = "Vehicle")
		void reaction_force();

		UPROPERTY(EditAnywhere, blueprintreadwrite, Category = "Vehicle" )
		USkeletalMeshComponent* physical_object;
	
};
