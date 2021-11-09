## Dockerfileを書いて，自分の書いたWebアプリケーションをコンテナ化

Goで書いたシンプルなWebアプリケーションをコンテナ化する．
サンプルアプリケーションをGoで実装した．

```go
package main

import (
  "net/http"
  "github.com/labstack/echo/v4"
  "github.com/labstack/echo/v4/middleware"
)

func main() {
  e := echo.New()

  e.Use(middleware.Logger())
  e.Use(middleware.Recover())

  e.GET("/", hello)

  e.Logger.Fatal(e.Start(":1323"))
}

func hello(c echo.Context) error {
  return c.String(http.StatusOK, "Hello, World!")
}
```

各行の先頭に書いてあるのはDockerfile特有の命令である．
それぞれの命令の詳細は公式ドキュメント[2]を読むことを勧める．

```Dockerfile
FROM golang:1.15-alpine3.12

WORKDIR /src

RUN apk add --no-cache make

# Goの外部依存のライブラリが書いてあるファイル
COPY go.mod go.mod 
COPY go.sum go.sum

# ライブラリのダウンロード
RUN go mod download

COPY . /src
RUN make build

ENTRYPOINT ["/src/bin/go-webapp"]
```

- `FROM golang:1.15-alpine`: ベースとなるコンテナイメージである．Golangが入ったAlpine Linuxという軽量Linuxディストリビューションを使う
- `WORKDIR /src`:  /srcをカレントワーキングディレクトリに指定． なければディレクトリを作る
- `COPY go.mod go.mod`: ローカルのgo.modをコンテナ内の/src/go.modにコピーする．
- `RUN go mod download` go.modやgo.sumに書いてあるpackageをダウンロードするコマンドを実行する
- `COPY . /src`: カレントディレクトリのすべてを/srcにコピーする
- `RUN make build`: makeでgolangのバイナリをビルドする
- `ENTRYPOINT ["go-webapp"]` :コンテナを実行時にどのコマンドを実行するか定義する

```
docker build -f Dockerfile.singlestage -t $USER/go-webapp:singlestage .
```

### マルチステージビルド

CやGoなどのコンパイラ言語でコンパイルされたシステムは，コンパイルした実行バイナリ(+ 動的リンクライブラリ) さえあれば，コンパイラはコンテナイメージには必要がない．

つまり，最終的にコンテナイメージにはバイナリ(+ シェル, ツール)があれば問題ない．コンテナイメージを作る過程を以下のような複数のコンテナイメージを跨いだ多段階形式(マルチステージビルド)にすることで，コンテナイメージの削減に繋がる．
よって，コンテナイメージをダウンロードする時間が短くなり，デプロイ時間を短縮することにもつながる．

1. Goコンパイラがあるイメージ内で，アプリケーションをコンパイルする．
1. コンパイルしたファイルを軽量なコンテナイメージ(Alpine Linux, Distroless, scratch)内にコピーする

という2段階でコンテナイメージをビルドするようにすると，コンテナイメージのサイズを小さくすることが可能になる．

```Dockerfile
FROM golang:1.15-alpine3.12 as builder

WORKDIR /src

RUN apk add --no-cache make

COPY go.mod go.mod
COPY go.sum go.sum
RUN go mod download

COPY . /src
RUN make build

## ↓ 実際に作られるコンテナイメージはここから
FROM alpine:3.12 as runner

COPY --from=builder /src/bin/go-webapp /usr/local/bin

ENTRYPOINT ["go-webapp"]
```

- `FROM alpine:3.12 as runner`: コンテナを実行するためのコンテナイメージを定義する．Goのバイナリを実行するだけであればGoの環境はいらないため，軽量なイメージにバイナリだけコピーして実行する．
- `COPY --from=builder /src/bin/go-webbapp /usr/local/bin` : Go言語のコンパイルに使ったコンテナから，軽量コンテナイメージにバイナリをコピーする

```
docker build -f Dockerfile.multistage -t $USER/go-webapp:multistage .
```

マルチステージビルドで作成したコンテナイメージは11.4MBで，数百MBものサイズの削減になった．

```
$ docker image ls
REPOSITORY                     TAG               IMAGE ID       CREATED          SIZE
go-webapp                      singlestage       6823e5b90199   33 seconds ago   395MB
go-webapp                      multistage        667da54679b2   3 hours ago      11.4MB
```

マルチステージビルドはコンテナイメージを削減できるだけでなく，ステージごとに並列で動作可能なので，ビルド時間の短縮にもつながる

### コンテナイメージを作るときに気をつけること

- 1コンテナにつき1プロセス(アプリ) にする
- Immutable Infrastructureにする
- アプリケーションが動作するために必要なものを把握(ライブラリ，ビルドツール，言語ランタイムなど)
- できるだけイメージや依存関係を最小限にする．

他にも気をつけるべきことは多いが今回は割愛する．
コンテナをビルドする際のベストプラクティス[3]があるため，読むことをすすめる．


