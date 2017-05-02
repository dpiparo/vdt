set CD=$PWD
echo $CD
pushd $TMPDIR
source /cvmfs/sft.cern.ch/lcg/views/LCG_88/x86_64-slc6-gcc62-opt/setup.csh
# source /cvmfs/sft.cern.ch/lcg/views/LCG_88/x86_64-centos7-gcc62-opt/setup.csh
git clone https://github.com/dpiparo/vdt.git
cd vdt/src/
python numpy_wrapper_generator.py
c++ -Ofast -shared -fPIC -Wall -o libvdtFatLibWrapper.so vdtFatLibWrapper.cc -I../include/
python setup.py build_ext --inplace
python ../progs/testExp.py
cp -r vdtnpfun_directory $CD/.
cp  libvdtFatLibWrapper.so $CD/.
cp vdt.py  $CD/.
cp ../progs/testExp.py $CD/.
popd
python testExp.py

