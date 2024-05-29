MatchLib
========
[![Build Status](https://travis-ci.org/NVlabs/matchlib.svg?branch=master)](https://travis-ci.org/NVlabs/matchlib)

MatchLib is a SystemC/C++ library of commonly-used hardware functions and components that can
be synthesized by most commercially-available HLS tools into RTL.

Doxygen-generated documentation can be found [here](https://NVlabs.github.io/matchlib/).

MatchLib is based on the Connections latency-insensitive channel implementation.
Connections is included with the Catapult HLS tool and is available open-source on
[HLSLibs](https://github.com/hlslibs/matchlib_connections).
Additional documentation on the Connections latency-insensitive channel implementation
can be found in the [Connections Guide](https://github.com/hlslibs/matchlib_connections/blob/master/pdfdocs/connections-guide.pdf).

# Getting Started

## Tool versions

MatchLib is regressed against the following tool/dependency versions:

* `gcc` - 10.3.0
* `boost` - 1.68.0
* `doxygen` - 1.8.11
* `make` - 3.82
* `binutils` - 2.34
* `catapult` - 2024.1\_1
* `systemc` - 2.3.3 (included with Catapult)
* `connections` - 2.1.1 (included with Catapult)
* `vcs` - 2023.03
* `verdi` - 2023.03
* `python` - 3.4.2
* `rapidjson` - 06d58b9 (included as submodule)

## Environment requirements

Makefiles assume the appropriate definition of the following environment variables:

* `BOOST_HOME`
* `CATAPULT_HOME`
* `VCS_HOME`
* `NOVAS_INST_DIR`

In addition, the boost and systemc library locations are expected to be in `LD_LIBRARY_PATH`.

## Build and run

### C++ compile and simulate
    cd cmod/<module>
    make
    make run 

### C++ compile and simulate all
    cd cmod
    make -f regress_Makefile

### HLS run and Verilog simulate
    cd hls/<module>
    make

### HLS run and Verilog simulate all
    cd hls
    make -f regress_Makefile

### Design Checker run
    cd hls/<module>
    make cdc

### Design Checker run all
    cd hls
    make -f regress_Makefile RUN_CDESIGN_CHECKER=1

# Directory structure

* `cmod/include/*.h` contains header files for functions and classes from MatchLib
* `cmod/<module>` sub-directories contain SystemC modules from MatchLib
* `cmod/examples/<module>` sub-directories contain SystemC example modules
* `cmod/unittests/<module>` sub-directories contain SystemC wrappers, testbenches and tests for various MatchLib functions, classes, and modules
* `hls/<module>` sub-directories contain HLS scripts for modules
* `doc` contains Makefiles for building Doxygen-based documentation

# Preprocessor Macro Definitions

* `HLS_CATAPULT` - Select if Catapult is selected as the HLS target. Catapult header files will not be included if not set. If enabled, `NVINT` is defined as `ac_int`. If disabled, `NVINT` is defined as `sc_int`. Currently MatchLib only supports Catapult, so `HLS_CATAPULT` must be set.
* `HLS_ALGORITHMICC` - Set to enable AlgorithmicC-specific optimizations in the code.
* `HLS_STRATUS` - Set to indicate that HLS is being performed with Stratus tool. This disables Catapult-specific flags and optimizations like the use of ac_types. This feature is not fully tested.
* `SC_INCLUDE_DYNAMIC_PROCESSES` - Set to enable SystemC dynamic processes.
* `__SYNTHESIS__` - Set by Catapult during the HLS flow (but not during sc_verify).
* `_SYNTHESIS_` - Flag to set when calling the HLS tool during both HLS and sc_verify.
* `SKIP_LV2TYPE` - Set to enable complex types to be stored as logic vectors in certain design-specific cases.
* `COV_ENABLE` - Set to enable coverage collection with CTC.
* `NVHLS_VERIFY_ISVCSMX` - Set for standalone VCS-MX co-simulations of SystemC with Catapult-generated RTL. Do not use in SystemC simulation or Catapult sc_verify.
* `ENABLE_SYNC_RESET` - Enables synchronous, active-low reset instead of asynchronous, active-low reset in MatchLib.
* `NV_ARRAY_MAX_SPECIALIZATIONS` - Set to redefine the maximum allowed vector length for nv_arrays (default 64). Larger values may result in increased compile times.

Additional macros such as `AUTO_PORT`, `FORCE_AUTO_PORT`, `CONNECTIONS_ACCURATE_SIM`, `CONNECTIONS_FAST_SIM`, `CONNECTIONS_SIM_ONLY`, `CONN_RAND_STALL`, `CONN_RAND_STALL_PRINT_DEBUG`, `CONNECTIONS_ASSERT_ON_QUERY`, and `DISABLE_PACER` may be used in MatchLib. These macros are primarily defined, implemented, and documented in Connections. For more detail on these macros, see the [Connections documentation](https://github.com/hlslibs/matchlib_connections).

# Command-line Simulation Settings

Many of the flags above are design-specific, and so are typically codified into design Makefiles. However, some of the variables pertain to different simulation modes, and it is desirable to simulate the same design under different settings for different purposes. Accordingly, we provide the following command-line environment flags for use with the cmod or hls steps:

* `SIM_MODE` - Set this variable to 1 (default) to use Connections sim-accurate mode, so that Connections ports and channels simulated in SystemC closely match the cycle-by-cycle behavior of their HLS-generated RTL counterparts. Set this variable to 2 to enable a TLM-based Connections mode that is faster to simulate but does not track the cycle behavior of HLS-generated RTL as closely. Set this variable to 0 to directly simulate the synthesized representation of Connections ports and channels (not recommended, as it may result in spurious failures).
* `RAND_STALL` - Set this variable to 1 to enable random stalling on Connections ports and channels. Set to 0 (default) to disable random stalling.
* `DEBUG_LEVEL` - Set to control the amount of debug information printed to the command line during execution. In general, higher debug levels will enable debug information from additional modules.
* `NVHLS_RAND_SEED` - Set to a number to use as a fixed seed for nvhls_rand (defaults to 0).

To accurately simulate expected RTL performance, use the default settings. For robust verification, simulate with four different modes: both `SIM_MODE=1` and `SIM_MODE=2`, with random stalling both enabled and disabled and various random seeds.

# Questions and Contributions

We welcome feedback and contributions from the open-source hardware community.
If you have a question or problem, please file an issue on GitHub.
To contribute bugfixes or new features, submit a pull request.
For business inquiries, please contact researchinquiries@nvidia.com.
For press and other inquiries, please contact Hector Marinez at hmarinez@nvidia.com.

# Contributors

MatchLib originated as a project of [NVIDIA Research](https://research.nvidia.com).

Contributors to the initial open-source release (alphabetical): Jason Clemons, Christopher Fletcher, Davide Giri, Ben Keller, Brucek Khailany, Alicia Klinefelter, Evgeni Krimer, Hyoukjun Kwon, Ziyun Li, Michael Pellauer, Nathaniel Pinckney, Antonio Puglielli, Sophia Shao, Shreesha Srinath, Gopalakrishnan Srinivasan, Christopher Torng, Rangharajan Venkatesan, Sam Xi

Portions of MatchLib are derived from code in Mentor Graphics' Algorithmic C Datatypes v3.7.1 (also released under the Apache 2.0 license). See individual file headers for details.

MatchLib's back annotation feature is dependent on RapidJSON released under the MIT License. 

# Attribution

If used for research, please cite the [DAC paper](https://research.nvidia.com/sites/default/files/pubs/2018-06_A-Modular-Digital//dac2018.submitted.pdf):

Brucek Khailany, Evgeni Krimer, Rangharajan Venkatesan, Jason Clemons, Joel S. Emer, Matthew Fojtik, Alicia Klinefelter, Michael Pellauer, Nathaniel Pinckney, Yakun Sophia Shao, Shreesha Srinath, Christopher Torng, Sam (Likun) Xi, Yanqing Zhang, and Brian Zimmer, "A modular digital VLSI flow for high-productivity SoC design," in _Proceedings of the ACM/IEEE Design Automation Conference_, June 2018.
