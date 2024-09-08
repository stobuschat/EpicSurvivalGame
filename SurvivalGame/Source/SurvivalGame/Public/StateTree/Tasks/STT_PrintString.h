#pragma once

#include "StateTreeTaskBase.h"

#include "STT_PrintString.generated.h"

enum class EStateTreeRunStatus : uint8;
struct FStateTreeTransitionResult;

USTRUCT()
struct SURVIVALGAME_API FSTT_PrintStringInstanceData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, Category = "Parameter")
    FString Text;

    UPROPERTY(EditAnywhere, Category = "Parameter")
    FColor TextColor = FColor::White;

    UPROPERTY(EditAnywhere, Category = "Parameter", meta = (ClampMin = 0, UIMin = 0))
    float Duration = 1.0f;

    UPROPERTY(EditAnywhere, Category = "Parameter")
    bool bPrintToScreen = true;

    UPROPERTY(EditAnywhere, Category = "Parameter")
    bool bPrintToLog = true;

    UPROPERTY(EditAnywhere, Category = "Parameter")
    bool bPrintToConsole = true;

    UPROPERTY(EditAnywhere, Category = "Parameter")
    bool bEnabled = true;

    UPROPERTY(EditAnywhere, Category = "Parameter")
    bool bFinishState = true;
};

/**
 * Prints a string.
 */
USTRUCT(meta = (DisplayName = "Print String (CPP)", Category = "SurvG Tasks|Debug"))
struct SURVIVALGAME_API FSTT_PrintString : public FStateTreeTaskCommonBase
{
    GENERATED_BODY()

    using FInstanceDataType = FSTT_PrintStringInstanceData;

    FSTT_PrintString() = default;

    virtual const UStruct* GetInstanceDataType() const override
    {
        return FInstanceDataType::StaticStruct();
    }

    virtual EStateTreeRunStatus EnterState(
        FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
    // virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;
};
