# C2 (C Container)

教育目的のための小規模コンテナランタイム

## Getting Started

```
mkdir rootfs
docker export $(docker create ubuntu) | tar -C rootfs -xvf -
make
./bin/c2
```
