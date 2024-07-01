#include "ForwardMover.h"
#include "Project_MK/Data/MagicData.h"
#include "Project_MK/Magic/Magic.h"

void UForwardMover::SetExecute(FMagicData* Row, const FMagicInfoContainer* Container)
{
	Super::SetExecute(Row, Container);
	
	OwnerActor = Container->OwnerActor;
	Speed = Row->Speed;
}

void UForwardMover::TickEvent(const float DeltaTime)
{
	OwnerActor->AddActorLocalOffset(FVector::ForwardVector * Speed * DeltaTime);
}
