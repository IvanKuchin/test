#!/usr/bin/perl

use POSIX;
use File::Copy;
use File::Copy::Recursive qw(fcopy rcopy dircopy fmove rmove dirmove);
use File::Path;
use Cwd;
use strict;
my	($str, $key, $DEBUG, $outi, $command, $archive_filename);
# my	($mysqlhost, $mysqllogin, $mysqlpass, $mysqldb);
my	($action);
my	$timestamp = strftime "%Y%m%d%H%M%S", localtime;
my	$num_args = $#ARGV + 1;

my	$config_files		= ["archive.config.pl", "archive.folders_to_backup.pl", "archive.persistent_folders.pl"];


#
# unbuffer output
#
$|++;

$DEBUG = 0;

#
# usage archive.pl [OPT]
# --backup - full backup
# --backup_structure - structure backup
# --restore - restore from archive
# --restore_structure - restore structure only
#
$action = "";
if($num_args eq 1)
{
	if($ARGV[0] eq "--backup")							{ $action = "--backup"; }
	if($ARGV[0] eq "--backup_structure") 				{ $action = "--backup_structure"; }
	if($ARGV[0] eq "--restore_production")				{ $action = "--restore_production"; }
	if($ARGV[0] eq "--restore_production_structure")	{ $action = "--restore_production_structure"; }
	if($ARGV[0] eq "--restore_dev")						{ $action = "--restore_dev"; }
	if($ARGV[0] eq "--restore_dev_structure")			{ $action = "--restore_dev_structure"; }
	if($ARGV[0] eq "--restore_demo")					{ $action = "--restore_demo"; }
	if($ARGV[0] eq "--restore_demo_structure")			{ $action = "--restore_demo_structure"; }
}

if($action eq "")
{
	usage();
	die;
}

print "action = $action\n" if($DEBUG);


if(($action =~ /^--restore_production/))
{
	if(isDevServer())
	{
		die "this is dev-server\n";
	}
	else
	{
		print "Changing from DEV -> WWW in src files\n";

		ChangeDevToWWW($config_files);
		ChangeDevImagesToImages($config_files);
	}
}

if(($action =~ /^--restore_dev/))
{
	if(isDevServer())
	{
		print "Changing from WWW -> DEV in src files\n";

		ChangeWWWToDev($config_files);
		ChangeImagesToDevImages($config_files);
	}
	else
	{
		die "this is not dev-server\n";
	}
}

if(($action =~ /^--restore_demo/))
{
	if(isDevServer())
	{
		die "this is dev-server, demo must be run on production server\n";
	}
	else
	{
		print "Changing from DEV -> DEMO in src files\n";

		ChangeDevToDemo($config_files);
		ChangeDevImagesToDemoImages($config_files);
	}
}

# --- DO NOT move it higher than previous conditions
# --- condition workflow changing configuration files
my	%config				= do 'archive.config.pl';
my	%folders_to_backup	= do 'archive.folders_to_backup.pl';
my	%persistent_folders	= do 'archive.persistent_folders.pl';
my	$domainSuffix		= $config{project_domain};

if(CheckDBConnectivity() == 0)
{
	die "ERROR: DB connectivity can't be establish. To configure DB credentials use:\n mysql_config_editor set --login-path=".$config{login_path}." --host=____ --user=___ -p\n"
}

if($action =~ /^--restore/)
{
	isFilesReadyToRestore() || die "ERROR: not all files are exists in the restore folder\n";
	isRoot() 				|| die "ERROR: recover can be run with root privilege only\n";

	if(($action =~ /^--restore_production/))
	{
		if(isDevServer())
		{
			die "this is dev-server\n";
		}
		else
		{
			print "Changing DEV -> WWW in src files\n";

			ChangeDevToWWW($config{replace_dev_to_www_files});
			ChangeDevImagesToImages($config{replace_devimages_to_images_files});
		}
	}

	if(($action =~ /^--restore_dev/))
	{
		if(isDevServer())
		{
			print "Changing WWW -> DEV in src files\n";

			ChangeWWWToDev($config{replace_dev_to_www_files});
			ChangeImagesToDevImages($config{replace_devimages_to_images_files});
		}
		else
		{
			die "this is not dev-server\n";
		}
	}

	if(($action =~ /^--restore_demo/))
	{
		if(isDevServer())
		{
			die "this is dev-server, demo must be run on production server\n";
		}
		else
		{
			print "Changing DEV -> DEMO in src files\n";

			ChangeDevToDemo($config{replace_dev_to_www_files});
			ChangeDevImagesToDemoImages($config{replace_devimages_to_images_files});
		}
	}
}


# Build_FoldersToBackup_FromMakefile();

print "\tdomain is the $domainSuffix\n";
foreach(keys %folders_to_backup)
{
	print "\t$_ rendered to ".$folders_to_backup{$_}."\n";
}

if($DEBUG)
{
	foreach my $key2 (keys %persistent_folders) {
		print "folders keep unchanged: $key2 = ".$persistent_folders{$key2}."\n";
	}
}

