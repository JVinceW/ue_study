// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CollidingPawnMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "CollidingPawn.generated.h"

UCLASS()
class MYPROJECT_API ACollidingPawn : public APawn
{
	GENERATED_BODY()
	UCollidingPawnMovementComponent* CollidingPawnMovementComponent;
	UParticleSystemComponent* FireParticle;
	void InitSpringArm();

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void ToggleParticle();


public:
	// Sets default values for this pawn's properties
	ACollidingPawn();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
