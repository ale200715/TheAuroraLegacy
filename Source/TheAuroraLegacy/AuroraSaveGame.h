#pragma once
#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "AuroraSaveGame.generated.h"

UCLASS()
class THEAURORALEGACY_API UAuroraSaveGame : public USaveGame
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    int32 SavedScore = 0;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    int32 SavedLives = 3;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    int32 HighScore = 0;

    UPROPERTY(BlueprintReadWrite, Category = "SaveData")
    FString SaveSlotName = TEXT("AuroraSlot");
};
