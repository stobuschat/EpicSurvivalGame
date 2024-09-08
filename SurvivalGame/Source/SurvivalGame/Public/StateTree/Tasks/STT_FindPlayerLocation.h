#pragma once

#include "StateTreeTaskBase.h"

#include "STT_FindPlayerLocation.generated.h"

enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

class AAIController;

USTRUCT()
struct SURVIVALGAME_API FSTT_FindPlayerLocationInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = Context)
    TObjectPtr<AAIController> AIController = nullptr;

    UPROPERTY(EditAnywhere, Category = Context)
    TObjectPtr<AActor> ContextActorClass;

    UPROPERTY(EditAnywhere, Category = Output)
    FVector TargetLocation = FVector::ZeroVector;

    UPROPERTY(EditAnywhere, Category = Output)
    TObjectPtr<AActor> TargetActor;
};

USTRUCT(meta = (DisplayName = "Find Player Location", Category = "SurvG Tasks|AI"))
struct SURVIVALGAME_API FSTT_FindPlayerLocation : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FSTT_FindPlayerLocationInstanceData;

    FSTT_FindPlayerLocation() = default;

    virtual const UStruct* GetInstanceDataType() const override
    {
        return FInstanceDataType::StaticStruct();
    }

    virtual EStateTreeRunStatus EnterState(
        FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
    // virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
