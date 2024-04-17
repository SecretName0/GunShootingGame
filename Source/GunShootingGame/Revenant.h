// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Revenant.generated.h"

/**
 * 
 */
UCLASS()
class GUNSHOOTINGGAME_API ARevenant : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	ARevenant();

	UFUNCTION(BlueprintCallable)
	void EnableUpgrade();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float R_HP = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float R_ATK = 90;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float R_Range = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	int32 Weapon_Magazine = 8;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float R_FireVelo = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float R_CoolDown = 20.f;

	UPROPERTY(EditAnywhere, Category = "Weapon Particle")
	UParticleSystem* SkillEffect;

	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* SkillVoice;

	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* SkillSound;

protected:
	virtual void BeginPlay() override;

	void ActiveSkill() override;
	void UpgradePower();
	void ReturnToDamage();

	void Passive() override;

	float GetTotalAtk() const override;

	float GetTotalHP() const override;

	float GetTotalCool() const override;

	float GetTotalRecoveryRate() const override;
};
