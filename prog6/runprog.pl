#!/usr/bin/perl


my @rowsMult;
my @rowsMultSum;
my @locals = ( 8, 32, 64, 128, 256, 512 );

foreach my $global ( 16, 160, 1280, 10240, 24000, 64000, 80000, 125000 ) {
  $globalSize = $global * 64;

  my $lineMult    = "".$globalSize;
  my $lineMultSum = "".$globalSize;

  foreach my $local ( @locals ) {
    $retval = system("g++ -o prog prog6.cpp -lm -fopenmp -lOpenCL -DNUM=".$global." -DLOCAL_SIZE=".$local." -DMULT");
    if ( $retval != 0 ) {
      print STDERR "Could not compile with Global=64*".$global." Local Size=".$local." with Mult f(x)\n";
      return 1;
    }  
    $retval = `prog`;
    $lineMult = $lineMult."\t".$retval;
  
    $retval = system("g++ -o prog prog6.cpp -lm -fopenmp -lOpenCL -DNUM=".$global." -DLOCAL_SIZE=".$local." -DMULTSUM");
    if ( $retval != 0 ) {
      print STDERR "Could not compile with Global=64*".$global." Local Size=".$local." with MultSum f(x)\n";
      return 1;
    }  
    $retval = `prog`;
    $lineMultSum = $lineMultSum."\t".$retval;
  }
  
  push @rowsMult,    $lineMult."\n";
  push @rowsMultSum, $lineMultSum."\n";
}

print "\nMult Table\n";
foreach my $local (@locals) {
  print "\t".$local;
}
print "\n";

foreach my $row (@rowsMult) {
  print $row;
}

print "\nMultSum Table\n";
foreach my $local (@locals) {
  print "\t".$local;
}
print "\n";

foreach my $row (@rowsMultSum) {
  print $row;
}

system("rm -f prog");
