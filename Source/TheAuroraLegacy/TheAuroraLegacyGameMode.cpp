
#include "TheAuroraLegacyGameMode.h"
#include "TheAuroraLegacyPawn.h"
#include "AuroraGameInstance.h"
#include "UI/GoodEndingWidget.h"
#include "Core/GameFacade.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

ATheAuroraLegacyGameMode::ATheAuroraLegacyGameMode()
{
    PrimaryActorTick.bCanEverTick = false; // no usamos Tick en el GameMode
    DefaultPawnClass = ATheAuroraLegacyPawn::StaticClass();
}

void ATheAuroraLegacyGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Buscar el Facade que debe existir en la escena como Actor
    AActor* Found = UGameplayStatics::GetActorOfClass(
        GetWorld(), AGameFacade::StaticClass());
    GameFacadeInstance = Cast<AGameFacade>(Found);

    if (!GameFacadeInstance)
    {
        UE_LOG(LogTemp, Error,
            TEXT("GameMode: NO se encontró GameFacade en la escena. "
                "Agrégalo como Actor en el nivel."));
    }

    // Arrancar el timer de spawn — los hijos sobreescriben SpawnEnemy()
    GetWorldTimerManager().SetTimer(
        SpawnTimerHandle,
        this,
        &ATheAuroraLegacyGameMode::SpawnEnemy,
        SpawnInterval,
        true);
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

    // NOTA: el score ya fue sumado por el Facade en NotifyEnemyDefeated().
    // No llamar AddScore() aquí — evita doblar el score.

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
    GetWorldTimerManager().SetTimer(
        TransitionTimer,
        this,
        &ATheAuroraLegacyGameMode::LoadNextLevel,
        2.f,
        false);
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

    // PhaseNumber y CurrentLevel ahora son dinámicos — no más hardcoding
    int32 CurrentLevel = 1;
    if (UAuroraGameInstance* GI = Cast<UAuroraGameInstance>(GetGameInstance()))
        CurrentLevel = GI->CurrentLevel;

    Widget->SetupGameOver(
        CurrentLevel,
        PhaseNumber,
        FName(*GetWorld()->GetMapName()));

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