# DefineDBCredentials();

#
# Build archive filename
#
$archive_filename  = $domainSuffix."_".($action eq "--backup" ? "full_" : "struct_").$timestamp.".tar.gz";

#
# Start main action
#
if($action =~ /^--backup/)
{
	my $system_err;

	print "\n\nPerforming ".($action eq "--backup" ? "full" : "structure")." backup\n";
	print "make clean...\n";
	$system_err = system("cd ".$config{project_binary_dir}." && make clean"); # --- save around 20MB per backup
	if($system_err) { print "\n\nmake clean\t[ERROR]\n"; }
	else			{ print "make clean\t[OK]\n"; }

	print "removing MaxMingDB...\t";
	unlink (glob($config{binary_maxmind_dir}.'/*')) and print "[OK]\n" or print "[ERROR]\n";

	print "removing old files (sql, *~, *.tar.gz, $domainSuffix.tar.gz)\n";
	unlink 'sql', 'sql_controltables', '$domainSuffix.tar.gz';
	unlink glob '*~';
	unlink glob '*.tar.gz';

	print "removing old dirs  ....\n";
	remove_dir_recursively($config{recovery_point_folder});
	Remove_Local_FoldersToBackup();

	print "copying production folders ...\n";
	Copy_Production_To_Local();

	if($action eq "--backup_structure")
	{
		print "removing persistent folder from backup env ...\n";
		Remove_Local_PersistentFolders();
	}

	Remove_CGI_From_Local_Folders();

	print "mysqldump ....\n";
	if($action =~ /_structure/)
	{
		system("mysqldump --login-path=".$config{login_path}." --default-character-set=utf8mb4 --no-data -Q $domainSuffix > sql");
		system("mysqldump --login-path=".$config{login_path}." --default-character-set=utf8mb4 --add-drop-table -Q $domainSuffix ".$config{tables_to_upgrade}." > sql_controltables");
	}
	else
	{
		system("mysqldump --login-path=".$config{login_path}." --default-character-set=utf8mb4 -Q $domainSuffix > sql");
	}

	print "archiving ....\n";
	$system_err = system("tar -czf ".$archive_filename." *");
	if($system_err) { print "\n\narchiving\t[ERROR]\n"; }
	else			{ print "archiving\t[OK]\n"; }

	print "copying ".$domainSuffix." environment ....\n";
	$system_err = system("cp ".$archive_filename." /storage/".$config{backup_local_username}."/backup/".$domainSuffix."/");
	if($system_err) { print "\n\copying\t[ERROR]\n"; }
	else			{ print "copying\t[OK]\n"; }

	if(isDevServer())
	{
		# if(isMDev())
		# {
		# 	print "copying ".$domainSuffix." environment ....\n";

		# 	system("cp ".$archive_filename." /storage/".$config{backup_local_username}."/backup/mdev.".$domainSuffix."/");
		# }
		# else
		# {
		# 	print "copying ".$domainSuffix." environment ....\n";

		# 	system("cp ".$archive_filename." /storage/".$config{backup_local_username}."/backup/dev.".$domainSuffix."/");
		# }

	}
	else
	{
		# if(isMobileVersion())
		# {
		# 	print "copying to backup location ....\n";

		# 	system("cp ".$archive_filename." /storage/".$config{backup_local_username}."/backup/m.".$domainSuffix."/");
		# }
		# else
		# {
		# 	print "copying to backup location ....\n";

		# 	system("cp ".$archive_filename." /storage/".$config{backup_local_username}."/backup/www.".$domainSuffix."/");

		# }


		print "rsync to remote server ....\n";
		$system_err = system("rsync -rptgoDzhe ssh --exclude '*.tar.gz' ./ ".$config{backup_remote_username}."\@".$config{backup_remote_hostname}.":/storage/".$config{backup_remote_username}."/backup/".$domainSuffix."/rsync/");
		if($system_err) { print "\n\nrsyncing\t[ERROR]\n"; }
		else			{ print "rsyncing\t[OK]\n"; }

		my @date = split(" ", localtime(time));
		if($date[0] eq "Sat") # --- today is Friday least busy day, do full backup
		{
			print "full backup to remote server ....\n";
			$system_err = system("scp ".$archive_filename." ".$config{backup_remote_hostname}.":/storage/".$config{backup_remote_username}."/backup/".$domainSuffix."/".$archive_filename."");
			if($system_err) { print "\n\nscp to remote server\t[ERROR]\n"; }
			else			{ print "scp to remote server\t[OK]\n"; }
		}
	}

	print "cleaning-up development folder ...\n";
	unlink $archive_filename;
	unlink 'sql';
	unlink 'sql_controltables';
	Remove_Local_FoldersToBackup();
	remove_dir_recursively("/home/".$config{build_username}."/home/"); # Sublime create this folder, when "browse remote files"


	if($action =~ /_structure/)
	{
		print "\n ---recover cli: \nsudo date && rm -rf ./tmp/recover/ && mkdir -p ./tmp/recover/ && cd tmp/recover/ && scp ".$config{backup_remote_hostname}.":/storage/".$config{backup_remote_username}."/backup/".$domainSuffix."/".$archive_filename." tmp/recover/ && tar -zxf ".$archive_filename." && time sudo ./archive.pl --restore_structure;\n\n";
	}
	else
	{
		print "\n ---recover cli: \nsudo date && rm -rf ./tmp/recover/ && mkdir -p ./tmp/recover/ && cd ./tmp/recover/ && cp /storage/".$config{backup_remote_username}."/backup/".$domainSuffix."/".$archive_filename." ./tmp/recover/ && tar -zxf ".$archive_filename." && time sudo ./archive.pl --restore;\n\n";
	}

	print "cmake project on local server...\n";
	$system_err = system("cd ".$config{project_binary_dir}." && cmake -DCMAKE_BUILD_TYPE=debug .."); # --- save around 20MB per backup
	if($system_err) { print "\n\nproject cmake\t[ERROR]\n"; }
	else			{ print "project cmake\t[OK]\n"; }
}

