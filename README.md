MatchLib
========
[![Build Status](https://travis-ci.org/NVlabs/matchlib.svg?branch=master)](https://travis-ci.org/NVlabs/matchlib)

MatchLib is a SystemC/C++ library of commonly-used hardware functions and components that can
be synthesized by most commercially-available HLS tools into RTL.

Doxygen-generated documentation can be found [here](https://NVlabs.github.io/matchlib/).
Additional documentation on the Connections
latency-insensitive channel implementation can be found in the [Connections Guide](doc/connections-guide.pdf).

# Getting Started

## Tool versions

MatchLib is regressed against the following tool/dependency verions:

* `gcc` - 4.9.3 (with C++11)
* `systemc` - 2.3.1
* `boost` - 1.55.0
* `doxygen` - 1.8.11
* `make` - 3.82
* `catapult` - 10.4b
* `vcs` - 2017.03-SP2-11
* `verdi` - 2017.12-SP2-11
* `rapidjson` - v1.1.0
* `python` - 3.4.2

## Environment requirements

Makefiles assume the appropriate definition of the following environment variables:

* `BOOST_HOME`
* `SYSTEMC_HOME`
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
* `connections` factors out the Connections latency-insensitive channel library components

# Preprocessor Macro Definitions

* `HLS_CATAPULT` - Select if Catapult is selected as the HLS target. Catapult header files will not be included if not set. If enabled, NVINT is defined as ac_int. If disabled, NVINT is defined as sc_int. NOTE: Currently MatchLib only supports Catapult, so HLS_CATAPULT must be set.
* `HLS_ALGORITHMICC` - Set to enable AlgorithmicC-specific optimizations in the code.
* `HLS_STRATUS` - Set to indicate that HLS is being performed with Stratus tool. This disables Catapult-specific flags and optimizations like the use of ac_types. This feature is not fully tested.
* `SC_INCLUDE_DYNAMIC_PROCESSES` - Set to enable SystemC dynamic processes.
* `__SYNTHESIS__` - Set by Catapult during the HLS flow (but not during sc_verify).
* `_SYNTHESIS_` - Flag to set when calling the HLS tool during both HLS and sc_verify.
* `AUTO_PORT` - Set Connections simulation port type (legal options are TLM_PORT, DIRECT_PORT, MARSHALL_PORT, SYN_PORT, from least to most accurate). If unset, defaults to SYN_PORT for synthesis, MARSHALL_PORT for CONNECTIONS_ACCURATE_SIM, and TLM_PORT for CONNNECTIONS_FAST_SIM. Always sets to SYN_PORT for HLS, unless if FORCE_AUTO_PORT is set. CONNECTIONS_ACCURATE_SIM and CONNECTIONS_FAST_SIM is favored over this flag.
* `FORCE_AUTO_PORT` - Like AUTO_PORT, but forces to a set port type for HLS too.
* `CONNECTIONS_ACCURATE_SIM` - Set to enable sim-accurate mode in SystemC simulation of the Connections library. This can be used to accurately estimate the performance of SystemC designs before HLS.
* `CONNECTIONS_FAST_SIM` - Set to enable fast sim mode in SystemC simulation of the Connections library. This reduces simulation runtime at the cost of cycle accuracy and I/O port fidelity. In particular, the simulation becomes event driven, and a TLM fifo replaces ports and combinational conncetions.
* `CONNECTIONS_SIM_ONLY` - Set this during simulation to enable sim-accurate mode. It should be disabled during HLS. CONNECTIONS_ACCURATE_SIM and CONNECTIONS_FAST_SIM automatically sets this.
* `CONN_RAND_STALL` - Set to enable random stall injection in Connections library by default.
* `CONN_RAND_STALL_PRINT_DEBUG` - Set to enable random stall print debug messages in Connections library by default.
* `CONNECTIONS_ASSERT_ON_QUERY` - Set to display a message when Connections features not supported by the tool are used. Specifically, it is used for Empty(), Peek(), and Full() checks in Connections with the Catapult tool.
* `DISABLE_PACER` - Set to disable the injection of additional stalls from the pacer logic.
* `RAND_SEED` - Set to a number to use as a fixed seed for easier debugging with nvhls_rand.
* `DEBUG_LEVEL` - Set to control the amount of debug information printed to the command line during execution. In general, higher debug levels will enable debug information from additional modules.
* `SKIP_LV2TYPE` - Set to enable complex types to be stored as logic vectors in certain design-specific cases.
* `COV_ENABLE` - Set to enable coverage collection with CTC.
* `NVHLS_VERIFY_ISVCSMX` - Set for standalone VCS-MX co-simulations of SystemC with Catapult-generated RTL. Do not use in SystemC simulation or Catapult sc_verify.
* `ENABLE_SYNC_RESET` - Enables synchronous, active-low reset instead of asynchronous, active-low reset in MatchLib.

# Command-line Simulation Settings

Many of the flags above are design-specific, and so are typically codified into design Makefiles. However, some of the variables pertain to different simulation modes, and it is desirable to simulate the same design under different settings for different purposes. Accordingly, we provide the following command-line environment flags for use with the cmod or hls steps:

* `SIM_MODE` - Set this variable to 1 (default) to use Connections sim-accurate mode, so that Connections ports and channels simulated in SystemC closely match the cycle-by-cycle behavior of their HLS-generated RTL counterparts. Set this variable to 2 to enable a TLM-based Connections mode that is faster to simulate but does not track the cycle behavior of HLS-generated RTL as closely. Set this variable to 0 to directly simulate the synthesized representation of Connections ports and channels (not recommended, as it may result in spurious failures).
* `RAND_STALL` - Set this variable to 1 to enable random stalling on Connections ports and channels. Set to 0 (default) to disable random stalling.

To accurately simulate expected RTL performance, use the default settings. For robust verification, simulate with four different modes: both `SIM_MODE=1` and `SIM_MODE=2`, with random stalling both enabled and disabled.

# Questions and Contributions

We welcome feedback and contributions from the open-source hardware community.
If you have a question or problem, please file an issue on GitHub.
To contribute bugfixes or new features, submit a pull request.

# Contributors

MatchLib originated as a project of [NVIDIA Research](https://research.nvidia.com).

Contributors to the initial open-source release (alphabetical): Jason Clemons, Christopher Fletcher, Davide Giri, Ben Keller, Brucek Khailany, Alicia Klinefelter, Evgeni Krimer, Hyoukjun Kwon, Ziyun Li, Michael Pellauer, Nathaniel Pinckney, Antonio Puglielli, Sophia Shao, Shreesha Srinath, Gopalakrishnan Srinivasan, Christopher Torng, Rangharajan Venkatesan, Sam Xi

Portions of MatchLib are derived from code in Mentor Graphics' Algorithmic C Datatypes v3.7.1 (also released under the Apache 2.0 license). See individual file headers for details.

MatchLib's back annotation feature is dependent on RapidJSON released under the MIT License. 

# Attribution

If used for research, please cite the [DAC paper](https://research.nvidia.com/sites/default/files/pubs/2018-06_A-Modular-Digital//dac2018.submitted.pdf):

Brucek Khailany, Evgeni Krimer, Rangharajan Venkatesan, Jason Clemons, Joel S. Emer, Matthew Fojtik, Alicia Klinefelter, Michael Pellauer, Nathaniel Pinckney, Yakun Sophia Shao, Shreesha Srinath, Christopher Torng, Sam (Likun) Xi, Yanqing Zhang, and Brian Zimmer, "A modular digital VLSI flow for high-productivity SoC design," in _Proceedings of the ACM/IEEE Design Automation Conference_, June 2018.
