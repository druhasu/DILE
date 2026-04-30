// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLWeaponSocketsServiceImpl.h"
#include "DLWeaponViewComponent.h"
#include "Systems/PlayerContext/IDLPlayerPawnService.h"
#include "Utils/DLEnsure.h"

#include "GameFramework/Pawn.h"

void UDLWeaponSocketsServiceImpl::InitDependencies(
    TScriptInterface<IDLPlayerPawnService> InPawnService
)
{
    PawnService = InPawnService;
}

FDLSocketInfo UDLWeaponSocketsServiceImpl::GetMuzzleSocket() const
{
    APawn* Pawn = PawnService->GetPawn();
    if (Pawn == nullptr)
        return {};

    UDLWeaponViewComponent* ViewComponent = Pawn->GetComponentByClass<UDLWeaponViewComponent>();
    DL_ENSURE_RETURN(ViewComponent != nullptr, {});

    if (AActor* WeaponActor = ViewComponent->GetWeaponActor())
    {
        // Weapon probably should have some interface for getting socket location by name
        // For now, just look for any primitive components
        if (UPrimitiveComponent* Primitive = WeaponActor->FindComponentByClass<UPrimitiveComponent>())
        {
            return { Primitive, ViewComponent->GetMuzzleSocketName() };
        }

        return { WeaponActor->GetRootComponent(), ViewComponent->GetMuzzleSocketName() };
    }

    return { Pawn->GetRootComponent(), FName() };
}
