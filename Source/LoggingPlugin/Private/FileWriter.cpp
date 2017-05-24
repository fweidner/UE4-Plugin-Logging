// Fill out your copyright notice in the Description page of Project Settings.
#include "LoggingPlugin.h"

#include "FileWriter.h"
#include <string>
#include "iostream"



FileWriter* FileWriter::s_instance = NULL;
int FileWriter::RefCount = 0;


FileWriter* FileWriter::Singleton() {
	FileWriter::RefCount += 1;
	if (!s_instance)
		s_instance = new FileWriter();

	return s_instance;
}

/*
* Private constructor for the FileWriter. Sets @bWrite to false and opens (prepares) a file via @openFile().
*/
FileWriter::FileWriter()
{
	//setup write mode for actor
	bWrite = false;
	openFile();
	//	initFile();

}

/*
* Opens a file named after the current time and date. Location is currently fixed to the location of the engine binary.
*/
void FileWriter::openFile()
{
	FString logdir = "Custom/Log/";
	FString gamedir = FPaths::GameDir();

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
		return;
	}
	else
	{
		UE_LOG(Car2IXSLog, Warning, TEXT("Could not open file. I'm gonna check the directory."));
		if (!doesLoggingDirectoryExist(logdir)) //does log dir exist?
		{
			createDirectory(FPaths::GameDir() + logdir);  //create log dir
		}
	}

	if (isFileOpen(&Myfile)) //second check
	{
		UE_LOG(Car2IXSLog, Log, TEXT("Successfully Set up FileWriter (second try). %s"), *filenameString);
		return;
	}
	else
	{
		UE_LOG(Car2IXSLog, Warning, TEXT("Could not open file. Tryimg default dir (game path)"));
		filenameString = gamedir + date + extension; //putting log file in gamedir
		char* newFilename = TCHAR_TO_UTF8(*filenameString);
		Myfile.open(newFilename);
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

void FileWriter::SetPlayerName(const FString _name)
{
	PlayerName = _name;

}

void FileWriter::SetEnvironment(const FString _env)
{
	Environment = _env;
}

void FileWriter::InitFile(FString _playerName, FString _condition)
{
	WriteToFile("Mae govannen.");
	WriteNewLine();
	WriteToFile("I'm a log.");
	WriteNewLine();
	WriteToFile("Here are some meta information about this session: ");
	WriteNewLine();
	PlayerName = _playerName;
	WriteToFile("Name: " + PlayerName);
	WriteNewLine();
	Environment = _condition;
	WriteToFile("Environment: " + Environment);
	WriteNewLine();
	FString date = FDateTime::Now().ToString();
	WriteToFile("Now: " + date + ". Have a good day.");

	WriteNewLine();

	WriteToFile("Live long, and prosper.");
	WriteNewLine();
	WriteNewLine();
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
	RefCount -= 1;

	if (FileWriter::RefCount == 0)
	{
		WriteNewLine();
		WriteToFile("Live long and prosper!");
		WriteNewLine();

		FString date = FDateTime::Now().ToString();

		WriteToFile("Fin. " + date + ". Have a good day.");
		Myfile.close();
		UE_LOG(Car2IXSLog, Log, TEXT("Closed file."));
	}
	else if (FileWriter::RefCount > 0)
	{
		UE_LOG(Car2IXSLog, Log, TEXT("File still used by %d actors."), FileWriter::RefCount);
	}
	else
	{
		UE_LOG(Car2IXSLog, Log, TEXT("Something is really really wrong: %d actors."), FileWriter::RefCount);
	}
}

FileWriter::~FileWriter()
{
	UE_LOG(Car2IXSLog, Log, TEXT("Shutting down file writer: %d actors."), FileWriter::RefCount);
	closeFile();
}

void FileWriter::WriteToFile(const FString _value)
{
	FScopeLock Lock(&MyMutexWrite);
	{

		if (bWrite)
			Myfile << std::string(TCHAR_TO_UTF8(*_value)).c_str() << ";";
	}
}

void FileWriter::WriteToFile(const float _value)
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile << std::to_string(_value) << ";";
	}
}

void FileWriter::WriteToFile(const int _value)
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile << std::to_string(_value) << ";";
	}
}

void FileWriter::WriteToFile(const float a, const float b, const float c, const float d = 0.f)
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile << a << ";" << b << ";" << c << ";" << d << ";";
	}
}

void FileWriter::WriteToFile(const FVector value)
{
	FScopeLock Lock(&MyMutexWrite);
	{
		if (bWrite)
			Myfile << value.X << ";" << value.Y << ";" << value.Z << ";";
	}
}

void FileWriter::WriteToFile(const FRotator value)
{
	FScopeLock Lock(&MyMutexWrite);
{
		if (bWrite)
			Myfile << value.Yaw << ";" << value.Pitch << "," << value.Roll << ";";
}
}

void FileWriter::WriteToFile(const FString _name, const FVector _position, const FRotator _orientation)
{
	if (bWrite)
	{
		writeTimestamp();
		WriteToFile("L");
		WriteToFile(_name);
		WriteToFile(_position);
		WriteToFile(_orientation);
		WriteNewLine();
	}
}

void FileWriter::WriteToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed)
{

	if (bWrite)
	{
		writeTimestamp();
		WriteToFile("L");
		WriteToFile(_position);
		WriteToFile(_orientation);
		WriteToFile(_forwardSpeed);
		WriteNewLine();
	}
}

void FileWriter::WriteToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed, const int _gear)
{
	if (bWrite)
	{
		writeTimestamp();
		WriteToFile("L");
		WriteToFile(_position);
		WriteToFile(_orientation);
		WriteToFile(_forwardSpeed);
		WriteToFile(_gear);
		WriteToFile(steer);
		WriteToFile(accelerate);
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

void FileWriter::StartStopWriting()
{
	FString t = "FileWriter: ";
	if (bWrite)
	{
		bWrite = false;
		t = t + "off.";
	}
	else
	{
		bWrite = true;
		t = t + "on.";
	}

	UE_LOG(Car2IXSLog, Log, TEXT("Toggled FileWriter. New Value: %d"), bWrite);
}

void FileWriter::StartWriting()
{
	bWrite = true;
	UE_LOG(Car2IXSLog, Log, TEXT("Toggled FileWriter. New Value: %d"), bWrite);
}

void FileWriter::StopWriting()
{
	bWrite = false;
	UE_LOG(Car2IXSLog, Log, TEXT("Toggled FileWriter. New Value: %d"), bWrite);
}

int64 FileWriter::getTimestamp()
{
	return FDateTime::Now().ToUnixTimestamp();
}

void FileWriter::writeTimestamp()
{
	FScopeLock Lock(&MyMutexWrite);

	{
		if (Myfile)
			Myfile << getTimestamp() << ";";
	}
}