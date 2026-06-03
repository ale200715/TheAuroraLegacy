#include "AuroraGameInstance.h"
#include "AuroraSaveGame.h"
#include "Kismet/GameplayStatics.h"

void UAuroraGameInstance::AddScore(int32 Amount)
{
    Score += Amount;
    UE_LOG(LogTemp, Warning, TEXT("Puntuacion: %d"), Score);
}

void UAuroraGameInstance::LoseLife()
{
    Lives--;
    UE_LOG(LogTemp, Warning, TEXT("Vidas restantes: %d"), Lives);
}

void UAuroraGameInstance::ResetStats()
{
    Score = 0;
    Lives = 3;
    CurrentLevel = 1;
    UE_LOG(LogTemp, Warning, TEXT("Stats reiniciados"));
}

void UAuroraGameInstance::SaveGame()
{
    UAuroraSaveGame* SaveData = Cast<UAuroraSaveGame>( UGameplayStatics::CreateSaveGameObject( UAuroraSaveGame::StaticClass()));

    SaveData->SavedScore = Score;
    SaveData->SavedLives = Lives;
    SaveData->HighScore = FMath::Max(Score, SaveData->HighScore);

    UGameplayStatics::SaveGameToSlot(SaveData, TEXT("AuroraSlot"), 0);
    UE_LOG(LogTemp, Warning, TEXT("Juego guardado!"));
}

void UAuroraGameInstance::LoadGame()
{
    UAuroraSaveGame* SaveData = Cast<UAuroraSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("AuroraSlot"), 0));

    if (SaveData)
    {
        Score = SaveData->SavedScore;
        Lives = SaveData->SavedLives;
        UE_LOG(LogTemp, Warning, TEXT("Juego cargado! Score: %d"), Score);
    }
}