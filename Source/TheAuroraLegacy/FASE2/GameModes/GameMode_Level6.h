#pragma once
#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "GameMode_Level6.generated.h"

UCLASS()
class THEAURORALEGACY_API AGameMode_Level6 : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()
public:
    AGameMode_Level6();
    virtual void SpawnEnemy() override;
};