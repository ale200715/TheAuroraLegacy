// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

UCLASS()
class THEAURORALEGACY_API AMainMenuGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    AMainMenuGameMode();

    virtual void BeginPlay() override;

    // Clase del widget del menu principal
    UPROPERTY(EditDefaultsOnly, Category = "Menu")
    TSubclassOf<class UUserWidget> MainMenuWidgetClass;

private:
    UUserWidget* MainMenuWidget;
};
