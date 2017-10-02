// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOpenRequest);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()	

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:
	UPROPERTY(BlueprintAssignable)
	FOnOpenRequest OnOpenRequest;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// The angle in which the door will be opened
	UPROPERTY(VisibleAnywhere)
	float OpenAngle = -15.0f;
	// The trigger volume that serves as a pressure plate
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	// Seconds in which the door will remain opened
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.0f;
	// Minimum sum of mass of the elements in the PressurePlate to open the door
	UPROPERTY(EditAnywhere)
	float MinimumMassToOpenDoor = 30.0f;
	// Last time the door was opened, it serves to control the DoorCloseDelay
	float LastDoorOpenTime = 0.0f;
private:
	// Actively opens the door
	void OpenDoor();
	// Actively closes the door
	void CloseDoor();
	// Returns the sum of the masses of the elements inside PressurePlate
	float GetTotalMassOfActorsInThePressurePlate() const;
};
