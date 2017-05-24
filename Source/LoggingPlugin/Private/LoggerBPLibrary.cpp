// Fill out your copyright notice in the Description page of Project Settings.

#include "LoggingPlugin.h"
#include "LoggerBPLibrary.h"

ULoggerBPLibrary::ULoggerBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void ULoggerBPLibrary::Test()
{
	UE_LOG(Car2IXSLog, Log, TEXT("Test"));
}

