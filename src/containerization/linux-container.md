# Linuxコンテナを構成する機能

Linuxにおけるポピュラーなコンテナは以下の機能によって実装されている．

## Namespace(名前空間)

`namespaces`(7) はLinuxカーネルのリソースを分離するための機構である．
`unshare`, `clone`, `clone3`, `setns` などのシステムコールで `namespace` を分離することができる．

Namespaceの種類
- `PID Namespace`: 
    Process IDを分離することができる．これにより，ホストのOSのプロセスの状態と関係なくコンテナ内のPIDは1からスタートする． `ps` コマンドなどの結果が変わる．
- `Network Namespace`:
    ネットワークインタフェースの情報をホストから分離する．コンテナ内からホストのNICの情報をみることができなくなる．ホストとの疎通もできなくなるため，仮想Ethernetなどの構築が必要
- `UTS Namespace`:
    UTSを分離する． これによりコンテナにホストOSと異なるホスト名をつけることができる．
- `Mount Namespace`:
    マウントしているファイルを分離することができる．
- `User Namespace`:
    UID, GID, Capability を分離する．コンテナ外の一般ユーザをコンテナ内の root ユーザにマッピングすることができるようになる．
- `Cgroup Namespace`:
    cgroups(7) の root ディレクトリを分離する．
- `Time Namespace`:
    コンテナの中のタイムゾーンをホストと異なるものが使用できる

## Cgroups (Control groups)

CgroupはLinuxカーネルのリソースの使用を制限する機能である．
プロセス群のCPU, RAM, Disk IO, Network IOなどのハードウェア資源の使用量やプロセスの生成数などを制限することができる．
それらを管理するシステムをCgroupコントローラと呼ぶ．
Cgroup コントローラは以下のような種類がある．

- `CPU`
- `Memory`
- `IO`
- `Device`
- `RDMA`
- `Misc`

**Note: Cgroupにはv1, v2が存在し，それぞれディレクトリの階層が異なる．Linuxカーネルのバージョンや設定に注意**
