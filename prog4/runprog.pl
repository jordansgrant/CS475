#!/usr/bin/perl
if ( $ARGV[0] eq "mic" ) {
  $retval1 = system("icc -mmic -o prog1 prog4.c -lm -openmp -no-vec -align");
  $retval2 = system("icc -mmic -o prog2 prog4.c -lm -openmp -no-vec -align -DBLIGHT");
}
elsif ($ARGV[0] eq "icc") {
  $retval1 = system("icc -o prog1 prog4.c -lm -openmp -no-vec -align");
  $retval2 = system("icc -o prog2 prog4.c -lm -openmp -no-vec -align -DBLIGHT");
}
else {
  $retval1 = system("gcc -o prog1 prog4.c -lm -fopenmp");
  $retval2 = system("gcc -o prog2 prog4.c -lm -fopenmp -DBLIGHT");
}
  
if ($retval1 != 0 || $retval2 != 0) {
  print "Failed to compile\n";
  return 1;
}

if ($ARGV[0] eq "mic") {
  system("micnativeloadex ./prog1");
  print "\n\n";
  system("micnativeloadex ./prog2");
}
else {
  system("./prog1");
  print "\n\n";
  system("./prog2");
}
system("rm -f prog1; rm -f prog2");

