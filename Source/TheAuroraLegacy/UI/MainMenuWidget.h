#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

UCLASS()
class THEAURORALEGACY_API UMainMenuWidget : public UUserWidget
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnPlayClicked();

    UFUNCTION(BlueprintCallable, Category = "Menu")
    void OnQuitClicked();

protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* PlayButton;

    UPROPERTY(meta = (BindWidget))
    class UButton* QuitButton;

    virtual void NativeConstruct() override;
};