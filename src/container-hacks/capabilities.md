# Linux Capability

Capabilitiy はプロセスやファイルに対して，権限を細分化して適用できる．
chroot システムコールを使うには特権が必要だが，root ユーザでなくとも CAP_SYS_CHROOT というCapabilityが与えられているなら使用することができる．
root ユーザはすべてのCapability を持つ．
Docker はホストに危険な影響を及ぼすような Capability は与えられていない．

## pscap のインストール

pscap はプロセスに与えられている capability を確認できるコマンドである．

Ubuntuの場合
```
sudo apt-get install libcap-ng-utils
```

Fedoraの場合
```
sudo dnf install libcap-ng-utils
```


## コンテナ内で実行したプロセスの Capability を見る

```
docker container run -d fedora sleep 5 >/dev/null; pscap | grep sleep
```


## CAP_NET_BIND_SERVICE を drop して nginx を実行

```
docker container run --cap-drop=net_bind_service --net=host nginx
```


## ping をコンテナ内で実行

CAP_NET_RAW がある状態
```
docker container run -it \
    guni1192/debug-tools:latest \
    ping contianer-workshop.guni.dev
```

CAP_NET_RAW を drop した場合
```
docker container run --cap-drop=net_raw -it \
    guni1192/debug-tools:latest \
    ping contianer-workshop.guni.dev
```
