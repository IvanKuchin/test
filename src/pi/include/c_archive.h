#ifndef __C_ARCHIVE__H__
#define __C_ARCHIVE__H__

using namespace std;

#include <archive.h>
#include <archive_entry.h>
#include <fcntl.h>
#include <libintl.h>
#include <sys/stat.h>
#include <sys/types.h>	// --- DIR
#include <dirent.h>		// --- opendir, rmdir
#include "clog.h"

class c_archive
{
private:
	string					__archive_filename	= "";
	string					__folder_to_archive	= "";
	struct archive *		__archive			= nullptr;

	auto		ArchiveOpen() -> string;
	auto		ArchiveClose() -> string;
	auto		GetArchiveErrorString() -> string;
	auto 		AddFileToArchive(string absolute_path, string archive_path) -> string;
	auto 		AddFolderToArchive(string dirname, string folder_in_archive) -> string;

public:
	auto		SetFilename(const string &param)			{ __archive_filename = param; };
	auto		SetFilename(string &&param)					{ __archive_filename = move(param); };
	auto		SetFolderToArchive(const string &param)		{ __folder_to_archive = param; };
	auto		SetFolderToArchive(string &&param)			{ __folder_to_archive = move(param); };

	auto		GetFilename()								{ return __archive_filename; }
	auto		GetFolderToArchive()						{ return __folder_to_archive; }

	auto		Archive() -> string;

				~c_archive();
};

#endif

