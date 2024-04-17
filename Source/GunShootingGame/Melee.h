// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Melee.generated.h"

/**
 * 
 */
UCLASS()
class GUNSHOOTINGGAME_API AMelee : public AGameModeBase
{
	GENERATED_BODY()

public:
	void KilledPawn(APawn* DeathPawn);

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 KillCount = 0;

	UFUNCTION(BlueprintCallable)
	int32 GetRemainEnemys();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 RemainEnemys;

	// Melee
	/*UPROPERTY(BlueprintReadWrite, EditAnyWhere)
	bool Level1_Clear;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
	bool Level2_Clear;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
	bool Level3_Clear;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
	bool NowPlayLevel1;

	UPROPERTY(BlueprintReadWrite, EditAnyWhere)
	bool NowPlayLevel2;*/

protected:
	virtual void BeginPlay() override;

private:
	void EndGame(bool bIsPlayerWin);
};
