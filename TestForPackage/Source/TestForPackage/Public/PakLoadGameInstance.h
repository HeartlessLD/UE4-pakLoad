// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PakLoadGameInstance.generated.h"

/**
 *
 */
UCLASS()
class TESTFORPACKAGE_API UPakLoadGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:

	virtual void Init() override;

	UFUNCTION(BlueprintCallable, Exec)
		void LoadPak(FString pakPath);

	UFUNCTION(BlueprintCallable)
		void SpawnActorFromPak();

	UFUNCTION(BlueprintCallable)
		void LoadLevel();


	class IPlatformFile* oldPlatForm;
	FString mountPoint;
	TSharedPtr<class FPakPlatformFile> PakPlatformFile;

};
