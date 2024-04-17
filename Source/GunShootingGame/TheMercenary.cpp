// Fill out your copyright notice in the Description page of Project Settings.


#include "TheMercenary.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"
#include "ShooterPlayerController.h"

void ATheMercenary::KilledPawn(APawn* DeathPawn)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	APlayerController* PController = Cast<APlayerController>(DeathPawn->GetController());

	if (DeathPawn->ActorHasTag("Enemy"))
	{
		if(DeathPawn->ActorHasTag("Minion"))
		{
			RemainEnemy--;
		}
	}

	else if (DeathPawn->ActorHasTag("Player"))
	{
		EndGame(false);

		return;
	}
}

void ATheMercenary::Win()
{
	EndGame(true);
}

void ATheMercenary::BeginPlay()
{
	Super::BeginPlay();

}

void ATheMercenary::EndGame(bool bIsPlayerWin)
{
	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AShooterPlayerController* PCont;

	PCont = Cast<AShooterPlayerController>(Player->GetController());

	if (PCont)
	{
		PCont->GameHasEnded(Player, bIsPlayerWin);
	}
}
