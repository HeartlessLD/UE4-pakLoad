// Fill out your copyright notice in the Description page of Project Settings.


#include "PakLoadGameInstance.h"
#include "IPlatformFilePak.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Classes/Engine/LevelStreamingDynamic.h"

void UPakLoadGameInstance::Init()
{

	oldPlatForm = &FPlatformFileManager::Get().GetPlatformFile();
	PakPlatformFile = MakeShareable(new FPakPlatformFile);
	PakPlatformFile->Initialize(oldPlatForm, TEXT(""));

	Super::Init();
}

void UPakLoadGameInstance::LoadPak(FString pakPath)
{

	UE_LOG(LogTemp, Warning, TEXT("start loadpak = %s"), *pakPath);
	FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile.Get());

	FPakFile* pak = new FPakFile(PakPlatformFile.Get(), *pakPath, false);
	FString oldMountPointPath = pak->GetMountPoint();
	UE_LOG(LogTemp, Warning, TEXT("oldMountPointPath = %s"), *oldMountPointPath);

	int32 ContentPos = oldMountPointPath.Find("Content/");
	FString newMoutPointPath = FPaths::ProjectDir() + oldMountPointPath.RightChop(ContentPos);
	mountPoint = newMoutPointPath;
	UE_LOG(LogTemp, Warning, TEXT("newMoutPointPath =%s"), *newMoutPointPath);
	pak->SetMountPoint(*newMoutPointPath);
	if (!PakPlatformFile->Mount(*pakPath, 0, *newMoutPointPath))
	{
		UE_LOG(LogTemp, Error, TEXT("Mount PakFile Failed =%s"), *pakPath);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mount PakFile succ =%s"), *pakPath);
		TArray<FString> FoundFileNames;
		pak->FindPrunedFilesAtPath(FoundFileNames, *(pak->GetMountPoint()), true);
		if (FoundFileNames.Num() > 0)
		{
			for (auto FoundFileName : FoundFileNames)
			{
				UE_LOG(LogTemp, Warning, TEXT("FoundFileName =%s"), *FoundFileName);
				FString NewFileName = FoundFileName;
				NewFileName.RemoveFromEnd(TEXT(".uasset"));
				int32 pos = NewFileName.Find("/Content/");
				NewFileName = NewFileName.RightChop(pos + 8);
				NewFileName = "/Game" + NewFileName;

				UE_LOG(LogTemp, Warning, TEXT("Found UObjectPath =%s"), *FoundFileName);
				/*		UObject* loadObj = StaticLoadObject(UObject::StaticClass(), NULL, *NewFileName);
						if (!loadObj)
						{
							UE_LOG(LogTemp, Error, TEXT("load Error path = %s"), *NewFileName);
							continue;;
						}*/
			}

		}

	}

	FPlatformFileManager::Get().SetPlatformFile(*oldPlatForm);



}

void UPakLoadGameInstance::SpawnActorFromPak()
{
	FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);
	UClass* uc = LoadClass<AActor>(nullptr, TEXT("Blueprint'/Game/TestPak/BP_testCube.BP_testCube_C'"));
	if (uc == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Load Class Error"));
	}
	else
	{
		FVector v(100, -100, 100);
		FRotator  r(0);
		GetWorld()->SpawnActor<AActor>(uc, v, r);
	}
	FPlatformFileManager::Get().SetPlatformFile(*oldPlatForm);
}

void UPakLoadGameInstance::LoadLevel()
{
	FPlatformFileManager::Get().SetPlatformFile(*PakPlatformFile);


	FString LevelName = "TestLevel";
	UGameplayStatics::OpenLevel(GetWorld(), *LevelName);


}

