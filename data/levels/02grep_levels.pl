#!/usr/bin/perl -w

my $outname = '+index_user.txt';
my $regCount = $#ARGV+1;
my $maxHits = 0;

if ($regCount le 0) {
  print "Usage: 02grep_levels.pl [<regex>]+\n";
  print "       Greps all levels containing all <regex>s and create index_test.txt for these levels\n";

  unlink $outname;
  print "$outname unlinked\n";
  exit 1;
}

my %levels = ();
my %maxHittenLevels = ();

foreach(@ARGV) {
  my @found = `grep -i '$_' *.lua */*.lua | sed -e "s/:.*//ig" | sort | uniq`;
  foreach (@found) {
    chomp;
    if (exists $levels{$_}) { $levels{$_}++; }
    else { $levels{$_} = 1; }
    if ($levels{$_} > $maxHits) { $maxHits = $levels{$_}; }
  }
}

foreach (keys %levels) {
  if ($levels{$_}==$maxHits) { $maxHittenLevels{$_}=$maxHits; }
}

# print "------------------------------------------------------------\n";
if ($maxHits eq 0) {
  print "No regex matched.\n";
  unlink($outname);
  print "$outname has been removed.\n";
  exit 1;
}
elsif ($maxHits<$regCount) {
  print "No level matched all $regCount expressions.\n";
  print "Saving levels that were hit by at least $maxHits expressions.\n";
}

print "maxHits=$maxHits expressions=$regCount\n";
my $index_regex = '';
foreach (keys %maxHittenLevels) {
  if (not /^todo.lua/) {
    s/\.lua//ig;
    $index_regex .= "\\|file=[ \"\}]*$_";
  }
}
$index_regex = '\\('.substr($index_regex,2).'\\|^file=[ \"]*test[^ \"\}]*\\)[^a-z0-9_]';

open FILE, ">$outname" || die "Can't open '$outname'";
my @hits= `grep -hi '$index_regex' index_*.txt`;
print FILE @hits;
close FILE;

my $hits = scalar(@hits);
print "$hits levels indexed into '$outname'\n";

