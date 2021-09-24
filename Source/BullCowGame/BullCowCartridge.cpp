// Fill out your copyright notice in the Description page of Project Settings.
#include "BullCowCartridge.h"
#include "HiddenWordList.h"

void UBullCowCartridge::BeginPlay() // When the game starts
{
    Super::BeginPlay();
    Isograms = GetValidWords(WordList);
    SetupGame();
}

void UBullCowCartridge::OnInput(const FString &PlayerInput) // When the player hits enter
{
    if (bGameOver)
    {
        ClearScreen();
        SetupGame();
    }
    else // Checking PlayerGuess
    {
        ProcessGuess(PlayerInput);
    }
}

void UBullCowCartridge::SetupGame() // Initial Game set-up method
{
    HiddenWord = Isograms[FMath::RandRange(0, GetValidWords(WordList).Num() - 1)]; // Get a random word from the list
    Lives = HiddenWord.Len() * 2;
    bGameOver = false;

    PrintLine(TEXT("Welcome to the Bulls and Cows Game."));
    // PrintLine(TEXT("The number of valid words is %i"), Isograms.Num());
    // PrintLine(TEXT("HiddenWord ==== %s"), *HiddenWord);
    PrintLine(TEXT("Guess the %i letter isogram word. "), HiddenWord.Len());
    PrintLine(TEXT("You start with %i lives."), Lives);
    PrintLine(TEXT("Press tab to continue..."));
}

void UBullCowCartridge::EndGame()
{
    bGameOver = true;
    PrintLine(TEXT("\nPress enter to play again."));
}

void UBullCowCartridge::ProcessGuess(const FString &Guess)
{

    // Check if Isogram
    if (!IsIsogram(Guess))
    {
        PrintLine(TEXT("No repeating letters, guess again."));
        Lives--;
        PrintLine(TEXT("Lost a life, %i remaining."), Lives);
        return;
    }

    // Show the player the Bulls and the Cows - (Right Letter on the right spot/ Right letter on the wrong spot)
    FBullCowCount Score = GetBullCows(Guess);
    PrintLine(TEXT("You have %i Bulls and %i Cows"), Score.Bulls, Score.Cows);

    if (Guess == HiddenWord)
    {
        PrintLine(TEXT("Bloody hell, you've guessed it!"));
        EndGame();
        return;
    }

    if (Guess.Len() != HiddenWord.Len())
    {
        PrintLine(TEXT("Wrong word length."));
        Lives--;
        PrintLine(TEXT("Lost a life, %i remaining."), Lives);
        return;
    }

    if (Lives <= 0)
    {
        ClearScreen();
        PrintLine(TEXT("You have no lives left :("));
        PrintLine(TEXT("\nThe Hidden Word is %s. \nYou have lost."), *HiddenWord);
        EndGame();
        return;
    }

    Lives--;
    PrintLine(TEXT("Lost a life, %i remaining."), Lives);
}

bool UBullCowCartridge::IsIsogram(const FString &Word) const
{
    // for each letter
    // start at element 0
    // compare against the next letter
    // untill we reach word.len()
    // if any are the same return false

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
    return true;
}

TArray<FString> UBullCowCartridge::GetValidWords(const TArray<FString> &WordList) const
{
    TArray<FString> ValidWords;

    for (FString Word : WordList)
    {
        if (Word.Len() >= 4 && Word.Len() <= 8 && IsIsogram(Word))
        {
            ValidWords.Emplace(Word);
        }
    }
    return ValidWords;
}

FBullCowCount UBullCowCartridge::GetBullCows(const FString &Guess) const
{
    FBullCowCount Count;

    // for every index guess is same as Hidden index, Bullcount ++
    // if not bull was it a cow? if yes Cowcount ++

    for (int32 GuessIndex = 0; GuessIndex < Guess.Len() && GuessIndex <= HiddenWord.Len() - 1; GuessIndex++)
    {
        if (Guess[GuessIndex] == HiddenWord[GuessIndex])
        {
            Count.Bulls++;
            continue;
        }

        for (int32 HiddenIndex = 0; HiddenIndex < HiddenWord.Len(); HiddenIndex++)
        {
            if (Guess[GuessIndex] == HiddenWord[HiddenIndex])
            {
                Count.Cows++;
                break;
            }
        }
    }
    return Count;
}
