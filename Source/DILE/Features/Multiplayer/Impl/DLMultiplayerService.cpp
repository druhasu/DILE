// Copyright Andrei Sudarikov. All Rights Reserved.

#include "DLMultiplayerService.h"

UE5Coro::TCoroutine<> UDLMultiplayerServiceImpl::RefreshSessionList()
{
    co_await UE5Coro::Async::PlatformSeconds(1);

    AvailableSessions.Reset();
}
