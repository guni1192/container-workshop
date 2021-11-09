GO=go
GOOS=$(shell go env GOOS)
GOARCH=$(shell go env GOARCH)
GO_LDFLAGS=-ldflags="-s -w"
TARGET_DIR=bin/

.PHONY: build test clean

build:
	mkdir -p bin
	GOOS=$(GOOS) GO_ARCH=$(GOARCH) $(GO) build $(GO_LDFLAGS) -o $(TARGET_DIR) ./...

test:
	$(GO) test -v ./...

clean:
	rm -rf bin
