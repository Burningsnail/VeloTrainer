// Fill out your copyright notice in the Description page of Project Settings.


#include "VehicleWheelExposed.h"

void UVehicleWheelExposed::setDumpingRate( float newDampingRate ){

    this->DampingRate = newDampingRate;

}
float UVehicleWheelExposed::getDumpingRate(){

    return this->DampingRate;

}