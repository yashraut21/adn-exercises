#!/usr/bin/env bash
set -euo pipefail

# Usage: ./port-forward.sh <username> <remote-host> [local-port]
# Example: ./port-forward.sh myuser faui7xx 7070

if [[ $# -lt 2 || $# -gt 3 ]]; then
    echo "Usage: $0 <username> <remote-host> [local-port]" >&2
    exit 2
fi

USERNAME="$1"
REMOTE_HOST="$2"
LOCAL_PORT="${3:-7070}"
JUMP_HOST="faui7t1.informatik.uni-erlangen.de"

if lsof -nP -iTCP:"$LOCAL_PORT" -sTCP:LISTEN >/dev/null 2>&1; then
    echo "Local port $LOCAL_PORT is already in use. Choose another port." >&2
    exit 1
fi

echo "Forwarding localhost:$LOCAL_PORT to $REMOTE_HOST:22 via $JUMP_HOST"
echo "Keep this process running. In another terminal, run:"
echo "ssh -XC -p $LOCAL_PORT $USERNAME@localhost"
exec ssh -N -L "$LOCAL_PORT:$REMOTE_HOST:22" "$USERNAME@$JUMP_HOST" -o ServerAliveInterval=60
