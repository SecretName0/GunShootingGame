// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Service_Player_IFSee.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"
#include "AIController.h"

UBT_Service_Player_IFSee::UBT_Service_Player_IFSee()
{
	NodeName = (TEXT("Update Player Location IF SEE"));
}

void UBT_Service_Player_IFSee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTService->PlayerLocation IFSee Error! Player is Null"));
		return;
	}

	if (OwnerComp.GetAIOwner() == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BTService->PlayerLocation IFSee Error! AIOwner is Null"));
		return;
	}

	if (OwnerComp.GetAIOwner()->LineOfSightTo(Player))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), Player);
	}

	else
	{
		OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());
	}
}
