# UCraft-printer
Run a Minecraft server on a **Samsung C410W** printer (firmware V3.00.02.20, DEC-15-2015).

## How it works

Two-stage exploit delivered over the network:

1. **UDP overflow** — sends a crafted `<WIRELESS_TEST>` packet that smashes the stack and executes `program_loader.bin` (a tiny TCP stager).
2. **TCP delivery** — the stager listens on port `25564`, receives `application.bin` (the UCraft Minecraft server), and spawns it as an RTOS task which starts the Minecraft server on port `25565`.

## Setup

```bash
./build_toolchain.sh                           # build crosstool-ng ARM big-endian toolchain
cmake -B build -DCMAKE_TOOLCHAIN_FILE=toolchain-armeb-eabi.cmake -G Ninja
ninja -C build
```

## Deploy

```bash
python3 run.py 192.168.1.132
```

The printer must be on the same network. `run.py` sends both stages automatically.

## Build artifacts

| File | Size | Purpose |
|---|---|---|
| `build/program_loader.bin` | ~1 KB | First-stage TCP stager (shellcode) |
| `build/application.bin` | ~1 MB | Second-stage Minecraft server (UCraft) |

## Notes

- `errno` is currently resolved to a dummy stub and is not useful.
- `U_gethostbyname` was not found in ROM, so the IP for `sessionserver.mojang.com` is hardcoded to `150.171.109.193`. Feel free to ping the auth server and replace it.
