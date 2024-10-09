// Copyright 2014-1016, 2Scars Games. All Rights Reserved.
#if PLATFORM_ANDROID
#include "AdMobJavaWrapper.h"
#endif
#include "AdMobPrivatePCH.h"
#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

static jmethodID AndroidThunkJava_ShowAdBoxInterstitial;
static jmethodID AndroidThunkJava_ShowAdBoxBanner;

void InitAdMobJavaMethods()
{
	if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
	{
		if (FJavaWrapper::GameActivityClassID)
		{
			AndroidThunkJava_ShowAdBoxInterstitial = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowAdBoxInterstitial", "(Ljava/lang/String;)V", true);
			AndroidThunkJava_ShowAdBoxBanner = FJavaWrapper::FindMethod(Env, FJavaWrapper::GameActivityClassID, "AndroidThunkJava_ShowAdBoxBanner", "(Ljava/lang/String;)V", true);
		}
	}
}


// call out to JNI to see if the application was packaged for GearVR
void AndroidThunkCpp_ShowAdBoxInterstitial(const FString& AdUnitID)
{
   if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
   {
	   if (AndroidThunkJava_ShowAdBoxInterstitial)
      { 
         jstring AdUnitIDArg = Env->NewStringUTF(TCHAR_TO_UTF8(*AdUnitID));
		   FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GoogleServicesThis, AndroidThunkJava_ShowAdBoxInterstitial, AdUnitIDArg);
         Env->DeleteLocalRef(AdUnitIDArg);
      }
   }
}

void AndroidThunkCpp_ShowAdBoxBanner(const FString& AdUnitID)
{
   if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
   {
	   if (AndroidThunkJava_ShowAdBoxBanner)
      { 
         jstring AdUnitIDArg = Env->NewStringUTF(TCHAR_TO_UTF8(*AdUnitID));
		   FJavaWrapper::CallVoidMethod(Env, FJavaWrapper::GoogleServicesThis, AndroidThunkJava_ShowAdBoxBanner, AdUnitIDArg);
         Env->DeleteLocalRef(AdUnitIDArg);
      }
   }
}
#endif
