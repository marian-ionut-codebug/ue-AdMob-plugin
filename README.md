# ue-AdMob-plugin

 Google Admob android plugin for Unreal Engine

![AdMob_final](https://github.com/user-attachments/assets/6973bea6-aa2d-4d43-8404-6b930a18dbbf)

> This document describes about UE project setting configuration for using AdMob plugin AdBox.

AdMob is a mobile advertising platform developed by Google, designed to help app developers monetize their mobile applications through in-app advertising.

AdBox is Android plugin for Unreal Engine which provides methods to integrate AdMob easily to UE projects, especially for UE version 4.27.

In fact, Unreal Engine also provides AdMob in it but UE 4.27 is older version and it doesn’t work anymore nowadays because AdMob has changed a lot from the time when UE 4.27 had been released.

You should configure some project settings in UE 4.27 for using AdBox by following steps below:

- [Android Minimum SDK Version](#android-minimum-sdk-version)
- [Add AdMob Advertising Unit IDs](#add-admob-advertising-unit-ids)
- [Disabling Google Plugins](#disabling-google-plugins)
- [How to use AdBox in the project](#how-to-use-adbox-in-the-project)

## Android Minimum SDK Version

```sh
You must change the minimum Android SDK version to 26.
```

This is because AdBox uses high version of AdMob module in it.

![Picture1](https://github.com/user-attachments/assets/52d1c3a6-6ec2-4eeb-b783-af254f565a50)

## Add AdMob Advertising Unit IDs

```sh
You must add Ad Mob Ad Unit IDs for Banner, Interstitial and Reward Ad.
```

This is because AdBox uses high version of AdMob module in it.

These IDs are essential for Using AdMob. For more information visit Google AdMob official website. (https://developers.google.com/admob/android/quick-start)

```sh
You must also disable Enable Google Play support and Include AdMob support for ads options.
```

This is because by enabling these items, UE 4.27 generate some deprecated google SDK functions causing crash errors and AdMob module version conflict.

![Picture2](https://github.com/user-attachments/assets/72658a65-8e12-4a8b-b509-00d23b1d483a)

```sh
Additionally, you must change the App ID defined in AdMob_APL.xml (folder path in plugin: AdBox\Source\AdMob).
```

![Picture3](https://github.com/user-attachments/assets/0d4869db-0b72-44f1-855c-ea0394dc1255)

The IDs shown in the above figures are for only testing.

## Disabling Google Plugins

```sh
You must disable google plugins shown in following figure.
```

This is because by enabling these plugins, UE 4.27 generate some deprecated google SDK functions causing crash errors.

![Picture4](https://github.com/user-attachments/assets/40cadfc8-07d0-4d47-b9f5-c49809318d79)

## How to use AdBox in the project

You can use AdBox by simply adding some blueprint functions.

![Picture5](https://github.com/user-attachments/assets/738534d9-5af4-4600-b162-d7b831876774)

- Banner
  - Show Banner
  - Close Banner

![Picture6](https://github.com/user-attachments/assets/73a044b5-0d34-4f63-92b8-b02617f10931)

The Ad ID is index value of Ad Mob Ad Unit IDs array described in step 2.
Index value 0 is for Banner.

- bInterstitial
  - Show Interstitial

![Picture7](https://github.com/user-attachments/assets/9486e3c1-751d-4d87-9407-ecb54a744f15)

Index value 1 is for Interstitial.

- Reward
  - Load Rewarded Video
  - Play Rewarded Video

![Picture8](https://github.com/user-attachments/assets/4bf501c3-c0b8-4f58-964a-c796f9f247b3)

Index value 2 is for Reward.

> That’s all and thanks for your careful reading.

# Donate

## Bitcoin

![donate_btc](https://github.com/user-attachments/assets/4660ef37-d75e-457c-8cb8-45f819767c62)

bc1qxp3thny294s79vhm0rt48j8wp4ssv4fppks6kz

## USDT (ERC20)

![donate_erc20](https://github.com/user-attachments/assets/3d72f2f6-aa3f-4d22-bc6a-1bc82cc4d5b2)

0x6Db413e5cE07a6C2DaD6bB4D864Ab05695D1c1bC

