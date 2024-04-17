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
	// 에너미 인식용 //
	Max_HP = W_HP;
	HP = Max_HP;

	GunDamage = W_ATK;
	// 에너미 용 //
	
	MaxRange = W_Range;

	MagazineMaxSize = Weapon_Magazine;
	MagazineNowSize = MagazineMaxSize;
	FireVelocity = W_FireVelo;

	SkillDelay = 1.f;
}

void AWraith::ActiveSkill()
{
	Super::ActiveSkill();

	// 탄창 10발 소모
	// 더욱 긴 사거리
	// 데미지 1000의 강력한 피해
	// 쿨타임 8초
	// 스킬 후딜레이 1초
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
			// 총이 온 방향을 뜻하기에 발사의 반대

			FCollisionQueryParams Params;
			Params.AddIgnoredActor(this); // 발사한 액터만 대상에서 제외

			FHitResult HitResult;


			UGameplayStatics::SpawnEmitterAttached(SkillFlash,
				GetMesh(), TEXT("GunFlashPoint"), FVector(ForceInit),
				GetMesh()->GetRelativeRotation() * -1);

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SkillSound,
				GetActorLocation(), GetActorRotation());

			FVector BW_EndPoint = Loca + Rota.Vector() * W_SkillReach;
			// Rota.Vector()는 회전하여 보고있는 그 방향을 의미한다.

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

	W_HP += PlusHP; // 최대 체력 추가
	W_ATK += PlusAtk; // 추가 공격력
	W_CoolDown -= PlusCoolLv; // 쿨타임 레벨 만큼 쿨타임 감소
	RecoveryRate += PlusRecoveryRate; // 회복 효율 레벨당 0.1 증가

	CoolTime = W_CoolDown;

	Max_HP = W_HP;
	HP = Max_HP;

	GunDamage = W_ATK;
}
