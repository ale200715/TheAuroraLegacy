#pragma once
#include "CoreMinimal.h"
#include "../../TheAuroraLegacyGameMode.h"
#include "GameMode_Level4.generated.h"

UCLASS()
class THEAURORALEGACY_API AGameMode_Level4 : public ATheAuroraLegacyGameMode
{
    GENERATED_BODY()
public:
    AGameMode_Level4();
    virtual void SpawnEnemy() override;
}; 