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

	// UFUNTION is set to be 
	float GetPower();

protected:
	// Set the amount of power in this charactor. Only blueprints that can change this variable are those that inherit from BatteryPickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float BatteryPower;
};
