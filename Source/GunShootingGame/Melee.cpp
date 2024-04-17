// Fill out your copyright notice in the Description page of Project Settings.


#include "Melee.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "EnemyController.h"
#include "BaseCharacter.h"
#include "ShooterPlayerController.h"

void AMelee::KilledPawn(APawn* DeathPawn)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APlayerController* PController = Cast<APlayerController>(DeathPawn->GetController());

	if (DeathPawn->ActorHasTag("Enemy"))
	{
		KillCount++;
		RemainEnemys--;

		if (RemainEnemys <= 0)
		{
			EndGame(true);

			return;
		}
	}

	else if (DeathPawn->ActorHasTag("Player"))
	{
		EndGame(false);

		return;
	}
}

int32 AMelee::GetRemainEnemys()
{
	TArray<AActor*> Enemys;
	UGameplayStatics::GetAllActorsOfClass(this, ABaseCharacter::StaticClass(),
		Enemys);
	// 이 맵에 존재하는 모든 베이스 캐릭터 타입 클래스를 모음

	int32 Temp = 0;

	for (AActor* NotEnemy : Enemys) // 모든 에너미 데이터를 순회
	{
		if (NotEnemy->ActorHasTag("Player"))
		{
			Temp++;
		}
	}

	RemainEnemys = Enemys.Num() - Temp;

	return RemainEnemys;
}

void AMelee::BeginPlay()
{
	Super::BeginPlay();

	RemainEnemys = GetRemainEnemys();
}

void AMelee::EndGame(bool bIsPlayerWin)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AShooterPlayerController* PCont;

	PCont = Cast<AShooterPlayerController>(Player->GetController());

	if (PCont)
	{
		PCont->GameHasEnded(Player, bIsPlayerWin);
	}

	else
	{
		//UE_LOG(LogTemp, Display, TEXT("Error"));
	}
}