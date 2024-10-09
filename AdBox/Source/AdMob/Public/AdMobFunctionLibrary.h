// Copyright 2014-1016, 2Scars Games. All Rights Reserved.
#pragma once
#include "AdMobFunctionLibrary.generated.h"

USTRUCT(BlueprintType)
struct FAdMobRewardItem
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(BlueprintReadOnly, Category = "RewardItem")
	FString type;

	UPROPERTY(BlueprintReadOnly, Category = "RewardItem")
	int32 amount = 0;
};

//-- Dispatchers
DECLARE_DYNAMIC_DELEGATE(FRewardDispatcher);
DECLARE_DYNAMIC_DELEGATE_OneParam(FRewardEarnedDispatcher, FAdMobRewardItem, reward);

UCLASS(MinimalAPI)
class UAdMobFunctionLibrary : public UBlueprintFunctionLibrary
{
   GENERATED_UCLASS_BODY()

	static void ExecuteOnGameThread(TFunction<void()> Function);

private:     
   static void CallAndroidThunkCpp(int32 adID, void(*)(const FString&));

public:
   // Delgates to send the return values from java
	static FRewardDispatcher RAL, RAF, ORAS, ORAFS, ORAD;
	static FRewardEarnedDispatcher OUER;
	static void callRewardDelegates(int32 index);
	static void callUserEarnedReward(FString rewardType, int32 amount);
	static FAdMobRewardItem rewardItem;

   // Banner Ads Functionalities
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Show Banner", Keywords = "Show Banner"), Category = "Plugin|AdBox")
   static void ShowAdBoxBanner(int32 adID);

   UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Close Banner", Keywords = "Close Banner"), Category = "Plugin|AdBox")
   static void CloseAdBoxBanner();

   // Interstitial Ads Functionalities
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Show Interstitial", Keywords = "Show Interstitial"), Category = "Plugin|AdBox")
   static void ShowAdBoxInterstitial(int32 adID);

   // Rewarded Ads Functionalities
   UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Load Rewarded Video", Keywords = "Load Rewarded Video"), Category = "Plugin|AdBox")
	static void LoadRewardedVideo(int32 Index, const FRewardDispatcher &onRewardedAdLoaded, const FRewardDispatcher &onRewardedAdFailedToLoad);

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "AdBox Play Rewarded Video", Keywords = "Play Rewarded Video"), Category = "Plugin|AdBox")
	static void PlayRewardedVideo(const FRewardDispatcher &onRewardedAdShowed, const FRewardDispatcher &onRewardedAdFailedToShow, const FRewardDispatcher &onRewardedAdDismissed, const FRewardEarnedDispatcher &onUserEarnedReward);

   UFUNCTION(BlueprintPure, meta = (DisplayName = "AdBox Get reward item", Keywords = "get reward item "), Category = "Plugin|AdBox")
	static FAdMobRewardItem getRewardItem();
};