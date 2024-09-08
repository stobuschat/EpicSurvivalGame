#include "StateTree/ST_AIComponent.h"

#include "StateTree/ST_AIComponentSchema.h"
#include "StateTreeExecutionContext.h"

TSubclassOf<UStateTreeSchema> UST_AIComponent::GetSchema() const
{
    return UST_AIComponentSchema::StaticClass();
}

bool UST_AIComponent::SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors /*= false*/)
{
    Context.SetCollectExternalDataCallback(
        FOnCollectStateTreeExternalData::CreateUObject(this, &UST_AIComponent::CollectExternalData));
    return UST_AIComponentSchema::SetContextRequirements(*this, Context, bLogErrors);
}

void UST_AIComponent::SetStateTree(UStateTree* InStateTree)
{
    StateTreeRef.SetStateTree(InStateTree);
    InitializeComponent();
}

void UST_AIComponent::SwapStateTree(UStateTree* InStateTree)
{
    StopLogic("TransitionStates");
    SetStateTree(InStateTree);
    RestartLogic();
}
