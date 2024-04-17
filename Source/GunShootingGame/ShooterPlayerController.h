// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class GUNSHOOTINGGAME_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	virtual void GameHasEnded
	(
		class AActor* EndGameFocus = nullptr,
		bool bIsWinner = false
	) override;

	UFUNCTION(BlueprintCallable)
	void GetPlayerData();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> WinScreen;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LoseScreen;

	class ABaseCharacter* Player;
};
