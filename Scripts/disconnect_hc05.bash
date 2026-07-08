#!/usr/bin/env bash
set -euo pipefail

HC05_ADDR="${1:-00:14:03:05:09:9E}"

echo "Disconnecting HC-05 at $HC05_ADDR..."

echo "1. Killing any process using /dev/rfcomm devices..."
for dev in /dev/rfcomm*; do
    if [ -e "$dev" ]; then
        sudo fuser -k "$dev" 2>/dev/null || true
    fi
done

echo "2. Releasing RFCOMM bindings..."
for i in {0..9}; do
    sudo rfcomm release "$i" 2>/dev/null || true
done

echo "3. Disconnecting Bluetooth device..."
bluetoothctl disconnect "$HC05_ADDR" 2>/dev/null || true

echo "4. Removing stale rfcomm device files if present..."
for dev in /dev/rfcomm*; do
    if [ -e "$dev" ]; then
        sudo rm -f "$dev" 2>/dev/null || true
    fi
done

echo "Done. HC-05 should now be disconnected from this laptop."
echo "You can now power-cycle the HC-05 and connect from your phone."