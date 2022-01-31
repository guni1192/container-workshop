# Seccomp

Seccomp は Linux のシステムコールフィルタ機構である．
システムコールに対して，発行の許可，不許可，監査ログへの記録などのアクションを設定できる．
Docker では，Seccomp Profile というJSON形式のフィルタルール定義ファイルをコンテナ起動時にロードすることで，
Seccomp のルールを設定できる．

```bash
cd ./samples/docker/seccomp/
```

`mkdir` システムコールが発行されたら EPERM を返すseccomp profile
```json
{{#include ../../samples/docker/seccomp/seccomp-profile.json}}
```

Seccomp Profile を指定してコンテナを実行する
```bash
docker run -it --security-opt seccomp=$(pwd)/seccomp-profile.json ubuntu:20.04 bash
```

コンテナ内でmkdir コマンドを実行してディレクトリが作成できるか試す．

```
root@d018f93240d2:/# mkdir foo
mkdir: cannot create directory 'foo': Operation not permitted
root@d018f93240d2:/#
```
