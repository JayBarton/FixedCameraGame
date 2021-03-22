// Fill out your copyright notice in the Description page of Project Settings.


#include "FCTowerPuzzle.h"

AFCTowerPuzzle::AFCTowerPuzzle()
{
}

void AFCTowerPuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
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
	if (selectedRod < 0)
	{
		if (towers[currentRod].disk.Num() > 0)
		{
			selectedRod = currentRod;
		}
	}
	else if (selectedRod != currentRod)
	{
		//move disk
		towers[currentRod].disk.Add(towers[selectedRod].disk.Pop());
		MoveDisk();
		selectedRod = -1;
	}
}


void AFCTowerPuzzle::StartPuzzle()
{
	Super::StartPuzzle();
	towers.SetNum(numberOfRods);
	towers[0].disk.SetNum(numberOfDisks);
	for (int i = 0; i < numberOfDisks; i++)
	{
		towers[0].disk[i] = i;
	}
	SetUp();
}

void AFCTowerPuzzle::ExitPuzzle()
{
	Super::ExitPuzzle();

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

void AFCTowerPuzzle::MoveDisk_Implementation()
{
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
