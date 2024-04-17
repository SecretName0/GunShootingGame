// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BT_Service_Player_IFSee.generated.h"

/**
 * 
 */
UCLASS()
class GUNSHOOTINGGAME_API UBT_Service_Player_IFSee : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBT_Service_Player_IFSee();

protected:
	virtual void TickNode
	(
		UBehaviorTreeComponent& OwnerComp,
		uint8* NodeMemory,
		float DeltaSeconds
	) override;
};
