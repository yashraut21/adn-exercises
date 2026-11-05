# Task 3 — SSH port forwarding and X11

The exercise's network topology is:

```text
Your Mac (localhost:7070)
        |
        | SSH tunnel
        v
faui7t1.informatik.uni-erlangen.de:22
        |
        | forwards to
        v
faui7xx:22
```

Replace `faui7xx` with the actual remote host assigned to you and `<username>` with your own CS7 account. The slide's `faui746` and `adn34` are examples; do not assume they are your credentials.

## 1. Check whether local port 7070 is free

```bash
lsof -nP -iTCP:7070 -sTCP:LISTEN
```

No output generally means nothing is listening on that port. If it is occupied, choose another unused local port and use it consistently below.

## 2. Establish the tunnel

```bash
ssh -N -L 7070:faui7xx:22 <username>@faui7t1.informatik.uni-erlangen.de -o ServerAliveInterval=60
```

Keep this terminal open. `-N` means no remote shell is opened; this process only maintains the tunnel.

## 3. Open a remote terminal through the local forwarded port

Open a second terminal:

```bash
ssh -XC -p 7070 <username>@localhost
```

`-X` requests X11 forwarding and `-C` enables compression. Accept the host key only if it is expected, and authenticate using your university account.

## 4. Test X11 forwarding

On the remote shell:

```bash
echo "$DISPLAY"
firefox
```

A non-empty `DISPLAY` indicates that the X11 display variable was set. Firefox should open on your local desktop if an X server is available and the remote environment supports it.

On macOS, install and start XQuartz before connecting if your setup requires a local X server. Log out and restart the terminal after installation if `DISPLAY` is not set.

## 5. Stop the tunnel

Return to the first terminal and press `Ctrl+C`.

## Troubleshooting

- `Address already in use`: choose another local port, e.g. `7071`.
- `Connection refused`: verify the jump host, target host, and that the target SSH service is reachable from the jump host.
- Authentication failure: check your actual university username and account access.
- No Firefox window: check XQuartz, `echo "$DISPLAY"`, and whether Firefox is installed on the remote host.