if($action =~ /^--restore/)
{
	print "\n\nPerforming ".($action =~ "_structure" ? "structure" : "full")." restore ($action)\n";

	#
	# clean-up from old garbage
	#
	print "removing old __tmp and recovery point dirs ...\n";

	remove_dir_recursively($config{recovery_point_folder});
	Remove_Local_Tmp_Folder();

	CleanBuildDirectory();

	#
	# create recovery point
	#
	print "creating 'recovery point'@".$config{recovery_point_folder}."\n";
	CreateRecoveryPoint($config{recovery_point_folder});


	if($action =~ /_structure/)
	{
		#
		# because of the hard naming in DATA dirs
		# for example: $dataDirs{"IMAGEAVATARDIR"} = "./html/images/avatars";
		#                                              ^^^^^^
		# we have to look for DATA-dirs in directory started from "./html", therefore:
		# 1) rename new directories with postfix _tmp
		# 2) copy existing dir's to ./html and ./cgi-bin
		# 3) copy DATA-dirs from ./html -> ./html_final
		# 4) rename back step (1)
		#

		print "moving local folders to ".$config{local_tmp_folder}.":\n";
		Dirmove_Local_To_LocalTmp();
		# print "\trenaming ./html -> ./html_final\n";
		# rename "./html","./html_final";
		# print "\trenaming ./cgi-bin -> ./cgi-bin_final\n";
		# rename "./cgi-bin","./cgi-bin_final";
		# print "\trenaming ".$backupImageDomainDir." -> ./images_final.".$domainSuffix."\n";
		# rename $backupImageDomainDir,"./images_final.".$domainSuffix;


		print "copying production folders to ./\n";
		Copy_Production_To_Local();
		system("find ./ -name *.cgi -exec rm -f {} \\;");
		system("find ./ -name *.o -exec rm -f {} \\;");

		#
		# create temporary dirs to extract from it CONST dirs
		# in our case all images: feed, avatars, chat, capchas, ...
		#
		# mkdir "./cgi-bin";
		# mkdir "./html";
		# print "\tcopying $cgidir -> ./cgi-bin\n";
		# dircopy($cgidir, "./cgi-bin");
		# print "\tcopying $htmldir -> ./html\n";
		# dircopy($htmldir, "./html");
		# system("find ./cgi-bin -name *.cgi -exec rm -f {} \\;");

	
		print "copying persistent folders to ".$config{local_tmp_folder}."\n";
		Copy_Local_To_LocalTmp_PersistentFolders();
		# foreach my $key (keys %dataDirs)
		# {
		# 	copy_data_dirs("./cgi-bin", "./cgi-bin_final", $dataDirs{$key});
		# 	copy_data_dirs("./html", "./html_final", $dataDirs{$key});
		# 	copy_data_dirs($backupImageDomainDir, "./images_final.".$domainSuffix, $dataDirs{$key});
		# }

		#
		# remove temporary dirs
		#
		print "removing temp folders\n";
		Remove_Local_FoldersToBackup();
		# remove_dir_recursively("./cgi-bin");
		# remove_dir_recursively("./html");
		# remove_dir_recursively($backupImageDomainDir);

	
		#
		# rename _tmp to original
		#
		print "moving original folders back\n";
		Dirmove_LocalTmp_To_Local();
		# print "\trenaming ./html_final -> ./html\n";
		# rename "./html_final","./html";
		# print "\trenaming ./cgi-bin_final -> ./cgi-bin\n";
		# rename "./cgi-bin_final","./cgi-bin";
		# print "\trenaming ./images_final.".$domainSuffix." -> ".$backupImageDomainDir."\n";
		# rename "./images_final.".$domainSuffix, $backupImageDomainDir;


		print "removing temporary folder\n";
		Remove_Local_Tmp_Folder();

		#
		# [IMPORTANT] supposed that sql file in current directory having only MySQL tables structure --no-data
		#
		print "adding MySQL production data to MySQL backup structure\n";
		system("mysqldump --login-path=".$config{login_path}." --default-character-set=utf8mb4 --no-create-info --skip-add-drop-table -Q $domainSuffix >> sql");
	}

	if(isDevServer())
	{
		# --- no need to activate .ht____ if it is develpment environment
	}
	else
	{
		#
		# rename all "..htaccess" to ".htaccess"
		# this trick used in development env to avoid headache with troubleshooting
		#
		print "activating .htaccess\n";
		Activate_Files(".htaccess");
		# activate_htaccess("./cgi-bin");
		# activate_htaccess("./html");
		# activate_htaccess($backupImageDomainDir);

		#
		# rename all "..htpasswd" to ".htpasswd"
		# this trick used in development env to avoid headache with troubleshooting
		#
		print "activating .htpasswd\n";
		Activate_Files(".htpasswd");
		# activate_htpasswd("./cgi-bin");
		# activate_htpasswd("./html");
		# activate_htpasswd($backupImageDomainDir);
	}

	print "\n\n--- SHUTDOWN PRODUCTION ENVIRONMENT ---\n\n";

	#
	# remove production Web-server folders
	#
	print "removing production folders\n";
	Remove_Production_Folders();

	print "restoring production DB from ./sql\n";
	system("mysql --login-path=".$config{login_path}." $domainSuffix < sql");
	if($action =~ /_structure/)
	{
		system("mysql --login-path=".$config{login_path}." $domainSuffix < sql_controltables");
	}

	print "restoring production folders\n";
	Dirmove_Local_To_Production();

	if(CompileAndInstall())
	{
		#
		# change ownership of html and cgi-bin folders
		#
		print "fix the owner and access rights ...\n";
		system("chown -R ".$config{build_username}.":".$config{build_username}." ./*");
		FixOwnerAndAccessRights();
	}
	else
	{
		print "[ERROR] during build stage\n";
	}
}

