// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Minion.generated.h"

/**
 * 
 */
UCLASS()
class GUNSHOOTINGGAME_API AMinion : public ABaseCharacter
{
	GENERATED_BODY()
	
public:

	AMinion();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float MW_HP = 150;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float ATK = 50;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Range = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Weapon_Magazine = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float M_FireVelo = 2.f;

protected:

	virtual void BeginPlay() override;
};
