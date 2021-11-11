# Webアプリケーションをコンテナ化する

Goで書いたシンプルなWebアプリケーションをコンテナ化して実行する．
コンテナイメージの作成には `docker image build(docker build)` コマンドを使用する．

```go
{{#include ../samples/docker/go-webapp/main.go}}
```

各行の先頭に書いてあるのはDockerfile特有の命令である．
それぞれの命令の詳細は公式ドキュメント[2]を読むことを勧める．

```
{{#include ../samples/docker/go-webapp/Dockerfile.singlestage}}
```

- `FROM golang:1.17-buster`: ベースとなるコンテナイメージである．Golangが入ったDebian 10上に構築する．
- `WORKDIR /src`:  /srcをカレントワーキングディレクトリに指定． なければディレクトリを作る
- `COPY go.mod go.mod`: ローカルのgo.modをコンテナ内の/src/go.modにコピーする．
- `RUN go mod download` go.modやgo.sumに書いてあるpackageをダウンロードするコマンドを実行する
- `COPY . /src`: カレントディレクトリのすべてを/srcにコピーする
- `RUN make build`: makeでgolangのバイナリをビルドする
- `ENTRYPOINT ["go-webapp"]` :コンテナを実行時にどのコマンドを実行するか定義する

```bash
docker image build -f Dockerfile.singlestage -t go-webapp:singlestage .
```

## 演習: 作ったコンテナイメージを実行する

`docker container run (docker run)` コマンドを使ってコンテナを実行する．

```bash
docker container run -d --name go-webapp -p 1323:1323 go-webapp:singlestage
```

`curl` コマンドでコンテナ上でWebアプリにHTTPリクエストを送る．

```bash
# コンテナの外から実行
curl http://localhost:1323
```

`Hello, World!` が出力されたら成功．

終わったらコンテナを終了させる．

```bash
docker stop go-webapp
```

## 発展: マルチステージビルド

CやGoなどのコンパイラ言語でコンパイルされたシステムは，コンパイルした実行バイナリ(+ 動的リンクライブラリ) さえあれば，コンパイラはコンテナイメージには必要がない．

つまり，最終的にコンテナイメージにはバイナリ(+ シェル, ツール)があれば問題ない．コンテナイメージを作る過程を以下のような複数のコンテナイメージを跨いだ多段階形式(マルチステージビルド)にすることで，コンテナイメージの削減に繋がる．
よって，コンテナイメージをダウンロードする時間が短くなり，デプロイ時間を短縮することにもつながる．

1. Goコンパイラがあるイメージ内で，アプリケーションをコンパイルする．
1. コンパイルしたファイルを軽量なコンテナイメージ(Alpine Linux, Distroless, scratch)内にコピーする

という2段階でコンテナイメージをビルドするようにすると，コンテナイメージのサイズを小さくすることが可能になる．

```
{{#include ../samples/docker/go-webapp/Dockerfile.multistage}}
```

- `FROM gcr.io/distroless/base-debian11`: アプリケーションを実行するためのコンテナイメージを定義する．Goのバイナリを実行するだけであればGoの環境はいらないため，軽量なイメージにバイナリだけコピーして実行する．
- `COPY --from=builder /src/bin/* /` : Go言語のコンパイルに使ったコンテナから，軽量コンテナイメージにバイナリをコピーする

```bash
docker image build -f Dockerfile.multistage -t go-webapp:multistage .
```

マルチステージビルドで作成したコンテナイメージは25.3MBで，数百MBものサイズの削減になった．

```
$ docker image ls
REPOSITORY                        TAG             IMAGE ID       CREATED          SIZE
go-webapp                         multistage      61199b8fff2c   7 seconds ago    25.3MB
go-webapp                         singlestage     2e402f9e54ba   29 seconds ago   1.04GB
```

マルチステージビルドはコンテナイメージを削減できるだけでなく，ステージごとに並列で動作可能なので，ビルド時間の短縮にもつながる

## コンテナイメージを作るときに気をつけること

- 1コンテナにつき1アプリにする
- Immutable Infrastructureにする
- アプリケーションが動作するために必要なものを把握(ライブラリ，ビルドツール，言語ランタイムなど)
- できるだけイメージや依存関係を最小限にする．

他にも気をつけるべきことは多いが今回は割愛する．
コンテナをビルドする際のベストプラクティス[3]があるため，読むことをすすめる．