#
# No error checks in here.
# We must do our best to install as much as possible to recover production.
#
sub CompileAndInstall
{
	#
	# ATTENTION !!!
	# if you want to move compilation phase up , then RemoveProdFolders and DirmoveLocalToProduction 
	# has to be split to treat SRCDIR separately from HTMLDIR.
	# current workflow: 
	#	RemoveProdDir -> MoveAllDirsToProd -> build
	# new workflow: 
	#	RemoveProdSrcDir -> MoveSrcDirsToProd -> compile -> RemoveProdHTMLDir -> MoveHTMLDirsToProd -> build
	#
	system("cd ".$folders_to_backup{SRCDIR}."/build && cmake -DCMAKE_BUILD_TYPE=debug ..");
	#
	# renice process to provide enough resources to mysql and apache.
	# don't change it to lowest value (19),
	# that way c-compiler can spend a lot of time in "CPU-wait" state, rather than use CPU-cycles
	#
	system("cd ".$folders_to_backup{SRCDIR}."/build && time nice -10 make -j4");
	system("cd ".$folders_to_backup{SRCDIR}."/build && make install");
	system("cd ".$folders_to_backup{SRCDIR}."/build && make clean");

	return 1;
};

sub activate_htaccess
{
    my ($from_dir) = @_;
    # print "activate_htaccess: start ($from_dir)\n";
    opendir my($dh), $from_dir or die "activate_htaccess:ERROR: Could not open dir '$from_dir': $!";
    for my $entry (readdir $dh) {
    	# if(-d $from_dir."/".$entry)
    	# {
	    # 	print "--- $entry\n";
    	# }
    	# print $from_dir."/".$entry."\n" if(-d $from_dir."/".$entry);
        if(($entry eq "..htaccess") and (-f $from_dir."/".$entry))
        {
	        # print "(".$from_dir."/".$entry.") =~ matched against ".." \n" if($DEBUG);
	        print "\tactivate_htaccess: renaming (".$from_dir."/".$entry.") -> (".$from_dir."/.htaccess)";
	        rename $from_dir."/".$entry, $from_dir."/.htaccess";
	        print "\t[ok]\n";
	        next;
        }
    	if(!($entry eq "..") and !($entry eq ".") and (-d $from_dir."/".$entry))
    	{
    		activate_htaccess($from_dir."/".$entry);
    	}
    }
    closedir $dh;
    return;
}

sub activate_htpasswd
{
    my ($from_dir) = @_;
    # print "activate_htpasswd: start ($from_dir)\n";
    opendir my($dh), $from_dir or die "activate_htpasswd:ERROR: Could not open dir '$from_dir': $!";
    for my $entry (readdir $dh) {
    	# if(-d $from_dir."/".$entry)
    	# {
	    # 	print "--- $entry\n";
    	# }
    	# print $from_dir."/".$entry."\n" if(-d $from_dir."/".$entry);
        if(($entry eq "..htpasswd") and (-f $from_dir."/".$entry))
        {
	        # print "(".$from_dir."/".$entry.") =~ matched against ".." \n" if($DEBUG);
	        print "\tactivate_htpasswd: renaming (".$from_dir."/".$entry.") -> (".$from_dir."/.htpasswd)";
	        rename $from_dir."/".$entry, $from_dir."/.htpasswd";
	        print "\t[ok]\n";
	        next;
        }
    	if(!($entry eq "..") and !($entry eq ".") and (-d $from_dir."/".$entry))
    	{
    		activate_htpasswd($from_dir."/".$entry);
    	}
    }
    closedir $dh;
    return;
}

