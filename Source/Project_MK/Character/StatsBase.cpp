#include "StatsBase.h"

#include "Project_MK/Character/Boss/BossBase.h"

UStatsBase::UStatsBase(): MaxHP(100), CurrentHP(0)
{
}

void UStatsBase::BeginPlay()
{
	Super::BeginPlay();
	CurrentHP = MaxHP;
}

float UStatsBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	float FinalDamage = DamageAmount;
	float AppliedDamage = FinalDamage;
	CurrentHP -= AppliedDamage;
	if(CurrentHP <= 0) Cast<ABossBase>(GetOwner())->Die();
	UE_LOG(LogTemp, Warning, TEXT("HP : %f"), CurrentHP)
	return FinalDamage;
}
