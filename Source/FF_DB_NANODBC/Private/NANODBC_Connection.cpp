#include "NANODBC_Connection.h"

// CONNECTION.

bool UNANODBC_Connection::Connection_Start(FString& Out_Code, FString In_Server, FString In_UserName, FString In_Password, bool bUseAutoCommit)
{
	return false;
}

bool UNANODBC_Connection::Connection_Stop(FString& Out_Code)
{
	return false;
}

bool UNANODBC_Connection::PrepareStatement(FString& Out_Code, UNANODBC_Statement*& Out_Statement, FString SQL_Statement)
{
	return false;
}

// STATEMENT.

void UNANODBC_Statement::SetInt(int32 Value, int32 ParamIndex)
{
	return;
}

void UNANODBC_Statement::SetString(FString Value, int32 ParamIndex)
{

}

void UNANODBC_Statement::AddBatch()
{

}

void UNANODBC_Statement::ExecuteBatch(FString& Out_Code)
{

}

bool UNANODBC_Statement::CommitStatement(FString& Out_Code)
{
	return true;
}

bool UNANODBC_Statement::ExecuteQuery(FString& Out_Code, UNANODBC_Result*& Out_Result)
{
	return true;
}

// RESULT.

bool UNANODBC_Result::GetColumnCount(int32& ColumnCount)
{
	return true;
}

bool UNANODBC_Result::GetMetaDataStruct(FString& Out_Code, FNANODBC_MetaData& Out_MetaData, int32 ColumnIndex)
{
	/*
	odbc::ResultSetMetaData* Result = nullptr;

	try
	{
		Result = this->QueryResult->getMetaData().get();
	}

	catch (const std::exception& Exception)
	{
		Out_Code = Exception.what();
		return false;
	}

	FNANODBC_MetaData MetaData;
	MetaData.ColumnScale = Result->getScale(ColumnIndex);
	MetaData.ColumnPrecision = Result->getPrecision(ColumnIndex);

	MetaData.ColumnDisplaySize = Result->getColumnDisplaySize(ColumnIndex);
	MetaData.ColumnLenght = Result->getColumnLength(ColumnIndex);
	MetaData.ColumnOctetLenght = Result->getColumnOctetLength(ColumnIndex);

	MetaData.ColumnType = Result->getColumnType(ColumnIndex);
	MetaData.ColumnTypeName = UTF8_TO_TCHAR(Result->getColumnTypeName(ColumnIndex).c_str());

	MetaData.BaseColumnName = UTF8_TO_TCHAR(Result->getBaseColumnName(ColumnIndex).c_str());
	MetaData.ColumnName = UTF8_TO_TCHAR(Result->getColumnName(ColumnIndex).c_str());
	MetaData.ColumnLabel = UTF8_TO_TCHAR(Result->getColumnLabel(ColumnIndex).c_str());

	MetaData.BaseTableName = UTF8_TO_TCHAR(Result->getBaseTableName(ColumnIndex).c_str());
	MetaData.TableName = UTF8_TO_TCHAR(Result->getTableName(ColumnIndex).c_str());

	MetaData.CatalogName = UTF8_TO_TCHAR(Result->getCatalogName(ColumnIndex).c_str());

	MetaData.SchemaName = UTF8_TO_TCHAR(Result->getSchemaName(ColumnIndex).c_str());

	MetaData.bIsNullable = Result->isNullable(ColumnIndex);
	MetaData.bIsAutoIncrement = Result->isAutoIncrement(ColumnIndex);
	MetaData.bIsNamed = Result->isNamed(ColumnIndex);
	MetaData.bIsCaseSensitive = Result->isCaseSensitive(ColumnIndex);
	MetaData.bIsReadOnly = Result->isReadOnly(ColumnIndex);
	MetaData.bIsSearchable = Result->isSearchable(ColumnIndex);
	MetaData.bIsSigned = Result->isSigned(ColumnIndex);

	Out_MetaData = MetaData;
	*/
	return true;
}

bool UNANODBC_Result::GetString(FString& Out_Code, TArray<FString>& Out_String, int32 ColumnIndex)
{
	return false;
}

bool UNANODBC_Result::GetInt(FString& Out_Code, TArray<int32>& Out_String, int32 ColumnIndex)
{
	return false;
}