sub Activate_Files
{
	my	$activate_type = shift;

	foreach my $folder_id(keys %folders_to_backup)
	{
		if($activate_type eq ".htpasswd") { activate_htpasswd($folder_id); }
		elsif ($activate_type eq ".htaccess") { activate_htaccess($folder_id); }
		else { die "ERROR: unknown activation type $activate_type\n"; }
	}
}

sub copy_data_dirs
{
    my ($from_dir, $to_dir, $folderName) = @_;
    my	$tmpStr;
    # print "copy_data_dirs: start ($from_dir, $folderName)\n";
    # --- Normally it stops and return's if a copy fails, to continue on regardless set $File::Copy::Recursive::SkipFlop to true.
    local $File::Copy::Recursive::SkipFlop = 1;
    opendir my($dh), $from_dir or die "copy_data_dirs: Could not open dir '$from_dir': $!";
    for my $entry (readdir $dh) {
    	# if(-d $from_dir."/".$entry)
    	# {
	    # 	print "--- $entry\n";
    	# }
    	print "copy_data_dirs: ".$from_dir."/".$entry."\n" if((-d $from_dir."/".$entry) and ($DEBUG));
    	$tmpStr = $from_dir."/".$entry;
        if(($tmpStr eq $folderName) and (-d $from_dir."/".$entry))
        {
	        print "\tcopy_data_dirs: cp $from_dir/$entry -> $to_dir/$entry\n";
    		if(-e $to_dir."/".$entry and -d $to_dir."/".$entry)
    		{
		        remove_dir_recursively($to_dir."/".$entry);
    		}
			mkdir $to_dir."/".$entry;

	        my($num_of_files_and_dirs,$num_of_dirs,$depth_traversed) = dircopy($from_dir."/".$entry, $to_dir."/".$entry);
	        if((defined $num_of_files_and_dirs) and (defined $num_of_dirs) and (defined $depth_traversed))
	        {
	        	print "\t\tstatistics: #files ".($num_of_files_and_dirs - $num_of_dirs).", #dirs $num_of_dirs, depth_traversed $depth_traversed\n";
	        }
	        else
	        {
	        	print "\t\tERROR copying\n";
	        }

	        next;
        }
    	if(!($entry eq "..") and !($entry eq ".") and (-d $from_dir."/".$entry))
    	{
    		copy_data_dirs($from_dir."/".$entry, $to_dir."/".$entry, $folderName);
    	}
    }
    closedir $dh;
    return;
}

# sub CheckValidityLoadedVariables
# {
# 	my $result;

# 	$result = 1;
# 	$result = 0 unless(length $mysqldb);
# 	$result = 0 unless(length $mysqllogin);
# 	$result = 0 unless(length $mysqlhost);

# 	return $result;
# }

sub remove_data_dirs
{
    my ($from_dir, $folderName) = @_;
    my	$tmpStr;
    # print "remove_data_dirs: start ($from_dir, $folderName)\n";
    opendir my($dh), $from_dir or die "remove_data_dirs: ERROR: Could not open dir '$from_dir': $!";
    for my $entry (readdir $dh) {
    	# if(-d $from_dir."/".$entry)
    	# {
	    # 	print "--- $entry\n";
    	# }
    	# print $from_dir."/".$entry."\n" if(-d $from_dir."/".$entry);
    	$tmpStr = $from_dir."/".$entry;
        if(($tmpStr eq $folderName) and (-d $from_dir."/".$entry))
        {
	        print "\tremove_data_dirs: removing directory with mask (".$from_dir."/".$entry.") \n" if($DEBUG);
	        remove_dir_recursively($from_dir."/".$entry);
	        next;
        }
    	if(!($entry eq "..") and !($entry eq ".") and (-d $from_dir."/".$entry))
    	{
    		remove_data_dirs($from_dir."/".$entry, $folderName);
    	}
    }
    closedir $dh;
    return;
}

sub remove_dir_recursively
{
	my	($dir_to_remove) = @_;

	if(isDirExists($dir_to_remove))
	{
		print "\tremoving ".$dir_to_remove." ";

		rmtree($dir_to_remove, {error => \my $err} );

		if (@$err) {
		  for my $diag (@$err) {
		      my ($file, $message) = %$diag;
		      if ($file eq '') {
		          print "remove_dir_recursively: ERROR: $message\n";
		      }
		      else {
		          print "remove_dir_recursively: ERROR with unlinking $file: $message\n";
		      }
		  }
		}
		else {
			print "\t[ok]\n";
		}
	}
}

