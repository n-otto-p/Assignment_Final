// Copyright Epic Games, Inc. All Rights Reserved.

#include "Assignment_FinalGameMode.h"
#include "Assignment_FinalCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "HttpModule.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Json.h"
#include "DynamicBox.h"

AAssignment_FinalGameMode::AAssignment_FinalGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AAssignment_FinalGameMode::SpawnBoxesFromJSON()
{
	FString URL = TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	//Setting up HTTP to get Data from URL
	Request->SetURL(URL);
	Request->SetVerb(TEXT("GET"));
	Request->OnProcessRequestComplete().BindUObject(this, &AAssignment_FinalGameMode::OnJSONDataReceived); // Runs when Process Request is Complete.
	Request->ProcessRequest();
}

void AAssignment_FinalGameMode::OnJSONDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!bWasSuccessful || !Response.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to fetch the JSON data: HTTP request was unsuccessful or response is invalid."));
		return;
	}
	//Deserializing JsonString into a TSharedPtr.
	FString JsonString = Response->GetContentAsString();  
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	if (!FJsonSerializer::Deserialize(Reader, JsonObject) || !JsonObject.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to fetch the JSON data!"));
		return;
	}
	// Parsing Types and Objects.
	TArray<FBoxType> BoxTypes; 
	ParseTypes(JsonObject, BoxTypes);
	ParseObjects(JsonObject, BoxTypes);
	SpawnDynamicBoxes(BoxTypes);
}

void AAssignment_FinalGameMode::ParseTypes(const TSharedPtr<FJsonObject>& JsonObject, TArray<FBoxType>& OutBoxTypes)
{
	const TArray<TSharedPtr<FJsonValue>>* TypesArray;
	if (JsonObject->TryGetArrayField(TEXT("types"), TypesArray))
	{
		for (const auto& TypeValue : *TypesArray)
		{
			TSharedPtr<FJsonObject> TypeObject = TypeValue->AsObject();
			if (TypeObject.IsValid())
			{
				FBoxType BoxType;
				BoxType.Name = TypeObject->GetStringField(TEXT("name"));

				const TArray<TSharedPtr<FJsonValue>>* ColorArray;
				if (TypeObject->TryGetArrayField(TEXT("color"), ColorArray))
				{
					for (const auto& ColorValue : *ColorArray)
					{
						BoxType.Color.Add(ColorValue->AsNumber());
					}
				}

				BoxType.Health = TypeObject->GetIntegerField(TEXT("health"));
				BoxType.Score = TypeObject->GetIntegerField(TEXT("score"));
				OutBoxTypes.Add(BoxType);
			}
		}
	}
}

void AAssignment_FinalGameMode::ParseObjects(const TSharedPtr<FJsonObject>& JsonObject, TArray<FBoxType>& BoxTypes)
{
	const TArray<TSharedPtr<FJsonValue>>* ObjectsArray;
	if (JsonObject->TryGetArrayField(TEXT("objects"), ObjectsArray))
	{
		for (const auto& ObjectValue : *ObjectsArray)
		{
			TSharedPtr<FJsonObject> ObjectObject = ObjectValue->AsObject();
			if (ObjectObject.IsValid())
			{
				// Getting type of object
				FString ObjectType = ObjectObject->GetStringField(TEXT("type"));
				
				FBoxType* MatchingBoxType = BoxTypes.FindByPredicate([&ObjectType](const FBoxType& BoxType)
				{
					return BoxType.Name == ObjectType;
				});

				if (MatchingBoxType)
				{
					// Parsing Transform, Location, Rotation and Scale.
					TSharedPtr<FJsonObject> TransformObject = ObjectObject->GetObjectField(TEXT("transform"));
					if (TransformObject.IsValid())
					{
						const TArray<TSharedPtr<FJsonValue>>* LocationArray;
						if (TransformObject->TryGetArrayField(TEXT("location"), LocationArray))
						{
							FVector Location;
							Location.X = (*LocationArray)[0]->AsNumber();
							Location.Y = (*LocationArray)[1]->AsNumber();
							Location.Z = (*LocationArray)[2]->AsNumber();
							MatchingBoxType->Locations.Add(Location);
						}
						
						const TArray<TSharedPtr<FJsonValue>>* RotationArray;
						if (TransformObject->TryGetArrayField(TEXT("rotation"), RotationArray))
						{
							FRotator Rotation;
							Rotation.Roll = (*RotationArray)[0]->AsNumber();
							Rotation.Pitch = (*RotationArray)[1]->AsNumber();
							Rotation.Yaw = (*RotationArray)[2]->AsNumber();
							MatchingBoxType->Rotations.Add(Rotation);
						}
						
						const TArray<TSharedPtr<FJsonValue>>* ScaleArray;
						if (TransformObject->TryGetArrayField(TEXT("scale"), ScaleArray))
						{
							FVector Scale;
							Scale.X = (*ScaleArray)[0]->AsNumber();
							Scale.Y = (*ScaleArray)[1]->AsNumber();
							Scale.Z = (*ScaleArray)[2]->AsNumber();
							MatchingBoxType->Scales.Add(Scale);
						}
					}
				}
			}
		}
	}
}

void AAssignment_FinalGameMode::SpawnDynamicBoxes(const TArray<FBoxType>& BoxTypes)
{
	for (const FBoxType& BoxType : BoxTypes)
	{
		// Iterating through each objects transform data.
		for (int32 i = 0; i < BoxType.Locations.Num(); ++i)
		{
			FVector Location = BoxType.Locations[i];
			FRotator Rotation = BoxType.Rotations[i];
			FVector Scale = BoxType.Scales[i];

			// Spawning Box Actor.
			FActorSpawnParameters SpawnParams;
			ADynamicBox* SpawnedBox = GetWorld()->SpawnActor<ADynamicBox>(ADynamicBox::StaticClass(), Location, Rotation, SpawnParams);

			if (SpawnedBox)
			{
				// Initializing Box Attributes.
				SpawnedBox->InitializeBox(BoxType.Health, BoxType.Score, FVector(BoxType.Color[0], BoxType.Color[1], BoxType.Color[2]));

				// Applying Scale.
				SpawnedBox->SetActorScale3D(Scale);

				// Set Dynamic Material with Color
				UMaterialInstanceDynamic* DynamicMaterial = SpawnedBox->BoxMesh->CreateAndSetMaterialInstanceDynamic(0);
				if (DynamicMaterial)
				{
					// Assuming the material has a parameter named "BaseColor" for RGB
					FLinearColor BoxColor(BoxType.Color[0] / 255.0f, BoxType.Color[1] / 255.0f, BoxType.Color[2] / 255.0f, 1.0f);
					DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), BoxColor);
				}
			}
		}
	}
}


void AAssignment_FinalGameMode::BeginPlay()
{
	Super::BeginPlay();
	SpawnBoxesFromJSON();
	UE_LOG(LogTemp, Display, TEXT("BeginPlay runs"));
}


