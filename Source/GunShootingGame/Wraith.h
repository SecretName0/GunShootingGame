// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Wraith.generated.h"

/**
 * 
 */
UCLASS()
class GUNSHOOTINGGAME_API AWraith : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AWraith();

	UFUNCTION(BlueprintCallable)
	void EnableUpgrade();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float W_HP = 600;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float W_ATK = 55; // 테스트용 이후 상향

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float W_Range = 4000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	int32 Weapon_Magazine = 40;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float W_FireVelo = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float W_SkillReach = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float W_SkillDamage = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Unique Character Status")
	float W_CoolDown = 16;

	UPROPERTY(EditAnywhere, Category = "Weapon Particle")
	UParticleSystem* SkillFlash;

	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* SkillSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Particle")
	UParticleSystem* SkillImpactParticle;

	UPROPERTY(EditAnywhere, Category = "Weapon Particle")
	USoundBase* SkillImpactSound;

protected:
	virtual void BeginPlay() override;

	void ActiveSkill() override;

	void Passive() override;

	float GetTotalAtk() const override;

	float GetTotalHP() const override;

	float GetTotalCool() const override;

	float GetTotalRecoveryRate() const override;

};
