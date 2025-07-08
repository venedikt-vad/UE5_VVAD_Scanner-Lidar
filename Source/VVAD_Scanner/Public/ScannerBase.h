// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ArrowComponent.h"
#include "ScanPointsGrid.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/LineBatchComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "ScannerBase.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VVAD_SCANNER_API UScannerBase : public UArrowComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UScannerBase();

	UPROPERTY()
	AScanPointsGrid* Grid;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner")
	float TraceDistance;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner")
	TEnumAsByte<ECollisionChannel> TraceChannel;
	
	UPROPERTY()
	ULineBatchComponent* LineBatchComponent = CreateDefaultSubobject<ULineBatchComponent>(TEXT("LineBatcher"));

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner")
	float ScanSpeed;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Optimization")
	int MaxTracesPerFrame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Optimization")
	bool bReduceCloseTraces;


	UFUNCTION(BlueprintCallable, Category = "Scanner")
	int CalculateNumTraces();

	UFUNCTION(BlueprintCallable, Category = "Scanner")
	bool TraceLocalDirection(FVector dir);
	
	UFUNCTION(BlueprintCallable, Category = "Scanner")
	void SendPointsToGrid();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner")
	bool bDrawTraceLines;
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Scanner")
	FLinearColor GetColorForTrace(FHitResult Hit);

	UFUNCTION(BlueprintCallable, Category = "Scanner")
	virtual void StartTracing();
	UFUNCTION(BlueprintCallable, Category = "Scanner")
	virtual void StopTracing();

private:
	UPROPERTY()
	TArray<FVector> PtsLocations;
	UPROPERTY()
	TArray<float> PtsData;
	
	UPROPERTY()
	TArray<FBatchedLine> LinesToDraw;

	UPROPERTY()
	float LastTraceTime = 0;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
