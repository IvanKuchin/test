#include "cmysql.h"
#include "cexception.h"

int CMysqlSkel::InitDB(const string &dbName, const string &user, const string &pass, const string &host)
{
    MESSAGE_DEBUG("", "", "start (db/login: " + dbName + "/" + user + ")");

    db = mysql_init( NULL );
    if( db == NULL )
    {
        MESSAGE_ERROR("", "", "failed to allocate memory when init MYSQL");
        return(-1);
    }

// --- that ma fix 

    db = mysql_real_connect(db, host.c_str(), user.c_str(), pass.c_str(), dbName.c_str(), 0, NULL, CLIENT_MULTI_STATEMENTS | CLIENT_MULTI_RESULTS);

    if( db == NULL )
    {
        MESSAGE_ERROR("", "", "Failed to connect to MYQSL server (mysql_real_connect return NULL)");
        return(-1);
    }

    MESSAGE_DEBUG("", "", "finish");

    return(0);
}

void CMysqlSkel::CloseDB( void )
{
    MESSAGE_DEBUG("", "", "start");

    if( db ) mysql_close( db );

    MESSAGE_DEBUG("", "", "finish");
}

// --- Run Insert query only with autoincrement field
// --- Input: string
// --- Output: 0 - error or no AUTO_INCREMENT_FIELD
//              >0 - success
unsigned long CMysqlSkel::InsertQueryDB(const string &query)
{
    int             r;
    my_ulonglong    resultAutoIncrement = 0;

    MESSAGE_DEBUG("", "", "start (query [" + query + "])");

    r = mysql_query( db, query.c_str());
    if( r )
    {
        MESSAGE_ERROR("", "", "failed insert query (" + query + ") " + mysql_error(db));
    }
    else
    {
        if (mysql_store_result(db) == 0 &&
            mysql_field_count(db) == 0 &&
            mysql_insert_id(db) != 0)
        {
            resultAutoIncrement = mysql_insert_id(db);
        }
        else
        {
            MESSAGE_ERROR("", "", "failed insert query " + query + ": (returned > 0 cols) or (resultSet != NULL) or (table missing AUTO_INCREMENT_FIELD)");
        }
    }

    MESSAGE_DEBUG("", "", "finish (return [" + to_string((unsigned long)resultAutoIncrement) + "])");

    return (unsigned long)resultAutoIncrement;
}

// --- Run MySQL query
// --- Input: string
// --- Output:     NULL - resultSet is empty OR error
//             not NULL - resultSet
// --- Remark: no error indication
MYSQL_RES *CMysqlSkel::QueryDB(const string &query)
{
    MYSQL_RES   *result;
    int         r;

    MESSAGE_DEBUG("", "", "start (query [" + query + "])");

    r = mysql_query( db, query.c_str());
    if( r )
    {
        MESSAGE_ERROR("", "", "failed query (" + query + ") " + mysql_error(db));

        return NULL;
    }
    else
    {
    	result = mysql_store_result( db );
    	numRows = mysql_affected_rows( db );
    	if( numRows == 0 )
    	{
        	if( result ) mysql_free_result( result );
    	    result = NULL;
    	}
    	else
    	{
    	    if(result)
    	    {
        		numFields = mysql_num_fields( result );
        		fieldsInfo = mysql_fetch_fields( result );
    	    }
    	    else
    	    {
        		numFields = 0;
        		result = NULL;
    	    }
    	}
    }
    return(result);
}

bool CMysqlSkel::isError(void)
{
    return strlen(mysql_error(db));
}

const char *CMysqlSkel::GetErrorMessage(void)
{
    return mysql_error(db);
}

int CMysqlSkel::FieldsIndex(const char *fieldName )
{
    for(auto i = 0u; i < numFields; i++ )
    	if( strcmp( fieldName, fieldsInfo[i].name ) == 0 )
            return(i);
    return(-1);
}

auto CMysqlSkel::GetColName(unsigned int idx) -> string
{
    auto    result = ""s;

    if(idx < numFields) result = fieldsInfo[idx].name;

    return result;
}

void CMysqlSkel::FreeResultSet()
{
    if( resultSet )
    {
    	mysql_free_result( resultSet );
    	resultSet = NULL;
    }
}

char *CMysqlSkel::ResultValue( MYSQL_RES *result, unsigned int row, int fi )
{
    MYSQL_ROW       fr;

    if(fi < 0)
    {
        MESSAGE_ERROR("", "", "field index can't be negative");

        throw CException("error db");
    }

    mysql_data_seek(result, row);
    fr = mysql_fetch_row(result);

    if(fr)
    {
        if(!fr[fi])
        {
            MESSAGE_ERROR("", "", "mysql result value(row " + to_string(row) + ", index " + to_string(fi) + ") is NULL");
        }
        return(fr[fi] ? fr[fi] : (char *)"");
    }
    else
    {
        MESSAGE_ERROR("", "", "mysql_fetch_row( " + to_string(row) + " ) returned NULL")

        return(NULL);
    }
}

