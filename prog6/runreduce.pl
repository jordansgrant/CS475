#!/usr/bin/perl


my @rows;
my @locals = ( 32, 64, 128, 256 );

foreach my $global ( 16, 160, 1280, 10240, 24000, 64000, 80000, 125000 ) {
  $globalSize = $global * 64;

  my $line  = "".$globalSize;

  foreach my $local ( @locals ) {
    $retval = system("g++ -o prog prog6red.cpp -lm -fopenmp -lOpenCL -DNUM=".$global." -DLOCAL_SIZE=".$local);
    if ( $retval != 0 ) {
      print STDERR "Could not compile with Global=64*".$global." Local Size=".$local." with Mult f(x)\n";
      exit 1;
    }  
    $retval = `prog`;
    $line = $line."\t".$retval;
  }
  push @rows, $line."\n";
}

foreach my $local (@locals) {
  print "\t".$local;
}
print "\n";

foreach my $row (@rows) {
  print $row;
}

system("rm -f prog");
