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

DEFINE_FUNCTION(UCustomNodesLibrary::execGetObjectProperty)
{
    check(Stack.Code && *Stack.Code == EExprToken::EX_Self);
    P_GET_OBJECT(UObject, Target);

    check(Stack.Code && *Stack.Code == EExprToken::EX_NameConst);
    FName PropertyName;
    Stack.Step(Stack.Object, &PropertyName);

    check(Stack.Code && *Stack.Code == EExprToken::EX_LocalVariable);
    Stack.Step(Stack.Object, nullptr);
    void* OutValuePtr = Stack.MostRecentPropertyAddress;
    FProperty* OutPropertyPtr = Stack.MostRecentProperty;

    check(Stack.Code && *Stack.Code == EExprToken::EX_EndFunctionParms);

    P_FINISH;

    if (!OutValuePtr)
    {
        UE_LOG(LogTemp, Error, TEXT("GetObjectProperty node: unexpected stack behavior, cannot find address for a new value"));
        return;
    }

    if (Target && PropertyName.GetStringLength() > 0)
    {
        FProperty* Property = Target->GetClass()->FindPropertyByName(PropertyName);
        if (Property)
        {
            // Output pin is either a primitive (float, int, etc) or a property.
            // For a primitive we assume the size to be not more than 8 bytes, 
            // and for a property we can directly ask for a property size.
            int32 NumberOfAllocatedBytes = OutPropertyPtr ? OutPropertyPtr->GetSize() : sizeof(int64);

            int32 PropertySize = Property->GetSize();
            if (NumberOfAllocatedBytes >= PropertySize)
            {
                void* ValuePtr = Property->ContainerPtrToValuePtr<void>(Target);
                // BVM already allocated enough space for the value, since output pin is defined by user.
                // So, we can copy the value with the size of Property.
                Property->CopyCompleteValue(OutValuePtr, ValuePtr);
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("GetObjectProperty node: output pin and property size mismatch, make sure that PropertyName [%s] has the same type that the output pin value"), *PropertyName.ToString());
            }
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("GetObjectProperty node: cannot find property named [%s], change PropertyName"), *PropertyName.ToString());
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("GetObjectProperty node: either Target or PropertyName is empty, make sure all inputs are set"));
    }
}

void UCustomNodesLibrary::GetObjectProperty(UObject* Target, FName PropertyName, int32& OutValue)
{
    checkNoEntry();
}
