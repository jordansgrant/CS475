#!/usr/bin/perl

print "Performance was calculated in MegaSums per Second (MS/s)\n";

my @trow;
my @num_threads = ( 1, 2, 4 );

foreach my $NUMPAD (1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ) {

  my $line = $NUMPAD."\t";
  foreach my $NUMT ( @num_threads ) {
    my $retval;

    if ($ARGV[0] eq "mic") {
      $retval = system("icc -mmic -o progf1 prog3fix1.c -lm -openmp -no-vec -align -DNUMT=".$NUMT." -DNUMPAD=".$NUMPAD);
    }
    elsif ($ARGV[0] eq "icc") {
      $retval = system("icc -o progf1 prog3fix1.c -lm -openmp -no-vec -align -DNUMT=".$NUMT." -DNUMPAD=".$NUMPAD);
    }
    else {
      $retval = system("gcc -o progf1 prog3fix1.c -lm -fopenmp -DNUMT=".$NUMT." -DNUMPAD=".$NUMPAD);
    }
    if ($retval != 0) {
      print "Failed to compile with NUMT=".$NUMT." NUMPAD=".$NUMPAD."\n";
      return 1;
    }

    print "\nNow Running NUMT=".$NUMT." NUMPAD=".$NUMPAD."\n";
    
    if ($ARGV[0] eq "mic") {
      chomp($retval = `micnativeloadex progf1`);
    }
    else {
      $retval = `progf1`;
    }

    $line .= $retval."\t";
  }

  push @trow, $line."\n";

}

system("rm -f progf1");

print "\t\t";
foreach my $num (@num_threads) {
  print $num."\t\t";
}
print "\n";

foreach my $row (@trow) {
  print $row;
}
