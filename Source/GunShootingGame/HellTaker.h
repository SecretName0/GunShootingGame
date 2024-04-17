// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "HellTaker.generated.h"

/**
 * 
 */
UCLASS()
class GUNSHOOTINGGAME_API AHellTaker : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	AHellTaker();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Boss_HP = 2000;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Boss_ATK = 150;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Range = 2000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weapon_Magazine = 99999;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float B_FireVelo = 1.3f;

protected:

	virtual void BeginPlay() override;
};
