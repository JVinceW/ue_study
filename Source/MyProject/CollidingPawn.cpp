// Fill out your copyright notice in the Description page of Project Settings.


#include "CollidingPawn.h"
#include "UObject/ConstructorHelpers.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CollidingPawnMovementComponent.h"


// Sets default values
ACollidingPawn::ACollidingPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	SphereComponent-> InitSphereRadius(40.0f);
	SphereComponent->SetCollisionProfileName(TEXT("Pawn"));

	// Create Sphere Mesh to match the sphere component
	UStaticMeshComponent* SphereVisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisualMesh->SetupAttachment(SphereComponent);
	static  ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere"));
	// According to the tutorial, the size of the sphere have to be set like this to fix the sphere component
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisualMesh->SetStaticMesh(SphereVisualAsset.Object);
		SphereVisualMesh->SetRelativeLocation(FVector(0.0f,0.0f,-40.0f));
		SphereVisualMesh->SetWorldScale3D(FVector(0.8f));
	}

	// Create Partical System
	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MovePartical"));
	FireParticle->SetupAttachment(SphereVisualMesh);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> FireParticleVisualAsset(TEXT("/Game/StarterContent/Particles/P_Fire.P_Fire"));
	if (FireParticleVisualAsset.Succeeded())
	{
		FireParticle->SetTemplate(FireParticleVisualAsset.Object);
	}

	InitSpringArm();

	AutoPossessPlayer = EAutoReceiveInput::Player0;
	
	// Create an instance of our movement component, and tell it to update the root.
	CollidingPawnMovementComponent = CreateDefaultSubobject<UCollidingPawnMovementComponent>(TEXT("CustomMovementComponent"));
	CollidingPawnMovementComponent->UpdatedComponent = RootComponent;
}

void ACollidingPawn::InitSpringArm()
{
	USpringArmComponent* SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));
	SpringArmComponent->TargetArmLength = 400.0f;
	SpringArmComponent->bEnableCameraLag = true;
	SpringArmComponent->CameraLagSpeed = 3.0f;

	// Create Camera into spring arm
	UCameraComponent* CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SpringArmCamera"));
	CameraComponent->SetupAttachment(SpringArmComponent, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ACollidingPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollidingPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACollidingPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("ParticleToggle", IE_Pressed, this, &ACollidingPawn::ToggleParticle);
	InputComponent->BindAxis("MoveForward", this, &ACollidingPawn::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ACollidingPawn::MoveRight);
	InputComponent->BindAxis("Turn", this, &ACollidingPawn::Turn);
}

UPawnMovementComponent* ACollidingPawn::GetMovementComponent() const
{
	return CollidingPawnMovementComponent;
}

void ACollidingPawn::MoveForward(float AxisValue)
{
	if (CollidingPawnMovementComponent && CollidingPawnMovementComponent->UpdatedComponent == RootComponent)
	{
		CollidingPawnMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
	}
}

void ACollidingPawn::MoveRight(float AxisValue)
{
	if (CollidingPawnMovementComponent && CollidingPawnMovementComponent->UpdatedComponent == RootComponent)
    	{
    		CollidingPawnMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
    	}
}

void ACollidingPawn::Turn(float AxisValue)
{
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += AxisValue;
	SetActorRotation(NewRotation);
}

void ACollidingPawn::ToggleParticle()
{
	if (FireParticle && FireParticle->Template)
	{
		FireParticle->ToggleActive();
	}
}


