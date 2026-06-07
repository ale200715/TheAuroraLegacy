
#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuroraGameInstance.generated.h"

UCLASS()
class THEAURORALEGACY_API UAuroraGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 Score = 0;

    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 Lives = 3;

    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 CurrentLevel = 1;
    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    FName CurrentLevelName = FName("Level1_Drone");

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void AddScore(int32 Amount);

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void LoseLife();

    UFUNCTION(BlueprintCallable, Category = "Stats")
    void ResetStats();

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
    void SaveGame();

    UFUNCTION(BlueprintCallable, Category = "SaveGame")
    void LoadGame();
};