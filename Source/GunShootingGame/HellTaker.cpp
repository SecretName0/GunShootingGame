// Fill out your copyright notice in the Description page of Project Settings.


#include "HellTaker.h"

AHellTaker::AHellTaker()
{
}

void AHellTaker::BeginPlay()
{
	Super::BeginPlay();

	Max_HP = Boss_HP;
	HP = Max_HP;

	GunDamage = Boss_ATK;
	MaxRange = Range;

	MagazineMaxSize = Weapon_Magazine;
	MagazineNowSize = MagazineMaxSize;

	FireVelocity = B_FireVelo;
}
