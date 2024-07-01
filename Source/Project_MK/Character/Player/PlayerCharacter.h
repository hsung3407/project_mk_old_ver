// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

class UInventoryComponent;
enum ERuneType : uint8;
class UItemObject;
struct FInputActionInstance;
struct FEnhancedInputActionValueBinding;
class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;
class UCharacterAnimInstance;

UCLASS()
class PROJECT_MK_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

	UPROPERTY()
	UItemObject* EquippedMagic;

protected:

private:
	UPROPERTY()
	UCharacterAnimInstance* AnimInstance;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category=Camera)
	USpringArmComponent* SpringArm;
	UPROPERTY(VisibleAnywhere)
	UInventoryComponent* InventoryComponent;

	UPROPERTY()
	UInputMappingContext* InputMappingContext;

	UPROPERTY()
	UInputAction* IA_Move;

	UPROPERTY()
	UInputAction* IA_MoveNegate;

	UPROPERTY()
	UInputAction* IA_Sprint;

	UPROPERTY()
	UInputAction* IA_Manifest;

	UPROPERTY()
	UInputAction* IA_Change;

	UPROPERTY()
	UInputAction* IA_Blink;

	UPROPERTY()
	UInputAction* IA_Concentrate;

	UPROPERTY()
	UInputAction* IA_CameraRotate;

	UPROPERTY()
	UInputAction* IA_Menu;

	FEnhancedInputActionValueBinding* MoveInput;
	FEnhancedInputActionValueBinding* MoveNegateInput;
	FEnhancedInputActionValueBinding* SprintInput;

	UPROPERTY(EditAnywhere)
	float BaseMaxMana;

	UPROPERTY()
	float CurrentMana;

	UPROPERTY(EditAnywhere)
	float BaseMaxSpirit;

	UPROPERTY()
	float CurrentSpirit;

	UPROPERTY(EditAnywhere, Category=Movement)
	float Speed;

	UPROPERTY(EditAnywhere, Category=Movement)
	float MouseSensitivity;

	UPROPERTY()
	bool bBlink;

	UPROPERTY(EditAnywhere, Category=Blink)
	float BlinkDistance;

	UPROPERTY(EditAnywhere, Category=Blink)
	float BlinkCoolTime;

	UPROPERTY()
	bool bConcentrate;

	UPROPERTY()
	int ConcentrateLevel;

	UPROPERTY()
	int MaxConcentrateLevel;

	UPROPERTY()
	TArray<TEnumAsByte<ERuneType>> CommandList;

	UPROPERTY()
	float CommandInputTime;

	UPROPERTY()
	float CommandTimer;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void UpdateAnim();

	void CameraRotate(const FInputActionInstance& Instance);
	void Move(float DeltaTime);
	
	void ChangeMagic();
	void Manifest();
	void Blink();
	void Concentrate();

	void CommandInput(int Index);

	void ToggleMenu();
};
