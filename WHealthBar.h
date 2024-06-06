// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WHealthBar.generated.h"

/**
 * 
 */

class UHorizontalBox;
class UImage;
class ABmTechnicalTestCharacter;

UCLASS()
class BMTECHNICALTEST_API UWHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void SetNewHealth(float inHealth);

	UPROPERTY(EditAnywhere)
	float DecreaseSpeed = 0.1f; // every second

	UFUNCTION()
	void OnCharacterPossessed(APawn *OldPawn = nullptr, APawn *NewPawn = nullptr);
	void OnCharacterPossessed(ABmTechnicalTestCharacter* Character = nullptr);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UHorizontalBox* HealthBox = nullptr;
	
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Background = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* Health = nullptr;
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	UImage* RemovePreview = nullptr;

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	void ChangeSlotSizeValue(UImage* ImageRef, float NewSize);
	void AddSlotSizeValue(UImage* ImageRef, float NewSize);
	const float GetCurrentSizeValue(UImage* ImageRef);

	bool ChangeLifeAnimation = false;
};
