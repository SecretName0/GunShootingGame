// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BaseCharacter.h"

void AEnemyController::DragDropTypes()
{
	GetBlackboardComponent()->SetValueAsVector(TEXT("SelfStartLocation"), GetOwner()->GetActorLocation());
}

void AEnemyController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyController::BeginPlay()
{
	Super::BeginPlay();

	if (EnemyTree != nullptr)
	{
		RunBehaviorTree(EnemyTree);

		APawn* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	}
}

bool AEnemyController::IsDead() const
{
	ABaseCharacter* ControlledPawn = Cast<ABaseCharacter>(GetPawn());

	if (ControlledPawn)
	{
		ControlledPawn->bIsDead = false;
		return false;
		// 아직 죽지 않았음
	}

	UE_LOG(LogTemp, Warning, TEXT("EnemyCont Error Is Dead -> Controlled Pawn OR IsDead"));
	ControlledPawn->bIsDead = true;

	FTimerHandle DespawnTimer;
	GetWorldTimerManager().SetTimer(DespawnTimer, ControlledPawn,
		&ABaseCharacter::DeSpawn, 2.f);

	return true;
}
