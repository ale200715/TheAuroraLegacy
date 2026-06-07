// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class THEAURORALEGACY_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    // Boton Jugar
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnPlayClicked();

    // Boton Salir
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnQuitClicked();

protected:
    // Botones del menu
    UPROPERTY(meta = (BindWidget))
    class UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

    virtual void NativeConstruct() override;
};
