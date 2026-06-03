#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class THEAURORALEGACY_API AEnemyProjectile : public AActor
{
    GENERATED_BODY()
public:
    AEnemyProjectile();
    virtual void Tick(float DeltaTime) override;

    void DeactivateSelf();

    USphereComponent* CollisionSphere;
    UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float Speed = 1800.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
    int32 Damage = 1;

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle DeactivateTimer;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp,AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
    void ScheduleDeactivation(float Delay);

private:
    FTimerHandle DeactivateTimerHandle;
};