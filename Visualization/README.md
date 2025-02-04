#   Cansat Visualization

## 概要
このディレクトリにはcansatのデータを処理するwebサーバが入っています。主に可視化を担当しています。

## 機能
- ✅ 機能1
- ✅ 機能2
- ✅ 機能3

## 動作環境(確認済み)
- OS: Windows11
- docker v3.8

## 使用方法

### 環境構築
リポジトリをクローン
```sh
git clone https://github.com/syurecat/cansat-2024
cd cansat-2024/Visualization
```

### 実行方法
```sh
docker-compose up
```
ブラウザで `http://localhost:8080` にアクセス。

### apiドキュメント
TODO


## 環境変数
以下の環境変数を設定する必要があります。.envにデフォルトの値がすでに入力されているため、適宜変更してください。
```
INFLUXDB_USERNAME=cansat
INFLUXDB_PASSWORD=cansat2024
INFLUXDB_ORG=koshi
INFLUXDB_BUCKET=cansat_data
INFLUXDB_TOKEN=0xDkk_QUWINO7cZX8U8CQlRKpNLUEZTO-MHYWlrBAEbYY9aqfCIcL4yJAPORtebdEcgBSjfjCC204w8c4Bqlcw==
GRAFANA_PASSWORD=cansat2024
AUTH_TOKEN=cansat
```

## 📜 ライセンス
TODO

## 📧 コンタクト
TODO


