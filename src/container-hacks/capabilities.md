# Linux Capability

pscap のインストール

```
sudo apt-get install libcap-ng-utils
```

```
docker container run --cap-drop=net_bind_service --net=host nginx
```


```
docker container run -it guni1192/debug-tools:latest ping contianer-workshop.guni.dev
docker container run --cap-drop=net_raw -it guni1192/debug-tools:latest -c ping contianer-workshop.guni.dev
```


```
docker container run -d fedora sleep 5 >/dev/null; pscap | grep sleep
```
