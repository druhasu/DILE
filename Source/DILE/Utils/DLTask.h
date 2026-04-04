// Copyright Andrei Sudarikov. All Rights Reserved.

#pragma once

#include "UE5Coro.h"

template <typename T>
using TDLTask = UE5Coro::TCoroutine<T>;
using FDLTask = UE5Coro::TCoroutine<>;
