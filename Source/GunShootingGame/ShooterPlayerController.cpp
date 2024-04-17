// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"

void AShooterPlayerController::GameHasEnded(AActor* EndGameFocus, bool bIsWinner)
{
	Super::GameHasEnded(EndGameFocus, bIsWinner);

	//UE_LOG(LogTemp, Display, TEXT("Finish Game"));

	bShowMouseCursor = true;

	if (bIsWinner) // ½Â¸®
	{
		UUserWidget* EndScreen = CreateWidget(this, WinScreen);
		EndScreen->AddToViewport();
	}

	else // ÆÐ¹è
	{
		UUserWidget* EndScreen = CreateWidget(this, LoseScreen);
		EndScreen->AddToViewport();
	}
}

void AShooterPlayerController::GetPlayerData()
{
	Player = Cast<ABaseCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();


}