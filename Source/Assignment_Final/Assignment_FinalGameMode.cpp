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
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

void AAssignment_FinalGameMode::SpawnBoxesFromJSON()
{
	FString URL = TEXT("https://raw.githubusercontent.com/CyrusCHAU/Varadise-Technical-Test/refs/heads/main/data.json");  // Replace with your actual URL
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();

	// Set up the HTTP request to GET data from the URL
	Request->SetURL(URL);
	Request->SetVerb(TEXT("GET"));
	Request->OnProcessRequestComplete().BindUObject(this, &AAssignment_FinalGameMode::OnJSONDataReceived);
	Request->ProcessRequest();
}

void AAssignment_FinalGameMode::OnJSONDataReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.IsValid())
    {
        FString JsonString = Response->GetContentAsString();  // The raw JSON data

        // Deserialize the JSON string into a TSharedPtr<FJsonObject>
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
        if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
        {
            // Handle the JSON data and spawn boxes
            TArray<TSharedPtr<FJsonValue>> ObjectsArray = JsonObject->GetArrayField(TEXT("objects"));
            for (auto& ObjectValue : ObjectsArray)
            {
                TSharedPtr<FJsonObject> ObjectObject = ObjectValue->AsObject();

                // Extract data for location, scale, rotation, type, etc.
                FVector Location = FVector(
                    ObjectObject->GetArrayField(TEXT("transform"))[0]->AsNumber(),
                    ObjectObject->GetArrayField(TEXT("transform"))[1]->AsNumber(),
                    ObjectObject->GetArrayField(TEXT("transform"))[2]->AsNumber()
                );

                // Parsing Scale
                FVector Scale = FVector(
                    ObjectObject->GetArrayField(TEXT("transform"))[3]->AsNumber(),
                    ObjectObject->GetArrayField(TEXT("transform"))[4]->AsNumber(),
                    ObjectObject->GetArrayField(TEXT("transform"))[5]->AsNumber()
                );

                // Parsing Rotation (converting from Euler angles to FRotator)
                FRotator Rotation = FRotator(
                    ObjectObject->GetArrayField(TEXT("transform"))[6]->AsNumber(),
                    ObjectObject->GetArrayField(TEXT("transform"))[7]->AsNumber(),
                    ObjectObject->GetArrayField(TEXT("transform"))[8]->AsNumber()
                );

                FString TypeName = ObjectObject->GetStringField(TEXT("type"));

                // Assuming you have a map of box types, fetch the relevant data for this type
                FBoxTypeData TypeData = BoxTypes[TypeName];

                // Spawn the dynamic box using the extracted data
                SpawnDynamicBox(Location, Rotation, TypeData.Health, TypeData.Score, TypeData.Color, Scale);
            }
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to fetch the JSON data!"));
    }
}

void AAssignment_FinalGameMode::BeginPlay()
{
	Super::BeginPlay();
	SpawnBoxesFromJSON();
	UE_LOG(LogTemp, Display, TEXT("BeginPlay runs"));
}

void AAssignment_FinalGameMode::SpawnDynamicBox(const FVector& Location, const FRotator& Rotation, float Health, int32 Score, const FColor& Color, const FVector& Scale)
{
	UWorld* World = GetWorld();
	if (World)
	{
		// Spawn a box actor
		ADynamicBox* Box = World->SpawnActor<ADynamicBox>(ADynamicBox::StaticClass(), Location, Rotation);
		if (Box)
		{
			Box->SetActorScale3D(Scale);
			Box->InitializeBox(Health, Score, FVector(Color.R, Color.G, Color.B));
		}
	}
}
