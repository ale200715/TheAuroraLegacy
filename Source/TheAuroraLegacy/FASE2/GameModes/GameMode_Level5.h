#pragma once
#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "GameMode_Level5.generated.h"

UCLASS()
class THEAURORALEGACY_API AGameMode_Level5 : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()
public:
    AGameMode_Level5();
    virtual void SpawnEnemy() override;
};