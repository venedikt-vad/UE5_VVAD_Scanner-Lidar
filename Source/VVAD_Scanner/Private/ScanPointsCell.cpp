// Fill out your copyright notice in the Description page of Project Settings.


#include "ScanPointsCell.h"

AScanPointsCell::AScanPointsCell() {
	PrimaryActorTick.bCanEverTick = false;
}

void AScanPointsCell::SetMaterial(UMaterialInterface* NewMaterial) {
	for (UInstancedStaticMeshComponent* inst : InstanceHolders) {
		inst->SetMaterial(0, NewMaterial);
	}
}

void AScanPointsCell::prepareForDestroy() {
	for (UInstancedStaticMeshComponent* inst : InstanceHolders) {
		inst->DestroyComponent();
	}
	InstanceHolders.Empty();
}

void AScanPointsCell::BeginPlay() {
	Super::BeginPlay();
}

void AScanPointsCell::EndPlay(const EEndPlayReason::Type EndPlayReason) {
	prepareForDestroy();
}


void AScanPointsCell::Tick(float DeltaTime) {
}
