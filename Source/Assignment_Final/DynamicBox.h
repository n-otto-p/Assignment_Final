// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DynamicBox.generated.h"

UCLASS()
class ASSIGNMENT_FINAL_API ADynamicBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ADynamicBox();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes")
	int Score;

	// Box mesh
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* BoxMesh;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	float GetScoreValue() const { return Score; }
	void InitializeBox(float InitialHealth, int InitialScore, const FVector& Color);
	virtual float TakeDamage(float DamageAmount);
	void DestroyBox();

};
