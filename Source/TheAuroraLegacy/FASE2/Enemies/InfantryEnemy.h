#pragma once
#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "InfantryEnemy.generated.h"

UCLASS()
class THEAURORALEGACY_API AInfantryEnemy : public AEnemyBase
{
    GENERATED_BODY()

public:
    AInfantryEnemy();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float FireRate = 0.3f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    int32 BurstCount = 3;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TSubclassOf<class AEnemyProjectile> ProjectileClass;

    UFUNCTION()
    void OnInfantryDestroyed(AActor* DestroyedActor);

private:
    FTimerHandle FireTimerHandle;
    int32 CurrentBurst = 0;
    void FireBurst();
    void NotifyDeath();
};
