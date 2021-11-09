FROM golang:1.15-alpine3.12 as builder

WORKDIR /src

RUN apk add --no-cache make

COPY go.mod go.mod
COPY go.sum go.sum
RUN go mod download

COPY . /src
RUN make build

FROM alpine:3.12 as runner

COPY --from=builder /src/bin/go-webapp /usr/local/bin

ENTRYPOINT ["go-webapp"]
