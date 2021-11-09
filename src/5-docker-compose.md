## Docker Composeの紹介

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

## Dockerの活用例

- チームでWebアプリケーションをコンテナ化して開発環境の共有
- LaTeXの環境をDocker上に構築
- 研究システムを動かすためのDockerfileを書いて再現性を上げて引き継ぎを楽にする
- DockerfileをGitHubにPushしたら，CircleCIやGitHub Actionsなどで自動的にコンテナのビルドを行い，サーバにデプロイ

## おわりに

Dockerを使用した開発は，開発環境の用意やチーム開発がかなりスムーズになる．
Dockerfileさえ用意していればチーム内で同じ環境が用意できるので，トラブルシュートなどでも役立つのでぜひ活用してほしい．
研究システムの再現性を上げることにもなる．
仮想マシンのようにOSやミドルウェアなど，大量の関心事に縛られることなく，アプリケーションの開発に専念できることもメリットとして挙げられる．

Dockerコンテナを本番環境で運用するという事例も増えている．
AmazonのElastic Container ServiceやKubernetesを利用し，疎結合な分散システムを構築するための有力なプラクティスとして，多数の企業で使用されている．

今後はコンテナで運用することが前提となる企業も多くなるだろう．
そのため，研究システムやツールの開発などで使ってDockerやコンテナ周辺の技術に慣れていくと良いと．

WindowsやmacOSでDockerをためしたいのであれば，
Docker Desktopをインストールしてみると良い．
WindowsであればWindows Subsytem for Linux 2(WSL2)を合わせて使うことを強く推奨する．


