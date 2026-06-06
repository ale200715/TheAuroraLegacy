
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
            // Quitar prefijo PIE (UEDPIE_0_, UEDPIE_1_, etc.)
            int32 UnderscoreIdx;
            if (MapName.StartsWith(TEXT("UEDPIE_")))
            {
                MapName.FindChar('_', UnderscoreIdx);         // primer _
                MapName = MapName.RightChop(UnderscoreIdx + 1); // quita UEDPIE_
                MapName.FindChar('_', UnderscoreIdx);         // segundo _
                MapName = MapName.RightChop(UnderscoreIdx + 1); // quita 0_
            }
            GI->CurrentLevelName = FName(*MapName);
            UE_LOG(LogTemp, Warning,
                TEXT("GameMode: Nivel guardado en GI: %s"), *MapName);

    }

    AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), AGameFacade::StaticClass());
    GameFacadeInstance = Cast<AGameFacade>(Found);

    if (!GameFacadeInstance)
    {
        UE_LOG(LogTemp, Error,TEXT("GameMode: NO se encontró GameFacade en la escena. Agrégalo como Actor en el nivel."));
    }

    // Actualizar HUD al iniciar nivel
    AAuroraHUD* HUD = Cast<AAuroraHUD>(
        UGameplayStatics::GetPlayerController(
            GetWorld(), 0)->GetHUD());
    if (HUD)
    {
        // Mostrar nivel actual desde GameInstance
        if (UAuroraGameInstance* GI =
            Cast<UAuroraGameInstance>(GetGameInstance()))
        {
            FString MapName = GetWorld()->GetMapName();
            MapName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);
            HUD->UpdateLevel(GI->CurrentLevel);
            UE_LOG(LogTemp, Warning, TEXT("Mapa actual: %s"), *MapName);
        }

        // Mostrar vida completa al iniciar nivel
        HUD->UpdateHealth(3);
    }

    // Resetear vida del jugador al iniciar nivel
    ATheAuroraLegacyPawn* Player =
        Cast<ATheAuroraLegacyPawn>(
            UGameplayStatics::GetPlayerPawn(
                GetWorld(), 0));
    if (Player)
    {
        Player->Lives = 3;
    }

    // Arrancar el timer de spawn — los hijos sobreescriben SpawnEnemy()
    GetWorldTimerManager().SetTimer(SpawnTimerHandle,this,&ATheAuroraLegacyGameMode::SpawnEnemy,SpawnInterval,true);

}

// ── Spawn ─────────────────────────────────────────────────────────────────────

void ATheAuroraLegacyGameMode::SpawnEnemy()
{
    // Base vacía — cada hijo implementa su lógica de spawn
}

// ── Flujo del nivel ───────────────────────────────────────────────────────────

void ATheAuroraLegacyGameMode::OnEnemyDefeated(int32 ScoreValue)
{
    EnemiesDefeated++;

    // Actualizar HUD con score actual del GameInstance
    AAuroraHUD* HUD = Cast<AAuroraHUD>(
        UGameplayStatics::GetPlayerController(
            GetWorld(), 0)->GetHUD());
    if (HUD)
    {
        if (UAuroraGameInstance* GI =
            Cast<UAuroraGameInstance>(GetGameInstance()))
        {
            HUD->UpdateScore(GI->Score);
        }
    }

    UE_LOG(LogTemp, Warning,
        TEXT("GameMode: %d / %d enemigos derrotados"),
        EnemiesDefeated, EnemiesRequired);

    CheckLevelComplete();
}

void ATheAuroraLegacyGameMode::CheckLevelComplete()
{
    if (EnemiesDefeated < EnemiesRequired) return;

    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

    UE_LOG(LogTemp, Warning, TEXT("GameMode: Nivel completado!"));

    // Level9 sobreescribe CheckLevelComplete() para llamar ShowGoodEnding().
    // El resto de niveles llega aquí y carga el siguiente nivel.
    FTimerHandle TransitionTimer;
    GetWorldTimerManager().SetTimer( TransitionTimer, this,&ATheAuroraLegacyGameMode::LoadNextLevel, 2.f,false);
}

void ATheAuroraLegacyGameMode::LoadNextLevel()
{
    // Actualizar CurrentLevel en el GameInstance antes de cambiar de mapa
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
        UE_LOG(LogTemp, Warning,
            TEXT("GameMode: NextLevelName no definido en este nivel."));
    }
}

// ── Muerte del jugador ────────────────────────────────────────────────────────

void ATheAuroraLegacyGameMode::OnPlayerDeath()
{
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
    ShowGameOver();
}

void ATheAuroraLegacyGameMode::ShowGameOver()
{
    if (!GameOverWidgetClass) return;

    UGameOverWidget* Widget = CreateWidget<UGameOverWidget>(
        GetWorld(), GameOverWidgetClass);
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

// ── Good Ending (solo Level9 llega aquí) ─────────────────────────────────────

void ATheAuroraLegacyGameMode::ShowGoodEnding()
{
    if (!GoodEndingWidgetClass) return;

    UGoodEndingWidget* Widget = CreateWidget<UGoodEndingWidget>(
        GetWorld(), GoodEndingWidgetClass);

    if (!Widget) return;

    Widget->AddToViewport();

    APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
    if (PC)
    {
        PC->SetShowMouseCursor(true);
        PC->SetInputMode(FInputModeUIOnly());
    }
}
