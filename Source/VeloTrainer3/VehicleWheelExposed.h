// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VehicleWheel.h"
#include "VehicleWheelExposed.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class VELOTRAINER3_API UVehicleWheelExposed : public UVehicleWheel
{
	GENERATED_BODY()
	public:
	/** Damping rate for this wheel (Kgm^2/s) */
	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	void setDumpingRate( float newDampingRate );

	UFUNCTION(BlueprintCallable, Category = "Vehicle")
	float getDumpingRate();

};
