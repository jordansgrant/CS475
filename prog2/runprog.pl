#!/usr/bin/perl

print "Performance was calculated in Mega Multiples per Second (MM/s)\n\n";
my @trow;

foreach my $NUMT (1, 2, 4, 6, 8, 10, 12, 16) {
    
  my $line = $NUMT."\t";
  
  foreach my $SCHED ( "static", "dynamic" ) {
    foreach my $CHUNK ( 1, 4096 ) {
      my $retval;
      if ($ARGV[0] eq "mic") {
        $retval = system("icpc -mmic -o prog prog2.cpp -lm -openmp -no-vec -align -DNUMT=".$NUMT." -DSCHED_TYPE=".$SCHED." -DCHUNK_SIZE=".$CHUNK);
      }
      elsif ($ARGV[0] eq "icpc") {
        $retval = system("icpc -o prog prog2.cpp -lm -openmp -no-vec -align -DNUMT=".$NUMT." -DSCHED_TYPE=".$SCHED." -DCHUNK_SIZE=".$CHUNK);
      }
      else {
        $retval = system("g++ -o prog prog2.cpp -lm -fopenmp -DNUMT=".$NUMT." -DSCHED_TYPE=".$SCHED." -DCHUNK_SIZE=".$CHUNK);
      }
      if ($retval != 0) {
        print "Failed to compile with NUMT=".$NUMT." Schedule=".$SCHED." Chunk=".$CHUNK."\n";
        return 1;
      }
      print "\nNow Running NUMT=".$NUMT." SCHED=".$SCHED." CHUNK=".$CHUNK."\n";
      if ($ARGV[0] eq "mic") {
        chomp($retval = `micnativeloadex prog`);
      }
      else {
        $retval = `prog`;
      }
      $line .= $retval."\t";
    }
  }
  
  push @trow, $line."\n";
}

system("rm -f prog");

print "\n\tstatic-1\tstatic-4096\tdynamic-1\tdynamic-4096\n";
foreach my $row (@trow) {
  print $row;
}
