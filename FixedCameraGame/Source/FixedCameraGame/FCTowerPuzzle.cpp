// Fill out your copyright notice in the Description page of Project Settings.


#include "FCTowerPuzzle.h"

AFCTowerPuzzle::AFCTowerPuzzle()
{
}

void AFCTowerPuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AFCTowerPuzzle::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AFCTowerPuzzle::MoveRight);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFCTowerPuzzle::SelectDisk);
	PlayerInputComponent->BindAction("Submit", IE_Pressed, this, &AFCTowerPuzzle::CheckSolution);
}

void AFCTowerPuzzle::MoveRight()
{
	currentRod++;
	if (currentRod >= numberOfRods)
	{
		currentRod = 0;
	}
	updateIndex();
}

void AFCTowerPuzzle::MoveLeft()
{
	currentRod--;
	if (currentRod <= -1)
	{
		currentRod = numberOfRods - 1;
	}
	updateIndex();
}

void AFCTowerPuzzle::SelectDisk()
{
	//initial selection
	if (selectedRod < 0)
	{
		if (towers[currentRod].disk.Num() > 0)
		{
			selectedRod = currentRod;
		}
	}
	//Can't place on the already selected rod
	else if (selectedRod != currentRod)
	{
		//Can't place larger disks on smaller disks. Also need to check if there are no disks on this rod
		if (towers[currentRod].disk.Num() == 0 || towers[selectedRod].disk.Last() > towers[currentRod].disk.Last())
		{
			MoveDisk();
		}
		selectedRod = -1;
	}
}

void AFCTowerPuzzle::MoveDisk()
{
	//move disk
	int currentDisk = towers[selectedRod].disk.Pop();
	towers[currentRod].disk.Add(currentDisk);

	float Y = startLocation.Y - 100 * currentRod;
	float Z = startLocation.Z + 6.0f * (towers[currentRod].disk.Num() - 1);

	disks[currentDisk]->SetActorLocation(FVector(startLocation.X, Y, Z));
}

void AFCTowerPuzzle::StartPuzzle()
{
	Super::StartPuzzle();

	currentRod = 0;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	rods.SetNum(numberOfRods);
	for (int i = 0; i < numberOfRods; i++)
	{
		float Y = startLocation.Y - i * 100;
		FVector location(startLocation.X, Y, startLocation.Z);

		AActor* rod = GetWorld()->SpawnActor<AActor>(prop, location, FRotator::ZeroRotator, SpawnParams);
		rods[i] = rod;
	}
	towers.SetNum(numberOfRods);
	towers[0].disk.SetNum(numberOfDisks);
	disks.SetNum(numberOfDisks);
	for (int i = 0; i < numberOfDisks; i++)
	{
		towers[0].disk[i] = i;

		float Z = startLocation.Z + i * 6;
		FVector location(startLocation.X, startLocation.Y, Z);
		AActor* disk = GetWorld()->SpawnActor<AActor>(diskProp, location, FRotator::ZeroRotator, SpawnParams);
		float scale = 1.0f - 0.25f * i;
		disk->SetActorScale3D(FVector(scale, scale, disk->GetActorScale().Z));
		disks[i] = disk;
	}
	SetUp();
}

void AFCTowerPuzzle::ExitPuzzle()
{
	Super::ExitPuzzle();

	towers.Empty();

	for (int i = 0; i < numberOfDisks; i ++)
	{
		disks[i]->Destroy();
	}
	for (int i = 0; i < numberOfRods; i++)
	{
		rods[i]->Destroy();
	}
}

void AFCTowerPuzzle::CheckSolution()
{
	int sum = 0;
	//No point checking if the final rod doesn't have all the disks
	if (towers[numberOfRods - 1].disk.Num() == numberOfDisks)
	{
		for (int i = 0; i < numberOfDisks; i++)
		{
			if (towers[numberOfRods - 1].disk[i] == i)
			{
				sum++;
			}

		}
	}
	if (sum == numberOfDisks)
	{
		SolvePuzzle();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Done"));
	}
}

void AFCTowerPuzzle::Toggle_Implementation()
{
}

void AFCTowerPuzzle::SetUp_Implementation()
{
}

void AFCTowerPuzzle::updateIndex_Implementation()
{
}
