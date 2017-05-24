// like... do what you want?! (But do not infring the EpicGames copyright notes)

#include "LoggingPlugin.h"

#include "FileWriterProxy.h"

int32 UFileWriterProxy::index = 0;

bool UFileWriterProxy::fileExist()
{
	if (f)
		return true;
	else
		return false;
}

UFileWriterProxy::UFileWriterProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	

}

UFileWriterProxy::~UFileWriterProxy()
{
	if (f)
		f->closeFile();
}

void UFileWriterProxy::BeginPlay()
{
	f = FileWriter::Singleton();
}

void UFileWriterProxy::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (f)
		f->closeFile();
}

void UFileWriterProxy::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

}

/*
Writes the current trigger element. Supports only unint16 elements.
*/
void UFileWriterProxy::writeTriggerLCT(const FString _name)
{
	if (!fileExist())
		return;

	FString tmp;

	tmp = "Trigger:";
	tmp.AppendInt(index);
	index += 1;

	tmp += "\tWorld-Position: ";
	tmp += 	this->GetComponentLocation().ToCompactString();
	
	tmp.Append(FString("\tName: "));
	tmp.Append(_name);

	f->writeToFile(tmp);
	f->writeNewLine();
}

void UFileWriterProxy::writeLaneLCT(const FString _lane)
{
	if (!fileExist())
		return;

	FString tmp;
	tmp = "Change to lane: ";
	tmp += _lane;
	f->writeToFile(tmp);
	f->writeNewLine();
}

void UFileWriterProxy::writeDefaultString(const FString _string)
{
	if (!f) return;
	f->writeTimestamp();
	f->writeToFile(_string);
	f->writeNewLine();
}

void UFileWriterProxy::startWriting()
{
	f->startWriting();
}

void UFileWriterProxy::stopWriting()
{
	f->stopWriting();
}

void UFileWriterProxy::closeFile() 
{
	f->closeFile();
}

void UFileWriterProxy::setPath(FString _path)
{
	f->setPath(_path);
}

void UFileWriterProxy::openFile()
{
	f->openFile();
}