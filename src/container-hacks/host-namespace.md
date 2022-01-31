# Host Namespace

コンテナは [Linux Namespaces](../containerization/linux-container.md) によって様々なリソースを分離している．
Namespace にはいろんな種類があるが，Dockerでは，Namespace の一部をホストと共有する事ができる．

## `--net=host`

`--net=host` オプションを用いて，Network Namespace をホストと同様のものを使ってコンテナを作成することができる．
コンテナ内からホストのNICやルーティングテーブルを見ることができる．

```bash
docker container run --rm -it guni1192/debug-tools ip addr
```

```bash
docker container run --rm --net=host -it guni1192/debug-tools ip addr
```

## `--uts=host`

`--uts=host` オプションを用いて，UTS Namespace をホストと同様のものを使ってコンテナを作成することができる．
コンテナ内からホストのhostnameを参照したり，変更できる．

```bash
docker container run --rm -it guni1192/debug-tools hostname
```

```bash
docker container run --rm --uts=host -it guni1192/debug-tools hostname
```

## `--pid=host`

`--pid=host` オプションを用いて，PID Namespace をホストと同様のものを使ってコンテナを作成することができる．
コンテナ内からホストと同じ `/proc` の情報を見ることができる．

```bash
docker container run --rm -it guni1192/debug-tools ps auxf
```

```bash
docker container run --rm --pid=host -it guni1192/debug-tools ps auxf
```

**Note**: これらのオプションはコンテナ内からホストのリソースを利用できてしまうため，
セキュリティホールになりやすい．そのため，できるだけ利用しないほうが良い．
