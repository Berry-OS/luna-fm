#!/bin/sh
set -eu
ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)
OUT=${1:-/tmp/luna-files.ppm}
START=${2:-${HOME:-/tmp}}
CC=${CC:-cc}
BIN=${TMPDIR:-/tmp}/luna-fm-screenshot-$$
trap 'rm -f "$BIN"' EXIT INT TERM
"$CC" -O2 -pipe -std=c11 -I"$ROOT/tools/screenshot-glfw" -I"$ROOT" \
  -o "$BIN" "$ROOT/luna-fm.c" "$ROOT/tools/screenshot-glfw/glfw_capture.c" \
  -l:libGL.so.1 -ldl -lm -lpthread
LUNA_CAPTURE_PATH="$OUT" \
LUNA_CAPTURE_WIDTH="${LUNA_CAPTURE_WIDTH:-1080}" \
LUNA_CAPTURE_HEIGHT="${LUNA_CAPTURE_HEIGHT:-620}" \
LUNA_CAPTURE_FRAME="${LUNA_CAPTURE_FRAME:-14}" \
"$BIN" --client-chrome "$START"
echo "wrote $OUT"
