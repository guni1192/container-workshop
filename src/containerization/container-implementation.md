# コンテナの実装

## 実装1. Bashで作るコンテナ

```bash
mkdir rootfs
docker export $(docker create ubuntu:20.04) | tar -C rootfs -xvf -
unshare --mount --uts --net --pid --user --fork /bin/bash
chroot rootfs /bin/bash
mount -t proc proc /proc
```

psコマンドでプロセスが隔離されていることを確認しよう

## 実装2. C言語で作るコンテナ

コンテナのミニマム実装の例: [c2](https://github.com/sai-lab/c2)

```c
{{#include ../../samples/c2/src/main.c}}
```


## その他の実装例

- jail
- lxc/lxd
- docker
- podman
