// Copyright Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"
#include "BatteryCollectorCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"
#include "SpawnVolume.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/PawnMovementComponent.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// Note: 
	// If your gamemode class inherits from AGameModeBase instead of AGameMode(for example if you created project in the newest UE(4.14.0)) you have to set
	// PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	// Base decay rate
	DecayRate = 0.05f;
}

float ABatteryCollectorGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Find all spawn volume actors
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);

	for (auto Actor : FoundActors)
	{
		ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
		if (SpawnVolumeActor != nullptr)
		{
			SpawnVolumeActors.AddUnique(SpawnVolumeActor);
		}
	}

	SetCurrentState(EBatteryPlayState::EPlaying);

	// Set the score to beat
	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)); // Get the character
	if (MyCharacter != nullptr)
	{
		PowerToWin = (MyCharacter->GetInitialPower()) * 1.25f;
	}

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}

EBatteryPlayState ABatteryCollectorGameMode::GetCurrentState() const
{
	return CurrentState;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlayState NewState)
{
	CurrentState = NewState;
	// Call the action
	HandleNewState(NewState);
}

// Handle state and call the action
void ABatteryCollectorGameMode::HandleNewState(EBatteryPlayState NewState)
{
	switch (NewState)
	{
		case EBatteryPlayState::EPlaying:
		{
			// Spawn volumes active
			for (ASpawnVolume* Volume : SpawnVolumeActors)
			{
				Volume->SetSpawningActive(true);
			}
		}
		break;
		case EBatteryPlayState::EWon:
		{
			// Spawn volumes inactive
			for (ASpawnVolume* Volume : SpawnVolumeActors)
			{
				Volume->SetSpawningActive(false);
			}
		}
		break;
		case EBatteryPlayState::EGameOver:
		{
			// Spawn volumes inactive
			for (ASpawnVolume* Volume : SpawnVolumeActors)
			{
				Volume->SetSpawningActive(false);
			}
			// Block player input
			APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
			if (PlayerController != nullptr)
			{
				// Disable the input
				PlayerController->SetCinematicMode(true, false, false, true, true);
			}
			// Ragdoll the character
			ACharacter* MyCharacter = Cast<ACharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			if (MyCharacter != nullptr)
			{
				// Collapes the character
				MyCharacter->GetMesh()->SetSimulatePhysics(true);
				MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
			}
		}
		break;
		case EBatteryPlayState::EUnknown:
		{

		}
		break;
	}
}

void ABatteryCollectorGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check that we are using the battery collector character
	ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0)); // Get the character
	if (MyCharacter != nullptr)
	{
		if (MyCharacter->GetCurrentPower() > PowerToWin)
		{
			SetCurrentState(EBatteryPlayState::EWon);
		}
		else if (MyCharacter->GetCurrentPower() > 0)
		{
			// Decrease the character power using the decay rate
			MyCharacter->UpdatePower(-DeltaTime * DecayRate * (MyCharacter->GetInitialPower()));
		}
		else 
		{
			SetCurrentState(EBatteryPlayState::EGameOver);
		}
	}
}