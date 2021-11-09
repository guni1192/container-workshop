# Docker Composeの紹介

Docker Composeは複数のコンテナの構成を1つのYAML形式のファイルで定義して管理することができます．
Webアプリケーションを例に取ると，フロントエンド，バックエンド，DBなどのコンテナを定義して，1つのサービスとして管理できます．

docker-compose.yamlの例
```yaml
version: '3'

services:
   db:
     image: mysql:5.7
     volumes:
       - db_data:/var/lib/mysql
     restart: always
     environment:
       MYSQL_ROOT_PASSWORD: somewordpress
       MYSQL_DATABASE: wordpress
       MYSQL_USER: wordpress
       MYSQL_PASSWORD: wordpress

   wordpress:
     depends_on:
       - db
     image: wordpress:latest
     ports:
       - "8000:80"
     restart: always
     environment:
       WORDPRESS_DB_HOST: db:3306
       WORDPRESS_DB_USER: wordpress
       WORDPRESS_DB_PASSWORD: wordpress
volumes:
    db_data:
```

かんたんなチートシート
```
# 起動
docker compose up -d
# 起動状態の確認
docker compose ps
# ログ
docker compose logs
docker compose logs -f
# 停止
docker compose stop
# 削除
docker compose rm
# 停止 + 削除
docker compose down
```


