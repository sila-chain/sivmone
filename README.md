# sivmone

[![ci badge]][ci]
[![readme style standard badge]][standard readme]
[![codspeed badge]][codspeed]
[![license badge]][Apache License, Version 2.0]

> Fast Sila Virtual Machine implementation

_sivmone_ is a C++ implementation of the Sila Virtual Machine (Sivm).
The project aims for a clean, standalone Sivm implementation
that can be imported as an execution module by Sila client projects.
The codebase of _sivmone_ is optimized to provide fast and efficient execution of Sivm smart contracts.

### Characteristic of sivmone

1. Exposes the [SIVMC] API.
2. Requires C++20 standard.
3. The [intx] library is used to provide 256-bit integer precision.
4. The silash Keccak hash function implementation (in `lib/sivmone_precompiles`) is used
   for the special `KECCAK256` instruction.
5. Contains two interpreters: 
   - **Baseline** (default)
   - **Advanced** (select with the `advanced` option)

### Baseline Interpreter

1. Provides relatively straight-forward but efficient Sivm implementation.
2. Performs only minimalistic `JUMPDEST` analysis.

### Advanced Interpreter

1. The _indirect call threading_ is the dispatch method used -
   a loaded Sivm program is a table with pointers to functions implementing virtual instructions.
2. The gas cost and stack requirements of block of instructions is precomputed 
   and applied once per block during execution.
3. Performs extensive and expensive bytecode analysis before execution.


## Usage

### As an SIVMC module

sivmone implements the [SIVMC] API. The shared library `libsivmone.so` exports
`sivmc_create_sivmone()` and can be loaded by any client with an SIVMC loader.

Prebuilt packages are published on [Releases].

### Building from source

To build the sivmone SIVMC module (shared library), test, and benchmark:

1. Fetch the source code:
   ```
   git clone --recursive https://github.com/sila-chain/sivmone
   cd sivmone
   ```

2. Configure the project build and dependencies:
   ##### Linux / OSX
   ```
   cmake -S . -B build -DSIVMONE_TESTING=ON
   ```

   ##### Windows
   ```
   cmake -S . -B build -DSIVMONE_TESTING=ON -G "Visual Studio 16 2019" -A x64
   ```
   
3. Build:
   ```
   cmake --build build --parallel
   ```


3. Run the unit tests or benchmarking tool:
   ```
   build/bin/sivmone-unittests
   build/bin/sivmone-bench test/sivm-benchmarks/benchmarks
   ```

### Precompiles

Sila Precompiled Contracts (_precompiles_ for short) are supported by sivmone with some exceptions:

1. The `ecrecover` is implemented directly by sivmone and has degraded performance.
2. For `expmod` stubs are enabled by default — they will correctly respond to known inputs. The CMake option `SIVMONE_PRECOMPILES_GMP=1` enables full implementation but this requires [GMP] (e.g. libgmp-dev) library at build and execution time.

### Docker

A Docker image with sivmone can be built from the [Dockerfile](Dockerfile).

Having the sivmone shared library inside a docker is not very useful on its own,
but the image can be used as the base of another one or you can run benchmarks 
with it.

```bash
docker build -t sivmone .
docker run --entrypoint sivmone-bench sivmone /src/test/sivm-benchmarks/benchmarks
```

## References

1. [Efficient gas calculation algorithm for Sivm](docs/efficient_gas_calculation_algorithm.md)

## Maintainer

[sila-chain]

## License

[![license badge]][Apache License, Version 2.0]

Licensed under the [Apache License, Version 2.0].


[sila-chain]: https://github.com/sila-chain
[ci]: https://github.com/sila-chain/sivmone/actions/workflows/ci.yml
[codspeed]: https://app.codspeed.io/sila-chain/sivmone
[Apache License, Version 2.0]: LICENSE
[SIVMC]: https://github.com/sila-chain/sivmc
[GMP]: https://gmplib.org
[intx]: https://github.com/chfast/intx
[Releases]: https://github.com/sila-chain/sivmone/releases
[standard readme]: https://github.com/RichardLitt/standard-readme
[silkpre]: https://github.com/torquem-ch/silkpre

[ci badge]: https://github.com/sila-chain/sivmone/actions/workflows/ci.yml/badge.svg
[codspeed badge]: https://img.shields.io/endpoint?url=https://codspeed.io/badge.json
[license badge]: https://img.shields.io/github/license/sila-chain/sivmone.svg?logo=apache
[readme style standard badge]: https://img.shields.io/badge/readme%20style-standard-brightgreen.svg
