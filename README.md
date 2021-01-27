# check_subnet
This is a small command line utility to check the  if IP belongs to subnet or not.

#How to compile
gcc -g check_subnet.c  -o check_subnet


#Usage:
./check_subnet <prefix:IPv4 prefix (IP/length)>  <IPv4 Address to check> 


#Example:
./check_subnet 192.168.0.12/24 192.168.0.23
Prefix Match = YES


./check_subnet 192.168.0.12/24 192.168.1.23
Prefix Match = NO
