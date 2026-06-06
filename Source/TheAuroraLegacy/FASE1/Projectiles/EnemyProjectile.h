
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

    UPROPERTY(EditAnywhere, Category = "Combat")
    float Speed = 1800.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    int32 Damage = 1;

    void ScheduleDeactivation(float Delay);
    void DeactivateSelf();

    USphereComponent* CollisionSphere;
    UStaticMeshComponent* ProjectileMesh;

protected:
    virtual void BeginPlay() override;

private:
    FTimerHandle DeactivateTimerHandle;

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,
        AActor* OtherActor,
        UPrimitiveComponent* OtherComp,
        int32 OtherBodyIndex,
        bool bFromSweep,
        const FHitResult& SweepResult);
};