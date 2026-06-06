#include "EnemyBase.h"
#include "../Core/GameFacade.h"
#include "Kismet/GameplayStatics.h"

AEnemyBase::AEnemyBase()
{
    PrimaryActorTick.bCanEverTick = true;
    SetActorEnableCollision(true);
}

void AEnemyBase::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemyBase::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (IsAlive())
        MoveEnemy(DeltaTime);
}

void AEnemyBase::MoveEnemy(float DeltaTime) {}

void AEnemyBase::TakeDamageEnemy(int32 DamageAmount)
{
    Health -= DamageAmount;
    UE_LOG(LogTemp, Warning, TEXT("Enemigo recibió daño. Vida restante: %d"), Health);

    if (Health <= 0)
        OnDeath();
}

void AEnemyBase::OnDeath()
{
    TArray<AActor*> FoundFacades;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGameFacade::StaticClass(), FoundFacades);

    if (FoundFacades.Num() > 0)
    {
        AGameFacade* Facade = Cast<AGameFacade>(FoundFacades[0]);
        if (Facade) {
            Facade->NotifyEnemyDefeated(this);
        }
    }

    Destroy();
}
