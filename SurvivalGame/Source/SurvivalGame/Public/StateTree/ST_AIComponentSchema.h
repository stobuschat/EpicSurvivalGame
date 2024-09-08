#pragma once

#include "Components/StateTreeComponentSchema.h"

#include "ST_AIComponentSchema.generated.h"

class AAIController;
/**
 *
 */
UCLASS(BlueprintType, EditInlineNew, CollapseCategories, meta = (DisplayName = "SurvG StateTree AI Component Sch", CommonSchema))
class SURVIVALGAME_API UST_AIComponentSchema : public UStateTreeComponentSchema
{
    GENERATED_BODY()

public:
    UST_AIComponentSchema(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

    virtual void PostLoad() override;

    virtual bool IsStructAllowed(const UScriptStruct* InScriptStruct) const override;

    static bool SetContextRequirements(
        UBrainComponent& BrainComponent, FStateTreeExecutionContext& Context, bool bLogErrors = false);

#if WITH_EDITOR
    virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& PropertyChangedEvent) override;
#endif  // WITH_EDITOR

protected:
    /** AIController class the StateTree is expected to run on. Allows to bind to specific Actor class' properties. */
    UPROPERTY(EditAnywhere, Category = "Defaults", NoClear)
    TSubclassOf<AAIController> AIControllerClass = nullptr;
};
