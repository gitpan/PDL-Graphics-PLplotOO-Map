# Makefile.PL for a package defined by PP code.

use PDL::Core::Dev;            # Pick up development utilities
use PDL::Bad;                  # check for bad value support
use ExtUtils::MakeMaker;
use Config;

# -- Add new subroutines here! --

my @src = qw(pscoast.c gmt_init.c gmt_map.c gmt_shore.c gmt_support.c);
my @obj = @src;
map {s/\.[fc]/\.o/;} @obj; # swap .f, .c for .o

my $install = $Config{'installsitearch'};

# 
## Search for netcdf library and include file
#

foreach my $libdir (
		    '/usr/local/netcdf/lib',
		    '/usr/local/lib',
		    '/opt/local/lib',
		    '/usr/lib',
		    '/opt/lib',
		                  # Add new library paths here!!
		    ) {

  if (-e "$libdir/libnetcdf.so") {
    $netcdf_lib_path = $libdir;
    print "Found libnetcdf.so at $libdir/libnetcdf.so\n";
    last;
  }

  if (-e "$libdir/libnetcdf.a") {
    $netcdf_lib_path = $libdir;
    print "Found libnetcdf.a at $libdir/libnetcdf.a\n";
    last;
  }

}

die "Cannot find netcdf library, libnetcdf.so or libnetcdf.a.  
Please add the correct library path to Makefile.PL or install NetCDF" 
  unless defined ($netcdf_lib_path);

foreach my $incdir (
		    '/usr/local/netcdf/include',
		    '/usr/local/include',
		    '/opt/local/include',
		    '/usr/include',
		    '/opt/include',
		                  # Add new header paths here!!
		    ) {

  if (-e "$incdir/netcdf.h") {
    $netcdf_include_path = $incdir;
    print "Found netcdf.h at $incdir/netcdf.h\n";
    last;
  }
}

die "Cannot find netcdf header file, netcdf.h.
Please add the correct library path to Makefile.PL or install NetCDF" 
  unless defined ($netcdf_include_path);

# Make sure bad value support is compiled in.
die "PDL::Graphics::PLplotOO::Map *requires* bad value support (Set WITH_BADVAL => 1 in perldl.conf)"
  if (!$PDL::Bad::Status);

# Check if compiled under gcc/Linux.  In which case, define bool for the compiler
$define_bool = '';
if ($Config{'osname'} =~ linux) {
  $define_bool = '-Dbool=int';
  print "Defining bool=int (linux seems to need this)\n";
}
            
my %pmfiles = map { $_ => "\$(INST_LIBDIR)/Map/$_" } glob ("binned*cdf");
$pmfiles{'Map.pm'} = '$(INST_LIBDIR)/Map.pm';

#print "pmfiles = \n";
#for (keys %pmfiles) { print "$_ => $pmfiles{$_}\n"; }

$package = ["map.pd",Map,PDL::Graphics::PLplotOO::Map];
WriteMakefile(
	      'NAME'  	     => 'PDL::Graphics::PLplotOO::Map',
	      'CCFLAGS'      => "$define_bool",
              'DEFINE'       => 
	      "-D_SVID_SOURCE -DGMT_DEFAULT_PATH=\\\".\\\" -DGMT_INSTALL_PATH=\\\"$install/PDL/Graphics/PLplotOO/Map\\\"",  
	      'VERSION_FROM' => 'map.pd',
	      'OBJECT'       => 'Map.o ' . join (" ", @obj),
	      'PM'           => \%pmfiles,
#             'OPTIMIZE'     => '-g',
	      'INC'          => &PDL_INCLUDE()." -I$netcdf_include_path -I./include", 
	      'LIBS'         => ["-L$netcdf_lib_path -lnetcdf"],  
	      'clean'        => {'FILES'  => 
				   'Map.pm Map.xs Map.o Map.c pscoast.o testmap.png'},
	      'dist'         => { COMPRESS => 'gzip', SUFFIX => 'gz' },	
	      );

sub MY::postamble { pdlpp_postamble($package); }	








