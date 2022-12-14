// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Active status
	bool bIsActive;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Get mesh component
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; };

	// Get the state
	UFUNCTION(BlueprintPure, Category = "Pickup")
	bool IsActive();

	// Set the state
	UFUNCTION(BlueprintCallable, Category = "Pickup")
	void SetActive(bool NewPickupState);

	// Function to call when the pickup is collected
	UFUNCTION(BlueprintNativeEvent)
	void WasCollected();
	virtual void WasCollected_Implementation();

private:
	// static mesh component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* PickupMesh;

};
