// Fill out your copyright notice in the Description page of Project Settings.

#include "Magic.h"
#include "MagicExcutor/MagicExecutor.h"
#include "Project_MK/Data/MagicData.h"

AMagic::AMagic()
{
	PrimaryActorTick.bCanEverTick = true;
}

FMagicFindByRunesResult AMagic::MagicFindByRunes(TArray<TEnumAsByte<ERuneType>> TriggerRunes)
{
	FMagicFindByRunesResult FindResult;
	const auto Table = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/Magic.Magic"));
	TArray<FMagicData*> Rows;
	Table->GetAllRows("", Rows);

	if(TriggerRunes.IsEmpty()) return FindResult;

	if(const auto RowPointer = Rows.FindByPredicate([&TriggerRunes](const FMagicData* Self)
	{
		if (Self->TriggerRunes.IsEmpty() || Self->TriggerRunes.Num() != TriggerRunes.Num()) return false;
		for (int i = 0; i < TriggerRunes.Num(); ++i)
			if(Self->TriggerRunes[i] != TriggerRunes[i]) return false;
		return true;
	})) FindResult.Row = *RowPointer;

	for (auto TriggerRune : TriggerRunes)
		if(TriggerRune == Enchant) FindResult.bIsEnchant = true;
	return FindResult;
}

void AMagic::SpawnMagic(FName MagicName, FMagicInfoContainer* Container)
{
	const auto Table = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/Magic.Magic"));
	const auto Row = Table->FindRow<FMagicData>(MagicName, "");

	SpawnMagic(Row, Container);
}

void AMagic::SpawnMagic(FMagicData* Row, FMagicInfoContainer* Container)
{
	const FString AssetPath = FString::Printf( TEXT( "Blueprint'/Game/Blueprint/Magic.Magic'" ));
	UObject* Class = StaticLoadObject(UObject::StaticClass(), nullptr, *AssetPath);
	const UBlueprint* BP_Actor = Cast<UBlueprint>(Class);

	auto World = Container->World;
	if(!World)
	{
		if(Container->OrdererActor)
			World = Container->OrdererActor->GetWorld();
		else return;
	}
	const auto MagicActor = World->SpawnActor<AActor>(BP_Actor->GeneratedClass, Container->SpawnLocation, Container->SpawnRotator);

	Container->OwnerActor = MagicActor;

	Cast<AMagic>(MagicActor)->Manifest(Row, Container);
}

void AMagic::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	for (const auto Executor : Executors)
		Executor->TickEvent(DeltaSeconds);
}

void AMagic::Manifest(FMagicData* Row, FMagicInfoContainer* Container)
{
	for (auto ExecutorClass : Row->MagicExecutors)
	{
		if (!ExecutorClass) continue;
		if (auto MagicExecutor = NewObject<UMagicExecutor>(this, ExecutorClass))
		{
			MagicExecutor->SetExecute(Row, Container);
			Executors.Emplace(MagicExecutor);
		}
	}
}

void AMagic::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	for (const auto Executor : Executors)
		Executor->DestroyedEvent(EndPlayReason == EEndPlayReason::Destroyed);
}

void AMagic::DetectEvent(TArray<AActor*> Actors)
{
	for (const auto Executor : Executors)
		Executor->DetectEvent(Actors);
}

void AMagic::RemoveExecutor(UMagicExecutor* Executor)
{
	if (Executors.Contains(Executor)) 
		Executors.Remove(Executor);
}
