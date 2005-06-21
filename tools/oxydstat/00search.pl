#!/usr/bin/perl -w

if ($#ARGV lt 0) {
  print "Usage: 00search.pl [regexp]+\n";
  die "Missing search expressions";
}

print "Updating statistics..\n".`make stat`."-----------------\n";

my @gameName = ( "Oxyd1",         "OxydExtra",   "OxydMagnum",    "OxydMagnumGold", "PerOxyd" );
my @statName = ( "oxyd1ibm.stat", "oxydex.stat", "oxydmibm.stat", "oxydmgg.stat",   "peroxyd.stat" );

sub stat2idx($) {
  my ($stat) = @_;
  my $idx;

  for ($idx=0; $idx<5; $idx++) {
    if ($stat eq $statName[$idx]) {
      return $idx;
    }
  }

  die "Unknown statistic file '$stat'";
}

my %found_objects  = ();
my %matched_levels = ();
my $pass    = 0;

foreach (@ARGV) {
  $pass++;
  my @result = `grep -i '$_' *.stat`;

  foreach (@result) {
    if (/^([^ ]+):([^ ]+) .*:(.*)$/ig) {
      my ($stat,$obj,$levels) = ($1,$2,$3);

      if ($levels ne '') {
        my $idx = stat2idx($stat);

        while ($levels ne '') {
          if ($levels =~ /\s*([0-9]+)\s*/ig) {
            my $lev = '000'.$1;
            $levels = $';
            $lev = substr($lev,length($lev)-3);

            my $levName=$gameName[$idx].' #'.$lev;

            if (exists $matched_levels{$levName}) {
              my $last = $matched_levels{$levName};
              if ($last == ($pass-1)) { # if matched last criteria
                $matched_levels{$levName} = $pass;
              }
            }
            elsif ($pass == 1) { # if this is the first criteria
              $matched_levels{$levName} = 1;
            }
          }
          else {
            $levels = '';
          }
        }
        $found_objects{$obj} = 1;
      }
    }
  }
}

@objs = keys %found_objects;
if ($#objs >= 0) {
  print "Matching objects: ";
  foreach (keys %found_objects) {
    print "$_ ";
  }
  print "\n";

  my @found = sort map {
    if ($matched_levels{$_} == $pass) {
      $_;
    } else {
      ;
    }
  } keys %matched_levels;

  if ($#found >= 0) {
    print "Levels matching all criteria:";
    my $last_prefix = '';
    foreach (@found) {
      if (/^(.*)\#/ig) {
        $prefix = $1;
        $_ = $';
        s/^0+//ig;
        if ($prefix eq $last_prefix) {

        } else {
          my $name = $prefix.'          ';
          $name = substr($name,0,15);
          print "\n$name";
          $last_prefix = $prefix;
        }
      }
      print "$_ ";
    }
    print "\n";
  } else {
    print "No level matched all criteria.\n";
  }
}
else {
  print "No object matched any criteria.\n";
}
