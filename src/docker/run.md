# Dockerでいろんなコンテナを実行する

実際に`docker container run` コマンドを使用してコンテナを実行する．
今回実行するコンテナイメージはすべて [DockerHub](https://hub.docker.com/) のものを用いる．

- Ubuntu
- Nginx
- Jupyter

サンプルファイルなどのダウンロード
```bash
git clone https://github.com/guni1192/container-workshop.git
cd container-workshop
```

演習環境のVMを構築
```bash
vagrant up
vagrant ssh
cd samples/
```

フォルダ構成は以下のようになっている
```
samples
├── docker
│   ├── go-webapp
│   │   ├── Dockerfile
│   │   ├── go.mod
│   │   ├── go.sum
│   │   ├── main.go
│   │   └── Makefile
│   └── jupyter
│       └── sample.ipynb
├── kubernetes
│   └── samples
└── README.md
```

## 実行環境

- OS: Fedora 35
- Kernel: Linux x86-64 5.14

```
[vagrant@fedora ~]$ docker version
Client: Docker Engine - Community
 Version:           20.10.10
 API version:       1.41
 Go version:        go1.16.9
 Git commit:        b485636
 Built:             Mon Oct 25 07:43:46 2021
 OS/Arch:           linux/amd64
 Context:           default
 Experimental:      true

Server: Docker Engine - Community
 Engine:
  Version:          20.10.10
  API version:      1.41 (minimum version 1.12)
  Go version:       go1.16.9
  Git commit:       e2f740d
  Built:            Mon Oct 25 07:41:31 2021
  OS/Arch:          linux/amd64
  Experimental:     false
 containerd:
  Version:          1.4.11
  GitCommit:        5b46e404f6b9f661a205e28d59c982d3634148f8
 runc:
  Version:          1.0.2
  GitCommit:        v1.0.2-0-g52b36a2
 docker-init:
  Version:          0.19.0
  GitCommit:        de40ad0
```


## Ubuntuの実行

Ubuntuのイメージを使用したコンテナでbashを起動する
```bash
                            ↓ コンテナイメージ
docker container run -it ubuntu:20.04 bash
                                        ↑ コンテナ内で実行するコマンド
```

今回はbashのような対話シェルを実行するため，以下のようなオプションが必要になる．
```
-i, --interactive    Keep STDIN open even if not attached
-t, --tty            Allocate a pseudo-TTY
```

### 演習: コンテナの内部と外部の比較

以下のコマンドをコンテナ内とコンテナ外で実行して比べてみよう
1. `uname -r` : カーネルのバージョンを表示
1. `cat /etc/*-release` : ディストリビューションの情報を表示
1. `ps auxf`: 実行中のプロセスの表示
1.  `hostname` : hostnameの表示
1. `ip addr`: ネットワークデバイスとIPの表示(事前に`apt update -y && apt install -y iproute2` で `ip` コマンドをインストールする必要がある)

## Nginxの実行

Nginx(エンジンエックス)は静的ファイルを高速にサーブできるHTTPサーバプログラム．

```
docker container run --name nginx -d -p 8080:80 nginx:1.19.9
                                      ↑ コンテナの80番ポートをホストの8080番ポートにフォワード
```

今回はバックグラウンドでNginxを動かすために`-d`を使用した．
また，`-p` でコンテナの80番ポートをホストの8080番にフォワードした．


```
--name string        Assign a name to the container
-d, --detach         Run container in background and print container ID
-p, --publish list   Publish a container's port(s) to the host
```

### 演習: コンテナの外からcurlを用いて，Nginxにリクエストを送信する

1. `curl http://localhost:8080` で`Welcome to nginx!`というレスポンスが返ってくると成功．
1. `docker ps` コマンドで現在動いているコンテナのIDを確認する．
1. `docker logs -f CONTIANER_ID` でコンテナのアクセスログを表示する．
1.  もう一度 `curl http://localhost:8080`でNginxにリクエストを送信し，アクセスログが表示されることを確認する．

終わったらコンテナを止める
```
docker container stop nginx
```

## Jupyter Labの実行

Jupyter LabはJupyter NotebookのIDEのようなもの．
Webブラウザで実行できる．

```
cd docker/jupyter
docker container run --name jupyterlab --rm -p 8888:8888 \
    -e JUPYTER_ENABLE_LAB=yes \
    -v "$PWD":/home/jovyan/work \
    jupyter/datascience-notebook
```

```
--rm                Automatically remove the container when it exits
-v, --volume list   Bind mount a volume
-e, --env list      Set environment variables
```

アクセスできることを確認できたら止める
```
docker container stop jupyterlab
```
