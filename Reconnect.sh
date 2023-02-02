#source /cvmfs/sphenix.sdcc.bnl.gov/gcc-12.1.0/opt/sphenix/core/bin/sphenix_setup.csh –n

source /opt/sphenix/core/bin/sphenix_setup.csh -n new

setenv MYINSTALL $PWD/install/
setenv LD_LIBRARY_PATH $MYINSTALL/lib:$LD_LIBRARY_PATH
set path = ( $MYINSTALL/bin $path )

#setenv ROOT_INCLUDE_PATH /sphenix/user/zshi/EvtGenPullRequest/macros/common:$ROOT_INCLUDE_PATH

setenv ROOT_INCLUDE_PATH ${PWD}/macros/common:$ROOT_INCLUDE_PATH

