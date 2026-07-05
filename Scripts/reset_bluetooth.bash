#!/usr/bin/env bash
set -euo pipefail

RFCOMM_NUM=1
BT_ADDR="00:14:03:05:09:9E"
BT_CHANNEL=1
DEV="/dev/rfcomm${RFCOMM_NUM}"
BAUD=9600

sudo rfcomm release "${RFCOMM_NUM}" 2>/dev/null || true

echo "Binding ${DEV} to ${BT_ADDR} channel ${BT_CHANNEL}..."
sudo rfcomm bind "${RFCOMM_NUM}" "${BT_ADDR}" "${BT_CHANNEL}"

if [ -c "${DEV}" ]; then
    echo "OK: ${DEV} exists"
    ls -l "${DEV}"
    sudo stty -F "${DEV}" "${BAUD}" cs8 -cstopb -parenb raw -echo

    echo "Ready. Test with:"
    echo "  printf 'f' > ${DEV}"
    echo "  printf 's' > ${DEV}"
    echo "  picocom -b ${BAUD} ${DEV}"
else
    echo "ERROR: ${DEV} was not created"
    exit 1
fi