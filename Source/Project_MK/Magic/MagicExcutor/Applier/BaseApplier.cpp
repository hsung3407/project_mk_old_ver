#include "BaseApplier.h"

#include "Engine/DamageEvents.h"
#include "Project_MK/Character/Boss/BossBase.h"
#include "Project_MK/Data/MagicData.h"
#include "Project_MK/Magic/Magic.h"

UBaseApplier::UBaseApplier()
{
}

void UBaseApplier::SetExecute(FMagicData* Row, const FMagicInfoContainer* Container)
{
	Super::SetExecute(Row, Container);
	Damage = Row->Damage;
}

void UBaseApplier::DetectEvent(TArray<AActor*> Actors)
{
	Super::DetectEvent(Actors);

	const float FinalDamage = Damage;

	FRadialDamageEvent DamageEvent;
	for (const auto Actor : Actors)
	{
		if(const auto Enemy = Cast<ACharacter>(Actor))
		{
			Enemy->TakeDamage(FinalDamage, DamageEvent, nullptr, nullptr);
		}
	}
}
