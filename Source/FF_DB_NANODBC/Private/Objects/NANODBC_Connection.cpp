#include "Objects/NANODBC_Connection.h"

void UNANODBC_Connection::SetConnection(connection In_Connection)
{
	this->NANODBC_Connection = In_Connection;
}

connection UNANODBC_Connection::GetConnection()
{
	return this->NANODBC_Connection;
}

bool UNANODBC_Connection::IsConnectionValid()
{
	if (this->NANODBC_Connection.connected())
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool UNANODBC_Connection::SetConnectionId(FString In_Id)
{
	if (In_Id.IsEmpty())
	{
		return false;
	}

	if (!this->ConnectionId.IsEmpty())
	{
		return false;
	}

	this->ConnectionId = In_Id;
	return true;
}

FString UNANODBC_Connection::GetConnectionId()
{
	return this->ConnectionId;
}

bool UNANODBC_Connection::JustExecute(FString& Out_Code, FString SQL_Query)
{
	if (!NANODBC_Connection.connected())
	{
		return false;
	}

	const ANSICHAR* QueryString = NANODBC_TEXT(TCHAR_TO_UTF8(*SQL_Query));

	try
	{
		just_execute(this->NANODBC_Connection, QueryString);
	}

	catch (const std::exception& Exception)
	{
		Out_Code = Exception.what();
		return false;
	}

	return true;
}

bool UNANODBC_Connection::ExecuteAndGetResult(FString& Out_Code, UNANODBC_Result*& Out_Result, FString SQL_Query)
{
	if (!NANODBC_Connection.connected())
	{
		return false;
	}
	
	const ANSICHAR* QueryString = NANODBC_TEXT(TCHAR_TO_UTF8(*SQL_Query));
	result QueryResult;
	
	try
	{
		QueryResult = execute(this->NANODBC_Connection, QueryString);
	}

	catch (const std::exception& Exception)
	{
		Out_Code = Exception.what();
		return false;
	}
	
	Out_Result = NewObject<UNANODBC_Result>();
	return Out_Result->SetQueryResult(Out_Code, QueryResult);
}