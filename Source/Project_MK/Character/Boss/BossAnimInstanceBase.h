#pragma once

#include "BossAnimInstanceBase.generated.h"

UCLASS()
class UBossAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

	UBossAnimInstanceBase();

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
public:
	
};
