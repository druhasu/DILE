// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInjectorProvider.h"
#include "Core/DLPlayerState.h"

#include "AIController.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetTree.h"
#include "DI/InjectOnConstruction.h"
#include "DI/ObjectContainer.h"

void UDLInjectorProvider::InitDependencies(TScriptInterface<IInjector>&& InInjector)
{
    DefaultInjector = MoveTemp(InInjector);

    // order matters here. entries will be tested in order they are added
    AddGetter(&ThisClass::GetForWorld);
    AddGetter(&ThisClass::GetForPlayerController);
    AddGetter(&ThisClass::GetForPlayerState);
    AddGetter(&ThisClass::GetForAIController);
    AddGetter(&ThisClass::GetForActor);
    AddGetter(&ThisClass::GetForActorComponent);
    AddGetter(&ThisClass::GetForUserWidget);
    AddGetter(&ThisClass::GetForWidget);
    AddGetter(&ThisClass::GetForUObject);
}

TScriptInterface<IInjector> UDLInjectorProvider::GetInjector(UObject* InjectTarget) const
{
    UObject* CurrentObject = InjectTarget;
    while (CurrentObject != nullptr)
    {
        FGetterFunction Function = FindGetterFunction(CurrentObject->GetClass());
        if (!ensureMsgf(Function != nullptr, TEXT("Unexpected class found '%s'"), *GetNameSafe(CurrentObject->GetClass())))
        {
            break;
        }

        UObject* NextObject = (*Function)(CurrentObject);
        TScriptInterface<IInjector> Result = NextObject;
        if (Result != nullptr)
        {
            return Result;
        }

        if (!ensure(CurrentObject != NextObject))
        {
            break;
        }

        CurrentObject = NextObject;
    }

    return DefaultInjector;
}

UObject* UDLInjectorProvider::GetForWorld(UWorld* World)
{
    return FInjectOnConstruction::GetContainerForWorld(World);
}

UObject* UDLInjectorProvider::GetForPlayerController(APlayerController* Controller)
{
    if (ADLPlayerState* PlayerState = Controller->GetPlayerState<ADLPlayerState>(); ensure(PlayerState != nullptr))
    {
        return PlayerState->GetContainer();
    }

    return Controller->GetWorld();
}

UObject* UDLInjectorProvider::GetForPlayerState(APlayerState* PlayerState)
{
    if (ADLPlayerState* CastedPlayerState = Cast<ADLPlayerState>(PlayerState); ensure(CastedPlayerState != nullptr))
    {
        return CastedPlayerState->GetContainer();
    }

    return PlayerState->GetWorld();
}

UObject* UDLInjectorProvider::GetForAIController(AAIController* Controller)
{
    // AIs don't have their own Container right now, fallback to world
    return Controller->GetWorld();
}

UObject* UDLInjectorProvider::GetForActor(AActor* Actor)
{
    if (AActor* Owner = Actor->GetOwner(); Owner != nullptr && Owner != Actor)
    {
        return Owner;
    }

    if (APawn* Instigator = Actor->GetInstigator(); Instigator != nullptr && Instigator != Actor)
    {
        return Instigator;
    }

    return Actor->GetWorld();
}

UObject* UDLInjectorProvider::GetForActorComponent(UActorComponent* ActorComponent)
{
    return ActorComponent->GetOwner();
}

UObject* UDLInjectorProvider::GetForUserWidget(UUserWidget* UserWidget)
{
    if (APlayerController* Controller = UserWidget->GetPlayerContext().GetPlayerController())
    {
        return Controller;
    }

    return UserWidget->GetWorld();
}

UObject* UDLInjectorProvider::GetForWidget(UWidget* Widget)
{
    if (UWidgetTree* WidgetTree = Widget->GetTypedOuter<UWidgetTree>())
    {
        return WidgetTree->GetOuter();
    }

    return Widget->GetWorld();
}

UObject* UDLInjectorProvider::GetForUObject(UObject* Object)
{
    if (UObject* Outer = Object->GetOuter())
    {
        return Outer;
    }

    return Object->GetWorld();
}

template<typename TClass>
void UDLInjectorProvider::AddGetter(UObject* (*Function)(TClass*))
{
    Getters.Add(FGetter{ TClass::StaticClass(), reinterpret_cast<FGetterFunction>(Function) });
}

UDLInjectorProvider::FGetterFunction UDLInjectorProvider::FindGetterFunction(UClass* Class) const
{
    const FGetter* FoundGetter = Getters.FindByPredicate(
        [&](const FGetter& Getter)
    {
        return Class->IsChildOf(Getter.ObjectClass);
    });

    return FoundGetter ? FoundGetter->Function : nullptr;
}
