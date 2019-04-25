// Fill out your copyright notice in the Description page of Project Settings.
#include "FileWriter.h"

#include "LoggingPlugin.h"

#include <string>
#include "iostream"

#include <chrono>



FileWriter* FileWriter::s_instance = NULL;


FileWriter* FileWriter::Singleton() {
	if (!s_instance)
	{
		s_instance = new FileWriter();
	}
	return s_instance;
}


FileWriter::FileWriter()
{

}

/*
* Opens a file named after the current time and date. Location is currently fixed to the location of the engine binary.
*/
void FileWriter::openFile(bool bStartWriting /*= true*/)
{
	if (bIsFileOpen)
	{
		UE_LOG(Car2IXSLog, Log, TEXT("There is already an open file!"));
		return;
	}

	FString logdir = "Custom/Log/";
	FString gamedir = FPaths::ProjectDir();

	Path = gamedir + logdir;

	FString extension = ".csv";
	FString date = FDateTime::Now().ToString();

	FString filenameString = "";

	filenameString += Path;
	filenameString += date;
	filenameString += extension;

	char* filename = TCHAR_TO_UTF8(*filenameString);

	Myfile.open(filename);

	if (isFileOpen(&Myfile)) //first check
	{
		UE_LOG(Car2IXSLog, Log, TEXT("Successfully Set up File Writer. %s"), *filenameString);

		if (bStartWriting)
			StartWriting();

		bIsFileOpen = true;

		return;
	}
	else
	{
		UE_LOG(Car2IXSLog, Warning, TEXT("Could not open file. I'm gonna check the directory."));

		if (!doesLoggingDirectoryExist(logdir)) //does log dir exist?
		{
			createDirectory(FPaths::ProjectDir() + logdir);  //create log dir
		}
	}

	Myfile.open(filename);

	if (isFileOpen(&Myfile)) //second check
	{

		UE_LOG(Car2IXSLog, Log, TEXT("Successfully Set up FileWriter (second try). %s"), *filenameString);

		if (bStartWriting)
			StartWriting();

		bIsFileOpen = true;
		return;
	}
	else
	{
		UE_LOG(Car2IXSLog, Warning, TEXT("Could not open file. Tryimg default dir (game path)"));
		filenameString = gamedir + date + extension; //putting log file in gamedir
		char* newFilename = TCHAR_TO_UTF8(*filenameString);

		Myfile.open(newFilename);

		if (bStartWriting)
			StartWriting();

		bIsFileOpen = true;
	}

	if (isFileOpen(&Myfile)) //third try
	{
	
		UE_LOG(Car2IXSLog, Log, TEXT("Successfully Set up FileWriter (third try) \\o/. %s"), *(gamedir + date + extension));
		return;
	}
	else
	{
		UE_LOG(Car2IXSLog, Fatal, TEXT("Could not open file. I'm useless :("));
	}
}

void FileWriter::SetPath(FString _path)
{
	Path = _path;
}

void FileWriter::setPlayerName(const FString _name)
{
	PlayerName = _name;

}

void FileWriter::setEnvironment(const FString _env)
{
	Environment = _env;
}

void FileWriter::InitFile(FString _comment1, FString _comment2, FString _playerName, FString _condition)
{

	if (bIsInitialized)
	{
		UE_LOG(Car2IXSLog, Log, TEXT("I have already been initialized!"));
		return;
	}
	StartWriting();

	WriteToFile("Mae govannen.");
	WriteNewLine();
	WriteToFile("I'm a log.");
	WriteNewLine();
	WriteToFile("Here are some meta information about this session: ");
	WriteNewLine();
	setPlayerName(_playerName);
	WriteToFile("Name: " + PlayerName);
	WriteNewLine();
	setEnvironment(_condition);
	WriteToFile("Environment: " + Environment);
	WriteNewLine();
	WriteToFile("Comment1: " + _comment1);
	WriteNewLine();
	WriteToFile("Comment2: " + _comment2);
	WriteNewLine();
	FString date = FDateTime::Now().ToString();
	WriteToFile("Now: " + date + ". Have a good day.");

	WriteNewLine();

	WriteToFile("Live long, and prosper.");
	WriteNewLine();
	WriteNewLine();


	StopWriting();
	bIsInitialized = true;
}


void FileWriter::SetDebugMessages(bool _newValue)
{
	bIsDebugMode = _newValue;
}

bool FileWriter::doesLoggingDirectoryExist(const FString _pathToCheck) const
{
	bool res = FPaths::DirectoryExists(*_pathToCheck);
	return res;
}

bool FileWriter::isFileOpen(std::ofstream* _file)
{
	return _file->is_open();
}


