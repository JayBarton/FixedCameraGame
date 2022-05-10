// Fill out your copyright notice in the Description page of Project Settings.


#include "FCDeadPlayer.h"

// Sets default values
AFCDeadPlayer::AFCDeadPlayer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
}

