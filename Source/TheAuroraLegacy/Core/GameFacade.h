#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFacade.generated.h"

class UAuroraGameInstance;
class ATheAuroraLegacyGameMode;
class AEnemyBase;

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
    Drone,
    Hunter,
    Support,
    Assault,
    Maneuver,
    Armored,
    Interceptor,
    Turret,
    Boss
};

UCLASS()
class THEAURORALEGACY_API AGameFacade : public AActor
{
    GENERATED_BODY()

public:
    AGameFacade();
    virtual void BeginPlay() override;

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void AddScore(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void LoseLife();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void SaveGame();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void LoadGame();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void TriggerGameOver();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void ConfigureEnemy(AEnemyBase* Enemy, EEnemyType Type);

    void NotifyEnemyDefeated(AEnemyBase* Enemy);

private:
    UAuroraGameInstance* GetGI();
    ATheAuroraLegacyGameMode* GetGM();
};