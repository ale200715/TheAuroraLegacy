#pragma once
#include "CoreMinimal.h"
#include "EnemyBase.h"
#include "DestroyerEnemy.generated.h"

// Estados de la State Machine
UENUM()
enum class EDestroyerState : uint8
{
    Normal,
    Shield,
    Attack,
    Dead
};

UCLASS()
class THEAURORALEGACY_API ADestroyerEnemy : public AEnemyBase
{
    GENERATED_BODY()

public:
    ADestroyerEnemy();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Estado actual
    UPROPERTY(BlueprintReadOnly, Category = "StateMachine")
    EDestroyerState CurrentState = EDestroyerState::Normal;

    // Escudo
    UPROPERTY(EditAnywhere, Category = "Combat")
    float ShieldInterval = 8.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float ShieldDuration = 3.f;

    // Disparo triple
    UPROPERTY(EditAnywhere, Category = "Combat")
    float AttackInterval = 3.f;

    UPROPERTY(EditAnywhere, Category = "Combat")
    TSubclassOf<class APhase2EnemyProjectile> ProjectileClass;

    // Sobreescribir daño para ignorarlo con escudo
    void TakeDamageDestroyer(int32 DamageAmount);

private:
    FTimerHandle ShieldTimerHandle;
    FTimerHandle AttackTimerHandle;
    FTimerHandle ShieldEndTimerHandle;

    void ActivateShield();
    void DeactivateShield();
    void TripleAttack();
    void NotifyDeath();
    void ChangeState(EDestroyerState NewState);

    UFUNCTION()
    void OnDestroyerDestroyed(AActor* DestroyedActor);
};
