#pragma once

#include "CoreMinimal.h"

#define NULL_CHECK(Ptr,Code)                                \
    if ((Ptr) == nullptr)                                   \
    {                                                       \
        UE_LOG(LogTemp, Warning, TEXT(#Ptr " is Nullptr")); \
    }                                                       \
    else                                                    \
    {                                                       \
        Code                                                \
	}  


#define LOG_CALLINFO ANSI_TO_TCHAR(__FUNCTION__)
#define JWGAS_LOG(LogCat, Verbosity, Format, ...) UE_LOG(LogCat, Verbosity, TEXT("%s %s"), LOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

DECLARE_LOG_CATEGORY_EXTERN(LogJWGAS, Log, All);