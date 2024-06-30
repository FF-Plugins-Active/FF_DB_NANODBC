#include "NANODBC_Connection.h"

// CONNECTION.

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

	int32 TempRowsCount = 0;
	TArray<TArray<FNANODBC_DataValue>> Temp_All_Data;

	try
	{
		const int32 ColumnCount = In_Result.columns();

		while (In_Result.next())
		{
			TempRowsCount += 1;
			TArray<FNANODBC_DataValue> Array_Row;

			for (int32 Index_Column = 0; Index_Column < ColumnCount; Index_Column++)
			{
				FNANODBC_DataValue EachData;

				const int32 DataType = In_Result.column_datatype(Index_Column);
				const FString DataTypeName = UTF8_TO_TCHAR(In_Result.column_datatype_name(Index_Column).c_str());
				
				switch (DataType)
				{
					case -1:

						EachData.ValString = UTF8_TO_TCHAR(In_Result.get<nanodbc::string>(Index_Column).c_str());
						EachData.DataType = DataType;
						EachData.DataTypeName = DataTypeName;
						EachData.ValueRepresentation = EachData.ValString;

						break;
				
					case 4:

						EachData.ValInt32 = In_Result.get<int>(Index_Column);
						EachData.DataType = DataType;
						EachData.DataTypeName = DataTypeName;
						EachData.ValueRepresentation = FString::FromInt(EachData.ValInt32);

						break;
				}

				Array_Row.Add(EachData);
			}

			Temp_All_Data.Add(Array_Row);
		}
	}

	catch (const std::exception& Exception)
	{
		FString Out_Code = Exception.what();
	}

	this->All_Data = Temp_All_Data;
	this->QueryResult = In_Result;
	this->RowsCount = TempRowsCount;

	return true;
}

bool UNANODBC_Result::GetAffectedRows(FString& Out_Code, int32& AffectedRows)
{
	if (!this->QueryResult)
	{
		Out_Code = "Query result is not valid !";
		return false;
	}

	if (!QueryResult.has_affected_rows())
	{
		Out_Code = "There is no affected rows for this query !";
		return false;
	}

	AffectedRows = QueryResult.affected_rows();
	return true;
}

int32 UNANODBC_Result::GetColumnsCount(FString& Out_Code)
{
	if (!this->QueryResult)
	{
		Out_Code = "Query result is not valid !";
		return 0;
	}

	return this->QueryResult.columns();
}

int32 UNANODBC_Result::GetRowsCount(FString& Out_Code)
{
	if (this->All_Data.IsEmpty())
	{
		Out_Code = "There is no data !";
		return 0;
	}
	
	return this ->RowsCount;
}

bool UNANODBC_Result::GetMetaData(FString& Out_Code, TArray<FNANODBC_MetaData>& Out_MetaData)
{
	if (!this->QueryResult)
	{
		Out_Code = "Query result is not valid !";
		return false;
	}

	TArray<FNANODBC_MetaData> Temp_MetaData;

	try
	{
		const int32 ColumnsCount = this->QueryResult.columns();

		for (int32 Index_Columns = 0; Index_Columns < ColumnsCount; Index_Columns++)
		{
			const nanodbc::string ColumnName = this->QueryResult.column_name(Index_Columns);

			FNANODBC_MetaData MetaData;
			MetaData.ColumnName = UTF8_TO_TCHAR(ColumnName.c_str());
			MetaData.ColumnNumber = this->QueryResult.column(ColumnName);
			MetaData.ColumnType = this->QueryResult.column_datatype(Index_Columns);
			MetaData.ColumnTypeName = UTF8_TO_TCHAR(this->QueryResult.column_datatype_name(Index_Columns).c_str());
			MetaData.ColumnDecimalDigit = this->QueryResult.column_decimal_digits(Index_Columns);
			MetaData.ColumnSize = this->QueryResult.column_size(Index_Columns);

			Temp_MetaData.Add(MetaData);
		}

		Out_MetaData = Temp_MetaData;
	}

	catch (const std::exception& Exception)
	{
		Out_Code = Exception.what();
		return false;
	}

	return true;
}

bool UNANODBC_Result::GetDataFromRow(TArray<FNANODBC_DataValue>& Out_Value, const int32 RowIndex)
{
	if (this->All_Data.IsEmpty())
	{
		return false;
	}

	if (RowIndex < 0 && this->All_Data.Num() >= RowIndex)
	{
		return false;
	}
	
	Out_Value = this->All_Data[RowIndex];
	return true;
}

bool UNANODBC_Result::GetDataFromColumnIndex(TArray<FNANODBC_DataValue>& Out_Value, const int32 ColumnIndex)
{
	if (this->All_Data.IsEmpty())
	{
		return false;
	}

	TArray<FNANODBC_DataValue> TempArray;

	for (TArray<FNANODBC_DataValue> EachRow : this->All_Data)
	{
		if (ColumnIndex >= 0 && ColumnIndex < EachRow.Num())
		{
			TempArray.Add(EachRow[ColumnIndex]);
		}
	}

	Out_Value = TempArray;
	return false;
}

bool UNANODBC_Result::GetDataFromColumnName(TArray<FNANODBC_DataValue>& Out_Value, FString ColumnName)
{
	if (this->All_Data.IsEmpty())
	{
		return false;
	}

	TArray<FNANODBC_DataValue> TempArray;
	const int ColumnIndex = this->QueryResult.column(NANODBC_TEXT(TCHAR_TO_UTF8(*ColumnName)));
	
	for (TArray<FNANODBC_DataValue> EachRow : this->All_Data)
	{
		if (ColumnIndex >= 0 && ColumnIndex < EachRow.Num())
		{
			TempArray.Add(EachRow[ColumnIndex]);
		}
	}

	Out_Value = TempArray;
	return false;
}