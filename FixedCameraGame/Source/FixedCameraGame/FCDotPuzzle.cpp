// Fill out your copyright notice in the Description page of Project Settings.


#include "FCDotPuzzle.h"

AFCDotPuzzle::AFCDotPuzzle()
{
	slots.SetNum(numberOfRows);
	for (int i = 0; i < numberOfRows; i++)
	{
		//TODO see if we can't find a better way to do this rather than sizing all of these arrays individually
		if (i % 2 == 0)
		{
			slots[i].row.SetNum(numberOfRows);
			slots[i].rowDefault.SetNum(numberOfRows);
			slots[i].prop.SetNum(numberOfRows);
			slots[i].solution.SetNum(numberOfRows);
		}
		else
		{
			slots[i].row.SetNum(numberOfRows - 1);
			slots[i].rowDefault.SetNum(numberOfRows - 1);
			slots[i].prop.SetNum(numberOfRows - 1);
			slots[i].solution.SetNum(numberOfRows - 1);
		}
	}
}

void AFCDotPuzzle::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Left", IE_Pressed, this, &AFCDotPuzzle::MoveLeft);
	PlayerInputComponent->BindAction("Right", IE_Pressed, this, &AFCDotPuzzle::MoveRight);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFCDotPuzzle::SelectSlot);
	PlayerInputComponent->BindAction("Submit", IE_Pressed, this, &AFCDotPuzzle::CheckSolution);
}

void AFCDotPuzzle::Tick(float DeltaTime)
{
}

void AFCDotPuzzle::MoveRight()
{
	currentSlot++;
	if (currentSlot >= numberOfSlots)
	{
		currentSlot = 0;
	}

	UE_LOG(LogTemp, Warning, TEXT("right %i"), currentSlot);
}

void AFCDotPuzzle::MoveLeft()
{
	currentSlot--;
	if (currentSlot < 0)
	{
		currentSlot = numberOfSlots - 1;
	}
	UE_LOG(LogTemp, Warning, TEXT("left %i"), currentSlot);
}

void AFCDotPuzzle::SelectSlot()
{
	int selectedSlot = currentSlot;

	for (int i = 0; i < numberOfRows; i++)
	{
		//want to skip over the next row if going left from the left most slot or right from the right most slot
		if (selectedSlot < 0)
		{
			//in the case of less than 0, we are moving left from the left most, 
			//so want to "fall through" to 0 of the next row
			selectedSlot = 0;
		}
		else if(selectedSlot < slots[i].row.Num())
		{
			bool right = slots[i].row[selectedSlot];
			if (right)
			{
				slots[i].row[selectedSlot] = false;
				if (slots[i].row.Num() < numberOfRows)
				{
					selectedSlot++;
				}
			}
			else
			{
				slots[i].row[selectedSlot] = true;
				if (slots[i].row.Num() == numberOfRows)
				{
					selectedSlot--;
				}
			}
		}
	}
	Toggle();
}

void AFCDotPuzzle::StartPuzzle()
{
	Super::StartPuzzle();
	for (int i = 0; i < numberOfRows; i++)
	{
		slots[i].row = slots[i].rowDefault;
	}
	Toggle();
}

void AFCDotPuzzle::ExitPuzzle()
{
	Super::ExitPuzzle();
	//implement later
	/*for (int i = 0; i < numberOfRows; i++)
	{
		for (int c = 0; c < slots[i].prop.Num(); c++)
		{
			slots[i].prop[c]->Destroy();
		}
	}*/
}

void AFCDotPuzzle::CheckSolution()
{
	for (int i = 0; i < numberOfRows; i++)
	{
		if (slots[i].row != slots[i].solution)
		{
			UE_LOG(LogTemp, Warning, TEXT("that's wrong"));
			return;
		}
	}
	SolvePuzzle();
}

void AFCDotPuzzle::Toggle_Implementation()
{
}

void AFCDotPuzzle::SetUp_Implementation()
{
}

void AFCDotPuzzle::updateIndex_Implementation()
{
}
