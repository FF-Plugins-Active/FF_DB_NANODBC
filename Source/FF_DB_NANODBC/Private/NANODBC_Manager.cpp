// Fill out your copyright notice in the Description page of Project Settings.

#include "NANODBC_Manager.h"

// Sets default values
ANANODBC_Manager::ANANODBC_Manager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ANANODBC_Manager::BeginPlay()
{
	Super::BeginPlay();
}

void ANANODBC_Manager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void ANANODBC_Manager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}