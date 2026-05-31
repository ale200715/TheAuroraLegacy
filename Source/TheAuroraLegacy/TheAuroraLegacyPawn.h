#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TheAuroraLegacyPawn.generated.h"

UCLASS(Config = Game)
class ATheAuroraLegacyPawn : public APawn
{
    GENERATED_BODY()

    UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* PlaneMesh;

    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class USpringArmComponent* SpringArm;

    UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UCameraComponent* Camera;

public:
    ATheAuroraLegacyPawn();

    virtual void Tick(float DeltaSeconds) override;
    virtual void NotifyHit(class UPrimitiveComponent* MyComp, class AActor* Other,class UPrimitiveComponent* OtherComp,bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;

    UFUNCTION()
    void Fire();

    UPROPERTY(EditAnywhere, Category = "Combat")
    TSubclassOf<class APlayerProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Combat")
    float FireRate = 0.2f;

    // Vidas 
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
    int32 Lives = 3;

    UFUNCTION()
    void TakeDamage_Ship(int32 DamageAmount);

    int32 GetLives() const { return Lives; }

    float GetMaxSpeed() const { return MaxSpeed; }
    void SetMaxSpeed(float NewSpeed)
    {
        MaxSpeed = NewSpeed;
    }

protected:
    virtual void SetupPlayerInputComponent( class UInputComponent* InputComponent) override;

    void ThrustInput(float Val);
    void MoveUpInput(float Val);
    void MoveRightInput(float Val);

private:
    UPROPERTY(Category = Plane, EditAnywhere)
    float Acceleration;

    UPROPERTY(Category = Plane, EditAnywhere)
    float TurnSpeed;

    UPROPERTY(Category = Pitch, EditAnywhere)
    float MaxSpeed;

    UPROPERTY(Category = Yaw, EditAnywhere)
    float MinSpeed;

    float CurrentForwardSpeed;
    float CurrentYawSpeed;
    float CurrentPitchSpeed;
    float CurrentRollSpeed;

    bool bCanFire = true;
    FTimerHandle FireTimerHandle;
    void ResetFire();

public:
    FORCEINLINE class UStaticMeshComponent* GetPlaneMesh()
        const {
        return PlaneMesh;
    }
    FORCEINLINE class USpringArmComponent* GetSpringArm()
        const {
        return SpringArm;
    }
    FORCEINLINE class UCameraComponent* GetCamera()
        const {
        return Camera;
    }
};