sub isRoot()
{
	my		$login;

	$login = getpwuid($>);
	return ($login eq "root" ? 1 : 0);
}

sub isDevServer()
{
	my 		$HOSTNAME = `hostname -s`;

	chomp($HOSTNAME);
	return ($HOSTNAME eq "dev" ? 1 : 0);
}

sub isMDev()
{
	my	$currentPath = getcwd;

	return ($currentPath =~ /\/mdev\./) && 1 || 0;
}

sub isMobileVersion()
{
	my	$currentPath = getcwd;

	return ($currentPath =~ /\/m\./) && 1 || 0;
}

sub isDev()
{
	my	$currentPath = getcwd;

	return ($currentPath =~ /\/dev\./) && 1 || 0;
}

sub isDesktopVersion()
{
	my	$currentPath = getcwd;

	return ($currentPath =~ /\/www\./) && 1 || 0;
}

sub isFilesReadyToRestore()
{
	if(($action =~ /_structure/) and (!isFileExist("sql_controltables")))
	{
		print "ERROR: sql_controltables missed, while you are trying to restore _structure. Probably you want to restore full backup.\n";
		return 0;
	}

	if(!($action =~ /_structure/) and (isFileExist("sql_controltables")))
	{
		print "ERROR: sql_controltables present, while you are trying to restore full backup. Probably you want to restore _structure\n";
		return 0;
	}

	unless(isFileExist("sql"))
	{
		print "ERROR: sql is missed\n";
		return 0;
	}

	foreach my $folder_id(keys %folders_to_backup)
	{
		unless(isDirExists($folder_id))
		{
			print "ERROR: folder ".$folder_id." is missed\n";
			return 0;
		}
	}

	return 1;
}

sub isFileExist
{
	my	($filename) = @_;

	if(-f $filename) { return 1; }

	return 0
}

sub isDirExists
{
	my	($dir_name) = @_;

	if(-d $dir_name) { return 1; }

	return 0
}

sub ReplaceSubstringInFile
{
    my	($filenameFrom, $substring_regex_src, $substring_regex_dst) = @_;
    my	($filenameTo) = $filenameFrom.".tmp";

    print "\tsed -e \"s/$substring_regex_src/$substring_regex_dst/\" $filenameFrom\n";

    open my $inF, "<", $filenameFrom or die "ReplaceSubstringInFile: ERROR: $filenameFrom is not exists\n";
    open my $outF, ">", $filenameTo or die "ReplaceSubstringInFile: ERROR: can't open $filenameTo\n";

    while(<$inF>)
    {
		s/$substring_regex_src/$substring_regex_dst/g;
		print $outF $_;
    }

    close $inF;
    close $outF;

    unlink $filenameFrom;
    rename $filenameTo, $filenameFrom;
}

sub ChangeWWWToDev
{
	my	@files_arr = @{$_[0]};
	my	$file_item;
	my	$result = 1;

	foreach $file_item(@files_arr)
	{
		if(isFileExist($file_item))
		{
		    ReplaceSubstringInFile($file_item, qr/\bwww\./, "dev\.");
		}
		else
		{
			print "ChangeWWWToDev: ERROR: file($file_item) not found\n";
			$result = 0;
		}
	}
	
	return $result;
}

sub ChangeDevToWWW
{
	my	@files_arr = @{$_[0]};
	my	$file_item;
	my	$result = 1;

	foreach $file_item(@files_arr)
	{
		if(isFileExist($file_item))
		{
		    ReplaceSubstringInFile($file_item, qr/\bdev\./, "www\.");
		}
		else
		{
			print "ChangeDevToWWW: ERROR: file($file_item) not found\n";
			$result = 0;
		}
	}

	return $result;
}

sub ChangeDevToDemo
{
	my	@files_arr = @{$_[0]};
	my	$file_item;
	my	$result = 1;

	foreach $file_item(@files_arr)
	{
		if(isFileExist($file_item))
		{
		    ReplaceSubstringInFile($file_item, qr/\bdev\./, "demo\.");
		}
		else
		{
			print "ChangeDevToDemo: ERROR: file($file_item) not found\n";
			$result = 0;
		}
	}

	return $result;
}

sub ChangeDevImagesToImages
{
	my	@files_arr = @{$_[0]};
	my	$file_item;
	my	$result = 1;

	foreach $file_item(@files_arr)
	{
		if(isFileExist($file_item))
		{
		    ReplaceSubstringInFile($file_item, qr/\bdevimages\./, "images\.");
		}
		else
		{
			print "ChangeDevImagesToImages: ERROR: file($file_item) not found\n";
			$result = 0;
		}
	}

	return $result;
}

sub ChangeDevImagesToDemoImages
{
	my	@files_arr = @{$_[0]};
	my	$file_item;
	my	$result = 1;

	foreach $file_item(@files_arr)
	{
		if(isFileExist($file_item))
		{
		    ReplaceSubstringInFile($file_item, qr/\bdevimages\./, "demoimages\.");
		}
		else
		{
			print "ChangeDevImagesToDemoImages: ERROR: file($file_item) not found\n";
			$result = 0;
		}
	}

	return $result;
}

