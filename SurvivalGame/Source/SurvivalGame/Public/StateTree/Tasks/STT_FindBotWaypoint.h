#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"

#include "STT_FindBotWaypoint.generated.h"

enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

class AAIController;

USTRUCT()
struct SURVIVALGAME_API FSTT_FindBotWaypointInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = Context)
    TObjectPtr<AAIController> AIController = nullptr;

    UPROPERTY(EditAnywhere, Category = Context)
    TObjectPtr<AActor> ContextActorClass;

    UPROPERTY(EditAnywhere, Category = "Paramaters", meta = (ToolTip = "Radius to search position close to waypoint."))
    float SearchRadius = 200.0;

    UPROPERTY(EditAnywhere, Category = Output)
    FVector Waypoint = FVector::ZeroVector;
};

USTRUCT(meta = (DisplayName = "Find Bot Waypoint", Category = "SurvG Tasks|AI"))
struct SURVIVALGAME_API FSTT_FindBotWaypoint : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FSTT_FindBotWaypointInstanceData;

    FSTT_FindBotWaypoint() = default;

    virtual const UStruct* GetInstanceDataType() const override
    {
        return FInstanceDataType::StaticStruct();
    }

    virtual EStateTreeRunStatus EnterState(
        FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
    // virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
