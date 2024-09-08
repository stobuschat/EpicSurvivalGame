#include "StateTree/Tasks/STT_PrintString.h"

#include "Kismet/KismetSystemLibrary.h"
#include "StateTreeExecutionContext.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(STT_PrintString)

EStateTreeRunStatus FSTT_PrintString::EnterState(
    FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const
{
    FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

    if (!InstanceData.bEnabled)
    {
        return EStateTreeRunStatus::Succeeded;
    }

    const UWorld* World = Context.GetWorld();

    // valid world is required.
    if (World == nullptr)
    {
        return EStateTreeRunStatus::Failed;
    }

    UKismetSystemLibrary::PrintString(World, InstanceData.Text, InstanceData.bPrintToScreen, InstanceData.bPrintToLog,
        InstanceData.TextColor.ReinterpretAsLinear(), InstanceData.Duration);

    auto currentState = Context.GetStateFromHandle(Context.GetCurrentlyProcessedState());
    if (currentState->TasksNum > 1)
    {
        /* There are other tasks in state: don't trigger completed. */
        return EStateTreeRunStatus::Running;
    }
    else
    {
        /* There are no other tasks in state: trigger completed. */
        return EStateTreeRunStatus::Succeeded;
    }
}
