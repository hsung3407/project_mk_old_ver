// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagicExcutor/MagicExecutor.h"
#include "Magic.generated.h"

struct FMagicData;
enum ERuneType : uint8;

USTRUCT()
struct FMagicFindByRunesResult
{
	GENERATED_BODY()

	FMagicFindByRunesResult(): MagicName(FName("None")), bIsEnchant(false), Row(nullptr)
	{
	}

	UPROPERTY()
	FName MagicName;

	UPROPERTY()
	bool bIsEnchant;

	FMagicData* Row;
};

USTRUCT()
struct FMagicInfoContainer
{
	GENERATED_BODY()

	FMagicInfoContainer(): World(nullptr), OrdererActor(nullptr), OwnerActor(nullptr), TargetActor(nullptr),
	                       TargetLocation(FVector::ZeroVector), SpawnLocation(FVector::ZeroVector),
	                       SpawnRotator(FRotator::ZeroRotator)
	{
	}

	UPROPERTY()
	UWorld* World;

	UPROPERTY()
	AActor* OrdererActor;

	UPROPERTY()
	AActor* OwnerActor;

	UPROPERTY()
	AActor* TargetActor;

	UPROPERTY()
	FVector TargetLocation;

	UPROPERTY()
	FVector SpawnLocation;

	UPROPERTY()
	FRotator SpawnRotator;
};

class UMagicExecutor;
/**
 * 
 */
UCLASS()
class PROJECT_MK_API AMagic : public AActor
{
	GENERATED_BODY()

	AMagic();

public:
	static FMagicFindByRunesResult MagicFindByRunes(TArray<TEnumAsByte<ERuneType>> TriggerRunes);
	
	static void SpawnMagic(FName MagicName, FMagicInfoContainer* Container);
	
	static void SpawnMagic(FMagicData* Row, FMagicInfoContainer* Container);

	virtual void Tick(float DeltaSeconds) override;

	virtual void Manifest(FMagicData* Row, FMagicInfoContainer* Container);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void DetectEvent(TArray<AActor*> Actors);

	void RemoveExecutor(UMagicExecutor* Executor);

private:
	UPROPERTY(VisibleAnywhere)
	TArray<UMagicExecutor*> Executors;
};
