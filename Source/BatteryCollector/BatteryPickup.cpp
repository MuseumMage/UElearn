// Fill out your copyright notice in the Description page of Project Settings.


#include "BatteryPickup.h"


ABatteryPickup::ABatteryPickup()
{
	// stuff falls from sky
	GetMesh()->SetSimulatePhysics(true);
}

void ABatteryPickup::WasCollected_Implementation()
{
	// use the base pickup behavior
	Super::WasCollected_Implementation();
	// Destroy the battery
	Destroy();
}