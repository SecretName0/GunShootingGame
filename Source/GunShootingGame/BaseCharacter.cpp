// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "Melee.h"
#include "TimerManager.h"
#include "ShooterPlayerController.h"
#include "TheMercenary.h"

ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BW_ParticlePoint = CreateDefaultSubobject<USceneComponent>
		(TEXT("ParticlePoint"));
	BW_ParticlePoint->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocation(ArmLoca);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	HP = Max_HP;
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// ----- Damage Funcs ----- //
void ABaseCharacter::Fire()
{
	if (bCanFire && !IsDash)
	{
		bCanFire = false;

		if (MagazineNowSize == 0)
		{
			Reload();
			// 장탄이 비었을 때 쏘려고하면 알아서 재장전 실행
			return;
		}

		MagazineNowSize--;

		AController* PController = this->GetController();
		if (PController == nullptr) { return; }

		FVector Loca;
		FRotator Rota;
		PController->GetPlayerViewPoint(Loca, Rota);
		// 보고 있는 방향 발사

		FVector ShotDirection = -Rota.Vector();
		// 총이 온 방향을 뜻하기에 발사의 반대

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this); // 발사한 액터만 대상에서 제외

		FHitResult HitResult;


		// 발사
		PlayAnimMontage(FireMontage);

		if (ActorHasTag("Player"))
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShootCameraShake);
		}

		UGameplayStatics::SpawnEmitterAttached(WeaponFlash,
			GetMesh(), TEXT("GunFlashPoint"), FVector(ForceInit),
			GetMesh()->GetRelativeRotation() * -1);

		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), WeaponSound,
			GetActorLocation(), GetActorRotation());

		FVector ShellingPoint = Loca + Rota.Vector() * MaxRange;
		// Rota.Vector()는 회전하여 보고있는 그 방향을 의미한다.

		bool bSuccessHit = GetWorld()->LineTraceSingleByChannel(HitResult,
			Loca, ShellingPoint, ECC_GameTraceChannel2, Params);

		if (bSuccessHit)
		{
			// DrawDebugPoint(GetWorld(), HitResult.ImpactPoint, 20, FColor::Green, true);

			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
				WeaponParticle,
				HitResult.ImpactPoint);

			AActor* HitActor = HitResult.GetActor();
			if (HitActor != nullptr)
			{
				FPointDamageEvent DamageEvent(GunDamage, HitResult,
					ShotDirection, nullptr);

				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), Weapon_ImpactSound,
					HitActor->GetActorLocation(),
					HitActor->GetActorRotation());

				HitActor->TakeDamage(GunDamage, DamageEvent, PController,
					this);
			}
		}

		FTimerHandle FireVelo;

		GetWorldTimerManager().SetTimer(FireVelo, this,
			&ABaseCharacter::EndFireVelo, FireVelocity);
	}
}

void ABaseCharacter::EndFireVelo()
{
	bCanFire = true;
}

float ABaseCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(HP, DamageToApply);
	HP -= DamageToApply;

	bNowFight = true; // 피해를 받은 경우 전투중으로 간주

	FTimerHandle ChangeState;

	if (bNowFight && bCanSelfRecovery) // 자힐 효과 적용 중 피격이 되는 경우
	{
		// UE_LOG(LogTemp, Warning, TEXT("Recovery Timer End and State Timer"));
		GetWorldTimerManager().ClearTimer(SelfRecover);
		GetWorldTimerManager().ClearTimer(ChangeState);
		// 회복 효과 중단
	}

	if (HP > 0) // 맞고 생존한 경우
	{
		PlayAnimMontage(HitAnim);

		if (ActorHasTag("Player"))
		{
			GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(DamagedCameraShake);
		}

		GetWorldTimerManager().SetTimer(ChangeState, this,
			&ABaseCharacter::NotFight, 4);
		// 맞고나서 4초후 비전투 상태로 변경
	}

	if (HP <= 0) // 피격자가 체력이 0이하가 된 경우
	{
		if (this->ActorHasTag("Enemy"))
		{
			DetachFromControllerPendingDestroy();
		}

		else
		{
			bCanFire = false;
		}

		AMelee* Melee;
		Melee = GetWorld()->GetAuthGameMode<AMelee>();

		ATheMercenary* Mercenary;
		Mercenary = GetWorld()->GetAuthGameMode<ATheMercenary>();

		GetCapsuleComponent()->
			SetCollisionEnabled(ECollisionEnabled::NoCollision);

		if(Melee) // 밀리 모드에서
		{
			if (bIsDead == false)
			{
				bIsDead = true;
				Melee->KilledPawn(this);
			}
		}

		else
		{
			//UE_LOG(LogTemp, Display, TEXT("Now Mode is Not Melee"));
		}

		if(Mercenary) // 머셔너리 모드에서
		{
			if (bIsDead == false)
			{
				bIsDead = true;
				Mercenary->KilledPawn(this);
			}
		}

		else
		{
			//UE_LOG(LogTemp, Display, TEXT("Now Mode is Not Mercenary"));
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Health left %f"), HP);

	return DamageToApply;
}

// ---- Event ---- //
void ABaseCharacter::DeSpawn()
{
	Destroy();
}

// ----- Getter ----- //
float ABaseCharacter::GetAxisValue(float& AxisValue, float Value)
{
	return AxisValue = Value;
}

float ABaseCharacter::GetHpPercent() const
{
	return HP / Max_HP;
}

float ABaseCharacter::GetTotalAtk() const
{
	return 0.f;
}

float ABaseCharacter::GetTotalHP() const
{
	return 0.f;
}

float ABaseCharacter::GetTotalCool() const
{
	return 0.f;
}

float ABaseCharacter::GetTotalRecoveryRate() const
{
	return 0.f;
}

int64 ABaseCharacter::GetMagazine() const
{
	return MagazineNowSize;
}

// ----- Inputs ----- //
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ABaseCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookPitchRotate"), this, &ABaseCharacter::RotatePitch);
	PlayerInputComponent->BindAxis(TEXT("LookYawRotate"), this, &ABaseCharacter::RotateYaw);
	PlayerInputComponent->BindAxis(TEXT("SetTargetArm"), this, &ABaseCharacter::SetTargetArm);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ABaseCharacter::Fire);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("CameraInversion"), IE_Pressed, this, &ABaseCharacter::CameraInversion);
	PlayerInputComponent->BindAction(TEXT("Reload"), IE_Pressed, this, &ABaseCharacter::Reload);
	PlayerInputComponent->BindAction(TEXT("UseToHpPotion"), IE_Pressed, this, &ABaseCharacter::UseToHpPotion);
	PlayerInputComponent->BindAction(TEXT("UesToDotPotion"), IE_Pressed, this, &ABaseCharacter::UseToHpDotPotion);
	PlayerInputComponent->BindAction(TEXT("UseToAtkPotion"), IE_Pressed, this, &ABaseCharacter::UseToAtkPotion);
	PlayerInputComponent->BindAction(TEXT("Skill"), IE_Pressed, this, &ABaseCharacter::ActiveSkill);
	PlayerInputComponent->BindAction(TEXT("Interaction"), IE_Pressed, this, &ABaseCharacter::Item_Interaction);
}

// ----- Axis Input ----- //
void ABaseCharacter::MoveForward(float Value)
{
	FVector FinalScale = GetActorForwardVector() * Value;
	AddMovementInput(FinalScale);

	GetAxisValue(VerticalValue, Value);
}

void ABaseCharacter::MoveRight(float Value)
{
	if(IsDash == false)
	{
		FVector FinalScale = GetActorRightVector() * Value;
		AddMovementInput(FinalScale);
	}

	GetAxisValue(HorizontalValue, Value);
}

void ABaseCharacter::RotateYaw(float Value)
{
	AddControllerYawInput(Value * MouseSensitivity * GetWorld()->DeltaTimeSeconds);
	// 좌우 옵션
}

void ABaseCharacter::RotatePitch(float Value)
{
	AddControllerPitchInput(Value * MouseSensitivity * GetWorld()->DeltaTimeSeconds);
	// 상하
}

