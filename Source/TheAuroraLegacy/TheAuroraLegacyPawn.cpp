// Copyright Epic Games, Inc. All Rights Reserved.
#include "TheAuroraLegacyPawn.h"
#include "Player/PlayerProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "TimerManager.h"

ATheAuroraLegacyPawn::ATheAuroraLegacyPawn()
{
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional <UStaticMesh> PlaneMesh;
        FConstructorStatics() : PlaneMesh(TEXT("/Game/Flying/Meshes/UFO.UFO"))
        {
        }
    };
    static FConstructorStatics ConstructorStatics;

    PlaneMesh = CreateDefaultSubobject
        <UStaticMeshComponent>(TEXT("PlaneMesh0"));
    PlaneMesh->SetStaticMesh(
        ConstructorStatics.PlaneMesh.Get());
    RootComponent = PlaneMesh;

    SpringArm = CreateDefaultSubobject
        <USpringArmComponent>(TEXT("SpringArm0"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 160.0f;
    SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
    SpringArm->bEnableCameraLag = false;
    SpringArm->CameraLagSpeed = 15.f;

    Camera = CreateDefaultSubobject
        <UCameraComponent>(TEXT("Camera0"));
    Camera->SetupAttachment(SpringArm,
        USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    Acceleration = 500.f;
    TurnSpeed = 50.f;
    MaxSpeed = 4000.f;
    MinSpeed = 0.f;
    CurrentForwardSpeed = 0.f;

    Lives = 3;
    bCanFire = true;

    static ConstructorHelpers::FClassFinder<APlayerProjectile> ProjectileBP(TEXT("/Game/Player/BP_PlayerProjectile"));
    if (ProjectileBP.Class != nullptr)
    {
        ProjectileClass = ProjectileBP.Class;
    }
}

void ATheAuroraLegacyPawn::Tick(float DeltaSeconds)
{
    const FVector LocalMove = FVector( CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f);

    AddActorLocalOffset(LocalMove, true);

    FRotator DeltaRotation(0, 0, 0);
    DeltaRotation.Pitch = CurrentPitchSpeed * DeltaSeconds;
    DeltaRotation.Yaw = CurrentYawSpeed * DeltaSeconds;
    DeltaRotation.Roll = CurrentRollSpeed * DeltaSeconds;

    AddActorLocalRotation(DeltaRotation);

    Super::Tick(DeltaSeconds);
}

void ATheAuroraLegacyPawn::NotifyHit(
    class UPrimitiveComponent* MyComp,
    class AActor* Other,
    class UPrimitiveComponent* OtherComp,
    bool bSelfMoved,
    FVector HitLocation,
    FVector HitNormal,
    FVector NormalImpulse,
    const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    FRotator CurrentRotation = GetActorRotation();
    SetActorRotation(FQuat::Slerp(CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

void ATheAuroraLegacyPawn::SetupPlayerInputComponent(
    class UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);

    PlayerInputComponent->BindAxis("Thrust", this, &ATheAuroraLegacyPawn::ThrustInput);
    PlayerInputComponent->BindAxis("MoveUp", this, &ATheAuroraLegacyPawn::MoveUpInput);
    PlayerInputComponent->BindAxis("MoveRight", this, &ATheAuroraLegacyPawn::MoveRightInput);

    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATheAuroraLegacyPawn::Fire);
}


void ATheAuroraLegacyPawn::ThrustInput(float Val)
{
    bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);

    if (bHasInput)// si se apreta Q este avancza osea CurrentFowardSpeed sera != 0 y en el tick se va a mover el actor hacia adelannte
    {
        float CurrentAcc = Val * Acceleration;
        float NewForwardSpeed = CurrentForwardSpeed +(GetWorld()->GetDeltaSeconds() * CurrentAcc);
        CurrentForwardSpeed = FMath::Clamp(NewForwardSpeed, -MaxSpeed, MaxSpeed);
    }
    else //si no se apreta nada currentFowardSpeed pa a ser 0 y el actor no se va a a mover 
    {
        CurrentForwardSpeed = FMath::FInterpTo(CurrentForwardSpeed, 0.f, GetWorld()->GetDeltaSeconds(), 5.f);
    }
}

void ATheAuroraLegacyPawn::MoveUpInput(float Val)
{
    float TargetPitchSpeed = (Val * TurnSpeed * -1.f);
    TargetPitchSpeed += (FMath::Abs(CurrentYawSpeed) * -0.2f);
    CurrentPitchSpeed = FMath::FInterpTo(CurrentPitchSpeed, TargetPitchSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ATheAuroraLegacyPawn::MoveRightInput(float Val)
{
    float TargetYawSpeed = (Val * TurnSpeed);
    CurrentYawSpeed = FMath::FInterpTo(CurrentYawSpeed, TargetYawSpeed, GetWorld()->GetDeltaSeconds(), 2.f);

    const bool bIsTurning = FMath::Abs(Val) > 0.2f;
    float TargetRollSpeed = bIsTurning ? (CurrentYawSpeed * 0.5f) :  (GetActorRotation().Roll * -2.f);
    CurrentRollSpeed = FMath::FInterpTo(
        CurrentRollSpeed, TargetRollSpeed,
        GetWorld()->GetDeltaSeconds(), 2.f);
}

void ATheAuroraLegacyPawn::Fire()
{
    UE_LOG(LogTemp, Warning, TEXT("Fire llamado!"));

    if (!ProjectileClass)
    {
        UE_LOG(LogTemp, Error, TEXT("ProjectileClass es NULL!"));
        return;
    }

    if (!bCanFire)
    {
        UE_LOG(LogTemp, Warning, TEXT("No puede disparar aun"));
        return;
    }

    FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 150.f;
    FRotator SpawnRotation = GetActorRotation();

    GetWorld()->SpawnActor<APlayerProjectile>( ProjectileClass, SpawnLocation, SpawnRotation);

    bCanFire = false;
    GetWorldTimerManager().SetTimer( FireTimerHandle, this, &ATheAuroraLegacyPawn::ResetFire, FireRate, false);
}

void ATheAuroraLegacyPawn::TakeDamage_Ship( int32 DamageAmount)
{
    Lives -= DamageAmount;

    if (Lives <= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Game Over - Sin vidas"));
        Destroy();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Vidas restantes: %d"), Lives);
    }
}

void ATheAuroraLegacyPawn::ResetFire()
{
    bCanFire = true;
}