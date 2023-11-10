// Fill out your copyright notice in the Description page of Project Settings.


#include "VeloBlueToothAdapter.h"

// Sets default values for this component's properties
UVeloBlueToothAdapter::UVeloBlueToothAdapter()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UVeloBlueToothAdapter::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UVeloBlueToothAdapter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

