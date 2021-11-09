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

- `FROM gcr.io/distroless/base-debian10`: アプリケーションを実行するためのコンテナイメージを定義する．Goのバイナリを実行するだけであればGoの環境はいらないため，軽量なイメージにバイナリだけコピーして実行する．
- `COPY --from=builder /src/bin/* /` : Go言語のコンパイルに使ったコンテナから，軽量コンテナイメージにバイナリをコピーする

```bash
docker image build -f Dockerfile.multistage -t go-webapp:multistage .
```

マルチステージビルドで作成したコンテナイメージは24.3MBで，数百MBものサイズの削減になった．

```
$ docker image ls
REPOSITORY                        TAG           IMAGE ID       CREATED         SIZE
go-webapp                         multistage    df68c5905756   3 minutes ago   24.3MB
go-webapp                         singlestage   98747e043f01   5 minutes ago   999MB
```

マルチステージビルドはコンテナイメージを削減できるだけでなく，ステージごとに並列で動作可能なので，ビルド時間の短縮にもつながる

## コンテナイメージを作るときに気をつけること

- 1コンテナにつき1アプリにする
- Immutable Infrastructureにする
- アプリケーションが動作するために必要なものを把握(ライブラリ，ビルドツール，言語ランタイムなど)
- できるだけイメージや依存関係を最小限にする．

他にも気をつけるべきことは多いが今回は割愛する．
コンテナをビルドする際のベストプラクティス[3]があるため，読むことをすすめる．