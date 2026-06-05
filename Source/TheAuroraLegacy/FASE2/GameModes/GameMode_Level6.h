#pragma once
#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "GameMode_Level6.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDestroyerCountChanged, int32, RemainingDestroyers);

UCLASS()
class THEAURORALEGACY_API AGameMode_Level6 : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()

public:
    AGameMode_Level6();
    virtual void BeginPlay() override;


    UFUNCTION()
    void OnEnemyDefeated();

    UPROPERTY(BlueprintAssignable, Category = "Observer")
    FOnDestroyerCountChanged OnDestroyerCountChanged;

private:
    void SpawnDestroyer();
};
