# playground
Everything. Everywhere. All at once. Useless.

## Build inside container to ensure it works correctly on immutable systems

Build container image

```bash
podman build -t playground:latest .
```

Run container from a subfolder

```bash
podman run -it --rm -v $(pwd):/app:z playground:latest
```
