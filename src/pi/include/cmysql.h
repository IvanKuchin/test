#ifndef __CMYSQL__H__
#define	__CMYSQL__H__

#include <mysql.h>
#include <string>
#include <map>
#include <vector>

using namespace std;

#include "c_config.h"
#include "clog.h"

class CMysqlSkel
{
	private:
		MYSQL_ROW       currentRow;

		char*			__ResultValue( MYSQL_RES *result, unsigned int row, const char *name, bool throw_exception );

	protected:
		MYSQL			*db;
		MYSQL_RES		*resultSet;
		MYSQL_FIELD		*fieldsInfo;
		unsigned int	numRows;
		unsigned int	numFields;

		int				FieldsIndex(const char *fieldName);
		MYSQL_ROW		NextFetch(MYSQL_RES *result);
		char*			ResultValueFast(unsigned int fi);

		int				InitDB(const string &dbName, const string &user, const string &pass, const string &host);

	public:
		struct			MysqlError {};

						CMysqlSkel( void ): db(NULL), fieldsInfo(NULL), numRows(0), numFields(0) {};
		void			CloseDB					( void );
		MYSQL_RES*		QueryDB					(const string &query );
		unsigned long	InsertQueryDB			(const string &query );

		char*			ResultValue				( MYSQL_RES *result, unsigned int row, int fi );
		char*			ResultValue				( MYSQL_RES *result, unsigned int row, const char *name );
		char*			ResultValue_with_NULL	( MYSQL_RES *result, unsigned int row, const char *name );

		int				ResultRows				( MYSQL_RES *result );
		void			FreeResultSet			();
		bool			isError					();
		const char *	GetErrorMessage			();
		auto			GetNumberOfCols			()	{ return numFields; };
		auto			GetColName				(unsigned int idx) -> string;

		virtual			~CMysqlSkel				() {};
};

class CMysql : public CMysqlSkel
{
	private:
		const	string			DB_CHARSET				= "utf8mb4"s;

		// --- this should only be used for CI/CD testing
		const	string			__DB_FALLBACK_NAME		= DB_FALLBACK_NAME;
		const	string			__DB_FALLBACK_LOGIN		= DB_FALLBACK_LOGIN;
		const	string			__DB_FALLBACK_PASSWORD	= DB_FALLBACK_PASSWORD;
		const	string			__DB_FALLBACK_HOST		= DB_FALLBACK_HOST;

		map<string, string>		GetCredentialsFromConfigFile(vector<string> param_list);

	public:
						CMysql();

		int				Connect();
		int				Connect(const string &dbname, const string &login, const string &pass, const string &host);
		int				Query(const string &query);
		unsigned long	InsertQuery(const string &query);
		
		string			Get(int rows, int col);
		string			Get(int rows, const string &field);
		string			Get_with_NULL(int rows, const string &field);

		MYSQL_ROW		NextRow(void);
		char*			GetFast(int col);
		int				CountFields();
		int				AffectedRows();

		virtual			~CMysql();
};

#endif
