// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunShootingGameGameModeBase.h"
#include "TheMercenary.generated.h"

/**
 * 
 */
UCLASS()
class GUNSHOOTINGGAME_API ATheMercenary : public AGunShootingGameGameModeBase
{
	GENERATED_BODY()
	
public:
	void KilledPawn(APawn* DeathPawn);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Score = 0;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 RemainEnemy = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RemainTime = 120;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float TimeBonus;

	UFUNCTION(BlueprintCallable)
	void Win();

	// The Mercenary
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 BestScore_Map1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	int32 BestScore_Map2;

protected:
	virtual void BeginPlay() override;

private:
	void EndGame(bool bIsPlayerWin);

	void SpawnRule(APawn* Enemy);
};
