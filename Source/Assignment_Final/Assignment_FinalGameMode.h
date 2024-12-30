// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HttpFwd.h"
#include "Assignment_FinalGameMode.generated.h"

class ADynamicBox;

USTRUCT(BlueprintType)
struct FBoxTypeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Data")
	int32 Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Data")
	int32 Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Data")
	FColor Color;

	FBoxTypeData()
		: Health(100), Score(10), Color(FColor::White) {}
};

UCLASS(minimalapi)
class AAssignment_FinalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	AAssignment_FinalGameMode();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Box Types")
	TMap<FString, FBoxTypeData> BoxTypes;  // BoxTypes is defined here using FBoxTypeData

	void SpawnBoxesFromJSON();
	void OnJSONDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "GameMode")
	void SpawnDynamicBox(const FVector& Location, const FRotator& Rotation, float Health, int32 Score, const FColor& Color, const FVector& Scale);
};



