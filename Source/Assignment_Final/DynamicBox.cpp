// Fill out your copyright notice in the Description page of Project Settings.


#include "DynamicBox.h"

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
}


float ADynamicBox::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= DamageAmount;
	if (Health <= 0.0f)
	{
		Destroy();
		UE_LOG(LogTemp, Log, TEXT("Box destroyed! Score awarded: %d"), Score); // Have to connect to Main Score and Add Later on.
	}
	return Health; 
}