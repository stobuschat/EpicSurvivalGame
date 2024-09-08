#include "AI/SurvG_ZombieAIController.h"

#include "AI/SZombieCharacter.h"
#include "Kismet/KismetMathLibrary.h"

/* AI Specific includes */
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ASurvG_ZombieAIController::ASurvG_ZombieAIController()

{
    // StateTree AI component for managing the AI's decision-making process
    StateTreeAI = CreateDefaultSubobject<UST_AIComponent>(TEXT("ST_AIComponent"));

    /* Initializes PlayerState so we can assign a team index to AI */
    bWantsPlayerState = true;
}

void ASurvG_ZombieAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    // Start the AI logic when the controller possesses a pawn
    if (StateTreeAI)
    {
        StateTreeAI->StartLogic();
        // StateTreeAI = ST_AIComponent_CPP;
    }
}

void ASurvG_ZombieAIController::OnUnPossess()
{
    Super::OnUnPossess();

    // Stop AI logic when the controller no longer possesses a pawn
    if (StateTreeAI)
    {
        StateTreeAI->StopLogic(TEXT("OnUnPossess"));
    }
}

void ASurvG_ZombieAIController::SetTargetEnemy(APawn* NewTarget)
{
    if (!StateTreeAI)
        return;

    // Handle logic for discovering a new target or losing the current target
    if (NewTarget && !TargetEnemyPawn)
    {
        TargetEnemyPawn = NewTarget;
        ControllerTags.AddTag(FGameplayTag::RequestGameplayTag(FName("ST.Zombie.SensedPlayer")));
        StateTreeAI->SendStateTreeEvent(PlayerSensedEvent);
    }
    else if (!NewTarget && TargetEnemyPawn)
    {
        TargetEnemyPawn = nullptr;
        ControllerTags.RemoveTag(FGameplayTag::RequestGameplayTag(FName("ST.Zombie.SensedPlayer")));
        // StateTreeAI->SendStateTreeEvent(PlayerSensedEvent);
        StateTreeAI->SendStateTreeEvent(PlayerLostEvent);
    }
}

ASBaseCharacter* ASurvG_ZombieAIController::GetTargetEnemy() const
{
    if (TargetEnemyPawn)
    {
        return Cast<ASBaseCharacter>(TargetEnemyPawn);
    }

    return nullptr;
}

void ASurvG_ZombieAIController::SetStateTreeAI(UST_AIComponent* inST)
{
    // Assign a new StateTree AI component, allowing dynamic changes during gameplay
    StateTreeAI = inST;
}
UST_AIComponent* ASurvG_ZombieAIController::GetStateTreeAI() const
{
    // Return the current StateTree AI component
    return StateTreeAI;
}
/* Smooth rotation to focus target.
 * based on https://dev.epicgames.com/community/learning/tutorials/ZYVB/unreal-engine-smooth-focus-using-ai-setfocus-setfocalpoint
 */
void ASurvG_ZombieAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
    // Call the original method without updating the pawn
    Super::UpdateControlRotation(DeltaTime, false);

    // Smoothly rotate the pawn towards the focus target if rotation update is required
    if (bUpdatePawn)
    {
        APawn* MyPawn = GetPawn();
        if (MyPawn)
        {
            // Calculate the smooth rotation towards the control rotation
            FRotator CurrentPawnRotation = MyPawn->GetActorRotation();
            SmoothTargetRotation =
                UKismetMathLibrary::RInterpTo_Constant(CurrentPawnRotation, ControlRotation, DeltaTime, SmoothFocusInterpSpeed);

            // Apply the smooth rotation if there's a significant difference
            if (!CurrentPawnRotation.Equals(SmoothTargetRotation, 1e-3f))
            {
                // Change rotation using the Smooth Target Rotation
                MyPawn->FaceRotation(SmoothTargetRotation, DeltaTime);
            }
        }
    }
}
