// Fill out your copyright notice in the Description page of Project Settings.


#include "WHealthBar.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "../Actors/BmTechnicalTestCharacter.h"
#include "../Components/Player/HealthComponent.h"
#include "../UEStates/BmTechnicalTestPlayerController.h"

void UWHealthBar::SetNewHealth(float inHealth)
{
	if (inHealth > GetCurrentSizeValue(Health))
	{
		const float Heal = inHealth - GetCurrentSizeValue(Health);
		ChangeSlotSizeValue(Health, inHealth);
		AddSlotSizeValue(RemovePreview, -Heal);
		ChangeSlotSizeValue(Background, 1.0f - GetCurrentSizeValue(Health) - GetCurrentSizeValue(RemovePreview));
		ChangeLifeAnimation = false;
	}
	else if (inHealth < GetCurrentSizeValue(Health))
	{
		float HealthToRemove = GetCurrentSizeValue(Health) - inHealth;
		ChangeSlotSizeValue(Health, inHealth);
		AddSlotSizeValue(RemovePreview, HealthToRemove);
		ChangeLifeAnimation = true;
	}
}

void UWHealthBar::OnCharacterPossessed(APawn* OldPawn /*= nullptr*/, APawn* NewPawn /*= nullptr*/)
{
	if (ABmTechnicalTestCharacter* Character = Cast<ABmTechnicalTestCharacter>(NewPawn))
	{
		OnCharacterPossessed(Character);
	}
}

void UWHealthBar::OnCharacterPossessed(ABmTechnicalTestCharacter* Character /*= nullptr*/)
{
	if (!Character)
		return;

	if (UHealthComponent* HealthComponent = Character->HealthComponent)
	{
		HealthComponent->OnHealthChange.AddUniqueDynamic(this, &UWHealthBar::SetNewHealth);
		SetNewHealth(HealthComponent->GetCurrentPercentageHealth());
	}
}

void UWHealthBar::NativeConstruct()
{
	Super::NativeConstruct();
	const UWorld* World = GetWorld();
	if (!World)
		return;

	if (ABmTechnicalTestPlayerController* PlayerController = Cast<ABmTechnicalTestPlayerController>(World->GetFirstPlayerController()))
	{
		//Bind to on player posses to change the character health reference if we change characters
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &UWHealthBar::OnCharacterPossessed);
		if (ABmTechnicalTestCharacter* Character = Cast<ABmTechnicalTestCharacter>(PlayerController->GetPawn()))
		{
			//If the current character is valid bind to on health change
			OnCharacterPossessed(Character);
		}
	}
}

void UWHealthBar::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//Preview damage animation
	if (ChangeLifeAnimation)
	{
		const float toChange = (DecreaseSpeed * InDeltaTime);
		AddSlotSizeValue(RemovePreview, -toChange);
		if (GetCurrentSizeValue(RemovePreview) <= 0.0f)
		{
			ChangeSlotSizeValue(Background, 1.0f - GetCurrentSizeValue(Health));
			ChangeLifeAnimation = false;
		}
		else
		{
			AddSlotSizeValue(Background, toChange);
		}
	}
}

void UWHealthBar::ChangeSlotSizeValue(UImage* ImageRef, float NewSize)
{
	NewSize = FMath::Clamp(NewSize, 0.0f, 1.0f);
	UHorizontalBoxSlot* SlotRef = Cast<UHorizontalBoxSlot>(ImageRef->Slot);
	if (SlotRef)
	{
		FSlateChildSize TempSize;
		TempSize.Value = NewSize;

		SlotRef->SetSize(TempSize);
	}
}

void UWHealthBar::AddSlotSizeValue(UImage* ImageRef, float NewSize)
{
	ChangeSlotSizeValue(ImageRef, GetCurrentSizeValue(ImageRef) + NewSize);
}

const float UWHealthBar::GetCurrentSizeValue(UImage* ImageRef)
{
	UHorizontalBoxSlot* SlotRef = Cast<UHorizontalBoxSlot>(ImageRef->Slot);
	if (SlotRef)
	{
		return SlotRef->GetSize().Value;
	}
	return 0.0f;
}