sub ChangeImagesToDevImages
{
	my	@files_arr = @{$_[0]};
	my	$file_item;
	my	$result = 1;

	foreach $file_item(@files_arr)
	{
		if(isFileExist($file_item))
		{
		    ReplaceSubstringInFile($file_item, qr/\bimages\./, "devimages\.");
		}
		else
		{
			print "ChangeDevImagesToImages: ERROR: file($file_item) not found\n";
			$result = 0;
		}
	}

	return $result;
}

# sub DefineDBCredentials
# {
# 	#
# 	# Look for DB access credentials
# 	#

# 	if($action =~ /^--restore/)
# 	{
# 		print "find ".$config{localy_h_file_restore_location};
# 		open(F, $config{localy_h_file_restore_location}) || die "\n(can't open ".$config{localy_h_file_restore_location}.")\n";
# 	}
# 	else
# 	{
# 		print "find ".$config{localy_h_file_backup_location};
# 		open(F, $config{localy_h_file_backup_location}) || die "\n(can't open ".$config{localy_h_file_backup_location}.")\n";
# 	}

# 	print "....\n";

# 	while (<F>)
# 	{
# 		$str = $_;
# 		if($str =~ /#define\s+DB_NAME\s*\"(.*)\"/)
# 		{
# 			$mysqldb = $1;
# 		}
# 		if($str =~ /#define\s+DB_LOGIN\s*\"(.*)\"/)
# 		{
# 			$mysqllogin = $1;
# 		}
# 		if($str =~ /#define\s+DB_PASSWORD\s*\"(.*)\"/)
# 		{
# 			$mysqlpass = $1;
# 		}
# 		if($str =~ /#define\s+DB_HOST\s*\"(.*)\"/)
# 		{
# 			$mysqlhost = $1;
# 		}
# 	}
# 	close(F);

# 	CheckValidityLoadedVariables() || die "ERROR: following variables must be defined in ".$config{localy_h_file_backup_location}.": DB_NAME, DB_LOGIN, DB_PASSWORD, DB_HOST\n";
# }

sub CheckDBConnectivity
{
	my	($result);

	$result = 0;

	if(system("mysql --login-path=".$config{login_path}." -e \"\"") == 0)
	{
		# --- good 2 go
		$result = 1;
	}

	return $result;
}

sub CleanBuildDirectory
{
	if(isDirExists("./SRCDIR"))
	{
		remove_dir_recursively("./SRCDIR/build/");
		mkdir("./SRCDIR/build/", 0755);
	}
}

sub Remove_Local_Tmp_Folder
{
	if(isDirExists($config{local_tmp_folder}))
	{
		remove_dir_recursively($config{local_tmp_folder});
	}
}

sub Remove_Local_PersistentFolders
{
	foreach my $key (keys %persistent_folders)
	{
		if(isDirExists($persistent_folders{$key}))
		{
			remove_dir_recursively($persistent_folders{$key});
		}
		else
		{
			print "ERROR: folder ".$persistent_folders{$key}." doesn't exist\n";
		}
	}
}

sub Remove_CGI_From_Local_Folders
{
	print "removing *.cgi from local folders";
	foreach my $folder_id(keys %folders_to_backup)
	{
		system("find $folder_id -name *.cgi -type f -exec rm -f {} \\;");
	}
	print "\t[ok]\n";
}

sub Remove_Production_Folders
{
	foreach my $folder_id(keys %folders_to_backup)
	{
		if(isDirExists($folders_to_backup{$folder_id}))
		{
			remove_dir_recursively($folders_to_backup{$folder_id});
		}
		else
		{
			print "ERROR: production folder(".$folders_to_backup{$folder_id}.") doesn't exist\n";
		}
	}
}

sub Remove_Local_FoldersToBackup
{
	foreach my $folder_id(keys %folders_to_backup)
	{
		if(isDirExists($folder_id))
		{
			remove_dir_recursively($folder_id);
		}
	}
}

sub Copy_Production_To_Local
{
	foreach my $folder_id(keys %folders_to_backup)
	{
		if(isDirExists($folders_to_backup{$folder_id}))
		{
			print "\tcopying ".$folders_to_backup{$folder_id}." -> $folder_id";
			system("cp -R ".$folders_to_backup{$folder_id}." $folder_id");
			print "\t[ok]\n";
		}
		else
		{
			print "ERROR copying production ".$folders_to_backup{$folder_id}." to $folder_id\n";
		}
	}
}

sub Copy_Local_To_LocalTmp_PersistentFolders
{
	foreach my $folder_id(keys %persistent_folders)
	{
		if(isDirExists($persistent_folders{$folder_id}))
		{
			print "\tcopying ".$persistent_folders{$folder_id}." -> ".$config{local_tmp_folder}.$persistent_folders{$folder_id};
			dircopy($persistent_folders{$folder_id}, $config{local_tmp_folder}.$persistent_folders{$folder_id}) || die "ERROR: can't copy ".$persistent_folders{$folder_id}." -> ".$config{local_tmp_folder}.$persistent_folders{$folder_id};
			print "\t[ok]\n";
		}
		else
		{
			print "Copy_Local_To_LocalTmp_PersistentFolders:ERROR: folder doesn't exist ".$persistent_folders{$folder_id}."\n";
		}
	}
}

