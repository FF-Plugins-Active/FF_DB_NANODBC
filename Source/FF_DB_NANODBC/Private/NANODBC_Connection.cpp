#include "NANODBC_Connection.h"

// CONNECTION.

void UNANODBC_Connection::SetConnection(connection In_Connection)
{
	this->NANODBC_Connection = In_Connection;
}

bool UNANODBC_Connection::SetConnectionId(FString In_Id)
{
	if (In_Id.IsEmpty())
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

bool UNANODBC_Connection::JustExecute(FString& Out_Code, FString SQL_Statement)
{
	if (!NANODBC_Connection.connected())
	{
		return false;
	}

	const ANSICHAR* QueryString = NANODBC_TEXT(TCHAR_TO_UTF8(*SQL_Statement));

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

bool UNANODBC_Connection::ExecuteAndGetResult(FString& Out_Code, int32& AffectedRows, UNANODBC_Result*& Out_Result, FString SQL_Statement)
{
	if (!NANODBC_Connection.connected())
	{
		return false;
	}
	
	const ANSICHAR* QueryString = NANODBC_TEXT(TCHAR_TO_UTF8(*SQL_Statement));
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

	if (QueryResult.has_affected_rows())
	{
		AffectedRows = QueryResult.affected_rows();
	}

	Out_Result = NewObject<UNANODBC_Result>();
	Out_Result->SetQueryResult(QueryResult);

	return true;
}

// RESULT.

bool UNANODBC_Result::SetQueryResult(result In_Result)
{
	if (!In_Result)
	{
		return false;
	}

	this->QueryResult = In_Result;
	return true;
}

bool UNANODBC_Result::GetColumnsCount(int32& Out_Count)
{
	if (!this->QueryResult)
	{
		return false;
	}

	Out_Count = this->QueryResult.columns();
	return true;
}

bool UNANODBC_Result::GetMetaData(FString& Out_Code, FNANODBC_MetaData& Out_MetaData, int32 ColumnIndex)
{
	if (!this->QueryResult)
	{
		return false;
	}

	nanodbc::string ColumnName = this->QueryResult.column_name(ColumnIndex);

	FNANODBC_MetaData MetaData;
	
	MetaData.ColumnType = this->QueryResult.column_datatype(ColumnName);
	MetaData.ColumnName = UTF8_TO_TCHAR(ColumnName.c_str());
	MetaData.ColumnTypeName = UTF8_TO_TCHAR(this->QueryResult.column_datatype_name(ColumnName).c_str());
	MetaData.bIsNull = this->QueryResult.is_null(ColumnName);
	MetaData.ColumnDecimalDigit = this->QueryResult.column_decimal_digits(ColumnName);
	MetaData.ColumnSize = this->QueryResult.column_size(ColumnName);
	MetaData.ColumnNumber = this->QueryResult.column(ColumnName);
	
	Out_MetaData = MetaData;

	return true;
}

bool UNANODBC_Result::GetInt(FString& Out_Code, TArray<int32>& Out_Value, const FString In_ColumnName)
{
	if (!this->QueryResult)
	{
		return false;
	}

	const ANSICHAR* ColumnName = NANODBC_TEXT(TCHAR_TO_UTF8(*In_ColumnName));
	TArray<int32> TempArray;

	try
	{
		while (this->QueryResult.next())
		{
			int32 EachValue = 0;
			this->QueryResult.get_ref<int>(ColumnName, EachValue);

			TempArray.Add(EachValue);
		}

		Out_Value = TempArray;
	}

	catch (const std::exception& Exception)
	{
		Out_Code = Exception.what();
		return false;
	}
	
	return true;
}

bool UNANODBC_Result::GetString(FString& Out_Code, TArray<FString>& Out_Value, const FString In_ColumnName)
{
	if (!this->QueryResult)
	{
		return false;
	}

	const ANSICHAR* ColumnName = NANODBC_TEXT(TCHAR_TO_UTF8(*In_ColumnName));
	TArray<FString> TempArray;

	try
	{
		while (this->QueryResult.next())
		{
			nanodbc::string EachValueRaw;
			this->QueryResult.get_ref<nanodbc::string>(ColumnName, EachValueRaw);
			FString EachValue = UTF8_TO_TCHAR(EachValueRaw.c_str());

			TempArray.Add(EachValue);
		}

		Out_Value = TempArray;
	}

	catch (const std::exception& Exception)
	{
		Out_Code = Exception.what();
		return false;
	}

	return true;
}

bool UNANODBC_Result::GetBool(FString& Out_Code, TArray<bool>& Out_Value, const FString In_ColumnName)
{
	if (!this->QueryResult)
	{
		return false;
	}

	const ANSICHAR* ColumnName = NANODBC_TEXT(TCHAR_TO_UTF8(*In_ColumnName));
	TArray<bool> TempArray;

	try
	{
		while (this->QueryResult.next())
		{
			int32 EachValue = 0; 
			this->QueryResult.get_ref<int>(ColumnName, EachValue);

			if (EachValue == 0)
			{
				TempArray.Add(false);
			}
			
			else
			{
				TempArray.Add(true);
			}
		}

		Out_Value = TempArray;
	}

	catch (const std::exception& Exception)
	{
		Out_Code = Exception.what();
		return false;
	}

	return true;
}