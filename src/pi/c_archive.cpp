#include "c_archive.h"

auto c_archive::GetArchiveErrorString() -> string
{
	return	"libarchive: errno="s + to_string(archive_errno(__archive)) + ", error_string=" + archive_error_string(__archive);
}

auto c_archive::ArchiveOpen() -> string
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(__archive)
	{
		// --- don't initialize again
		MESSAGE_DEBUG("", "", "archive already initialized");
	}
	else
	{
		if((__archive = archive_write_new()) == NULL)
		{
			error_message = "archive: fail to initialize archive";
			MESSAGE_ERROR("", "", "fail to initialize archive(" + GetArchiveErrorString() + ")");
		}
		else
		{
			if(archive_write_set_format_zip(__archive) == ARCHIVE_OK)
			{
				if(archive_write_open_filename(__archive, GetFilename().c_str()) == ARCHIVE_OK)
				{
				}
				else
				{
					error_message = "archive: fail to open file for writing";
					MESSAGE_ERROR("", "", "fail to open archive(" + GetFilename() + ") for writing(" + GetArchiveErrorString() + ")");
				}
			}
			else
			{
				error_message = "archive: fail to set zip format";
				MESSAGE_ERROR("", "", "fail to set zip format(" + GetArchiveErrorString() + ")");
			}
		}
	}

	MESSAGE_DEBUG("", "", "finish (error_message length is " + to_string(error_message.length()) + ")");

	return error_message;
}

auto c_archive::AddFileToArchive(string absolute_path, string archive_path) -> string
{
	struct stat st;
	auto		error_message = ""s;

	MESSAGE_DEBUG("", "", "start (" + absolute_path + ", " + archive_path + ")");

    if(stat(absolute_path.c_str(), &st) == 0)
    {
    	struct archive_entry *entry = NULL;

		if((entry = archive_entry_new()) != NULL)
		{
			int	fd;

			// archive_entry_set_pathname(entry, archive_path.c_str());
			archive_entry_set_pathname(entry, archive_path.c_str());
			archive_entry_set_size(entry, st.st_size); // Note 3
			archive_entry_set_filetype(entry, AE_IFREG);
			archive_entry_set_perm(entry, 0644);
			archive_write_header(__archive, entry);
			if((fd = open(absolute_path.c_str(), O_RDONLY)) == -1)
			{
				error_message = "can't read archived file";
				MESSAGE_ERROR("", "", "can't read file(" + absolute_path + ") to add to archive");
			}
			else
			{
				char *	buff[16384];
				auto	len = read(fd, buff, sizeof(buff));

				while ( len > 0 ) {
					archive_write_data(__archive, buff, len);
					len = read(fd, buff, sizeof(buff));
				}
				close(fd);
			}

			archive_entry_free(entry);
		}
		else
		{
			error_message = "fail to allocate memory";
			MESSAGE_ERROR("", "", "fail to create archive_entry_object (" + GetArchiveErrorString() + ")");
		}
    }
    else
    {
    	error_message = "can't read archived file";
    }

	MESSAGE_DEBUG("", "", "finish (" + absolute_path + ") (error_message length is " + to_string(error_message.length()) + ")");

	return error_message;
}

auto c_archive::AddFolderToArchive(string dirname, string folder_in_archive) -> string
{
	DIR		*dir;
	struct	dirent *entry;
	char	path[4096];
	char	path_in_archive[4096];
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start (" + dirname + ", " + folder_in_archive + ")");
	
	dir = opendir(dirname.c_str());

	if(dir == NULL) 
	{
		MESSAGE_ERROR("", "", "fail to open dir("s + dirname + ")")
		error_message = "fail to open folder";
	}
	else
	{
		while ((entry = readdir(dir)) != NULL)
		{
			if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, "..")) 
			{
				snprintf(path, 				(size_t) 4095, "%s%s", dirname.c_str(), entry->d_name);
				snprintf(path_in_archive,	(size_t) 4095, "%s%s", folder_in_archive.c_str(), entry->d_name);

				if(entry->d_type == DT_DIR)
				{
					if((error_message = AddFolderToArchive(string(path) + "/", string(path_in_archive) + "/")).empty())
					{
						// --- everything is fine
					}
					else
					{
						MESSAGE_ERROR("", "", "fail to add folder(" + path + ") to archive");
						break;
					}
				} 
				else
				{
					if((error_message = AddFileToArchive(path, path_in_archive)).empty())
					{
						// --- everything is fine
					}
					else
					{
						MESSAGE_ERROR("", "", "fail to add file(" + path + ") to archive");
						break;
					}
				}
			}
		}
		
		closedir(dir);
	}
	
	MESSAGE_DEBUG("", "", "finish (" + dirname + ") (error_message length is " + to_string(error_message.length()) + ")");

	return error_message;
}

auto c_archive::Archive() -> string
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(GetFilename().length())
	{
		if(GetFolderToArchive().length())
		{
			if((error_message = ArchiveOpen()).empty())
			{
				if((error_message = AddFolderToArchive(GetFolderToArchive(), "")).empty())
				{

				}
				else
				{
					MESSAGE_ERROR("", "", "can't add folder(" + GetFolderToArchive() + ") to archive");
				}
			}
			else
			{
				MESSAGE_ERROR("", "", "fail to initialize archive")
			}
		}
		else
		{
			MESSAGE_ERROR("", "", "set folder to archive prior to invoke Archive()")
			error_message = gettext("can't create archive");
		}
	}
	else
	{
		MESSAGE_ERROR("", "", "set archive filename prior to invoke Archive()")
		error_message = gettext("can't create archive");
	}

	MESSAGE_DEBUG("", "", "finish (error_message length is " + to_string(error_message.length()) + ")");

	return error_message;
}

auto c_archive::ArchiveClose() -> string
{
	auto	error_message = ""s;

	MESSAGE_DEBUG("", "", "start");

	if(__archive)
	{
		archive_write_close(__archive);
		if(archive_write_free(__archive) == ARCHIVE_OK) {}
		else
		{
			MESSAGE_ERROR("", "", "fail close archive(" + GetArchiveErrorString() + ")");
			error_message = gettext("can't close archive");
		}
		__archive = NULL;
	}
	else
	{
		MESSAGE_DEBUG("", "", "no need to close");
	}

	MESSAGE_DEBUG("", "", "finish (error_message length is " + to_string(error_message.length()) + ")");

	return error_message;
}

c_archive::~c_archive()
{
	ArchiveClose();
}
