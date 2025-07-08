// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScannerBase.h"
#include "ScannerLine.generated.h"

UENUM()
enum class ELineScanPattern {
	Random,
	Grid,
	OffsetedGrid
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnScanEndedSignature);

UCLASS(Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class VVAD_SCANNER_API UScannerLine : public UScannerBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FOnScanEndedSignature OnScanEnded;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Line")
	float LineAngle = 45.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Line")
	float TravelAngle = 45.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Line")
	float TravelTime = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Line")
	ELineScanPattern ScanPattern;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Line")
	bool bStopWhenCompleted = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Line")
	bool bAllowStartDuringScan = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Line")
	float LinePointPeriod = 2.f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Scanner | Line")
	float TravelPointPeriod = 3.f;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void StartTracing() override;
	virtual void StopTracing() override;

private:
	UPROPERTY()
	float StartScanTime;
	UPROPERTY()
	bool bTracePeriodCompleted = true;
};
