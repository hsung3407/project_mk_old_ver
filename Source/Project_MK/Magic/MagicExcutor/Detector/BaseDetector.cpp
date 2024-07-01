#include "BaseDetector.h"

#include "Project_MK/Data/MagicData.h"
#include "Project_MK/Magic/Magic.h"

void UBaseDetector::SetExecute(FMagicData* Row, const FMagicInfoContainer* Container)
{
	Super::SetExecute(Row, Container);

	IgnoreActors.Emplace(Container->OrdererActor);

	CollisionTypes = Row->CollisionTypes;
	
	if (const auto MagicObject = Cast<AMagic>(Container->OwnerActor))
		DetectNotifyDelegate.BindUObject(MagicObject, &AMagic::DetectEvent);
}