// --- throw_exception: if true then Exception will be thrown in case of "field" not found
// --- if not sure what function to use, then avoid use _with_NULL. This will keep your code safe 
char *CMysqlSkel::__ResultValue( MYSQL_RES *result, unsigned int row, const char *name, bool throw_exception )
{
    auto            fi = FieldsIndex( name );

    if(fi < 0)
    {
        if(throw_exception)
        {
            MESSAGE_ERROR("", "", "DB-field [" + name + "] doesn't exists");

            throw CException("error db");
        }
        else
        {
            MESSAGE_DEBUG("", "", "DB-field [" + name + "] has NULL value");

            return (char *)"";
        }
    }

    return ResultValue(result, row, fi);
}

char *CMysqlSkel::ResultValue_with_NULL( MYSQL_RES *result, unsigned int row, const char *name )
{
    return __ResultValue(result, row, name, false);
}

char *CMysqlSkel::ResultValue( MYSQL_RES *result, unsigned int row, const char *name )
{
    return __ResultValue(result, row, name, true);
}

MYSQL_ROW CMysqlSkel::NextFetch(MYSQL_RES *result)
{
	currentRow = mysql_fetch_row(result);
	if(!currentRow)
	{
        MESSAGE_ERROR("", "", "fail calling mysql_fetch_row()");
        return NULL;
	}
	return currentRow;
}

char *CMysqlSkel::ResultValueFast(unsigned int fi)
{
	if(currentRow)
    	return(currentRow[fi]);
	else
	{
        MESSAGE_ERROR("", "", "fail extracting info from currentRow( " + to_string(fi) + " )");
    	return(NULL);
	}
}

int CMysqlSkel::ResultRows( MYSQL_RES *result )
{
    return numRows;
}



CMysql::CMysql()
{
    resultSet = NULL;
}

int CMysql::Connect(const string &dbName, const string &login, const string &pass, const string &host)
{
    auto    result = InitDB(dbName, login, pass, host);

    if(result == 0)
    {
        // --- successful init
        Query("set names " + DB_CHARSET);
    }

    return result;
}

int CMysql::Connect()
{
    c_config    config;
    auto        credentials = config.Read({"DB_NAME"s, "DB_LOGIN"s, "DB_PASSWORD"s, "DB_HOST"s});
    auto        valid_cred  = (credentials.size() ? true : false);
    auto        db_name     = (valid_cred ? credentials["DB_NAME"]      : __DB_FALLBACK_NAME);
    auto        db_login    = (valid_cred ? credentials["DB_LOGIN"]     : __DB_FALLBACK_LOGIN);
    auto        db_pass     = (valid_cred ? credentials["DB_PASSWORD"]  : __DB_FALLBACK_PASSWORD);
    auto        db_host     = (valid_cred ? credentials["DB_HOST"]      : __DB_FALLBACK_HOST);

    if(!valid_cred)
    {
        MESSAGE_ERROR("", "", "no valid DB credentials found in the config file. Fallback credentials are used. This only acceptable for CI/CD workflow. Hardcoded credentials could be serious security flaw.");
    }

    return Connect(db_name, db_login, db_pass, db_host);
}

int CMysql::Query(const string &query)
{
    FreeResultSet();
    resultSet = QueryDB(query);
    if(resultSet != NULL) {
       return AffectedRows();
    }
    return 0;
}

unsigned long CMysql::InsertQuery(const string &query)
{
    FreeResultSet();
    return InsertQueryDB(query);
}

string CMysql::Get_with_NULL(int row, const string &name)
{
    if(resultSet)
        return ResultValue_with_NULL(resultSet, row, name.c_str());
    return(NULL);
}

string CMysql::Get(int row, const string &name)
{
    if(resultSet)
        return ResultValue(resultSet, row, name.c_str());
    return(NULL);
}

string CMysql::Get(int row, int col)
{
    if(resultSet)
        return ResultValue(resultSet, row, col);
    return(NULL);
}

MYSQL_ROW CMysql::NextRow(void)
{
    return NextFetch(resultSet);
}

char *CMysql::GetFast(int index)
{
    return ResultValueFast(index);
}


int CMysql::CountFields()
{
    return numFields;
}

int CMysql::AffectedRows()
{
    return numRows;
}

CMysql::~CMysql()
{
    FreeResultSet();
    CloseDB();
}

