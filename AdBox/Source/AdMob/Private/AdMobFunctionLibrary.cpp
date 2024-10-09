// Copyright 2014-1016, 2Scars Games. All Rights Reserved.
#include "AdMobFunctionLibrary.h"
#include "AdMobPrivatePCH.h"
#include "AdMobJavaWrapper.h"

#include "Misc/ConfigCacheIni.h"
#include "Async/TaskGraphInterfaces.h"
#include "Engine.h"

#if PLATFORM_ANDROID
#include "Runtime/Core/Public/CoreGlobals.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif

DEFINE_LOG_CATEGORY(LogAdBox);

UAdMobFunctionLibrary::UAdMobFunctionLibrary(FObjectInitializer const&) {}

void UAdMobFunctionLibrary::ShowAdBoxInterstitial(int32 adID)
{
#if PLATFORM_ANDROID
   CallAndroidThunkCpp(adID, &AndroidThunkCpp_ShowAdBoxInterstitial);   
#endif
}

void UAdMobFunctionLibrary::ShowAdBoxBanner(int32 adID)
{
#if PLATFORM_ANDROID
   CallAndroidThunkCpp(adID, &AndroidThunkCpp_ShowAdBoxBanner);   
#endif
}

void UAdMobFunctionLibrary::CloseAdBoxBanner()
{
#if PLATFORM_ANDROID
	if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_CloseAdBoxBanner", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
#endif
}

void UAdMobFunctionLibrary::CallAndroidThunkCpp(int32 adID, void(*f)(const FString&))
{
#if PLATFORM_ANDROID
   
   TArray<FString> AdUnitIDs;
   int32 count = GConfig->GetArray(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("AdMobAdUnitIDs"), AdUnitIDs, GEngineIni);

   if (count == 0)
   {
      // Fall back to checking old setting
      FString AdUnitID;
      bool found = GConfig->GetString(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("AdMobAdUnitID"), AdUnitID, GEngineIni);
      if (found && !AdUnitID.IsEmpty())
      {
         f(AdUnitID);
      }

      return;
   }

   if (adID >= 0 && adID < count && !AdUnitIDs[adID].IsEmpty())
   {
      f(AdUnitIDs[adID]);
   }
#endif
}

// Rewarded video Ads

FRewardDispatcher UAdMobFunctionLibrary::RAL;
FRewardDispatcher UAdMobFunctionLibrary::RAF;

void UAdMobFunctionLibrary::LoadRewardedVideo(int32 Index, const FRewardDispatcher &onRewardedAdLoaded, const FRewardDispatcher &onRewardedAdFailedToLoad)
{
	UAdMobFunctionLibrary::RAL = onRewardedAdLoaded;
	UAdMobFunctionLibrary::RAF = onRewardedAdFailedToLoad;

	TArray<FString> AdUnitIDs;
	int32 count = GConfig->GetArray(TEXT("/Script/AndroidRuntimeSettings.AndroidRuntimeSettings"), TEXT("AdMobAdUnitIDs"), AdUnitIDs, GEngineIni);

#if PLATFORM_ANDROID
	if (count != 0 && !AdUnitIDs[Index].IsEmpty())
	{
		if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
		{

			static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_LoadRewardedVideo_AdMob", "(Ljava/lang/String;)V", false);

			auto AdUnitIDArg = FJavaHelper::ToJavaString(Env, AdUnitIDs[Index]);
			FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method, *AdUnitIDArg);
		}
	}

#endif
}

FRewardDispatcher UAdMobFunctionLibrary::ORAS;
FRewardDispatcher UAdMobFunctionLibrary::ORAFS;
FRewardDispatcher UAdMobFunctionLibrary::ORAD;
FRewardEarnedDispatcher UAdMobFunctionLibrary::OUER;

void UAdMobFunctionLibrary::PlayRewardedVideo(const FRewardDispatcher &onRewardedAdShowed, const FRewardDispatcher &onRewardedAdFailedToShow, const FRewardDispatcher &onRewardedAdDismissed, const FRewardEarnedDispatcher &onUserEarnedReward)
{
	UAdMobFunctionLibrary::ORAS = onRewardedAdShowed;
	UAdMobFunctionLibrary::ORAFS = onRewardedAdFailedToShow;
	UAdMobFunctionLibrary::ORAD = onRewardedAdDismissed;
	UAdMobFunctionLibrary::OUER = onUserEarnedReward;

#if PLATFORM_ANDROID

	if (JNIEnv *Env = FAndroidApplication::GetJavaEnv())
	{
		static jmethodID Method = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_PlayRewardedVideo_AdMob", "()V", false);
		FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GameActivityThis, Method);
	}
#endif
}

//-- Functions CallBack for Java code
#if PLATFORM_ANDROID
JNI_METHOD void Java_com_epicgames_ue4_GameActivity_CPPRewardAd(JNIEnv *env, jclass clazz, jint index)
{

	UAdMobFunctionLibrary::callRewardDelegates(index);
}

JNI_METHOD void Java_com_epicgames_ue4_GameActivity_CPPUserEarnRewardAd(JNIEnv *env, jclass clazz, jstring rewardType, jint amount)
{

	FString CPPrewardType = FJavaHelper::FStringFromLocalRef(env, rewardType);
	UAdMobFunctionLibrary::callUserEarnedReward(CPPrewardType, amount);
}

#endif // PLATFORM_ANDROID

void UAdMobFunctionLibrary::callRewardDelegates(int32 index)
{
	switch (index)
	{
	case 1:
		UAdMobFunctionLibrary::ExecuteOnGameThread([]() { RAL.ExecuteIfBound(); });
		break;
	case 2:
		UAdMobFunctionLibrary::ExecuteOnGameThread([]() { RAF.ExecuteIfBound(); });
		break;
	case 3:
		UAdMobFunctionLibrary::ExecuteOnGameThread([]() { ORAS.ExecuteIfBound(); });
		break;
	case 4:
		UAdMobFunctionLibrary::ExecuteOnGameThread([]() { ORAFS.ExecuteIfBound(); });
		break;
	case 5:
		UAdMobFunctionLibrary::ExecuteOnGameThread([]() { ORAD.ExecuteIfBound(); });
		break;
	}
}

FAdMobRewardItem UAdMobFunctionLibrary::rewardItem;

void UAdMobFunctionLibrary::callUserEarnedReward(FString rewardType, int32 amount)
{
	UAdMobFunctionLibrary::rewardItem.type = rewardType;
	UAdMobFunctionLibrary::rewardItem.amount = amount;

	//UAdMobFunctionLibrary::OUER.ExecuteIfBound(UAdMobFunctionLibrary::rewardItem);
	UAdMobFunctionLibrary::ExecuteOnGameThread([]() { OUER.ExecuteIfBound(UAdMobFunctionLibrary::rewardItem); });
}

FAdMobRewardItem UAdMobFunctionLibrary::getRewardItem()
{
	return UAdMobFunctionLibrary::rewardItem;
}

void UAdMobFunctionLibrary::ExecuteOnGameThread(TFunction<void()> Function)
{
	if (!GEngine)
		return;

	// Dispatch the function to run on the game thread
	FFunctionGraphTask::CreateAndDispatchWhenReady(
		[Function]()
		{
			Function();
		},
		TStatId(),	// No stat id
		nullptr,	// Do not specify a dependency
		ENamedThreads::GameThread
	);
}