#!/bin/bash
source ../../config.sh
OS_NAME=$(uname)

\rm -f exe
\rm -f *.o

#runtime dynamic library path
RPATH="$(pwd)/../.."

# Build the main executable
echo "$CC $FLAGS $DEBUGFLAGS -I$(pwd)/../../src/ -c *.cpp"
$CC $FLAGS $DEBUGFLAGS -Wall -I$(pwd)/../../src/ -c *.cpp

case ${OS_NAME} in
   "Darwin")
      echo "$CC $FLAGS $DEBUGFLAGS -I$(pwd)/../../src -L$(pwd)/../.. -Wl,-rpath,${RPATH} -o exe *.o -l${LIBNAME}"
      $CC $FLAGS $DEBUGFLAGS -I$(pwd)/../../src -L$(pwd)/../.. -Wl,-rpath,${RPATH} -o exe *.o -l${LIBNAME}
      echo "install_name_tool -change libffnn.so ${RPATH}/libffnn.so exe"
      install_name_tool -change libffnn.so ${RPATH}/libffnn.so exe
      ;;
   "Linux")
      echo "$CC $FLAGS $DEBUGFLAGS -I$(pwd)/../../src -L$(pwd)/.. -Wl,-rpath=${RPATH} -o exe *.o -l${LIBNAME}"
      $CC $FLAGS $DEBUGFLAGS -I$(pwd)/../../src/ -L$(pwd)/../ -Wl,-rpath=${RPATH} -o exe *.o -l${LIBNAME}
      ;;
esac

echo "Rebuilt the executable file"
echo ""
echo ""

# Run the debugging executable
echo "Ready to run!"
echo ""
echo "--------------------------------------------------------------------------"
echo ""
echo ""
echo ""
./exe