
#include "TheAuroraLegacyGameMode.h"
#include "TheAuroraLegacyPawn.h"
#include "AuroraGameInstance.h"
#include "UI/GoodEndingWidget.h"
#include "Core/GameFacade.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "UI/AuroraHUD.h"

ATheAuroraLegacyGameMode::ATheAuroraLegacyGameMode()
{
    PrimaryActorTick.bCanEverTick = false; 
    DefaultPawnClass = ATheAuroraLegacyPawn::StaticClass();
}

void ATheAuroraLegacyGameMode::BeginPlay()
{
    Super::BeginPlay();

    if (UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(GetGameInstance()))
    {
            FString MapName = GetWorld()->GetMapName();
            int32 UnderscoreIdx;
            if (MapName.StartsWith(TEXT("UEDPIE_"))) 
            {
                MapName.FindChar('_', UnderscoreIdx);
				MapName = MapName.RightChop(UnderscoreIdx + 1);
				MapName.FindChar('_', UnderscoreIdx);
				MapName = MapName.RightChop(UnderscoreIdx + 1);
            }
            GI->CurrentLevelName = FName(*MapName);
            UE_LOG(LogTemp, Warning,TEXT("GameMode: Nivel guardado en GI: %s"), *MapName);
    }

    AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), AGameFacade::StaticClass());
    GameFacadeInstance = Cast<AGameFacade>(Found);

    AAuroraHUD* HUD = Cast<AAuroraHUD>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetHUD());
    if (HUD)
    {
        if (UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(GetGameInstance()))
        {
            FString MapName = GetWorld()->GetMapName();
            MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
            HUD->UpdateLevel(GI->CurrentLevel);
            UE_LOG(LogTemp, Warning, TEXT("Mapa actual: %s"), *MapName);
        }
        HUD->UpdateHealth(3);
    }
    ATheAuroraLegacyPawn* Player = Cast<ATheAuroraLegacyPawn>( UGameplayStatics::GetPlayerPawn( GetWorld(), 0));
    if (Player)
    {
        Player->Lives = 3;
    }
    GetWorldTimerManager().SetTimer(SpawnTimerHandle,this,&ATheAuroraLegacyGameMode::SpawnEnemy,SpawnInterval,true);

}

void ATheAuroraLegacyGameMode::SpawnEnemy() {

}

void ATheAuroraLegacyGameMode::OnEnemyDefeated(int32 ScoreValue)
{
    EnemiesDefeated++;

    // Actualizar HUD con score actual del GameInstance
    AAuroraHUD* HUD = Cast<AAuroraHUD>( UGameplayStatics::GetPlayerController( GetWorld(), 0)->GetHUD());
    if (HUD)
    {
        if (UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(GetGameInstance()))
        {
            HUD->UpdateScore(GI->Score);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("GameMode: %d / %d enemigos derrotados"), EnemiesDefeated, EnemiesRequired);

    CheckLevelComplete();
}

void ATheAuroraLegacyGameMode::CheckLevelComplete()
{
    if (EnemiesDefeated < EnemiesRequired) return;

    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

    UE_LOG(LogTemp, Warning, TEXT("GameMode: Nivel completado!"));

    FTimerHandle TransitionTimer;
    GetWorldTimerManager().SetTimer( TransitionTimer, this,&ATheAuroraLegacyGameMode::LoadNextLevel, 2.f,false);
}

void ATheAuroraLegacyGameMode::LoadNextLevel()
{
    if (UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(GetGameInstance()))
    {
        GI->CurrentLevel++;
    }

    if (NextLevelName != NAME_None)
    {
        UGameplayStatics::OpenLevel(this, NextLevelName);
    }
    else
    {
        UE_LOG(LogTemp, Warning,TEXT("GameMode: NextLevelName no definido en este nivel."));
    }
}

void ATheAuroraLegacyGameMode::OnPlayerDeath()
{
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    ShowGameOver();
}

void ATheAuroraLegacyGameMode::ShowGameOver()
{
    if (!GameOverWidgetClass) return;

    UGameOverWidget* Widget = CreateWidget<UGameOverWidget>( GetWorld(), GameOverWidgetClass);
    if (!Widget) return;

    int32 CurrentLevelNum = 1;
    FName LevelToRetry = FName("Level1_Drone");

    if (UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(GetGameInstance()))
    {
        CurrentLevelNum = GI->CurrentLevel;
        LevelToRetry = GI->CurrentLevelName; // nombre limpio sin prefijo PIE
    }

    Widget->SetupGameOver(CurrentLevelNum, PhaseNumber, LevelToRetry);
    Widget->AddToViewport();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }
}

void ATheAuroraLegacyGameMode::ShowGoodEnding()
{
    if (!GoodEndingWidgetClass) return;

    UGoodEndingWidget* Widget = CreateWidget<UGoodEndingWidget>( GetWorld(), GoodEndingWidgetClass);

    if (!Widget) return;

    Widget->AddToViewport();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }
}
