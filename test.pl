# Before `make install' is performed this script should be runnable with
# `make test'. After `make install' it should work as `perl test.pl'

######################### We start with some black magic to print on failure.

# Change 1..1 below to 1..last_test_to_print .
# (It may become useful if the test is moved to ./t subdirectory.)

BEGIN { $| = 1; print "1..11\n"; }
END {print "not ok 1\n" unless $loaded;}
use PDL;
use PDL::Graphics::PLplotOO::Map;
$loaded = 1;
print "ok 1\n";

######################### End of black magic.

# Insert your test code below (better if it prints "ok 13"
# (correspondingly "not ok 13") depending on the success of chunk 13
# of the test code):

sub ok ($$) {
  my $bool = shift;
  my $n    = shift;
  if ($bool) {
    print "ok $n\n";
  } else {    print "not ok $n\n";
  }
}


my ($lon, $lat) = PDL::Graphics::PLplotOO::Map::fetch(RESOLUTION => 'crude');
my $pl = PDL::Graphics::PLplotOO->new (DEV => "png", FILE => "test2.png");
$pl->xyplot($lon, $lat);
$pl->close;
ok -s "test2.png" > 0, 2;

my $lat100 = pdl [79.15938, 79.621118,   79.3608, 80.058747,  78.90547, 79.788663, 79.263752, 79.579919, 79.877165, 79.546654, 78.204471, 78.850538, 78.457771, 78.716258, 78.429389, 78.056764, 77.716182, 77.929198, 77.653315, 77.459525, 77.524224, 76.999924, 76.999924,  76.59617,  78.42237, 78.548714,-999, 80.429236, 80.505531, 80.136263, 80.125887, 79.711147, 79.749905,-999, 78.526741, 78.206912, 78.903639, 78.527352, 78.526741,-999,70, 70.172427, 70.069581, 70.397345, 70.884718, 70.426947, 70.970169, 71.131304, 70.463264, 70.358282, 70.968032, 70.065309, 70.874037, 70.992142,70,-999, 79.749905, 79.802396, 79.452964, 79.181353, 79.943389,  79.94461, 80.346227, 80.103609, 80.504616,  79.97116, 80.429236,-999, 77.623713, 78.253605, 78.113222, 77.438773, 77.614557, 77.237049, 77.623713, 77.623713,-999, 78.204471, 78.612802, 78.480049, 78.205692, 78.204471,-999, 70.970474, 71.097429, 70.970474, 70.970474,-999,  70.33875, 70.621347,  70.33875,  70.33875,-999, 78.838636, 78.907607, 78.838636, 78.838636,-999, 70.266728, 70.334173];

my $tol = 1e-4;
ok ((all approx $lat100, $lat->slice("1:100"), $tol), 3);

my @bounds = (-180, 0, -90, 0);
my ($lon, $lat) = PDL::Graphics::PLplotOO::Map::fetch(RESOLUTION => 'low',
                                                      BOX        => \@bounds,
                                                      BOUNDARIES => [1],
                                                      COASTS     => 1);

my $pl = PDL::Graphics::PLplotOO->new (DEV => "png", FILE => "test4.png");
$pl->xyplot($lon, $lat);
$pl->close;
ok -s "test4.png" > 0, 4;

my @bounds = (0, 180, 0, 90);
my ($lon, $lat) = PDL::Graphics::PLplotOO::Map::fetch(RESOLUTION => 'low',
                                                      BOX        => \@bounds,
                                                      BOUNDARIES => [1],
                                                      COASTS     => 1);

my $pl = PDL::Graphics::PLplotOO->new (DEV => "png", FILE => "test6.png");
$pl->xyplot($lon, $lat);
$pl->close;
ok -s "test6.png" > 0, 6;

my $lat100 = pdl [80.40238, 80.505379, 80.204166,  80.33341, 80.136263,   80.0795, 80.202029, 80.125887, 80.020447, 80.043336,80,-99999, 80.282597, 80.233005, 80.299992, 80.363012, 80.290837, 80.282597,-99999, 80.536202, 80.536202,  80.46952, 80.536049,-99999,80, 80.058747,80,-99999,80, 80.009613,80,-99999,80, 80.150759,  80.15198,  80.27039, 80.241703, 80.346227, 80.243839, 80.314183, 80.347143, 80.269169, 80.278782, 80.121614,  80.19379, 80.103761, 80.420844, 80.504616, 80.311284, 80.416571, 80.396582,80,-99999,80, 80.110018, 80.110018, 80.260777, 80.196231, 80.404974, 80.384527,  80.40238,-99999, 80.128328, 80.135805, 80.141604, 80.128328,-99999, 80.649882, 80.615854, 80.654154, 80.649882,-99999, 80.384222, 80.443732, 80.389258, 80.384222,-99999, 80.668193,  80.67155, 80.674907, 80.668193,-99999, 80.536049, 80.536202, 80.536202,-99999, 80.077058, 80.220035, 80.085756, 80.076295, 80.077058,-99999, 80.132906, 80.171664, 80.132906, 80.132906,-99999, 80.923476, 80.780804, 80.674907];


ok ((all approx $lat100, $lat->slice("1:100"), $tol), 7);

#---------------------
# Start of plots
#---------------------

my $pl = PDL::Graphics::PLplotOO::Map->new (DEV => "png", FILE => "test8.png");
$pl->worldmap;
$pl->close;
ok -s "test8.png" > 0, 8;

my $pl = PDL::Graphics::PLplotOO::Map->new (DEV => "png", FILE => "test9.png");
$pl->worldmap (PROJECTION => 'AZEQDIST', # Azimuthal Equidistant projection
               CENTER  => [-170, 70],    # map centered at 170 deg west lon, 70 deg north lat
               RADIUS  => 3000);         # 3000 kilometer minimum radius
$pl->close;
ok -s "test9.png" > 0, 9;

# equatorial case
my $pl = PDL::Graphics::PLplotOO::Map->new (DEV => "png", FILE => "test10.png");
$pl->worldmap (PROJECTION => 'AZEQDIST', # Azimuthal Equidistant projection
               CENTER  => [-10,  20],    # map centered at 170 deg west lon, 70 deg north lat
               RADIUS  => 3000,          # 3000 kilometer minimum radius
	       RESOLUTION => 'low',
	       LONGRID => 10,            # longitude grid lines every 10 degrees
	       LATGRID => 10);           # latitude grid lines every 10 degrees

# plot projection center point
$pl->map_plot (pdl([-10]), pdl([20]), PLOTTYPE => 'POINTS',
                                      PROJECTION => 'AZEQDIST',
                                      CENTER  => [-10,  20], COLOR => 'GREEN');
$pl->close;
ok -s "test10.png" > 0, 10;

# polar case near date line
my $pl = PDL::Graphics::PLplotOO::Map->new (DEV => "png", FILE => "test11.png");
$pl->worldmap (PROJECTION => 'AZEQDIST', # Azimuthal Equidistant projection
               CENTER  => [-170,  80],   # map centered at 170 deg west lon, 80 deg north lat
               RADIUS  => 3000,          # 3000 kilometer minimum radius
	       RESOLUTION => 'low',
	       LONGRID => 20,            # longitude grid lines every 10 degrees
	       LATGRID => 10);           # latitude grid lines every 10 degrees

$pl->map_plot (pdl(-170, -160), pdl(80, 90), PLOTTYPE => 'LINE', 
                                             PROJECTION => 'AZEQDIST', 
                                             CENTER  => [-170,  80], COLOR => 'PINK');

$pl->close;
ok -s "test11.png" > 0, 11;












