#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerProjectile.generated.h"


UCLASS()
class THEAURORALEGACY_API APlayerProjectile : public AActor
{
    GENERATED_BODY()

public:
    APlayerProjectile();
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float ProjectileSpeed = 1500.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    int32 Damage = 1;

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* ProjectileMesh;

    UPROPERTY(VisibleAnywhere)
    class USphereComponent* CollisionSphere;
};