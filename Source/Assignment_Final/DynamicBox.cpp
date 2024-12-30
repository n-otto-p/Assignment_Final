// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicBox.h"
#include "Assignment_FinalCharacter.h"
#include "Assignment_FinalCharacter.h"
ADynamicBox::ADynamicBox()
{
	//Setting up Static Mesh Component
	BoxMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxMesh"));
	RootComponent = BoxMesh;
	Health = 100.0f;
	Score = 10;

	// Loading a Default Cube Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("/Engine/BasicShapes/Cube.Cube"));
	if (CubeMesh.Succeeded())
	{
		BoxMesh->SetStaticMesh(CubeMesh.Object);
	}
	static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("/Game/Static/Materials/M_BoxDynamicColor"));
	if (Material.Succeeded())
	{
		BoxMesh->SetMaterial(0, Material.Object);
	}

}



// Called when the game starts or when spawned
void ADynamicBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADynamicBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ADynamicBox::InitializeBox(float InitialHealth, int InitialScore, const FVector& Color) // Color not considered yet.
{
	Health = InitialHealth;
	Score = InitialScore;
	UMaterialInstanceDynamic* DynamicMaterial = BoxMesh->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicMaterial)
	{
		FLinearColor BoxColor(Color.X / 255.0f, Color.Y / 255.0f, Color.Z / 255.0f, 1.0f);
		DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), BoxColor);
	}
}


float ADynamicBox::TakeDamage(float DamageAmount)
{
	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		Destroy();
		UE_LOG(LogTemp, Log, TEXT("Box destroyed! Score awarded: %d"), Score);

		// Call the function to update score
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
			// Assuming you have a method in the player controller or character to update the score
			AAssignment_FinalCharacter* YourPC = Cast<AAssignment_FinalCharacter>(PlayerController);
			if (YourPC)
			{
				//YourPC->AddScore(Score); // Add the box's score to the player
			}
		}
	}
	return Health; 
}