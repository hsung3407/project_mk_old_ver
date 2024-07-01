#include "BossBase.h"

#include "MathUtil.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Pattern/PatternBase.h"
#include "Project_MK/Character/StatsBase.h"

ABossBase::ABossBase(): NextPatternHoldSecond(1), CurrentPattern(nullptr)
{
	PrimaryActorTick.bCanEverTick = true;
	InitPatternObjects();
}

void ABossBase::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	InitPatternObjects();
}

void ABossBase::PostCDOCompiled(const FPostCDOCompiledContext& Context)
{
	Super::PostCDOCompiled(Context);
	InitPatternObjects();
}

void ABossBase::BeginPlay()
{
	Super::BeginPlay();
	Stats = Cast<UStatsBase>(FindComponentByClass(UStatsBase::StaticClass()));
	PlayerActor = GetWorld()->GetFirstPlayerController()->GetCharacter();
	InitPatternObjects();
}

void ABossBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	Timer += DeltaSeconds;
	if(Timer > WaitTime)
	{
		if(!NextPattern)
			NextPattern = SelectRandomPattern(PatternObjects);
		if(!CurrentPattern)
			ExecutePattern(NextPattern);
		else
		{
			Timer = 0;
			if(CurrentPattern->bAnimLoop && CurrentPattern == NextPattern)
			{
				WaitTime = CurrentPattern->LoopQuantum;
				NextPattern = nullptr;
				return;
			}
			WaitTime = CurrentPattern->DelayTime;
			if(WaitTime > NextPatternHoldSecond) NextPattern = nullptr;
			CurrentPattern = nullptr;
			StopAnimMontage();
			RefreshBossState();
		}
	}
}

void ABossBase::InitPatternObjects()
{
	PatternObjects.Reset();
	for (auto PatternClass : PatternClasses)
		if(PatternClass) PatternObjects.Emplace(NewObject<UPatternBase>(GetTransientPackage(), PatternClass));
	if(DiePatternClass)	DiePattern = NewObject<UPatternBase>(GetTransientPackage(), DiePatternClass);
}

UPatternBase* ABossBase::SelectRandomPattern(TArray<UPatternBase*> PatternBaseObjects) const
{
	if(PatternBaseObjects.IsEmpty()) return nullptr;
	
	int TopPriority = 0;
	TArray<UPatternBase*> TopPriorityPatterns;
	for (auto PatternObject : PatternBaseObjects)
	{
		if(!CheckPattern(PatternObject)) continue;
		if(TopPriority < PatternObject->Priority)
		{
			TopPriorityPatterns.Reset();
			TopPriority = PatternObject->Priority;
			TopPriorityPatterns.Emplace(PatternObject);
		}
		else if(TopPriority == PatternObject->Priority)
			TopPriorityPatterns.Emplace(PatternObject);
	}
	if(TopPriorityPatterns.IsEmpty()) return nullptr;

	const auto SelectedPattern = TopPriorityPatterns[UKismetMathLibrary::RandomInteger(TopPriorityPatterns.Num())];
	return SelectedPattern;
}

bool ABossBase::CheckPattern(const UPatternBase* Pattern) const
{
	if(!PlayerActor) return true;
	if(!Pattern) return false;
	const float Distance = UKismetMathLibrary::Vector_Distance(PlayerActor->GetActorLocation(), GetActorLocation());
	const float HPRatio = 100;
	const bool bDistanceCheck = Pattern->MinDistance <= Distance && Pattern->MaxDistance >= Distance;
	const bool bHPCheck = Pattern->MinHPRatio <= HPRatio && Pattern->MaxHPRatio >= HPRatio;
	return bDistanceCheck && bHPCheck;
}

void ABossBase::ExecutePattern(UPatternBase* Pattern)
{
	if(!Pattern) return;
	if(Pattern->bIgnoreGravity) GetCapsuleComponent()->SetEnableGravity(false);

	CurrentPattern = Pattern;
	NextPattern = nullptr;
	Timer = 0;
	const float PlayTime = PlayAnimMontage(Pattern->Montage);
	WaitTime = FMathf::Max(PlayTime, Pattern->MinAnimTime);
}

void ABossBase::RefreshBossState()
{
	GetCapsuleComponent()->SetEnableGravity(true);

	auto Rotation = GetActorRotation();
	Rotation.Pitch = 0;
	Rotation.Roll = 0;
	SetActorRotation(Rotation);
}

void ABossBase::Die()
{
	if(!DiePattern)
	{
		Destroy();
		return;
	}
	ExecutePattern(DiePattern);

	FTimerHandle Handle;
	FTimerDelegate Delegate;
	Delegate.BindLambda([this] {Destroy();});
	GetWorldTimerManager().SetTimer(Handle, Delegate, WaitTime, false);
}

float ABossBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	return Stats->TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}
