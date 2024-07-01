#include "MagicExecutor.h"

#include "Project_MK/Magic/Magic.h"

void UMagicExecutor::SetExecute(FMagicData* Row, const FMagicInfoContainer* Container)
{
	if (const auto MagicActor = Cast<AMagic>(Container->OwnerActor))
		RemoveSelf.BindUObject(MagicActor, &AMagic::RemoveExecutor);
}
