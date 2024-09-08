#include "StateTree/ST_AIComponentSchema.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "StateTreeExecutionContext.h"
#include "Tasks/StateTreeAITask.h"

UST_AIComponentSchema::UST_AIComponentSchema(
    const FObjectInitializer& ObjectInitializer /*= FObjectInitializer::Get()*/)
    : AIControllerClass(AAIController::StaticClass())
{
    ContextDataDescs.Emplace(TEXT("AIController"), AIControllerClass.Get(), FGuid(0xEDB3CD97, 0x95F94E0A, 0xBD15207B, 0x98645CDC));
}

void UST_AIComponentSchema::PostLoad()
{
    Super::PostLoad();
    ContextDataDescs[1].Struct = AIControllerClass.Get();
}

bool UST_AIComponentSchema::IsStructAllowed(const UScriptStruct* InScriptStruct) const
{
    return Super::IsStructAllowed(InScriptStruct) || InScriptStruct->IsChildOf(FStateTreeAITaskBase::StaticStruct());
}

bool UST_AIComponentSchema::SetContextRequirements(
    UBrainComponent& BrainComponent, FStateTreeExecutionContext& Context, bool bLogErrors /*= false*/)
{
    if (!Context.IsValid())
    {
        return false;
    }

    const FName AIControllerName(TEXT("AIController"));
    Context.SetContextDataByName(AIControllerName, FStateTreeDataView(BrainComponent.GetAIOwner()));

    return Super::SetContextRequirements(BrainComponent, Context, bLogErrors);
}

#if WITH_EDITOR
void UST_AIComponentSchema::PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent)
{
    Super::PostEditChangeChainProperty(PropertyChangedEvent);

    if (FProperty* Property = PropertyChangedEvent.Property)
    {
        if (Property->GetOwnerClass() == UST_AIComponentSchema::StaticClass() &&
            Property->GetFName() == GET_MEMBER_NAME_CHECKED(UST_AIComponentSchema, AIControllerClass))
        {
            ContextDataDescs[1].Struct = AIControllerClass.Get();
        }
    }
}
#endif  // WITH_EDITOR
