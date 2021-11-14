## Linuxコンテナを構成する機能

Linuxにおけるポピュラーなコンテナは以下の機能によって実装されている．

### Namespace(名前空間)

namespaces(7)はLinuxカーネルのリソースを分離するための機構である．
unshareシステムコールやcloneシステムコールでnamespaceを分離することができる．

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

### Cgroups (Control groups)
CgroupはLinuxカーネルのリソースの使用を制限する機能．プロセス群のCPU, RAM, Disk IO, Network IOなどのハードウェア資源の使用量やプロセスの生成数などを制限することができる．

Linuxにおけるコンテナの実装は非常にシンプルであり，C言語で数百行程度で記述することができる．
