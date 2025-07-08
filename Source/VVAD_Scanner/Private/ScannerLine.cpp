// Fill out your copyright notice in the Description page of Project Settings.


#include "ScannerLine.h"

void UScannerLine::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int Num = CalculateNumTraces();

	if (Num > 0) {
		float TravelAmount = ((UGameplayStatics::GetRealTimeSeconds(this) - StartScanTime) / TravelTime) * 2 - 1;
		if (TravelAmount > 1) {
			bTracePeriodCompleted = true;
			StopTracing();
			return;
		}
		int currentLineInt = floor(TravelAmount * TravelAngle / TravelPointPeriod);
		float currentLineAngle = (currentLineInt * TravelPointPeriod) / 45.f;

		for (int i = 0; i < Num; i++) {
			FVector v;
			switch (ScanPattern) {
			case ELineScanPattern::Random:
			{
				float w = FMath::FRandRange(-LineAngle, LineAngle) / 45.f;
				v = FVector(1.f, w, currentLineAngle);
			}
			break;

			case ELineScanPattern::Grid:
			{
				float ptStepCount = floor((LineAngle * 2) / LinePointPeriod);
				float w = (floor(FMath::FRandRange(0, ptStepCount))* LinePointPeriod - LineAngle) / 45.f;
				v = FVector(1.f, w, currentLineAngle);
			}
			break;
			case ELineScanPattern::OffsetedGrid:
			{
				float ptStepCount = floor((LineAngle * 2) / LinePointPeriod);
				float w = (((currentLineInt%2)?LinePointPeriod/2:0) + (floor(FMath::FRandRange(0, ptStepCount)) * LinePointPeriod - LineAngle)) / 45.f;
				v = FVector(1.f, w, currentLineAngle);
			}
			break;
			default:
				break;
			}
			TraceLocalDirection(v);

		}

		SendPointsToGrid();
	}
}

void UScannerLine::StartTracing() {
	if (bTracePeriodCompleted || bAllowStartDuringScan) {
		StartScanTime = UGameplayStatics::GetRealTimeSeconds(this);
		bTracePeriodCompleted = false;
		UScannerBase::StartTracing();
	}
}

void UScannerLine::StopTracing() {
	if (!bStopWhenCompleted || bTracePeriodCompleted) {
		OnScanEnded.Broadcast();
		UScannerBase::StopTracing();
		bTracePeriodCompleted = true;
	}
}
