#!/usr/bin/perl

my @rows;

foreach my $size (1000, 10000, 100000, 500000, 1000000, 5000000, 10000000, 15000000, 20000000, 32000000) {
  my $line = "".$size;
  foreach my $function ( "SSE_MULT", "MULT", "SSE_REDUCE", "REDUCE" ) {
    my $retval = system("g++ -o prog prog5.cpp simd.p5.cpp -lm -fopenmp -DLEN=".$size." -D=".$function);
    if ( $retval != 0) {
      print STDERR "Failed to compile with LEN = ".$size." and Function = ".$function."\n";
      return 1;
    }
    print "\nRunning with LEN = ".$size." Function = ".$function."\n";
    $retval = `prog`;
    $line .= "\t".$retval;
  }

  push @rows, $line."\n";

}

print "\tSSE_MULT\tMULT\tSSE_REDUCTION\tREDUCTION\n";

foreach my $row (@rows) {
  print $row;
}

system("rm -f prog");
