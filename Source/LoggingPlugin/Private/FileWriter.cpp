// Fill out your copyright notice in the Description page of Project Settings.

#include "ds_02.h"
#include "FileWriter.h"
#include <string>
#include "FileReader.h"
#include "iostream"
#include "ds_02GameMode.h"


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
		UE_LOG(C2SLog, Log, TEXT("Successfully Set up File Writer. %s"), *filenameString);
		return;
	}
	else
	{
		UE_LOG(C2SLog, Warning, TEXT("Could not open file. I'm gonna check the directory."));
		if (!doesLoggingDirectoryExist(logdir)) //does log dir exist?
		{
			createDirectory(FPaths::GameDir() + logdir);  //create log dir
		}
	}

	if (isFileOpen(&Myfile)) //second check
	{
		UE_LOG(C2SLog, Log, TEXT("Successfully Set up FileWriter (second try). %s"), *filenameString);
		return;
	}
	else
	{
		UE_LOG(C2SLog, Warning, TEXT("Could not open file. Tryimg default dir (game path)"));
		filenameString = gamedir + date + extension; //putting log file in gamedir
		char* newFilename = TCHAR_TO_UTF8(*filenameString);
		Myfile.open(newFilename);
	}

	if (isFileOpen(&Myfile)) //third try
	{
		UE_LOG(C2SLog, Log, TEXT("Successfully Set up FileWriter (third try) \\o/. %s"), *(gamedir + date + extension));
		return;
	}
	else
	{
		UE_LOG(C2SLog, Fatal, TEXT("Could not open file. I'm useless :("));
	}
}

void FileWriter::setPath(FString _path)
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

void FileWriter::setAtomicSteerValue(float _val)
{
	steer = _val;
	//UE_LOG(C2SLog, Log, TEXT("%f"), (float)steer);

}

void FileWriter::setAtomicAccelarteValue(float _val)
{
	accelerate = _val;
}

void FileWriter::initFile()
{
	writeToFile("Mae govannen.");
	writeNewLine();
	writeToFile("I'm a log.");
	writeNewLine(); 
	writeToFile("My structure:v Timestamp;L;X;Y;Z;Yaw;Pitch;Roll;km/h;gear;SteeringValue;AccelerationValue");
	writeNewLine();
	//writeToFile("Sometimes, i.e. when the player hits reaches a sign, lines are different.");
	//writeNewLine();
	//writeToFile("Then the line will have the following items: Trigger, Number, World-Position and a Name. ");
	//writeNewLine();
	writeToFile("Here are some meta information about this session: ");
	writeNewLine();
	writeToFile("Name: " + PlayerName);
	//		writeNewLine();
	//		writeToFile("Environment: " + Environment);
	writeNewLine();

	FString date = FDateTime::Now().ToString();

	writeToFile("Now: " + date + ". Have a good day.");

	writeNewLine();

	writeToFile("Live long, and prosper.");
	writeNewLine();
	writeNewLine();
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
		writeNewLine();
		writeToFile("Live long and prosper!");
		writeNewLine();

		FString date = FDateTime::Now().ToString();

		writeToFile("Fin. " + date + ". Have a good day.");
		Myfile.close();
		UE_LOG(C2SLog, Log, TEXT("Closed file."));
	}
	else if (FileWriter::RefCount > 0)
	{
		//UE_LOG(C2SLog, Log, TEXT("File still used by %d actors."), FileWriter::RefCount);
	}
	else
	{
		//UE_LOG(C2SLog, Log, TEXT("Something is really really wrong: %d actors."), FileWriter::RefCount);
	}
}

FileWriter::~FileWriter()
{
	UE_LOG(C2SLog, Log, TEXT("Shutting down file writer: %d actors."), FileWriter::RefCount);
	closeFile();
}

/*
* Writes position and orientation of the actor to the file. Finishes with a tab stop.
*/
void FileWriter::writeToFile(const FString _value)
{
	//write actor label, pos and orientation to file. data can be used for later analysis

	if (bWrite)
		Myfile << std::string(TCHAR_TO_UTF8(*_value)).c_str() << ";";
}

void FileWriter::writeToFile(const float _value)
{
	if (bWrite)
		Myfile << std::to_string(_value) << ";";
}

void FileWriter::writeToFile(const int _value)
{
	if (bWrite)
		Myfile << std::to_string(_value) << ";";
}

/**
* Writes three or four floats. Finishes with a tab stop.
*/
void FileWriter::writeToFile(const float a, const float b, const float c, const float d = 0.f)
{
	if (bWrite)
		Myfile << a << ";" << b << ";" << c << ";" << d << ";";
}

/*
* Writes a FVector (three values) to file. Usage is intended for 3D coordinates (positional values). Finishes with a tab stop.
*/
void FileWriter::writeToFile(const FVector value)
{
	if (bWrite)
		Myfile << value.X << ";" << value.Y << ";" << value.Z << ";";
}

/*
* Writes yaw, pitch and roll to file (in this order). Finishes with a tab stop.
*/
void FileWriter::writeToFile(const FRotator value)
{
	if (bWrite)
		Myfile << value.Yaw << ";" << value.Pitch << "," << value.Roll << ";";
}

/*
* Writes the complete state of an actor to file. Just for convenience.
*/
void FileWriter::writeToFile(const FString _name, const FVector _position, const FRotator _orientation)
{
	if (bWrite)
	{
		writeTimestamp();
		writeToFile("L");
		writeToFile(_name);
		writeToFile(_position);
		writeToFile(_orientation);
		writeNewLine();
	}
}

/*
* Writes the complete state of an actor to file. Just for convenience.
*/
void FileWriter::writeToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed)
{

	if (bWrite)
	{
		writeTimestamp();
		writeToFile("L");
		writeToFile(_position);
		writeToFile(_orientation);
		writeToFile(_forwardSpeed);
		writeNewLine();
	}
}

/*
* Writes the complete state of an actor to file. Just for convenience.
*/
void FileWriter::writeToFile(const FString _name, const FVector _position, const FRotator _orientation, const float _forwardSpeed, const int _gear)
{
	if (bWrite)
	{
		writeTimestamp();
		writeToFile("L");
		writeToFile(_position);
		writeToFile(_orientation);
		writeToFile(_forwardSpeed);
		writeToFile(_gear);
		writeToFile(steer);
		writeToFile(accelerate);
		writeNewLine();
	}
}

/*
* Writes a new line into file. Best used after a single data set.
*/
void FileWriter::writeNewLine()
{
	if (bWrite) Myfile << "\n";
}

/*
* Toggles the variable @bWrite. Each user of FileWriter should invoke a check before writing something into the log file.
*/
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

	UE_LOG(C2SLog, Log, TEXT("Toggled FileWriter. New Value: %d"), bWrite);
}

void FileWriter::startWriting()
{
	bWrite = true;
	UE_LOG(C2SLog, Log, TEXT("Toggled FileWriter. New Value: %d"), bWrite);
}

void FileWriter::stopWriting()
{
	bWrite = false;
	UE_LOG(C2SLog, Log, TEXT("Toggled FileWriter. New Value: %d"), bWrite);
}

int64 FileWriter::getTimestamp()
{
	return FDateTime::Now().ToUnixTimestamp();
}

void FileWriter::writeTimestamp()
{
	if (Myfile)
		Myfile << getTimestamp() << ";";
}