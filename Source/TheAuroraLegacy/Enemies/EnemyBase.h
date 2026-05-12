#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.generated.h"

UCLASS()
class THEAURORALEGACY_API AEnemyBase
    : public AActor
{
    GENERATED_BODY()

public:
    AEnemyBase();
    virtual void Tick(float DeltaTime) override;

    // Vida del enemigo
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Stats")
    int32 Health = 3;

    // Daño por contacto con el jugador
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Stats")
    int32 ContactDamage = 1;

    // Puntos que da al morir
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Stats")
    int32 ScoreValue = 100;

    // Velocidad de movimiento
    UPROPERTY(EditAnywhere, BlueprintReadWrite,
        Category = "Stats")
    float MoveSpeed = 300.f;

    // Recibir daño del proyectil
    UFUNCTION()
    void TakeDamageEnemy(int32 DamageAmount);

    // Verificar si está vivo
    bool IsAlive() const { return Health > 0; }

protected:
    virtual void BeginPlay() override;

    // Cada enemigo implementa su propio
    // movimiento sobreescribiendo esto
    virtual void MoveEnemy(float DeltaTime);

    // Qué pasa cuando muere
    // Virtual para que cada fase personalice
    virtual void OnDeath();
};