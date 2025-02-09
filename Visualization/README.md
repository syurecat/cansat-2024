# CanSat Visualization

## 概要
このディレクトリにはCanSatのデータを処理するWebサーバが含まれています。主に可視化を担当しています。

## 動作環境(確認済み)
- OS: Windows 11
- Docker v3.8

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
ブラウザで `http://localhost:8080` にアクセスしてください。

### APIドキュメント
[API ドキュメント](https://syurecat.github.io/cansat-2024/docs/api/)をご参照ください。

Docker を起動した状態で、[http://localhost:7080/debug](http://localhost:7080/debug) にアクセスすると、WebSocket 通信や POST リクエストのテストが可能です。

### 初期設定について
セキュリティー上の理由から、GrafanaやInfluxDBのconfigファイルはリポジトリに含まれていません。`./Visualization` 内にconfigやデータファイルが作成されるため、必要な場合はバックアップを取ることをおすすめします。また、これらのファイルが生成済みの場合、環境変数での設定は適用されません。手動で設定を変更するかディレクトリを削除してから、Dockerを起動してください。

#### InfluxDB

InfluxDBは通常、設定変更なく使用可能ですが、必要な場合は [influxdbの公式ドキュメント](https://docs.influxdata.com/influxdb/v2/) を参照してください。

#### Grafana

Grafanaのセットアップの詳細については [Grafanaセットアップ](https://github.com/syurecat/cansat-2024/tree/main/docs/Grafanaセットアップ.md) もしくは、 [公式のドキュメント](https://grafana.com/docs/) を参照してください。

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
