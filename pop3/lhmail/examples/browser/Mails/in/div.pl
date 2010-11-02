#!/usr/bin/perl

$name = 0;

open(MAIL, ">>../$name") or die "Could not open $name";

while(<>)
{
    if (/^From[^\:]/)
    {
	close(MAIL);
	$name++;
        open(MAIL, ">>../__$name") or die "Could not open $name";
    }
    
    print MAIL $_;
    
}

close(MAIL);