// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

// Enum to store the current state of gameplay
// Allow us to use it later in our HUD_BP
UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
	EPlaying,
	EGameOver,
	EWon,
	EUnknown
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState() const;

	// UFUNCTION(BlueprintPure, Category = "Power")
	void SetCurrentState(EBatteryPlayState NewState);

protected:
	// GameMode sets the rules of the game. Different mode has a different value.
	// EditDefaultsOnly means you cannot change a property on every instance of the class, just on the base class itself.
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float DecayRate;

	// Power needed to win the game
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	float PowerToWin;

	// It is restricted to only things which inherit from UserWidget
	// The widget class to use for our HUD screen
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// The instance of the HUD
	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	// Current playing state
	EBatteryPlayState CurrentState;

	// Store all the Spawn Volumes in the world
	TArray<class ASpawnVolume*> SpawnVolumeActors;

	// Handle any function calls that rely upon changing the playing state of our game
	void HandleNewState(EBatteryPlayState NewState);
};



