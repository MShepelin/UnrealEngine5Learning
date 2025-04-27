// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomNodesLibrary.h"
#include "EnhancedInputComponent.h"

class FCustomDelayAction : public FPendingLatentAction
{
public:
    float TimeRemaining;
    bool IsInputPressed;
    FName ExecutionFunction;
    int32 OutputLink;
    FWeakObjectPtr CallbackTarget;

    FCustomDelayAction(float Duration, const FLatentActionInfo& LatentInfo)
        : TimeRemaining(Duration)
        , IsInputPressed(false)
        , ExecutionFunction(LatentInfo.ExecutionFunction)
        , OutputLink(LatentInfo.Linkage)
        , CallbackTarget(LatentInfo.CallbackTarget)
    {}

    void OnInputPressed()
    {
        IsInputPressed = true;
    }

    virtual void UpdateOperation(FLatentResponse& Response) override
    {
        if (!IsInputPressed)
        {
            return;
        }

        TimeRemaining -= Response.ElapsedTime();

        Response.FinishAndTriggerIf(TimeRemaining <= 0.0f, ExecutionFunction, OutputLink, CallbackTarget);
    }
};

void UInputWaiter::Initialize(UEnhancedInputComponent* InputComp, UInputAction* InputAction, int32 InUUID)
{
    LatentActionUUID = InUUID;

    if (InputComp && InputAction)
    {
        InputComp->BindAction(InputAction, ETriggerEvent::Triggered, this, &UInputWaiter::HandleInputPressed);
    }
}

void UInputWaiter::HandleInputPressed(const FInputActionInstance& Instance)
{
    UObject* Outer = GetOuter();

    if (IsValid(Outer))
    {
        UWorld* World = GEngine->GetWorldFromContextObjectChecked(Outer);
        if (World)
        {
            // Delay action should be linked to the outer, so we should use it during action search.
            FCustomDelayAction* Action = World->GetLatentActionManager().FindExistingAction<FCustomDelayAction>(Outer, LatentActionUUID);
            if (Action)
            {
                Action->OnInputPressed();
                MarkAsGarbage();
            }
        }
    }
}

void UCustomNodesLibrary::CustomDelay(APlayerController* Controller, UInputAction* InputAction, float Duration, FLatentActionInfo LatentInfo)
{
    if (!(Controller && InputAction))
    {
        return;
    }

    UWorld* World = Controller->GetWorld(); 
    if (!World)
    {
        return;
    }

    FLatentActionManager& LatentManager = World->GetLatentActionManager();

    if (!LatentManager.FindExistingAction<FCustomDelayAction>(LatentInfo.CallbackTarget, LatentInfo.UUID))
    {
        UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(Controller->InputComponent);
        if (EnhancedInput)
        {
            if (FCustomDelayAction* NewAction = new FCustomDelayAction(Duration, LatentInfo))
            {
                LatentManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, NewAction);

                if (UInputWaiter* Waiter = NewObject<UInputWaiter>(LatentInfo.CallbackTarget))
                {
                    Waiter->Initialize(EnhancedInput, InputAction, LatentInfo.UUID);
                }
            }
        }
    }
}
