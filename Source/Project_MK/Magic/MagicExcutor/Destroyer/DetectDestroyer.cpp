#include "DetectDestroyer.h"

#include "Project_MK/Magic/Magic.h"

void UDetectDestroyer::SetExecute(FMagicData* Row, const FMagicInfoContainer* Container)
{
	Super::SetExecute(Row, Container);

	OwnerActor = Container->OwnerActor;
}

void UDetectDestroyer::DetectEvent(TArray<AActor*> Actors)
{
	Super::DetectEvent(Actors);
	
	if(OwnerActor)
		OwnerActor->Destroy();
}
