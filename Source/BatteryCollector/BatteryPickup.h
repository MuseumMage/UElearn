// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup
{
	GENERATED_BODY()

public:
	// constructor
	ABatteryPickup();

	// Override the WasCollected function - use Implementation because its a Blueprint native event
	void WasCollected_Implementation() override;
};
