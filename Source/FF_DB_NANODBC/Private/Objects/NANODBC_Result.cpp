#include "Objects/NANODBC_Result.h"

bool UNANODBC_Result::SetQueryResult(FString& Out_Code, result In_Result)
{
	if (!In_Result)
	{
		Out_Code = "FF DB NANODBC : Input result is not valid !";
		return false;
	}

	this->QueryResult = In_Result;
	this->Count_Column = In_Result.columns();
	this->Affected_Rows = In_Result.affected_rows();

	Out_Code = "FF DB NANODBC : Result set successfully";

	if (In_Result.has_affected_rows())
	{
		Out_Code += " but there is no result because this query is only for updating entries.";
	}

	return true;
}

bool UNANODBC_Result::Result_Record(FString& Out_Code)
{
	if (!this->QueryResult)
	{
		Out_Code = "FF DB NANODBC : Query result is not valid !";
		return false;
	}

	if (!this->QueryResult.has_affected_rows() && Count_Column == 0)
	{
		Out_Code = "FF DB NANODBC : There is no result to record because this query is only for updating entries.";
		return false;
	}

	int32 Index_Row = 0;
	TMap<FVector2D, FNANODBC_DataValue> Temp_Data;

	try
	{
		while (this->QueryResult.next())
		{
			for (int32 Index_Column = 0; Index_Column < this->Count_Column; Index_Column++)
			{
				const FVector2D Position = FVector2D(Index_Column, Index_Row);

				const int32 DataType = this->QueryResult.column_datatype(Index_Column);
				const FString DataTypeName = UTF8_TO_TCHAR(this->QueryResult.column_datatype_name(Index_Column).c_str());
				const FString ColumnName = UTF8_TO_TCHAR(this->QueryResult.column_name(Index_Column).c_str());

				FNANODBC_DataValue EachData;
				EachData.DataType = DataType;
				EachData.DataTypeName = DataTypeName;
				EachData.ColumnName = ColumnName;

				switch (DataType)
				{
					case -9:
					{
						// NVARCHAR & DATE & TIME

						EachData.String = UTF8_TO_TCHAR(this->QueryResult.get<nanodbc::string>(Index_Column).c_str());
						EachData.Preview = EachData.String;
						break;
					}

					case -5:
					{
						// INT64 & BIGINT

						EachData.Integer64 = this->QueryResult.get<long long int>(Index_Column);
						EachData.Preview = FString::FromInt(EachData.Integer64);
						break;
					}

					case -2:
					{
						// TIMESTAMP: nanodbc::timestamp is not SQL timestamp. We use it to check if rows changed since last retriving or not.

						std::vector<uint8_t> TempData = this->QueryResult.get<std::vector<std::uint8_t>>(Index_Column);

						std::stringstream StringStream;
						for (auto&& EachByte : TempData)
						{
							StringStream << std::hex << static_cast<int>(EachByte);
						}

						const std::string RawString = StringStream.str();
						const unsigned int TimeStampInt = std::stoul(RawString, nullptr, 16);
						const FString TimeStampString = UTF8_TO_TCHAR(RawString.c_str());

						EachData.Integer64 = TimeStampInt;
						EachData.Preview = TimeStampString + " - " + FString::FromInt(TimeStampInt);
						break;
					}

					case -1:
					{
						// TEXT

						EachData.String = UTF8_TO_TCHAR(this->QueryResult.get<nanodbc::string>(Index_Column).c_str());
						EachData.Preview = EachData.String;
						break;
					}

					case 4:
					{
						// INT32

						EachData.Integer32 = this->QueryResult.get<int>(Index_Column);
						EachData.Preview = FString::FromInt(EachData.Integer32);
						break;
					}

					case 6:
					{
						// FLOAT & DOUBLE

						EachData.Double = this->QueryResult.get<double>(Index_Column);
						EachData.Preview = FString::SanitizeFloat(EachData.Double);
						break;
					}

					case 93:
					{
						// DATETIME : nanodbc gives "9" as datatype of DateTime.

						nanodbc::timestamp Raw_TimeStamp = this->QueryResult.get<nanodbc::timestamp>(Index_Column);

						int32 Year = Raw_TimeStamp.year;
						int32 Month = Raw_TimeStamp.month;
						int32 Day = Raw_TimeStamp.day;
						int32 Hours = Raw_TimeStamp.hour;
						int32 Minutes = Raw_TimeStamp.min;
						int32 Seconds = Raw_TimeStamp.sec;

						// We need only first 3 digits.
						int32 Milliseconds = Raw_TimeStamp.fract / 1000000;

						EachData.DateTime = FDateTime(Year, Month, Day, Hours, Minutes, Seconds, Milliseconds);
						EachData.Preview = FString::Printf(TEXT("%d-%d-%d %d:%d:%d:%d"), Year, Month, Day, Hours, Minutes, Seconds, Milliseconds);
						break;
					}

					default:
					{
						EachData.Note = "Currently there is no parser for this data type. Please convert it to another known type in your query !";
						break;
					}
				}

				Temp_Data.Add(Position, EachData);
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

	this->Count_Row = Index_Row;

	Out_Code = "FF DB NANODBC : Result successfully recorded !";
	return true;
}

bool UNANODBC_Result::Result_Fetch(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Values, int32 Index_Column)
{
	if (!this->QueryResult)
	{
		Out_Code = "FF DB NANODBC : Query result is not valid !";
		return false;
	}

	if (!this->QueryResult.has_affected_rows() && Count_Column == 0)
	{
		Out_Code = "FF DB NANODBC : There is no result to record because this query is only for updating entries.";
		return false;
	}

	int32 Index_Row = 0;
	TArray<FNANODBC_DataValue> Temp_Data;

	try
	{
		while (this->QueryResult.next())
		{
			const int32 DataType = this->QueryResult.column_datatype(Index_Column);
			const FString DataTypeName = UTF8_TO_TCHAR(this->QueryResult.column_datatype_name(Index_Column).c_str());
			const FString ColumnName = UTF8_TO_TCHAR(this->QueryResult.column_name(Index_Column).c_str());

			FNANODBC_DataValue EachData;
			EachData.DataType = DataType;
			EachData.DataTypeName = DataTypeName;
			EachData.ColumnName = ColumnName;

			switch (DataType)
			{
				case -9:
				{
					// NVARCHAR & DATE & TIME

					EachData.String = UTF8_TO_TCHAR(this->QueryResult.get<nanodbc::string>(Index_Column).c_str());
					EachData.Preview = EachData.String;
					break;
				}

				case -5:
				{
					// INT64 & BIGINT

					EachData.Integer64 = this->QueryResult.get<long long int>(Index_Column);
					EachData.Preview = FString::FromInt(EachData.Integer64);
					break;
				}

				case -2:
				{
					// TIMESTAMP: nanodbc::timestamp is not SQL timestamp. We use it to check if rows changed since last retriving or not.

					std::vector<uint8_t> TempData = this->QueryResult.get<std::vector<std::uint8_t>>(Index_Column);

					std::stringstream StringStream;
					for (auto&& EachByte : TempData)
					{
						StringStream << std::hex << static_cast<int>(EachByte);
					}

					const std::string RawString = StringStream.str();
					const unsigned int TimeStampInt = std::stoul(RawString, nullptr, 16);
					const FString TimeStampString = UTF8_TO_TCHAR(RawString.c_str());

					EachData.Integer64 = TimeStampInt;
					EachData.Preview = TimeStampString + " - " + FString::FromInt(TimeStampInt);
					break;
				}

				case -1:
				{
					// TEXT

					EachData.String = UTF8_TO_TCHAR(this->QueryResult.get<nanodbc::string>(Index_Column).c_str());
					EachData.Preview = EachData.String;
					break;
				}

				case 4:
				{
					// INT32

					EachData.Integer32 = this->QueryResult.get<int>(Index_Column);
					EachData.Preview = FString::FromInt(EachData.Integer32);
					break;
				}

				case 6:
				{
					// FLOAT & DOUBLE

					EachData.Double = this->QueryResult.get<double>(Index_Column);
					EachData.Preview = FString::SanitizeFloat(EachData.Double);
					break;
				}

				case 93:
				{
					// DATETIME : nanodbc gives "9" as datatype of DateTime.

					nanodbc::timestamp Raw_TimeStamp = this->QueryResult.get<nanodbc::timestamp>(Index_Column);

					int32 Year = Raw_TimeStamp.year;
					int32 Month = Raw_TimeStamp.month;
					int32 Day = Raw_TimeStamp.day;
					int32 Hours = Raw_TimeStamp.hour;
					int32 Minutes = Raw_TimeStamp.min;
					int32 Seconds = Raw_TimeStamp.sec;

					// We need only first 3 digits.
					int32 Milliseconds = Raw_TimeStamp.fract / 1000000;

					EachData.DateTime = FDateTime(Year, Month, Day, Hours, Minutes, Seconds, Milliseconds);
					EachData.Preview = FString::Printf(TEXT("%d-%d-%d %d:%d:%d:%d"), Year, Month, Day, Hours, Minutes, Seconds, Milliseconds);
					break;
				}

				default:
				{
					EachData.Note = "Currently there is no parser for this data type. Please convert it to another known type in your query !";
					break;
				}
			}

			Temp_Data.Add(EachData);

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
	
	this->Count_Row = Index_Row;

	Out_Code = "FF DB NANODBC : Result successfully fetched !";
	return true;
}

int32 UNANODBC_Result::GetAffectedRows()
{
	return this->Affected_Rows;
}

int32 UNANODBC_Result::GetColumnsCount()
{
	return this->Count_Column;
}

int32 UNANODBC_Result::GetRowsCount()
{
	return this->Count_Row;
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

bool UNANODBC_Result::GetRow(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, const int32 Index_Row)
{
	if (this->Data_Pool.IsEmpty())
	{
		Out_Code = "FF DB NANODBC : Data pool is empty !";
		return false;
	}

	if (Index_Row < 0 || Index_Row >= this->Count_Row)
	{
		Out_Code = "FF DB NANODBC : Given row index is out of data pool's range !";
		return false;
	}

	TArray<FNANODBC_DataValue> Temp_Array;
	const int32 ColumnsCount = this->QueryResult.columns();

	for (int32 Index_Column = 0; Index_Column < ColumnsCount; Index_Column++)
	{
		const FVector2D Position = FVector2D(Index_Column, Index_Row);

		if (!this->Data_Pool.Contains(Position))
		{
			Out_Code = "FF DB NANODBC : Target position couldn't be found ! : " + Position.ToString();
			return false;
		}

		FNANODBC_DataValue* EachData = this->Data_Pool.Find(Position);

		if (!EachData)
		{
			Out_Code = "FF DB NANODBC : Found data is not valid : " + Position.ToString();
			return false;
		}

		Temp_Array.Add(*EachData);
	}

	Out_Value = Temp_Array;
	return true;
}

bool UNANODBC_Result::GetColumnFromIndex(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, const int32 Index_Column)
{
	if (this->Data_Pool.IsEmpty())
	{
		Out_Code = "Data pool is empty !";
		return false;
	}

	if (Index_Column < 0 || Index_Column >= this->Count_Column)
	{
		Out_Code = "FF DB SQLAPI : Given column index is out of data pool's range !";
		return false;
	}

	TArray<FNANODBC_DataValue> Temp_Array;

	for (int32 Index_Row = 0; Index_Row < this->Count_Row; Index_Row++)
	{
		const FVector2D Position = FVector2D(Index_Column, Index_Row);

		if (!this->Data_Pool.Contains(Position))
		{
			Out_Code = "FF DB NANODBC : Target position couldn't be found ! : " + Position.ToString();
			return false;
		}

		FNANODBC_DataValue* EachData = this->Data_Pool.Find(Position);

		if (!EachData)
		{
			Out_Code = "FF DB NANODBC : Found data is not valid : " + Position.ToString();
			return false;
		}

		Temp_Array.Add(*EachData);
	}

	Out_Value = Temp_Array;
	return true;
}

bool UNANODBC_Result::GetColumnFromName(FString& Out_Code, TArray<FNANODBC_DataValue>& Out_Value, FString ColumnName)
{
	if (this->Data_Pool.IsEmpty())
	{
		Out_Code = "Data pool is empty !";
		return false;
	}

	TArray<FNANODBC_DataValue> Temp_Array;
	const int Index_Column = this->QueryResult.column(NANODBC_TEXT(TCHAR_TO_UTF8(*ColumnName)));

	for (int32 Index_Row = 0; Index_Row < this->Count_Row; Index_Row++)
	{
		const FVector2D Position = FVector2D(Index_Column, Index_Row);

		if (!this->Data_Pool.Contains(Position))
		{
			Out_Code = "FF DB NANODBC : Target position couldn't be found ! : " + Position.ToString();
			return false;
		}

		FNANODBC_DataValue* EachData = this->Data_Pool.Find(Position);

		if (!EachData)
		{
			Out_Code = "FF DB NANODBC : Found data is not valid : " + Position.ToString();
			return false;
		}

		Temp_Array.Add(*EachData);
	}

	Out_Value = Temp_Array;
	return true;
}

bool UNANODBC_Result::GetSingleData(FString& Out_Code, FNANODBC_DataValue& Out_Value, FVector2D Position)
{
	if (this->Data_Pool.IsEmpty())
	{
		Out_Code = "FF DB NANODBC : Data pool is empty !";
		return false;
	}

	if (Position.X < 0 || Position.Y < 0 || Position.X >= this->Count_Column || Position.Y >= this->Count_Row)
	{
		Out_Code = "FF DB NANODBC : Given position is out of data pool's range !";
		return false;
	}

	if (!this->Data_Pool.Contains(Position))
	{
		Out_Code = "FF DB NANODBC : Target position couldn't be found ! : " + Position.ToString();
		return false;
	}

	FNANODBC_DataValue* DataValue = this->Data_Pool.Find(Position);

	if (!DataValue)
	{
		Out_Code = "FF DB NANODBC : Found data is not valid !" + Position.ToString();
		return false;
	}

	Out_Code = "FF DB NANODBC : Target Cell exported successfuly !";
	Out_Value = *DataValue;
	return true;
}