# https://www.youtube.com/watch?v=WEghIXs8F6c

use strict;
use warnings;
use diagnostics;

use feature 'say';
use feature 'switch';
use v5.14.1;

print "Hello World\n";
print "I am happy\n";

my $name = 'Jeong Hwan Kim';

print $name;
print "\n";

my ($age, $street) = (40, 'Seoul, Korea');
print $age;
print "\n";

my $my_info = "$name lives on \"$street\"\n";
print $my_info;

$my_info = qq{$name lives on "$street"\n};
print $my_info;

my $bunch_on_info = <<"END";
This is a 
bunch of information
on multiple lines
END

say $bunch_on_info;

my $big_int = 5555555555555;
# %c: character
# %s: string
# %d: decimal
# %u: unsigned integer
# %f: floating point (Decimal Notation)
# %e: floating point (Scientific Notation)

printf("%u \n", $big_int + 1);

my $big_float = .10000000000000000001;

printf("%.16f \n", $big_float +
.100000000000001);

my $first = 1;
my $second = 2;
($first, $second) = ($second, $first);

say "$first $second";

say "5 + 4 = ", 5 + 4;
say "5 % 4 = ", 5 % 4;
say "5 ** 4 = ", 5 ** 4;
say "EXP 1 = ", exp 1;
say "HEX 10 = ", hex 10;
say "OCT 10 = ", oct 10;
say "INT 6.45 = ", int(6.45);
say "LOG 2 = ", log(2);
say "Random between 0 - 10 = ", int(rand 11);
say "SQRT 9 = ", sqrt 9;

