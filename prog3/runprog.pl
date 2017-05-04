#!/usr/bin/perl

print "Performance was calculated in MegaSums per Second (MS/s)\n";

my @trow;
my @num_threads = ( 1, 2, 4 );

foreach my $NUMT (@num_threads) {

  my $line = $NUMT."\t";

  foreach my $NUMPAD ( 1, 2, 4, 8, 10, 16 ) {
    my $retval;

    if ($ARGV[0] eq "mic") {
      $retval = system("icc -mmic -o prog prog3.c -lm -openmp -no-vec -align -DNUMT=".$NUMT." -DNUMPAD=".$NUMPAD);
    }
    elsif ($ARGV[0] eq "icc") {
      $retval = system("icc -o prog prog3.c -lm -openmp -no-vec -align -DNUMT=".$NUMT." -DNUMPAD=".$NUMPAD);
    }
    else {
      $retval = system("gcc -o prog prog3.c -lm -fopenmp -DNUMT=".$NUMT." -DNUMPAD=".$NUMPAD);
    }
    if ($retval != 0) {
      print "Failed to compile with NUMT=".$NUMT." NUMPAD=".$NUMPAD."\n";
      return 1;
    }

    print "\nNow Running NUMT=".$NUMT." NUMPAD=".$NUMPAD."\n";
    
    if ($ARGV[0] eq "mic") {
      chomp($retval = `micnativeloadex prog`);
    }
    else {
      $retval = `prog`;
    }

    $line .= $retval."\t";
  }

  push @trow, $line."\n";

}

system("rm -f prog");

print "\t";
foreach my $num (@num_threads) {
  print $num."\t";
}
foreach my $row (@trow) {
  print $row;
}