void ABaseCharacter::SetTargetArm(float Value)
{
	if (Value > 0.f && SpringArm->TargetArmLength > 200)
	{
		SpringArm->TargetArmLength -= 50;
		// 마우스 휠을 올리는 경우
	}

	else if (Value < 0.f && SpringArm->TargetArmLength < 700)
	{
		SpringArm->TargetArmLength += 50;
		// 마우스 휠을 내릴 경우
	}
}

// ----- Action Input ----- //
void ABaseCharacter::CameraInversion()
{
	ArmLoca.Y *= -1;

	SpringArm->SetRelativeLocation(ArmLoca);
}

void ABaseCharacter::Reload()
{
	if (MagazineNowSize < MagazineMaxSize && !IsDash)
	{
		FTimerHandle ReloadTimer;

		bCanFire = false;

		PlayAnimMontage(ReloadAnim);
		UGameplayStatics::PlaySound2D(GetWorld(), ReloadSound);

		GetWorldTimerManager().SetTimer(ReloadTimer, this,
			&ABaseCharacter::CompleteReloading, ReloadSpeed);
	}
}

void ABaseCharacter::CompleteReloading()
{
	MagazineNowSize = MagazineMaxSize;
	bCanFire = true;
}

void ABaseCharacter::UseToHpPotion()
{
	if (HP_PotionNum > 0)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), UsePotionSound);

		HP_PotionNum--; // 포션 사용
		float Heal = 150;

		Recovery(Heal);
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Enough Potion"));
	}
}

void ABaseCharacter::UseToHpDotPotion()
{
	if (HP_Recovery_PotionNum > 0)
	{
		if(IsNowHeal == false)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), UsePotionSound);

			HP_Recovery_PotionNum--; // 포션 사용

			// Dot Heal Logic
			IsNowHeal = true;
			if(IsNowHeal)
			{
				for(int32 a = 1; a<6; a++)
				{
					float HealDelay = 2;
					HealDelay *= a;

					FTimerHandle DotHeal;

					GetWorldTimerManager().SetTimer(DotHeal, this,
						&ABaseCharacter::DotRecovery, HealDelay);

					UE_LOG(LogTemp, Error, TEXT("Heal %d"), a);
				}
			}

			FTimerHandle DotHealEnd;

			GetWorldTimerManager().SetTimer(DotHealEnd, this,
				&ABaseCharacter::CanNowHeal, 10);
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Enough Potion"));
	}
}

void ABaseCharacter::UseToAtkPotion()
{
	if (PowerUp_PotionNum > 0)
	{
		if(IsNowUpAtk == false)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), UsePotionSound);

			PowerUp_PotionNum--; // 포션 사용

			// PowerLogic
			PowerUp();

			FTimerHandle BackDamage;

			GetWorldTimerManager().SetTimer(BackDamage, this,
				&ABaseCharacter::DotRecovery, 30.f);

			IsNowUpAtk = true;
		}
	}

	else
	{
		UE_LOG(LogTemp, Error, TEXT("Not Enough Potion"));
	}
}

void ABaseCharacter::Item_Interaction()
{
	FHitResult HitResult;
	bool HasHit = GetItemInReach(HitResult);

	if (HasHit) // 범위 내에 파밍이 가능한 아이템이 존재가 할 경우
	{
		UPrimitiveComponent* TargetComponent = HitResult.GetComponent();

		AActor* HitActor = HitResult.GetActor();

		/*DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 20.f,
			10, FColor::Red, true, 3.f);*/

		if (HitActor->ActorHasTag("Item")) // 상호작용이 가능한 아이템일 때
		{
			// 본 컴포넌트 소지자 정보
			// ABaseCharacter* Owner = Cast<ABaseCharacter>(GetOwner());

			if(HitActor->ActorHasTag("HpPotion")) // 일반 체력 포션
			{
				HP_PotionNum++;
				UE_LOG(LogTemp, Warning, TEXT("%d"), HP_PotionNum);
			}

			else if (HitActor->ActorHasTag("DotPotion")) // 도트 체력 포션
			{
				HP_Recovery_PotionNum++;
				UE_LOG(LogTemp, Warning, TEXT("%d"), HP_Recovery_PotionNum);
			}

			else if (HitActor->ActorHasTag("AtkPotion")) // 공격력 포션
			{
				PowerUp_PotionNum++;
				UE_LOG(LogTemp, Warning, TEXT("%d"), PowerUp_PotionNum);
			}

			UGameplayStatics::SpawnSoundAtLocation(GetWorld(), RootSound,
				GetActorLocation(),
				GetActorRotation());

			HitActor->Destroy();
		}

		else if (!HitActor->ActorHasTag("Item"))
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't Farming"));
		}
	}
}

