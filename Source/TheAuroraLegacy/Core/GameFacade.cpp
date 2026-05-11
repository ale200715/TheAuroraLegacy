#include "GameFacade.h"
#include "../AuroraGameInstance.h"
#include "../TheAuroraLegacyGameMode.h"
#include "Kismet/GameplayStatics.h"

AGameFacade::AGameFacade()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AGameFacade::BeginPlay()
{
    Super::BeginPlay();
}

UAuroraGameInstance* AGameFacade::GetGI()
{
    return Cast<UAuroraGameInstance>(
        GetGameInstance());
}

ATheAuroraLegacyGameMode* AGameFacade::GetGM()
{
    return Cast<ATheAuroraLegacyGameMode>(
        GetWorld()->GetAuthGameMode());
}

void AGameFacade::AddScore(int32 Amount)
{
    if (UAuroraGameInstance* GI = GetGI())
        GI->AddScore(Amount);
}

void AGameFacade::LoseLife()
{
    if (UAuroraGameInstance* GI = GetGI())
        GI->LoseLife();
}

void AGameFacade::SpawnEnemy()
{
    if (ATheAuroraLegacyGameMode* GM = GetGM())
        GM->SpawnEnemy();
}

void AGameFacade::SaveGame()
{
    if (UAuroraGameInstance* GI = GetGI())
        GI->SaveGame();
}

void AGameFacade::LoadGame()
{
    if (UAuroraGameInstance* GI = GetGI())
        GI->LoadGame();
}

void AGameFacade::TriggerGameOver()
{
    SaveGame();
    UE_LOG(LogTemp, Warning, TEXT("GAME OVER"));
    UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}

