// Fill out your copyright notice in the Description page of Project Settings.


#include "Revenant.h"
#include "Kismet/GameplayStatics.h"

ARevenant::ARevenant()
{
	
}

void ARevenant::EnableUpgrade()
{
	R_HP += PlusHP; // �ִ� ü�� �߰�
	R_ATK += PlusAtk; // �߰� ���ݷ�
	R_CoolDown -= PlusCoolLv; // ��Ÿ�� ���� ��ŭ ��Ÿ�� ����
	RecoveryRate += PlusRecoveryRate; // ȸ�� ȿ�� ������ 0.1 ����

	CoolTime = R_CoolDown;

	Max_HP = R_HP;
	HP = Max_HP;

	GunDamage = R_ATK;
}

void ARevenant::BeginPlay()
{
	Super::BeginPlay();

	if (ActorHasTag("Player"))
	{
		Passive();
	}
	// ���ʹ� �νĿ� //
	Max_HP = R_HP;
	HP = Max_HP;

	GunDamage = R_ATK;
	// ���ʹ� �νĿ� //

	MaxRange = R_Range;

	MagazineMaxSize = Weapon_Magazine;
	MagazineNowSize = MagazineMaxSize;

	FireVelocity = R_FireVelo;

	SkillDelay = 2.f;
}

void ARevenant::ActiveSkill()
{
	Super::ActiveSkill();

	if (CanSkill)
	{
		if(HP > 50)
		{
			HP -= 50;

			CanSkill = false;
			bCanFire = false;

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SkillVoice,
				GetActorLocation(), GetActorRotation());

			FTimerHandle UPGRADE;
			GetWorldTimerManager().SetTimer(UPGRADE, this,
				&ARevenant::UpgradePower, 2.f);

			FTimerHandle ReturnDamage;
			GetWorldTimerManager().SetTimer(ReturnDamage, this,
				&ARevenant::ReturnToDamage, 6.f);
		}
	}
}

void ARevenant::UpgradePower()
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SkillSound,
		GetActorLocation(), GetActorRotation());

	UGameplayStatics::SpawnEmitterAttached(SkillEffect, GetMesh(),
		TEXT("weapon_l"), GetActorLocation(), GetActorRotation());

	MagazineNowSize = MagazineMaxSize;
	GunDamage = R_ATK * 3;
}

void ARevenant::ReturnToDamage()
{
	R_ATK += 5;
	GunDamage = R_ATK;
}

void ARevenant::Passive()
{
	Super::Passive();

	//UE_LOG(LogTemp, Error, TEXT("Called Revenant Passive Player"));

	R_ATK = 90;
	R_HP = 800;
	RecoveryRate = 1.25f;

	ReloadSpeed = 0.6f;

	BonusHP = 20;
	BonusAtk = 400;
}

float ARevenant::GetTotalAtk() const
{
	Super::GetTotalAtk();

	return GunDamage;
}

float ARevenant::GetTotalHP() const
{
	Super::GetTotalHP();

	return R_HP;
}

float ARevenant::GetTotalCool() const
{
	Super::GetTotalCool();

	return R_CoolDown;
}

float ARevenant::GetTotalRecoveryRate() const
{
	Super::GetTotalRecoveryRate();

	return RecoveryRate;
}
