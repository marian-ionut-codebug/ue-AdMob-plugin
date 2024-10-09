// Copyright 2014-1016, 2Scars Games. All Rights Reserved.
#pragma once

#if PLATFORM_ANDROID
void InitAdMobJavaMethods();
void AndroidThunkCpp_ShowAdBoxInterstitial(const FString& AdUnitID);
void AndroidThunkCpp_ShowAdBoxBanner(const FString& AdUnitID);
#endif
