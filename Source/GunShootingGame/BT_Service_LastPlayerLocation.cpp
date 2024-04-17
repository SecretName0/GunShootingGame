// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_Service_LastPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Pawn.h"

UBT_Service_LastPlayerLocation::UBT_Service_LastPlayerLocation()
{
	NodeName = (TEXT("Update Last Player Location"));
}

void UBT_Service_LastPlayerLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

	if (Player == nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("BTService->(Last)PlayerLocation Error! Player is Null"));
		return;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Player->GetActorLocation());
}