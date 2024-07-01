#pragma once

#include "StatsBase.generated.h"

UCLASS(meta=(BlueprintSpawnableComponent))
class UStatsBase : public UActorComponent
{
	GENERATED_BODY()

	UStatsBase();
	
public:
	virtual bool CheckState() const {return CurrentHP > 0;}
	
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

private:
	UPROPERTY(EditAnywhere, Category="Default Stats")
	float MaxHP;

	UPROPERTY(VisibleAnywhere, Category="Default Stats")
	float CurrentHP;
	
	virtual void BeginPlay() override;
};
