if !DEBUG
if !COVERAGE
  AM_CXXFLAGS += $(OPTFLAGS)
endif
endif

if COVERAGE
  AM_CPPFLAGS += $(COVERAGE_CPPFLAGS)
  AM_CXXFLAGS += $(COVERAGE_CXXFLAGS)
  AM_LDFLAGS += $(COVERAGE_LDFLAGS)
endif

if PROFILING
  AM_CXXFLAGS += $(PROF_CFLAGS)
  AM_LDFLAGS += $(PROF_LFLAGS)
endif

if OPENMP
  AM_CXXFLAGS += $(OMP_CFLAGS)
  AM_LDFLAGS += $(OMP_LFLAGS)
endif

lib_LTLIBRARIES = libffnn.la
include headers.am
include sources.am

if COVERAGE
clean-local:
	rm -f ../src/*/*.gcda
	rm -f ../src/*/*.gcno
	rm -f ../src/*/*.gcov
endif

source-lists:
	echo "pkginclude_HEADERS = \\" > headers.am
	find ../src/ -name *.hpp | tr '\n' ' ' >> headers.am
	echo "libffnn_la_SOURCES = \\" > sources.am
	find ../src/ -name *.cpp | tr '\n' ' ' >> sources.am

.PHONY: source-lists
