#pragma once
#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "GameMode_Level5.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTankCountChanged, int32, RemainingTanks);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTankGroupSpawned, int32, GroupSize);

UCLASS()
class THEAURORALEGACY_API AGameMode_Level5 : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()

public:
    AGameMode_Level5();
    virtual void BeginPlay() override;


    UFUNCTION()
    void OnEnemyDefeated();

    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnTankCountChanged OnTankCountChanged;

    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnTankGroupSpawned OnTankGroupSpawned;

private:
    void SpawnTank();
};
