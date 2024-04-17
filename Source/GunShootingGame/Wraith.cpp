// Fill out your copyright notice in the Description page of Project Settings.


#include "Wraith.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"

AWraith::AWraith()
{
	
}


void AWraith::BeginPlay()
{
	Super::BeginPlay();

	if (ActorHasTag("Player"))
	{
		Passive();
	}
	// ���ʹ� �νĿ� //
	Max_HP = W_HP;
	HP = Max_HP;

	GunDamage = W_ATK;
	// ���ʹ� �� //
	
	MaxRange = W_Range;

	MagazineMaxSize = Weapon_Magazine;
	MagazineNowSize = MagazineMaxSize;
	FireVelocity = W_FireVelo;

	SkillDelay = 1.f;
}

void AWraith::ActiveSkill()
{
	Super::ActiveSkill();

	// źâ 10�� �Ҹ�
	// ���� �� ��Ÿ�
	// ������ 1000�� ������ ����
	// ��Ÿ�� 8��
	// ��ų �ĵ����� 1��
	if(CanSkill && !IsDash)
	{
		CanSkill = false;
		bCanFire = false;

		if(MagazineNowSize >= 10)
		{
			MagazineNowSize -= 10;

			AController* PController = this->GetController();
			if (PController == nullptr) { return; }

			FVector Loca;
			FRotator Rota;
			PController->GetPlayerViewPoint(Loca, Rota);

			FVector ShotDirection = -Rota.Vector();
			// ���� �� ������ ���ϱ⿡ �߻��� �ݴ�

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this); // �߻��� ���͸� ��󿡼� ����

			FHitResult HitResult;


			UGameplayStatics::SpawnEmitterAttached(SkillFlash,
				GetMesh(), TEXT("GunFlashPoint"), FVector(ForceInit),
				GetMesh()->GetRelativeRotation() * -1);

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SkillSound,
				GetActorLocation(), GetActorRotation());

			FVector BW_EndPoint = Loca + Rota.Vector() * W_SkillReach;
			// Rota.Vector()�� ȸ���Ͽ� �����ִ� �� ������ �ǹ��Ѵ�.

			bool bSuccessHit = GetWorld()->LineTraceSingleByChannel(HitResult,
				Loca, BW_EndPoint, ECC_GameTraceChannel2, Params);

			if (bSuccessHit)
			{
				// DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 20, FColor::Green, true);

				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					SkillImpactParticle,
					HitResult.ImpactPoint);

				AActor* HitActor = HitResult.GetActor();
				if (HitActor != nullptr)
				{
					FPointDamageEvent DamageEvent(GunDamage, HitResult,
						ShotDirection, nullptr);

					UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SkillImpactSound,
						HitActor->GetActorLocation(),
						HitActor->GetActorRotation());

					HitActor->TakeDamage(W_SkillDamage, DamageEvent, PController,
						this);
				}
			}
		}
	}
}

void AWraith::Passive()
{
	Super::Passive();

	//UE_LOG(LogTemp, Error, TEXT("Called Wraith Passive Player"));
	W_ATK = 50;
	W_HP = 1000;

	ReloadSpeed = 0.6f;

	BonusHP = 300;
	BonusAtk = 7;
}

float AWraith::GetTotalAtk() const
{
	Super::GetTotalAtk();

	return GunDamage;
}

float AWraith::GetTotalHP() const
{
	Super::GetTotalHP();

	return W_HP;
}

float AWraith::GetTotalCool() const
{
	Super::GetTotalCool();

	return W_CoolDown;
}

float AWraith::GetTotalRecoveryRate() const
{
	Super::GetTotalRecoveryRate();

	return RecoveryRate;
}

void AWraith::EnableUpgrade()
{
	// Start UPGRADE Options

	W_HP += PlusHP; // �ִ� ü�� �߰�
	W_ATK += PlusAtk; // �߰� ���ݷ�
	W_CoolDown -= PlusCoolLv; // ��Ÿ�� ���� ��ŭ ��Ÿ�� ����
	RecoveryRate += PlusRecoveryRate; // ȸ�� ȿ�� ������ 0.1 ����

	CoolTime = W_CoolDown;

	Max_HP = W_HP;
	HP = Max_HP;

	GunDamage = W_ATK;
}
