#pragma once

#include "AIController.h"
#include "CoreMinimal.h"
#include "SurvivalGame/Public/StateTree/ST_AIComponent.h"

#include "SurvG_ZombieAIController.generated.h"

class ASBaseCharacter;

UCLASS()
class SURVIVALGAME_API ASurvG_ZombieAIController : public AAIController
{
    GENERATED_BODY()

public:
    ASurvG_ZombieAIController();

    virtual void OnPossess(APawn* InPawn) override;
    virtual void OnUnPossess() override;

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    ASBaseCharacter* GetTargetEnemy() const;

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void SetTargetEnemy(APawn* NewTarget);

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    void SetStateTreeAI(UST_AIComponent* InStateTreeAI);

    UFUNCTION(BlueprintCallable, Category = "Zombie")
    UST_AIComponent* GetStateTreeAI() const;

    virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn) override;

protected:
    UPROPERTY(VisibleAnywhere, Category = "AI")
    UST_AIComponent* StateTreeAI;

    UPROPERTY(VisibleAnywhere)
    FGameplayTagContainer ControllerTags;

    UPROPERTY(EditDefaultsOnly, Category = "AI", BlueprintReadWrite, meta = (ToolTip = "Speed to rotate to focus target."))
    float SmoothFocusInterpSpeed = 90.0f;

    UPROPERTY(EditAnywhere, Category = "AI")
    FStateTreeEvent PlayerSensedEvent;

    UPROPERTY(EditAnywhere, Category = "AI")
    FStateTreeEvent PlayerLostEvent;

private:
    APawn* TargetEnemyPawn;
    FRotator SmoothTargetRotation;
};
