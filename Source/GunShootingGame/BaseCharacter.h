// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"

UCLASS()
class GUNSHOOTINGGAME_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public: // Funcs

	ABaseCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	float GetHpPercent() const;

	UFUNCTION(BlueprintPure)
	virtual float GetTotalAtk() const;

	UFUNCTION(BlueprintPure)
	virtual float GetTotalHP() const;

	UFUNCTION(BlueprintPure)
	virtual float GetTotalCool() const;

	UFUNCTION(BlueprintPure)
	virtual float GetTotalRecoveryRate() const;

	UFUNCTION(BlueprintPure)
	int64 GetMagazine() const;

	UFUNCTION(BlueprintCallable)
	void DeSpawn();

	void Fire();

private: // Funcs

	float GetAxisValue(float& AxisValue, float Value);

	void CompleteReloading();

	void EndFireVelo();

public: // Values

	// ------ Option Settings ------ //
	UPROPERTY(EditAnywhere, Category = "Mouse Option")
	float MouseSensitivity = 15.f;

	UPROPERTY(EditAnywhere, Category = "Camera Option")
	FVector ArmLoca = FVector(0.f, 65.f, 70.f);

	// ------ Character Stats ------ //
	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float Max_HP = 150;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float HP;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float ReloadSpeed = 1.f;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float SpeedRevision = 1.f; // Now Dummy

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float MaxRootingReach = 500;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float GrabRadius = 100;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float FireVelocity; // 연사속도 제한

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float CoolTime;

	UPROPERTY(BlueprintReadWrite)
	float TempCoolTime = CoolTime;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	bool CanSkill = true;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float BonusAtk;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float BonusHP;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	float RecoveryRate = 1;

	float TempDamage;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	bool IsNowHeal = false;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	bool IsNowUpAtk = false;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	bool bNowFight = false;

	UPROPERTY(EditAnywhere, Category = "Character Status", BlueprintReadWrite)
	bool bCanSelfRecovery = false;

	FTimerHandle SelfRecover;

	// ------ Weapon Stats ------ //
	UPROPERTY(EditAnywhere, Category = "Weapon Status", BlueprintReadWrite)
	float GunDamage = 10;

	UPROPERTY(EditAnywhere, Category = "Weapon Status", BlueprintReadWrite)
	int32 MagazineMaxSize;

	UPROPERTY(VisibleAnywhere, Category = "Weapon Status", BlueprintReadWrite)
	int32 MagazineNowSize;

	UPROPERTY(EditAnywhere)
	float MaxRange = 300;

	// ----- etc State ------ //
	UPROPERTY(VisibleAnywhere, Category = "State", BlueprintReadWrite)
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, Category = "State", BlueprintReadWrite)
	bool bCanFire = true;

	UPROPERTY(VisibleAnywhere, Category = "State", BlueprintReadWrite)
	bool IsDash = false;

	// ------ Pointers ------ //
	class AShooterPlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> ShootCameraShake;

	UPROPERTY(EditAnywhere, Category = "CameraShake")
	TSubclassOf<UCameraShakeBase> DamagedCameraShake;

	// ----- Anim Class Data ----- //
	UPROPERTY(VisibleAnywhere, Category = "Anim Offset", BlueprintReadWrite)
	float HorizontalValue;

	UPROPERTY(VisibleAnywhere, Category = "Anim Offset", BlueprintReadWrite)
	float VerticalValue;

	// ----- Items Information ----- //
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 HP_PotionNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 HP_Recovery_PotionNum = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 PowerUp_PotionNum = 0;

	// ----- UPGRADE Level ----- //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPGRADE Bonus")
	float PlusAtk = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPGRADE Bonus")
	float PlusHP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPGRADE Bonus")
	float PlusCoolLv = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPGRADE Bonus")
	float PlusRecoveryRate = 0;

	// ----- Total Stat ----- //
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPGRADE Bonus")
	float TotalPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPGRADE Bonus")
	float TotalHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPGRADE Bonus")
	float TotalCool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UPGRADE Bonus")
	float TotalRecoveryRate;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite) // 체크 필요
		USceneComponent* BW_ParticlePoint;
private: // Values

	// ----- Anim Montages ----- //
	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* SkillMontage;

	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* HitAnim;

	UPROPERTY(EditAnywhere, Category = "Anim Montage")
	UAnimMontage* ReloadAnim;

	// ----- Sub Components ----- //
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UCharacterMovementComponent* MovementComp = GetCharacterMovement();

	// ----- Weapon Particle System ----- //
	UPROPERTY(EditAnywhere, Category = "Weapon Particle")
	UParticleSystem* WeaponFlash;

	UPROPERTY(EditAnywhere, Category = "Weapon Particle")
	UParticleSystem* WeaponParticle;

	// ----- Sounds ----- //
	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* WeaponSound;

	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* Weapon_ImpactSound;

	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* UsePotionSound;

	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* RootSound;

	UPROPERTY(EditAnywhere, Category = "SoundEffect")
	USoundBase* ReloadSound;

protected:

	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateYaw(float Value);
	void RotatePitch(float Value);
	void SetTargetArm(float Value);

	void CameraInversion();

	void Reload();
	void UseToHpPotion();
	void UseToHpDotPotion();
	void UseToAtkPotion();
	void Item_Interaction();
	bool GetItemInReach(FHitResult &OutHitResult);

	virtual void ActiveSkill();
	UPROPERTY(VisibleAnywhere)
	float SkillDelay;

	void CanUseToSkill();
	void CoolDownComplete();

	virtual void Passive();

	void Recovery(float HealBase);

	void DotRecovery();

	void PowerUp();

	void OriginPower();

	void CanNowHeal();

	void NotFight();

	void SelfRecovery();
	void SelfRecoveryOption();
};
