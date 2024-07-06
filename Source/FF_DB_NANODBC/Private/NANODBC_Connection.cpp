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
	return Out_Result->SetQueryResult(Out_Code, QueryResult);
}

// RESULT.

bool UNANODBC_Result::SetQueryResult(FString& Out_Code, result In_Result)
{
	if (!In_Result)
	{
		return false;
	}

	this->QueryResult = In_Result;
	const int32 ColumnCount = In_Result.columns();

	if (In_Result.has_affected_rows() && ColumnCount == 0)
	{
		Out_Code = "NANODBC : This query is only for updating data. There is no result to show !";
		return true;
	}

	int32 Index_Row = 0;
	TMap<FVector2D, FNANODBC_DataValue> Temp_Data;

	try
	{
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
					// NVARCHAR & DATE & TIME
					case -9:
					{
						EachData.String = UTF8_TO_TCHAR(In_Result.get<nanodbc::string>(Index_Column).c_str());
						EachData.Preview = EachData.String;
						break;
					}

					// INT64 & BIGINT
					case -5:
					{
						EachData.Integer64 = In_Result.get<long long int>(Index_Column);
						EachData.Preview = FString::FromInt(EachData.Integer64);
					}

					// TIMESTAMP: nanodbc::timestamp is not SQL timestamp. We use it to check if rows changed since last retriving or not.
					case -2:
					{
						std::vector<uint8_t> TempData = In_Result.get<std::vector<std::uint8_t>>(Index_Column);
						
						std::stringstream StringStream;
						for (auto&& EachByte : TempData)
						{
							StringStream << std::hex << static_cast<int>(EachByte);
						}
						
						/*
						for (std::size_t Index_Bytes = 0; Index_Bytes < TempData.size(); Index_Bytes++)
						{
							if (Index_Bytes != 0)
							{
								StringStream << "";
							}

							StringStream << std::hex << static_cast<int>(TempData[Index_Bytes]);
						}
						*/

						unsigned int TimeStampInt = std::stoul(StringStream.str(), nullptr, 16);
						FString TimeStampString = UTF8_TO_TCHAR(StringStream.str().c_str());

						EachData.Integer64 = TimeStampInt;
						EachData.Preview = TimeStampString + " - " + FString::FromInt(TimeStampInt);
						break;
					}

					// TEXT
					case -1:
					{
						EachData.String = UTF8_TO_TCHAR(In_Result.get<nanodbc::string>(Index_Column).c_str());
						EachData.Preview = EachData.String;
						break;
					}

					// INT32
					case 4:
					{
						EachData.Integer32 = In_Result.get<int>(Index_Column);
						EachData.Preview = FString::FromInt(EachData.Integer32);
						break;
					}

					// FLOAT & DOUBLE
					case 6:
					{
						EachData.Double = In_Result.get<double>(Index_Column);
						EachData.Preview = FString::SanitizeFloat(EachData.Double);
						break;
					}

					// DATETIME
					case 93:
					{
						nanodbc::timestamp Raw_TimeStamp = In_Result.get<nanodbc::timestamp>(Index_Column);

						int32 Year = Raw_TimeStamp.year;
						int32 Month = Raw_TimeStamp.month;
						int32 Day = Raw_TimeStamp.day;
						int32 Hours = Raw_TimeStamp.hour;
						int32 Minutes = Raw_TimeStamp.min;
						int32 Seconds = Raw_TimeStamp.sec;

						// We need only first 3 digits.
						int32 Milliseconds = Raw_TimeStamp.fract / 1000000;

						FDateTime DateTime = FDateTime(Year, Month, Day, Hours, Minutes, Seconds, Milliseconds);
						EachData.DateTime = DateTime;
						EachData.Preview = FString::Printf(TEXT("%d-&d-%d %d:%d:%d:%d"), Year, Month, Day, Hours, Minutes, Seconds, Milliseconds);

						break;
					}

					default:
					{
						EachData.Note = "Currently there is no parser for this data type. Please convert it to another known type in your query !";
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
		FString ExceptionString = Exception.what();

		if (ExceptionString.Contains("invalid descriptor index"))
		{
			ExceptionString += " -> Please update your query like putting columns with long data (for example: text and nvarchar) at the end. This is an ODBC limitation.";
		}

		Out_Code = ExceptionString;

		return false;
	}

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