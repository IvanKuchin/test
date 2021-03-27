#!/usr/bin/perl

use Cwd;
use strict;

my	@anonimazition = ("src/pi/include/localy.h.in", "tests/selenium/login.py");
my	$DEBUG;
my	$git_commit_message;

$|++;

$DEBUG = 1;

if(isCurrentDirProjectRoot()) {} else { die "ERROR: run this script from project root directory\n"; }

ParseParameters();
# AnonimizeFiles(@anonimazition); # not needed anymore
AddAndCommit();
# SaveLatestGitCommitID();

sub ParseParameters()
{
	my ($key0, $val0) = @ARGV;
	 
	if (not defined $key0) 
	{
	}
	else
	{
		if($key0 eq "-h")
		{
			PrintHelp();
			exit(0);
		}
		if($key0 eq "-m")
		{
			if (defined $val0) 
			{
				$git_commit_message = $val0;
			}
			else
			{
				die "git commit message is empty"
			}
		}
	}
}

sub PrintHelp()
{
	print("Usage:\n");
	print("-h	print help\n");
	print("-m	git commit message (for ex: -m \"test commit\"\n");
}

sub SaveLatestGitCommitID
{
	system("git rev-parse HEAD > ./supplemental/git/git_commit_id");
}



sub AddAndCommit
{
	system("git add .");
	if(defined $git_commit_message)
	{
		system("git commit -m \"$git_commit_message\"");
	}
	else
	{
		print("git commit message: ");
		system("git commit --file -");
	}
	system("git push");
}

sub AnonimizeFiles
{
	my	@files = @_;

	if(areFilesExist(@files))
	{
		if(BackupFiles(@files))
		{
			if(HideSensitiveInfoInFiles(@files))
			{
				AddAndCommit();

				if(RestoreFiles(@files)) {}
				else
				{
					print "ERROR: error recovering files\n";
				}
			}
			else
			{
				print "ERROR: error removing passwords\n";
			}
		}
		else
		{
			print "ERROR: error backing up files\n";
		}
	}
	else
	{
		print "ERROR: some files are missed\n";
	}
}

sub isCurrentDirProjectRoot
{
    my	$result = 1;
    my	$curr_dir = getcwd();

    if(($curr_dir =~ /git/) || ($curr_dir =~ /supplemental/)) { $result = 0; }

    return $result;
}

sub BackupFiles
{
	my	@files = @_;
	my	$result = 1;

	foreach my $file (@files)
	{
		system("cp ".$file." ".$file.".original");
	}

	return $result;
}

sub RestoreFiles
{
	my	@files = @_;
	my	$result = 1;

	foreach my $file (@files)
	{
		system("cp ".$file.".original"." ".$file);
		unlink $file.".original";
	}

	return $result;
}

sub areFilesExist
{
	my	@files = @_;
	my	$result = 1;

	foreach my $file (@files)
	{
		if(isFileExist($file)) {}
		else
		{
			$result = 0;
		}
	}

	return $result;
}

sub isFileExist
{
	my	($filename) = @_;

	if(-f $filename) { return 1; }

	return 0
}

sub HideSensitiveInfoInFiles
{
	my	@files = @_;
	my	$result = 1;

	foreach my $file (@files)
	{
		if(HideSmthInFile($file, "_LOGIN")) {}
		else
		{
			$result = 0;
		}
		if(HideSmthInFile($file, "_PASSWORD")) {}
		else
		{
			$result = 0;
		}
	}

	return $result;
}

sub HideSmthInFile
{
    my	($filenameFrom, $hidden_word) = @_;
    my	($filenameTo) = $filenameFrom.".tmp";

    # print "\tsed -e \"s/$substring_regex_src/$substring_regex_dst/\" $filenameFrom\n";

    open my $inF, "<", $filenameFrom or die "ReplaceSubstringInFile: ERROR: $filenameFrom is not exists\n";
    open my $outF, ">", $filenameTo or die "ReplaceSubstringInFile: ERROR: can't open $filenameTo\n";

    while(<$inF>)
    {
		s/([\w\d]+$hidden_word[\s\=]+)"(.*)"/\1"\$\{PROJECT_NAME\}"/g;
		print $outF $_;
    }

    close $inF;
    close $outF;

    unlink $filenameFrom;
    rename $filenameTo, $filenameFrom;
}
