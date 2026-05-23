// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "Components/StaticMeshComponent.h"
#include "Interceptor.generated.h"

UCLASS()
class THEAURORALEGACY_API AInterceptor : public AEnemyBase
{
    GENERATED_BODY()

public:
    AInterceptor();
    virtual void Tick(float DeltaTime) override;

    // Componente visual del Interceptor
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly,
        Category = "Components")
    UStaticMeshComponent* MeshComponent;

protected:
    virtual void BeginPlay() override;
    virtual void MoveEnemy(float DeltaTime) override;

private:
    FVector MoveDirection;
    AActor* PlayerActor;
    bool bGoingRight;
};
