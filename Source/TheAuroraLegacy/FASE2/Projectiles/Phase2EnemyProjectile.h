#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Phase2EnemyProjectile.generated.h"

UCLASS()
class THEAURORALEGACY_API APhase2EnemyProjectile : public AActor
{
    GENERATED_BODY()

public:
    APhase2EnemyProjectile();
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float ProjectileSpeed = 800.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    int32 Damage = 1;

    FVector Direction;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    class USphereComponent* CollisionSphere;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* ProjectileMesh;

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        FVector NormalImpulse,
        const FHitResult& Hit);
};