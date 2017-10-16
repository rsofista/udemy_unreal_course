// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Math/Rotator.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	this->PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UOpenDoor::BeginPlay()
{
	this->Super::BeginPlay();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const float WorldTimeSeconds = this->GetWorld()->GetTimeSeconds();

	if (this->GetTotalMassOfActorsInThePressurePlate() > this->MinimumMassToOpenDoor) {
		this->OnOpen.Broadcast();
	}
	else {
		this->OnClose.Broadcast();
	}
}

float UOpenDoor::GetTotalMassOfActorsInThePressurePlate() const
{
	float TotalMass = 0.0f;

	if (this->PressurePlate) {
		TArray<AActor*> OverlappingActors;

		this->PressurePlate->GetOverlappingActors(OUT OverlappingActors);

		for (const AActor *Actor : OverlappingActors) {
			UPrimitiveComponent* ActorPrimitiveComponent = Actor->FindComponentByClass<UPrimitiveComponent>();

			if (ActorPrimitiveComponent) {
				TotalMass += ActorPrimitiveComponent->GetMass();
			}
		}
	}

	return TotalMass;
}
