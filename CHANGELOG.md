# Changelog

Documentation of all notable changes to the **sivmone** project.

The format is based on [Keep a Changelog],
and this project adheres to [Semantic Versioning].

## [Unreleased]

sivmone, the Sila Virtual Machine (Sivm) implementation, starts here.

### Changed

- The project, its libraries, headers, CMake targets and options, binaries and the
  exported `evmc_create_sivmone` symbol are named sivmone.
- Revisions are named after the Sila forks (`SilaHomestead` … `SilaAmsterdam`,
  `SIP150`, `SIP158`) through `sivm_revision_to_string()`; each revision has a single name.
- The benchmarks come from [sivm-benchmarks], generated for `SilaLondon`.
- Improvement proposals are referenced as SIPs.
- The vendored Keccak implementation is named silash.


[Unreleased]: https://github.com/sila-chain/sivmone/commits/master
[sivm-benchmarks]: https://github.com/sila-chain/sivm-benchmarks
[Keep a Changelog]: https://keepachangelog.com/en/1.1.0/
[Semantic Versioning]: https://semver.org
