#include "NANODBC_Connection.h"

// UE Includes.
#include "Kismet/KismetMathLibrary.h"

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

	int32 Index_Row = 0;
	TMap<FVector2D, FNANODBC_DataValue> Temp_Data;

	try
	{
		In_Result.unbind();
		const int32 ColumnCount = In_Result.columns();

		while (In_Result.next())
		{
			for (int32 Index_Column = 0; Index_Column < ColumnCount; Index_Column++)
			{
				const int32 DataType = In_Result.column_datatype(Index_Column);
				const FString DataTypeName = UTF8_TO_TCHAR(In_Result.column_datatype_name(Index_Column).c_str());
				const FString ColumnName = UTF8_TO_TCHAR(In_Result.column_name(Index_Column).c_str());

				FNANODBC_DataValue EachData;
				EachData.DataType = DataType;
				EachData.DataTypeName = DataTypeName;
				EachData.ColumnName = ColumnName;

				switch (DataType)
				{
					case -9:
					{
						// MSSQL's "time" dateType is an nvarchar, too.

						EachData.ValString = UTF8_TO_TCHAR(In_Result.get<nanodbc::string>(Index_Column).c_str());
						EachData.ValueRepresentation = EachData.ValString;

						break;
					}

					case -2:
					{
						nanodbc::timestamp TimeStamp = In_Result.get<nanodbc::timestamp>(Index_Column);
						
						FDateTime DateTime = UKismetMathLibrary::MakeDateTime(TimeStamp.year, TimeStamp.month, TimeStamp.day, TimeStamp.hour, TimeStamp.min, TimeStamp.sec, TimeStamp.fract);
						EachData.ValDateTime = DateTime;
						EachData.ValueRepresentation = DateTime.ToString();
					}
					
					case -1:
					{
						EachData.ValString = UTF8_TO_TCHAR(In_Result.get<nanodbc::string>(Index_Column).c_str());
						EachData.ValueRepresentation = EachData.ValString;

						break;
					}

					case 4:
					{
						EachData.ValInt32 = In_Result.get<int>(Index_Column);
						EachData.ValueRepresentation = FString::FromInt(EachData.ValInt32);

						break;
					}
				
					case 6:
					{
						EachData.ValFloat = In_Result.get<float>(Index_Column);
						EachData.ValueRepresentation = FString::SanitizeFloat(EachData.ValFloat);

						break;
					}
						
					case 93:
					{
						nanodbc::date Section_Date = In_Result.get<nanodbc::date>(Index_Column);
						nanodbc::time Section_Time = In_Result.get<nanodbc::time>(Index_Column);
						
						FDateTime DateTime = UKismetMathLibrary::MakeDateTime(Section_Date.year, Section_Date.month, Section_Date.day, Section_Time.hour, Section_Time.min, Section_Time.sec);
						EachData.ValDateTime = DateTime;
						EachData.ValueRepresentation = DateTime.ToString();

						break;
					}

					default:
					{
						break;
					}
				}

				Temp_Data.Add(FVector2D(Index_Row, Index_Column), EachData);
			}

			Index_Row += 1;
		}
	}

	catch (const std::exception& Exception)
	{
		FString Out_Code = Exception.what();
	}

	this->QueryResult = In_Result;
	this->All_Data = Temp_Data;
	this->RowsCount = Index_Row;
	
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
	
	return this->RowsCount;
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

bool UNANODBC_Result::GetDataFromRow(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, const int32 RowIndex)
{
	if (this->All_Data.IsEmpty())
	{
		Out_Code = "Data pool is empty !";
		return false;
	}

	if (RowIndex < 0 && this->RowsCount >= RowIndex)
	{
		Out_Code = "Given row index is out of data pool's range !";
		return false;
	}
	
	const int32 ColumnsCount = this->QueryResult.columns();
	TArray<FNANODBC_DataValue> Temp_Array;

	for (int32 Index_Column = 0; Index_Column < ColumnsCount; Index_Column++)
	{
		Temp_Array.Add(*this->All_Data.Find(FVector2D(RowIndex, Index_Column)));
	}

	Out_Value = Temp_Array;
	return true;
}

bool UNANODBC_Result::GetDataFromColumnIndex(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, const int32 ColumnIndex)
{
	if (this->All_Data.IsEmpty())
	{
		Out_Code = "Data pool is empty !";
		return false;
	}

	TArray<FNANODBC_DataValue> Temp_Array;

	for (int32 Index_Row = 0; Index_Row < this->RowsCount; Index_Row++)
	{
		Temp_Array.Add(*this->All_Data.Find(FVector2D(Index_Row, ColumnIndex)));
	}

	Out_Value = Temp_Array;
	return true;
}

bool UNANODBC_Result::GetDataFromColumnName(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, FString ColumnName)
{
	if (this->All_Data.IsEmpty())
	{
		Out_Code = "Data pool is empty !";
		return false;
	}

	const int ColumnIndex = this->QueryResult.column(NANODBC_TEXT(TCHAR_TO_UTF8(*ColumnName)));
	TArray<FNANODBC_DataValue> Temp_Array;

	for (int32 Index_Row = 0; Index_Row < this->RowsCount; Index_Row++)
	{
		Temp_Array.Add(*this->All_Data.Find(FVector2D(Index_Row, ColumnIndex)));
	}

	Out_Value = Temp_Array;
	return true;
}

bool UNANODBC_Result::GetSingleData(FString& Out_Code, FNANODBC_DataValue& Out_Value, FVector2D Position)
{
	if (this->All_Data.IsEmpty())
	{
		Out_Code = "Data pool is empty !";
		return false;
	}

	FNANODBC_DataValue* TempValue = this->All_Data.Find(Position);

	if (TempValue)
	{
		Out_Value = *TempValue;
		return true;
	}

	else
	{
		return false;
	}
}