void FileWriter::createDirectory(FString _path)
{
	FPlatformFileManager::Get().GetPlatformFile().CreateDirectory(*_path);
}

/*
* Closes the file. Best invoked at EndGame().
*/
void FileWriter::closeFile()
{
	if (!isFileOpen(&Myfile))
	{
		UE_LOG(Car2IXSLog, Log, TEXT("There is no open file!"));
		return;
	}


	WriteNewLine();
	WriteToFile("Live long and prosper!");
	WriteNewLine();

	FString date = FDateTime::Now().ToString();

	WriteToFile("Fin. " + date + ". Have a good day.");
	Myfile.close();
	UE_LOG(Car2IXSLog, Log, TEXT("Closed file."));

	bIsInitialized = false;
	bIsFileOpen = false;

	/*
	else if (FileWriter::RefCount > 0)
	{
		UE_LOG(Car2IXSLog, Log, TEXT("File still used by %d actors."), FileWriter::RefCount);
	}
	else
	{
		UE_LOG(Car2IXSLog, Log, TEXT("Something is really really wrong: %d actors."), FileWriter::RefCount);
	}
*/
}

FileWriter::~FileWriter()
{
	closeFile();
}

void FileWriter::WriteToFile(const FString _value)
{

	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite) {
			if (bIsDebugMode)
				UE_LOG(Car2IXSLog, Log, TEXT("WriteToFile: %s ; (%s)"), *_value, bWrite ? TEXT("True") : TEXT("False"));
			Myfile << std::string(TCHAR_TO_UTF8(*_value)).c_str() << getDelimiter();
		}
	}
}


void FileWriter::WriteToFile(const FString _value, bool _addTimestamp, bool _addNewline)
{

	if (bWrite)
	{
		if (bIsDebugMode)
			UE_LOG(Car2IXSLog, Log, TEXT("WriteToFile: %s ; (%s)"), *_value, bWrite ? TEXT("True") : TEXT("False"));

		if(_addTimestamp)
			WriteTimestamp();

		WriteToFile(_value);

		if (_addNewline)
			WriteNewLine();
	}
}

void FileWriter::WriteToFileEv(const FString _value, bool _addTimestamp, bool _addNewline)
{

	if (bWrite)
	{
		if (bIsDebugMode)
			UE_LOG(Car2IXSLog, Log, TEXT("WriteToFile: %s ; (%s)"), *_value, bWrite ? TEXT("True") : TEXT("False"));

		if (_addTimestamp)
			WriteTimestamp();

		WriteToFile("Event");

		WriteToFile(_value);

		if (_addNewline)
			WriteNewLine();
	}
}

void FileWriter::WriteToFile(const float _value)
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile << std::to_string(_value) << getDelimiter();
	}
}

void FileWriter::WriteToFile(const int _value)
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile << std::to_string(_value) << getDelimiter();
	}
}

void FileWriter::WriteToFile(const float a, const float b, const float c, const float d = 0.f)
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile 
			<< a << getDelimiter()
			<< b << getDelimiter()
			<< c << getDelimiter() 
			<< d << getDelimiter();
	}
}

void FileWriter::WriteToFile(const FVector value)
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile 
			<< value.X << getDelimiter()
			<< value.Y << getDelimiter() 
			<< value.Z << getDelimiter() ;
	}
}

void FileWriter::WriteToFile(const FRotator value)
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile 
			<< value.Yaw << getDelimiter()
			<< value.Pitch << getDelimiter() 
			<< value.Roll << getDelimiter() ;
	}
}

void FileWriter::WriteToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed, const int _gear)
{
	if (bWrite)
	{
		WriteTimestamp();
		WriteToFile("L");
		WriteToFile(_position);
		WriteToFile(_orientation);
		WriteToFile(_forwardSpeed);
		WriteToFile(_gear);
		WriteNewLine();
	}
}

void FileWriter::WriteNewLine()
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile << "\n";
	}
}

void FileWriter::StartWriting()
{
	bWrite = true;
	UE_LOG(Car2IXSLog, Log, TEXT("Toggled FileWriter StartWriting. New Value: %s"), bWrite ? TEXT("True") : TEXT("False"));
}

void FileWriter::StopWriting()
{
	bWrite = false;
	UE_LOG(Car2IXSLog, Log, TEXT("Toggled FileWriter. New Value: %s"), bWrite ? TEXT("True") : TEXT("False"));
}

int64 FileWriter::getTimestamp()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void FileWriter::WriteTimestamp()
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile << getTimestamp() << ";";
			//Myfile << std::string(TCHAR_TO_UTF8(*getTimestamp2())).c_str()
	}
}

std::string FileWriter::getDelimiter()
{
	return std::string(TCHAR_TO_UTF8(*Delimiter)).c_str();
}