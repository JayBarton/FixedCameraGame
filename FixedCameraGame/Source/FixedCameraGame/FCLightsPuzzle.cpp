// Fill out your copyright notice in the Description page of Project Settings.


#include "FCLightsPuzzle.h"

AFCLightsPuzzle::AFCLightsPuzzle()
{
	int level = size * size;
	gridDefault.SetNum(level);
	grid.SetNum(level);
	boxes.SetNum(level);
}

void AFCLightsPuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Up", IE_Pressed, this, &AFCLightsPuzzle::MoveUp);
	PlayerInputComponent->BindAction("Down", IE_Pressed, this, &AFCLightsPuzzle::MoveDown);
	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AFCLightsPuzzle::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AFCLightsPuzzle::MoveRight);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFCLightsPuzzle::ToggleLight);
	PlayerInputComponent->BindAction("Submit", IE_Pressed, this, &AFCLightsPuzzle::CheckSolution);
}

void AFCLightsPuzzle::MoveRight()
{
	index++;
	if (index % size == 0)
	{
		index -= size;
	}
	updateIndex();
}

void AFCLightsPuzzle::MoveLeft()
{
	index--;
	if (index < 0 || index % size == size - 1)
	{
		index += size;
	}
	updateIndex();
}

void AFCLightsPuzzle::MoveUp()
{
	index -= size;
	if (index < 0)
	{
		index += (grid.Num());
	}
	updateIndex();
}

void AFCLightsPuzzle::MoveDown()
{
	index += size;
	if (index >= grid.Num())
	{
		index -= (grid.Num());
	}
	updateIndex();
}

void AFCLightsPuzzle::ToggleLight()
{
	grid[index] = !grid[index];
	//toggle adjacent
	int32 aIndex = index + 1;
	if (aIndex % size > 0)
	{
		grid[aIndex] = !grid[aIndex];
	}
	aIndex = index - 1;
	if (aIndex >= 0 && aIndex % size < size - 1)
	{
		grid[aIndex] = !grid[aIndex];
	}
	//down check
	aIndex = index + size;
	if (aIndex < grid.Num())
	{
		grid[aIndex] = !grid[aIndex];
	}
	aIndex = index - size;
	if (aIndex >= 0)
	{
		grid[aIndex] = !grid[aIndex];
	}
	Toggle();
}

void AFCLightsPuzzle::StartPuzzle()
{
	Super::StartPuzzle();
	grid = gridDefault;
	index = 0;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::Undefined;
	for (int i = 0; i < grid.Num(); i++)
	{
		float X = startLocation.X - (i / size) * 110;
		float Y = startLocation.Y + (i % size) * 110;

		FVector location(X, Y, startLocation.Z);

		AActor* box = GetWorld()->SpawnActor<AActor>(prop, location, FRotator::ZeroRotator, SpawnParams);

		boxes[i] = box;
	}
	SetUp();
}

void AFCLightsPuzzle::ExitPuzzle()
{
	Super::ExitPuzzle();

	for (int i = 0; i < boxes.Num(); i++)
	{
		boxes[i]->Destroy();
	}
}

void AFCLightsPuzzle::CheckSolution()
{
	for (int i = 0; i < grid.Num(); i++)
	{
		if (grid[i] == false)
		{
			UE_LOG(LogTemp, Warning, TEXT("No Way"));
			return;
		}
	}
	SolvePuzzle();
}

void AFCLightsPuzzle::SetUp_Implementation()
{
}

void AFCLightsPuzzle::updateIndex_Implementation()
{
}

void AFCLightsPuzzle::Toggle_Implementation()
{
}