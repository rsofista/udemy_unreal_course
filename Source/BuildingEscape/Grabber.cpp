// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Math/Color.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	this->PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	const AActor* Owner = this->GetOwner();

	this->PhysicsHandle  = Owner->FindComponentByClass<UPhysicsHandleComponent>();
	this->InputComponent = Owner->FindComponentByClass<UInputComponent>();

	if (!this->PhysicsHandle) {
		UE_LOG(LogTemp, Error, TEXT("%s needs UPhysicsHandleComponent attached"), *Owner->GetName());
	}

	if (this->InputComponent) {
		this->InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		this->InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::ReleaseGrab);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s needs UInputComponent attached"), *Owner->GetName());
	}
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	this->Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!this->PhysicsHandle) { return; }

	if (this->PhysicsHandle->GrabbedComponent) {
		this->PhysicsHandle->SetTargetLocation(this->GetLineTraceEnd());
	}
}

void UGrabber::Grab()
{
	if (!this->PhysicsHandle) { return; }

	FHitResult HitResult = this->GetFirstPhysicalBodyInReach();

	if (HitResult.GetActor()) {
		UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

		FRotator RotatorDummy;

		this->PhysicsHandle->GrabComponentAtLocationWithRotation(
			ComponentToGrab,
			::NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			RotatorDummy
		);
	}
}

void UGrabber::ReleaseGrab()
{
	if (!this->PhysicsHandle) { return; }

	this->PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicalBodyInReach() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	this->GetPlayerViewPointInfo(PlayerViewPointLocation, PlayerViewPointRotator);

	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotator.Vector() * this->Reach);

	FHitResult ObjectHit;

	this->GetWorld()->LineTraceSingleByObjectType(
		OUT ObjectHit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		FCollisionQueryParams(::NAME_None, false, this->GetOwner())
	);

	return ObjectHit;
}

void UGrabber::GetPlayerViewPointInfo(FVector &out_PlayerViewPointLocation, FRotator &out_PlayerViewPointRotator) const
{
	this->GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		out_PlayerViewPointLocation,
		out_PlayerViewPointRotator
	);
}

FVector UGrabber::GetLineTraceEnd() const
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	this->GetPlayerViewPointInfo(PlayerViewPointLocation, PlayerViewPointRotator);

	return PlayerViewPointLocation + (PlayerViewPointRotator.Vector() * this->Reach);
}
