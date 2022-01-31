# procfs と コンテナ

`procfs` ( proc ファイルシステム)は，Linux の疑似ファイルシステムである．
プロセスごとの状態や，CPUやメモリなどの利用状況など見ることができる．

基本的には `/proc` にマウントされており，コンテナの場合も例外ではない．
コンテナの procfs からは PID Namespaceの分離によってホストの情報を見ることはできない．
しかし，ホストからコンテナ内のプロセスを procfs を通じて見ることはできる．

コンテナを実行する．
```
docker container run --rm -d -p 8080:80 nginx
```

ホストからNginxのプロセスIDを特定する

```
ps ax | grep nginx
```

以下のような実行結果が得られる．
Nginxの master process の PID である 27103 をもとにプロセスの状態を調べていく．

```
  27103 ?        Ss     0:00 nginx: master process nginx -g daemon off;
  27167 ?        S      0:00 nginx: worker process
  27168 ?        S      0:00 nginx: worker process
  27169 ?        S      0:00 nginx: worker process
  27170 ?        S      0:00 nginx: worker process
  27171 ?        S      0:00 nginx: worker process
  27172 ?        S      0:00 nginx: worker process
  27173 ?        S      0:00 nginx: worker process
  27174 ?        S      0:00 nginx: worker process
  27175 ?        S      0:00 nginx: worker process
  27176 ?        S      0:00 nginx: worker process
  27220 pts/0    S+     0:00 grep --color=auto nginx
```


まずは`/proc/$PID`のディレクトリを見てみる．

```
ls /proc/27103/
```

実行結果
```
arch_status  clear_refs       cwd      gid_map    maps        net        oom_score_adj  root       smaps         status         uid_map
attr         cmdline          environ  io         mem         ns         pagemap        sched      smaps_rollup  syscall        wchan
autogroup    comm             exe      limits     mountinfo   numa_maps  patch_state    schedstat  stack         task
auxv         coredump_filter  fd       loginuid   mounts      oom_adj    personality    sessionid  stat          timers
cgroup       cpuset           fdinfo   map_files  mountstats  oom_score  projid_map     setgroups  statm         timerslack_ns
```

この中の `root` というディレクトリにアクセスする．
```
sudo su
cd /proc/27103/root
ls
```

ls を実行したら以下のようなディレクトリが見える．
これはコンテナイメージ内のルートディレクトリであることがわかる．
ホストから直接コンテナ内のファイルにアクセスすることができる．
シェルがコンテナイメージ内にない場合などのデバッグに用いると良いだろう．

```
bin  boot  dev  docker-entrypoint.d  docker-entrypoint.sh  etc  home  lib  lib64  media  mnt  opt  proc  root  run  sbin  srv  sys  tmp  usr  var
```

## 演習

1. `/proc/$PID/fd/` を `ls -al` で見て，その内容はなにか考察せよ
1. `/proc/$PID/mounts` を `cat` で見て，その内容はなにか考察せよ
1. `/proc/$PID/ns/` を `ls -al` で見て，その内容はなにか考察せよ