sub Dirmove_Local_To_Production
{
	foreach my $folder_id(keys %folders_to_backup)
	{
		if(isDirExists($folder_id))
		{
			print "\tmoving ".$folder_id." -> ".$folders_to_backup{$folder_id};
			if(dirmove($folder_id, $folders_to_backup{$folder_id}))
			{
				print "\t[ok]\n";
			}
			else
			{
				print "\t[ERROR]\n";
			}
		}
		else
		{
			print "Dirmove_Local_To_Production:ERROR: folder doesn't exist ".$folder_id."\n";
		}
	}
}

sub Dirmove_LocalTmp_To_Local
{
	foreach my $folder_id(keys %folders_to_backup)
	{
		if(isDirExists($config{local_tmp_folder}.$folder_id))
		{
			print "\tmoving ".$config{local_tmp_folder}.$folder_id." -> ".$folder_id;
			dirmove($config{local_tmp_folder}.$folder_id, $folder_id) || die "can't move ".$config{local_tmp_folder}.$folder_id."-> ".$folder_id;
			print "\t[ok]\n";
		}
		else
		{
			print "ERROR dirmove ".$config{local_tmp_folder}.$folder_id." to ".$folder_id."\n";
		}
	}
}

sub Dirmove_Local_To_LocalTmp
{
	foreach my $folder_id(keys %folders_to_backup)
	{
		if(isDirExists($folder_id))
		{
			print "\trenaming ".$folder_id." -> ".$config{local_tmp_folder}.$folder_id;
			dirmove($folder_id, $config{local_tmp_folder}.$folder_id) || die "can't move ".$folder_id."-> ".$config{local_tmp_folder}.$folder_id;
			print "\t[ok]\n";
		}
		else
		{
			print "ERROR dirmove ".$folder_id." to ".$config{local_tmp_folder}.$folder_id."\n";
		}
	}
}

sub	CreateRecoveryPoint
{
	my	$recovery_point_folder = shift;

	if(-d $recovery_point_folder)
	{
		print "ERROR: $recovery_point_folder already exist\n";
	}
	else
	{
		mkdir($recovery_point_folder);

		foreach my $folder_id(keys %folders_to_backup)
		{
			if(isDirExists($folders_to_backup{$folder_id}))
			{
				print "\tcopying ".$folders_to_backup{$folder_id}." -> ".$recovery_point_folder.$folder_id;
				mkdir($recovery_point_folder.$folder_id);
				dircopy($folders_to_backup{$folder_id}, $recovery_point_folder.$folder_id);
				print "\t[ok]\n";
			}
			else
			{
				die "ERROR creating recovery point to production folder (".$folders_to_backup{$folder_id}.")\n";
			}
		}

		print "\tMySQL backup to $recovery_point_folder/sql\n";
		system("mysqldump --login-path=".$config{login_path}." --default-character-set=utf8mb4 -Q $domainSuffix > $recovery_point_folder/sql");
	}
}

sub	FixOwnerAndAccessRights
{
	foreach my $folder_id(keys %folders_to_backup)
	{
		if(isDirExists($folders_to_backup{$folder_id}))
		{
			system("chown -R ".$config{http_username}.":".$config{http_username}." ".$folders_to_backup{$folder_id});
			system("chmod -R g+rw ".$folders_to_backup{$folder_id});
			system("chmod -R o-rw ".$folders_to_backup{$folder_id});
		}
		else
		{
			print "FixOwnerAndAccessRights:ERROR: folder doesn't exist ".$folders_to_backup{$folder_id}."\n";
		}
	}

	system("chown -R ".$config{build_username}.":".$config{build_username}." ".$folders_to_backup{SRCDIR});
	system("chown -R ".$config{build_username}.":".$config{build_username}." ".$config{local_install_dir});

	# --- if directory is empty then bash will drop an error due to inability to change ownership on empty file list
	# --- redirect stderr to /dev/null applied as a workaround
	system("chown -R ".$config{http_username}.":".$config{http_username}." ".$config{project_log_dir}."* 2>/dev/null");
}

sub usage
{
	print "ERROR: ".$ARGV[0]." is not correct parameter\n\n";
	print "usage: archive.pl [OPT]\n";
	print " --backup - full backup\n";
	print " --backup_structure - structure backup\n";
	print " --restore_production - restore production from backup\n";
	print " --restore_production_structure - upgrade production structure\n";
	print " --restore_dev - restore dev from backup\n";
	print " --restore_dev_structure - upgrade dev structure (seems useless)\n";
	print " --restore_demo - restore demo from backup\n";
	print " --restore_demo_structure - upgrade demo structure\n";
}
