// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "NaveNodrizaProjectile.generated.h"

UCLASS()
class THEAURORALEGACY_API ANaveNodrizaProjectile : public AActor
{
    GENERATED_BODY()

public:
    ANaveNodrizaProjectile();
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float ProjectileSpeed = 600.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    int32 Damage = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* CollisionSphere;

protected:
    virtual void BeginPlay() override;

};
