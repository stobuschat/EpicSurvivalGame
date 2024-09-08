#include "StateTree/Tasks/STT_FindBotWaypoint.h"

#include "AI/SBotWaypoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NavigationSystem.h"
#include "StateTreeExecutionContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(STT_FindBotWaypoint)

EStateTreeRunStatus FSTT_FindBotWaypoint::EnterState(
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
    AAIController* AIController = InstanceData.AIController;

    // Select a new waypoint
    TArray<AActor*> AllWaypoints;
    UGameplayStatics::GetAllActorsOfClass(World, ASBotWaypoint::StaticClass(), AllWaypoints);

    if (AllWaypoints.IsEmpty())
    {
        return EStateTreeRunStatus::Failed;
    }

    AActor* NewWaypoint = AllWaypoints[FMath::RandRange(0, AllWaypoints.Num() - 1)];

    /* Find a position that is close to the waypoint. */
    if (NewWaypoint)
    {
        FNavLocation ResultLocation;
        UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(const_cast<UWorld*>(World));

        if (NavSystem &&
            NavSystem->GetRandomPointInNavigableRadius(NewWaypoint->GetActorLocation(), InstanceData.SearchRadius, ResultLocation))
        {
            // Output position close to the waypoint
            InstanceData.Waypoint = ResultLocation.Location;
            return EStateTreeRunStatus::Running;
        }
    }

    return EStateTreeRunStatus::Failed;
}
