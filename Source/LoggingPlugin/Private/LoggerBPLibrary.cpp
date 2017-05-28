// Fill out your copyright notice in the Description page of Project Settings.

#include "LoggingPlugin.h"
#include "LoggerBPLibrary.h"

FileWriter* ULoggerBPLibrary::f = NULL;

ULoggerBPLibrary::ULoggerBPLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}


void ULoggerBPLibrary::InitLogger()
{
	//UE_LOG(Car2IXSLog, Log, TEXT("InitLogger"));

	f = FileWriter::Singleton();
}

void ULoggerBPLibrary::Test()
{
	//UE_LOG(Car2IXSLog, Log, TEXT("Test"));
}

void ULoggerBPLibrary::OpenFile(bool bStartWriting /*= true*/)
{
	//UE_LOG(Car2IXSLog, Log, TEXT("openFile"));

	
	if (f)
	{
		f->openFile();
	}

}

void ULoggerBPLibrary::SetPath(FString _path)
{
	//UE_LOG(Car2IXSLog, Log, TEXT("setPath"));

	if (f)
		f->SetPath(_path);

}

void ULoggerBPLibrary::WriteString(const FString _string, bool _addTimestamp, bool _addNewLine)
{
	//UE_LOG(Car2IXSLog, Log, TEXT("WriteString"));

	if (f)
		f->WriteToFile(_string, _addTimestamp, _addNewLine);
}

void ULoggerBPLibrary::CloseFile()
{
	if (f)
		f->closeFile();
}

void ULoggerBPLibrary::WriteNewLine()
{
	if (f)
		f->WriteNewLine();
}

void ULoggerBPLibrary::InitFile(FString _playerName, FString _condition)
{
	if(f)
		f->InitFile(_playerName, _condition);
}

void ULoggerBPLibrary::StartWriting()
{
	if (f)
		f->StartWriting();
}

void ULoggerBPLibrary::StopWriting()
{
	if (f)
		f->StopWriting();
}

void ULoggerBPLibrary::WriteTimestamp()
{
	if (f)
		f->WriteTimestamp();
}

void ULoggerBPLibrary::SetDebugMessages(bool _newVal)
{
	if (f)
		f->SetDebugMessages(_newVal);
}