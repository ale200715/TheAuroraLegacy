// Copyright Epic Games, Inc. All Rights Reserved.

#include "TheAuroraLegacyPawn.h"
#include "Player/PlayerProjectile.h"
#include "UObject/ConstructorHelpers.h"
#include "Camera/CameraComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/GameModeBase.h"
#include "TheAuroraLegacyGameMode.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "UI/AuroraHUD.h"


ATheAuroraLegacyPawn::ATheAuroraLegacyPawn()
{
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UStaticMesh> PlaneMesh;
        FConstructorStatics(): PlaneMesh(TEXT("StaticMesh'/Game/Flying/Meshes/UFO.UFO'")) {}
    };
    static FConstructorStatics ConstructorStatics;

    PlaneMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PlaneMesh0"));
    PlaneMesh->SetStaticMesh(ConstructorStatics.PlaneMesh.Get());

    PlaneMesh->SetCollisionObjectType(ECC_Pawn);
    PlaneMesh->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    PlaneMesh->SetCollisionResponseToAllChannels(ECR_Ignore);
    PlaneMesh->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
    PlaneMesh->SetGenerateOverlapEvents(true);

    RootComponent = PlaneMesh;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>( TEXT("SpringArm0"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = 160.f;
    SpringArm->SocketOffset = FVector(0.f, 0.f, 60.f);
    SpringArm->bEnableCameraLag = false;
    SpringArm->CameraLagSpeed = 15.f;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera0"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    Acceleration = 500.f;
    TurnSpeed = 50.f;
    MaxSpeed = 4000.f;
    CurrentForwardSpeed = 0.f;
    Lives = 3;
    bCanFire = true;

    static ConstructorHelpers::FClassFinder<APlayerProjectile> ProjectileBP(TEXT("/Game/Player/BP_PlayerProjectile"));
    if (ProjectileBP.Class) {
        ProjectileClass = ProjectileBP.Class;
    }
}

void ATheAuroraLegacyPawn::Tick(float DeltaSeconds)
{
    AddActorLocalOffset( FVector(CurrentForwardSpeed * DeltaSeconds, 0.f, 0.f), true);

    FRotator DeltaRotation( CurrentPitchSpeed * DeltaSeconds,CurrentYawSpeed * DeltaSeconds, CurrentRollSpeed * DeltaSeconds);
    AddActorLocalRotation(DeltaRotation);

    Super::Tick(DeltaSeconds);
}

void ATheAuroraLegacyPawn::NotifyHit(  UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved,HitLocation, HitNormal, NormalImpulse, Hit);

    FRotator CurrentRotation = GetActorRotation();
    SetActorRotation(FQuat::Slerp( CurrentRotation.Quaternion(), HitNormal.ToOrientationQuat(), 0.025f));
}

void ATheAuroraLegacyPawn::SetupPlayerInputComponent( UInputComponent* PlayerInputComponent)
{
    check(PlayerInputComponent);
    PlayerInputComponent->BindAxis("Thrust", this,&ATheAuroraLegacyPawn::ThrustInput);
    PlayerInputComponent->BindAxis("MoveUp", this, &ATheAuroraLegacyPawn::MoveUpInput);
    PlayerInputComponent->BindAxis("MoveRight", this,&ATheAuroraLegacyPawn::MoveRightInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this,&ATheAuroraLegacyPawn::Fire);
}

void ATheAuroraLegacyPawn::ThrustInput(float Val)
{
    bool bHasInput = !FMath::IsNearlyEqual(Val, 0.f);
    if (bHasInput)
    {
        float NewForwardSpeed = CurrentForwardSpeed + (GetWorld()->GetDeltaSeconds() * Val * Acceleration);
        CurrentForwardSpeed = FMath::Clamp( NewForwardSpeed, -MaxSpeed, MaxSpeed);
    }
    else
    {
        CurrentForwardSpeed = FMath::FInterpTo( CurrentForwardSpeed, 0.f,GetWorld()->GetDeltaSeconds(), 5.f);
    }
}

void ATheAuroraLegacyPawn::MoveUpInput(float Val)
{
    float TargetPitchSpeed = Val * TurnSpeed * -1.f;
    TargetPitchSpeed += FMath::Abs(CurrentYawSpeed) * -0.2f;
    CurrentPitchSpeed = FMath::FInterpTo( CurrentPitchSpeed, TargetPitchSpeed,GetWorld()->GetDeltaSeconds(), 2.f);
}

void ATheAuroraLegacyPawn::MoveRightInput(float Val)
{
    float TargetYawSpeed = Val * TurnSpeed;
    CurrentYawSpeed = FMath::FInterpTo( CurrentYawSpeed, TargetYawSpeed,GetWorld()->GetDeltaSeconds(), 2.f);

    float TargetRollSpeed = FMath::Abs(Val) > 0.2f ? CurrentYawSpeed * 0.5f : GetActorRotation().Roll * -2.f;
    CurrentRollSpeed = FMath::FInterpTo( CurrentRollSpeed, TargetRollSpeed, GetWorld()->GetDeltaSeconds(), 2.f);
}

void ATheAuroraLegacyPawn::Fire()
{
    FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 150.f;
    GetWorld()->SpawnActor<APlayerProjectile>( ProjectileClass, SpawnLocation, GetActorRotation());
    bCanFire = false;
    GetWorldTimerManager().SetTimer(FireTimerHandle, this,&ATheAuroraLegacyPawn::ResetFire, FireRate, false);
}

void ATheAuroraLegacyPawn::TakeDamage_Ship(int32 DamageAmount)
{
    Lives -= DamageAmount;

    Lives = FMath::Max(Lives, 0);

    AAuroraHUD* HUD = Cast<AAuroraHUD>(
        UGameplayStatics::GetPlayerController(
            GetWorld(), 0)->GetHUD());
    if (HUD)
    {
        HUD->UpdateHealth(Lives);
    }

    UE_LOG(LogTemp, Warning, TEXT("Vidas restantes: %d"), Lives);

    if (Lives <= 0)
    {
        ATheAuroraLegacyGameMode* GM = Cast<ATheAuroraLegacyGameMode>(GetWorld()->GetAuthGameMode());
        if (GM) GM->OnPlayerDeath();

        SetActorHiddenInGame(true);
        SetActorEnableCollision(false);
        SetActorTickEnabled(false);
    }
}
void ATheAuroraLegacyPawn::ResetFire()
{
    bCanFire = true;
}