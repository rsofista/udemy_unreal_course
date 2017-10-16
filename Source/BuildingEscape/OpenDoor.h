// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

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
	FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable)
	FDoorEvent OnClose;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// The trigger volume that serves as a pressure plate
	UPROPERTY(EditAnywhere)
	ATriggerVolume* PressurePlate = nullptr;
	// Minimum ammount of mass to open the door in Kg
	UPROPERTY(EditAnywhere)
	float MinimumMassToOpenDoor = 30.0f;
private:

	// Returns the sum of the masses of the elements inside PressurePlate
	float GetTotalMassOfActorsInThePressurePlate() const;
};
