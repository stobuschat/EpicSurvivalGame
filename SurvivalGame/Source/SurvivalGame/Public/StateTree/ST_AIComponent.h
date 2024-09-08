#pragma once

#include "Components/StateTreeComponent.h"
#include "CoreMinimal.h"

#include "ST_AIComponent.generated.h"

/**
 *
 */
UCLASS(ClassGroup = (Custom), HideCategories = (Activation, Collision), meta = (BlueprintSpawnableComponent))
class SURVIVALGAME_API UST_AIComponent : public UStateTreeComponent
{
    GENERATED_BODY()
public:
    // BEGIN IStateTreeSchemaProvider
    virtual TSubclassOf<UStateTreeSchema> GetSchema() const override;
    // END

    virtual bool SetContextRequirements(FStateTreeExecutionContext& Context, bool bLogErrors = false) override;

    /**
     * Will set new State Tree (StateTreeRef).
     *
     * @note This will not handle the logic (i.e. no Start, Stop, Pause, Restart of the logic).
     *
     * @param InStateTree New State Tree
     */

    UFUNCTION(BlueprintCallable, Category = "State Tree")
    void SetStateTree(UStateTree* InStateTree);

    /**
     * Swaps the State Tree.
     *
     * Will first stop current logic, then change to the new State Tree and restart logic.
     *
     * @param InStateTree New State Tree
     */
    UFUNCTION(BlueprintCallable, Category = "State Tree", meta = (DisplayName = "Swap State Tree"))
    void SwapStateTree(UStateTree* InStateTree);
};
