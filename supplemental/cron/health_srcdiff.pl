#!/usr/bin/perl

use strict;
use File::Find;

my $DEBUG = 0;
my %config = do 'health_srcdiff.conf';

print LookforDiff();

sub PrintError
{
    my	($err_msg) = @_;

    print "ERROR: ".$err_msg."\n";
};

sub LookforDiff()
{
    my	$root_src_dir;
    my	$root_html_dir;
    my	@domains;
    my	$domains_size;
    my	$error_message = "";

    $root_src_dir = DefineRootSrcDir();
    if($root_src_dir eq "")
    {
	PrintError("fail to define root SRC dir");
    }
    else
    {
	print "root_src_dir: ".$root_src_dir."\n" if($DEBUG);

	$root_html_dir = DefineRootHTMLDir();
	if($root_src_dir eq "")
	{
	    PrintError("fail to define root HTML dir");
	}
	else
	{
	    print "root_html_dir: ".$root_html_dir."\n" if($DEBUG);

	    @domains = &GetDomains($root_src_dir);
	    $domains_size = scalar @domains;

	    if($domains_size eq 0)
	    {
		PrintError("domain dirs not found in src folder");
	    }
	    else
	    {
		print "number of domains: ".$domains_size."\n" if($DEBUG);
		$error_message = DiffFiles($root_src_dir, $root_html_dir, @domains);
	    }
	}
    }

    return $error_message;
};

sub DefineParentDir
{
    my	$result = "";
    my	($root_src_dir, $curr_domain) = @_;

    if (index($root_src_dir, $curr_domain) == -1) 
    {
	PrintError("can't find ".$curr_domain." in ".$root_src_dir);
    }
    else
    {
	$result = substr $root_src_dir, 0, index($root_src_dir, $curr_domain);
    }

    return $result;
};

sub DefineRootSrcDir
{
    return DefineParentDir($config{project_source_dir}, $config{project_domain});
};

sub DefineRootHTMLDir
{
    return DefineParentDir($config{ROOTDIR_HTML}, $config{project_domain});
};

sub IncludeDomain
{
	my	$result = 1;
	my	$curr_domain = shift;

	foreach(@{$config{exclude_domains}})
	{
		my	$domain = $_;

		print("check $curr_domain against $domain\n") if($DEBUG);

		if(index($curr_domain, $domain) != -1)
		{
			$result = 0;
			print("domain $curr_domain excluded by the config\n") if($DEBUG);
		}
	}

	return $result;
};

sub GetDomains
{
    my	@result;
    my	($from_dir) = @_;

    opendir my($dh), $from_dir or die "ERROR: Could not open dir '$from_dir': $!";
    for my $entry (readdir $dh) 
    {
        if((-d $from_dir.$entry) and !($entry eq "..") and !($entry eq ".") and IncludeDomain($entry))
        {
            push(@result, $entry);
        }
    }

    return @result;
};

sub DiffFile
{
    my	($root_dir, $curr_file, @domains) = @_;
    my	@files;
    my	$result = "";

	foreach(@domains)
	{
	    my	$domain = $_;
	    my	$file = FindFile($root_dir.$domain, $curr_file);

	    chomp($file);

	    if(length($file))
	    {
			push(@files, $file);
	    }
	    else
	    {
			if($#files + 1)
			{
			    PrintError("$curr_file not found in $domain");
			}
			else
			{
			}
	    }
	}

	#if($#files > 0)
	{
	    if($#domains == $#files)
	    {
		for my $i(1 .. $#files)
		{
		    my $diff_line = "diff ".$files[0]." ".$files[$i];
		    my $ret_code = system($diff_line." 1>/dev/null 2>/dev/null");

		    print $diff_line."\n" if($DEBUG);
		    if($ret_code)
		    {
			$result .= $diff_line."\n";
		    }
		}
	    }
	    else
	    {
		PrintError("found ".($#files + 1)." x ".$curr_file." in ".$root_dir.", must be ".($#domains + 1)) if($DEBUG);
	    }
	}

	return $result;
};

sub DiffFiles
{
    my	$result = "";
    my	($root_src_dir, $root_html_dir, @domains) = @_;
    my	@ndf = @{$config{nodiff_files}};

    foreach(@ndf)
    {
	my $curr_file = $_;

	$result .= DiffFile($root_html_dir, $curr_file, @domains);
	$result .= DiffFile($root_src_dir, $curr_file, @domains);
    }

    return $result;
};

# --- Find file part

my	@__file_arr;
my	$__file_pattern;
sub __find_sub
{
    my $F = $File::Find::name;
    if($F =~ m/\/build\//)
    {
    	# --- skip build folder
    }
    elsif ($F =~ m/\/$__file_pattern$/ ) 
    {
    	push(@__file_arr,$F);
    	# print("FOUND: pattern:", $__file_pattern, ", file: ", $F, " arr size:", scalar @file_arr, "\n");
    }
}

sub FindFile
{
    my	($folder, $file) = @_;

    my	$result = "";

    $__file_pattern = $file;
    @__file_arr = ();

	find({ wanted => \&__find_sub, no_chdir=>1}, $folder);

	my	$arr_size = @__file_arr;
	# print("-- try to find ", $file, " in folder ", $folder, " -> ", $arr_size, "\n");

	if($arr_size == 1)
	{
		$result = $__file_arr[0];

		# print("-----", $result, "\n");
	}
	elsif($arr_size == 0)
	{
		# --- not an error, file just not found.
		# --- that could happen if looking for *.cpp in web-folders
		# --- or *.html in src folder
	}
	else
	{
	    PrintError($arr_size." x ".$file." found in ".$folder);
	}

    return $result;
};

# --- Find file part