bool ABaseCharacter::GetItemInReach(FHitResult &OutHitResult)
{
	FVector Start = Camera->GetComponentLocation();
	FVector End = Start + Camera->GetForwardVector() * MaxRootingReach;

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);

	return GetWorld()->SweepSingleByChannel(OutHitResult,
		Start, End,
		FQuat::Identity,
		ECC_GameTraceChannel1, Sphere);
}

// ----- Skills ----- //
void ABaseCharacter::ActiveSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("Called Skill"));

	if(CanSkill && !IsDash)
	{
		PlayAnimMontage(SkillMontage);

		FTimerHandle SkillCool;
		GetWorldTimerManager().SetTimer(SkillCool, this,
			&ABaseCharacter::CanUseToSkill, CoolTime);

		FTimerHandle NoFire;
		GetWorldTimerManager().SetTimer(NoFire, this,
			&ABaseCharacter::CoolDownComplete, SkillDelay);
	}
}

void ABaseCharacter::CanUseToSkill()
{
	CanSkill = true;
}

void ABaseCharacter::CoolDownComplete()
{
	bCanFire = true;
}

void ABaseCharacter::Passive()
{
	UE_LOG(LogTemp, Display, TEXT("Called Passive from Base"));
	// ToDo
	// 자식 클래스들 중, 플레이어블 캐릭터로 가서
	// 고유 능력 작성 및 비긴이나 생성자에 호출
	// 필요에 따라 쿨타임, UI등 작성
}

// ----- Chara Base Ability Change ----- //
void ABaseCharacter::Recovery(float HealBase)
{
	HP += (HealBase * RecoveryRate);

	if (HP > Max_HP)
	{
		HP = Max_HP;
	}
}

void ABaseCharacter::DotRecovery()
{
	float Heal = ((Max_HP / 100) * 3) * RecoveryRate; // 최대 체력 3% * 효율

	HP += Heal;

	if (Max_HP < HP)
	{
		HP = Max_HP;
	}
}

void ABaseCharacter::PowerUp()
{
	TempDamage = GunDamage;
	GunDamage += GunDamage / 10;
	// '기본' 공격력의 10%만큼 공격력 추가
}

void ABaseCharacter::OriginPower()
{
	GunDamage = TempDamage;

	IsNowUpAtk = false;
}

void ABaseCharacter::NotFight()
{
	bNowFight = false;

	if (bCanSelfRecovery)
	{
		SelfRecovery();
	}
}

void ABaseCharacter::CanNowHeal()
{
	IsNowHeal = false;
}

void ABaseCharacter::SelfRecovery()
{
	if (bCanSelfRecovery)
	{
		GetWorldTimerManager().SetTimer(SelfRecover, this,
			&ABaseCharacter::SelfRecoveryOption, 4, true, 5);
	}
}

void ABaseCharacter::SelfRecoveryOption()
{
	float Heal = (Max_HP / 100) * RecoveryRate; // 최대 체력 1% * 효율

	HP += Heal;

	if (Max_HP < HP)
	{
		HP = Max_HP;

		if(GetWorldTimerManager().IsTimerActive(SelfRecover))
		{
			UE_LOG(LogTemp, Warning, TEXT("Timer End (Full)"));
			GetWorldTimerManager().ClearTimer(SelfRecover);
		}
	}
}




