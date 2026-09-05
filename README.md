# vey

A high-performance network tool and IP stresser.

## Usage

The application uses a sub-command structure. To target a network host, use the `inet` command.

```bash
vey.exe inet [ip] [port] [protocol] [options]
```

### Protocol Flags (Required — Mutually Exclusive)
* `-t, --tcp` : Use TCP protocol.
* `-u, --udp` : Use UDP protocol.

### Optional Flags
* `-s, --socket-count` : Number of concurrent sockets to use (default: `1`).
* `-p, --packet-size`  : Packet size in bytes (default: `1024`).
* `-v4, --ipv4`        : Force IPv4 addressing.
* `-v6, --ipv6`        : Force IPv6 addressing.
* `--verbose`          : Enable detailed logging / verbose output.

### Examples

**Basic UDP test with default values:**
```bash
vey.exe inet 192.168.0.1 80 --udp
```

**Advanced TCP test with 5 sockets and 2048-byte packets:**
```bash
vey.exe inet 192.168.0.1 152 --tcp -s 5 -p 2048
```

---

## Build Instructions

```bash
cd vey
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

### Build Options (CMake)

You can customize the build by passing variables to CMake via `-D<OPTION>=ON|OFF`.

* `VEY_OPTIMIZE_ON_RELEASE` (Default: `OFF`)
  Enables aggressive compiler optimization flags when building in Release mode.

* `VEY_STATIC_ON_RELEASE` (Default: `OFF`)
  Links libraries statically, eliminating external dynamic library dependencies for the release binary.

**Example with all options enabled:**
```bash
cmake .. -DCMAKE_BUILD_TYPE=Release -DVEY_OPTIMIZE_ON_RELEASE=ON -DVEY_STATIC_ON_RELEASE=ON
```
