#!/usr/bin/perl
print "Performance of Volume Calculation is measured in Mega Heights Calculated per Second (MH/s)\n\n";

@trow;

foreach my $numnode (10, 50, 100, 200, 500, 1000, 1500, 2000, 3000, 4000, 5000) {
  foreach my $numt ( 1, 2, 4, 6, 8, 10, 12 ) {
    my $retval = system('g++ -o prog prog1.cpp -lm -fopenmp -DNUMNODES='.$numnode.' -DNUMT='.$numt);
    if ($retval != 0) {
      print 'Failed to Compile with NUMT '.$numt.' and NUMNODES '.$numnodes."\n";
      return 1;
    }

    $retval = `prog`;

    push @trow, $numnode."\t".$numt."\t".$retval."\n";

  }
}

system('rm -f prog');

print "NUMNODES\tNUMT\tPerformance (MH/s)\n";
foreach my $row (@trow) {
  print $row;
}
