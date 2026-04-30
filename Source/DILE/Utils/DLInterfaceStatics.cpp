// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLInterfaceStatics.h"
#include "Utils/DLEnsure.h"

#include "DI/IResolver.h"

UObject* UDLInterfaceStatics::FindImplementingObject(UObject* Target, UClass* InterfaceClass)
{
    DL_ENSURE_RETURN(Target != nullptr, nullptr);
    DL_ENSURE_RETURN(InterfaceClass != nullptr, nullptr);

    if (Target->GetClass()->ImplementsInterface(InterfaceClass))
    {
        return Target;
    }

    if (InterfaceClass != UResolver::StaticClass())
    {
        if (IResolver* Resolver = Cast<IResolver>(FindImplementingObject(Target, UResolver::StaticClass())))
        {
            return Resolver->TryResolve(InterfaceClass);
        }
    }

    AActor* Actor = Cast<AActor>(Target);
    if (Actor != nullptr)
    {
        if (UActorComponent* Component = Actor->FindComponentByInterface(InterfaceClass))
        {
            return Component;
        }

        if (Actor->GetRootComponent())
        {
            if (AActor* ParentActor = Actor->GetRootComponent()->GetAttachmentRootActor(); ParentActor != nullptr && ParentActor != Actor)
            {
                return FindImplementingObject(ParentActor, InterfaceClass);
            }
        }
    }

    return nullptr;
}
