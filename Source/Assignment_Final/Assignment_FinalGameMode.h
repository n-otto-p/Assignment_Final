// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HttpFwd.h"
#include "Assignment_FinalGameMode.generated.h"

USTRUCT(BlueprintType) //Struct to hold Parsed Data.
struct FBoxType
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name;

	UPROPERTY()
	TArray<int32> Color;

	UPROPERTY()
	int32 Health;

	UPROPERTY()
	int32 Score;

	// Transform Data from here.
	UPROPERTY()
	TArray<FVector> Locations;

	UPROPERTY()
	TArray<FRotator> Rotations;

	UPROPERTY()
	TArray<FVector> Scales;
};


UCLASS(minimalapi)
class AAssignment_FinalGameMode : public AGameModeBase
{
	
	GENERATED_BODY()

public:
	
	AAssignment_FinalGameMode();
	void SpawnBoxesFromJSON();
	void OnJSONDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	virtual void BeginPlay() override;
	static void ParseTypes(const TSharedPtr<FJsonObject>& JsonObject, TArray<FBoxType>& OutBoxTypes);
	void ParseObjects(const TSharedPtr<FJsonObject>& JsonObject, TArray<FBoxType>& BoxTypes);
};



