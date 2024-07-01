#include "SphereDetector.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Project_MK/Character/StatsBase.h"
#include "Project_MK/Data/MagicData.h"
#include "Project_MK/Magic/Magic.h"

void USphereDetector::SetExecute(FMagicData* Row, const FMagicInfoContainer* Container)
{
	Super::SetExecute(Row, Container);
	OwnerActor = Container->OwnerActor;
	Radius = Row->Radius;
}

void USphereDetector::TickEvent(const float DeltaTime)
{
	Super::TickEvent(DeltaTime);
	
	if(TArray<AActor*> OutActors; UKismetSystemLibrary::SphereOverlapActors(GetWorld(), OwnerActor->GetActorLocation(), Radius, CollisionTypes,AActor::StaticClass(), IgnoreActors, OutActors))
	{
		OutActors = OutActors.FilterByPredicate([](const AActor* Actor)
		{
			if(const auto Stats = Actor->GetComponentByClass<UStatsBase>()) return Stats->CheckState();
			return false;
		});
		if(OutActors.Num() == 0) return;
		
		DetectNotify(OutActors);
	}

	//ToDo: Gizmo Debug for Test need Remove
	DrawDebugSphere(GetWorld(), OwnerActor->GetActorLocation(), Radius, 26, FColor::Red, false, -1, 0, 0);
}
