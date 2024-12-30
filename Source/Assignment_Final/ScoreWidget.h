// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreWidget.generated.h"

/**
 * 
 */
UCLASS()
class ASSIGNMENT_FINAL_API UScoreWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
		// Method to Update Score
		void UpdateScore(int NewScore);

	protected:
		// Binding to UMG Widget
		UPROPERTY(meta = (BindWidget))
		class UTextBlock* ScoreText;

	private:
		int CurrentScore = 0;
};

