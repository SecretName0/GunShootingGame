// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Shoot.h"
#include "BaseCharacter.h"
#include "AIController.h"


UBTTask_Shoot::UBTTask_Shoot()
{
	NodeName = TEXT("Shooting");
}

EBTNodeResult::Type UBTTask_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIOwner Is Null!"));

		return EBTNodeResult::Failed;
	}

	ThisPawn = Cast<ABaseCharacter>(OwnerComp.GetAIOwner()->GetPawn());

	if (ThisPawn == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ThisPawn is null!"));

		return EBTNodeResult::Failed;
	}

	ThisPawn->Fire();
	return EBTNodeResult::Succeeded;
}
