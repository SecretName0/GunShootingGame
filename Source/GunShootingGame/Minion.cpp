// Fill out your copyright notice in the Description page of Project Settings.


#include "Minion.h"

AMinion::AMinion()
{
	Max_HP = MW_HP;
}

void AMinion::BeginPlay()
{
	Super::BeginPlay();

	HP = Max_HP;

	GunDamage = ATK;
	MaxRange = Range;

	MagazineMaxSize = Weapon_Magazine;
	MagazineNowSize = MagazineMaxSize;

	FireVelocity = M_FireVelo;
}
