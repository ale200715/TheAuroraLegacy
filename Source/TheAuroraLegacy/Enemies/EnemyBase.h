#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

UCLASS()
class THEAURORALEGACY_API AEnemyBase : public AActor
{
    GENERATED_BODY()

public:
    AEnemyBase();

    virtual void Tick(float DeltaTime) override;

    // Vida del enemigo
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Health = 3;

    // Daño que recibe
    UFUNCTION()
    void TakeDamageEnemy(int32 DamageAmount);

    // Velocidad de movimiento
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    float MoveSpeed = 300.f;

protected:
    virtual void BeginPlay() override;
};