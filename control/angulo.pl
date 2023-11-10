#!/usr/bin/perl
use strict;
use warnings;
use Math::Complex;

if (@ARGV != 4) {
    die "Usage: $0 x1 y1 x2 y2\n";
}

my ($x1, $y1, $x2, $y2) = @ARGV;

# Función para determinar el cuadrante de un vector
sub determinar_angulo {
    my ($x, $y) = @_;

    my $cuadrante;
    my $angulo;

    if ($x > 0 && $y > 0) {
        $cuadrante = 1;
        $angulo = atan($y / $x);
    } elsif ($x < 0 && $y > 0) {
        $cuadrante = 2;
        $angulo = pi - atan($y / ($x * -1));
    } elsif ($x < 0 && $y < 0) {
        $cuadrante = 3;
        $angulo = pi + atan(($y * -1) / ($x * -1));
    } elsif ($x > 0 && $y < 0) {
        $cuadrante = 4;
        $angulo = 2*pi - atan(($y * -1) / $x);
    } else {
        $cuadrante = "Origen";
    }

    return $angulo;
}

# Procesar cada vector
    my $ang1= (determinar_angulo($x1, $y1)* 180)/pi;
    my $ang2= (determinar_angulo($x2, $y2)* 180)/pi;
    my $difang= $ang1 - $ang2;
    #print "El vector ($x1, $y1) tiene un angulo de  $ang1.\n";
    #print "El vector ($x2, $y2) tiene un angulo de  $ang2.\n";
    my $difent=int($difang);
    print "$difent\n";

