# Docker Composeの紹介

Docker Composeは複数のコンテナの構成を1つのYAML形式のファイルで定義して管理することができます．
Webアプリケーションを例に取ると，フロントエンド，バックエンド，DBなどのコンテナを定義して，1つのサービスとして管理できます．

docker-compose.yamlの例

```yaml
{{#include ../../samples/docker/wordpress/docker-compose.yaml}}
```

かんたんなチートシート
```bash
# 起動
docker compose up -d
# 起動状態の確認
docker compose ps
# コンテナのログを出力
docker compose logs
# コンテナの最新のログを追従して出力する
docker compose logs -f
# 停止
docker compose stop
# 削除
docker compose rm
# 停止 + 削除
docker compose down
```
