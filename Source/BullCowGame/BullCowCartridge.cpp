// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    TArray<FString> ValidWords = GetValidWords(WordList);
    SetupGame();
    PrintLine(TEXT("The number of possible words is %i"), WordList.Num());
    PrintLine(TEXT("The number of valid words is %i"), ValidWords.Num());
}

void UBullCowCartridge::OnInput(const FString &Input) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PlayerGuess
    {
        ProcessGuess(Input);
    }
}

void UBullCowCartridge::SetupGame()
{
    HiddenWord = TEXT("Dream");
    Lives = HiddenWord.Len();
    bGameOver = false;

    PrintLine(TEXT("Welcome to the Bulls and Cows Game."));
    PrintLine(TEXT("Guess the %i letter isogram word. "), HiddenWord.Len());
    PrintLine(TEXT("You start with %i lives."), Lives);
    PrintLine(TEXT("Press tab to continue..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(FString Guess)
{

    // Check if Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again."));
        return;
    }

    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Bloody hell, you've guessed it!"));
        EndGame();
        return;
    }

    PrintLine(TEXT("You lose a life."));
    Lives--;

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left :("));
        PrintLine(TEXT("\nThe Hidden Word is %s. \nYou have lost."), *HiddenWord);
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Sorry, try guessing again. \nYou have %i lives remaining."), Lives);
        return;
    }

    // Show the player the Bulls and the Cows
}

bool UBullCowCartridge::IsIsogram(FString Word) const
{
    // int32 Index = 0;
    // int32 Comparison = Index + 1;

    // for (; Comparison < Word.Len(); Index++)
    // {
    //     if (Word[Index] == Word[Comparison])
    //     {
    //         return false;
    //     }
    // }

    for (int32 Index = 0; Index < Word.Len(); Index++)
    {
        for (int32 Comparison = Index + 1; Comparison < Word.Len(); Comparison++)
        {
            if (Word[Index] == Word[Comparison])
            {
                return false;
            }
        }
    }

    // for each letter
    // start at element 0
    // compare against the next letter
    // untill we reach word.len() -1
    // if any are the same return false

    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(TArray<FString> WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }

    // for (int32 Index = 0; Index < ValidWords.Num(); Index++)
    // {
    //     PrintLine(TEXT("%s"), *ValidWords[Index]);
    // }

    return ValidWords;
}
