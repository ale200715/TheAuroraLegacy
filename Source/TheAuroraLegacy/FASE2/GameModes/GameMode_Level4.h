#pragma once
#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "GameMode_Level4.generated.h"

// Observer Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyCountChanged, int32, RemainingEnemies);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyGroupSpawned, int32, GroupSize);

UCLASS()
class THEAURORALEGACY_API AGameMode_Level4 : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()

public:
    AGameMode_Level4();
    virtual void BeginPlay() override;

    // Enemigos a derrotar en este nivel 

    UFUNCTION()
    void OnEnemyDefeated();

    // Observer
    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnEnemyCountChanged OnEnemyCountChanged;

    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnEnemyGroupSpawned OnEnemyGroupSpawned;

private:
    int32 ActiveEnemies = 0;
    void SpawnEnemyGroup();
}; 