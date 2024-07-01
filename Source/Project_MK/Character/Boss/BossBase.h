#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Pattern/PatternBase.h"
#include "BossBase.generated.h"

class UStatsBase;
class UPatternBase;

UCLASS()
class PROJECT_MK_API ABossBase : public ACharacter
{
	GENERATED_BODY()

	ABossBase();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostCDOCompiled(const FPostCDOCompiledContext& Context) override;
	
public:
	void Die();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void InitPatternObjects();

	UPatternBase* SelectRandomPattern(TArray<UPatternBase*> PatternBaseObjects) const;

	bool CheckPattern(const UPatternBase* Pattern) const;

	void ExecutePattern(UPatternBase* Pattern);

	void RefreshBossState();

private:
	UPROPERTY()
	UStatsBase* Stats;
	
	UPROPERTY(EditAnywhere, Category="Pattern")
	float NextPatternHoldSecond;
	
	UPROPERTY(EditAnywhere, Category="Pattern")
	TArray<TSubclassOf<UPatternBase>> PatternClasses;

	UPROPERTY(EditAnywhere, Category="Pattern")
	TSubclassOf<UPatternBase> DiePatternClass;

	UPROPERTY()
	UPatternBase* DiePattern;
	
	UPROPERTY()
	TArray<UPatternBase*> PatternObjects;

	UPROPERTY()
	UPatternBase* CurrentPattern;

	UPROPERTY()
	UPatternBase* NextPattern;

	UPROPERTY()
	AActor* PlayerActor;

	UPROPERTY()
	float WaitTime;
	
	UPROPERTY()
	float Timer;
};
