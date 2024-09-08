#include "StateTree/Tasks/STT_FindPlayerLocation.h"

#include "AI/SurvG_ZombieAIController.h"
#include "Player/SBaseCharacter.h"
#include "StateTreeExecutionContext.h"

EStateTreeRunStatus FSTT_FindPlayerLocation::EnterState(
    FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    // Get the UWorld from the context
    const UWorld* World = Context.GetWorld();

    // Valid world is required
    if (World == nullptr)
    {
        return EStateTreeRunStatus::Failed;
    }

    // Obtain the AIController from the instance data
    ASurvG_ZombieAIController* AIController = Cast<ASurvG_ZombieAIController>(InstanceData.AIController);

    check(AIController != nullptr);
    if (AIController)
    {
        ASBaseCharacter* TargetChar = AIController->GetTargetEnemy();

        if (TargetChar)
        {
            FVector target_location = TargetChar->GetNavAgentLocation();
            InstanceData.TargetLocation = target_location;
            InstanceData.TargetActor = TargetChar;
            return EStateTreeRunStatus::Running;
        }
    }
    return EStateTreeRunStatus::Failed;
}
