#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFacade.generated.h"

// Forward declarations
class UAuroraGameInstance;
class ATheAuroraLegacyGameMode;

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
    void SpawnEnemy();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void SaveGame();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void LoadGame();

    UFUNCTION(BlueprintCallable, Category = "Facade")
    void TriggerGameOver();

private:
    UAuroraGameInstance* GetGI();
    ATheAuroraLegacyGameMode* GetGM();
};
