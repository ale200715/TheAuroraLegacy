#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AuroraGameInstance.generated.h"

UCLASS()
class THEAURORALEGACY_API UAuroraGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    // Puntuaci�n
    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 Score = 0;

    // Vidas
    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 Lives = 3;

    // Nivel actual
    UPROPERTY(BlueprintReadWrite, Category = "Stats")
    int32 CurrentLevel = 1;

    // Funciones
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
