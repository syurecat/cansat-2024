## three.js用メモ

### カメラ
描画する際ににどこから見るかやアスペクト比、視野角を決定する。
```js
camera = new THREE.PerspectiveCamera(fov, aspect, near, far);
camera.position.set(x, y, z);
```
### シーン
3D空間のこと。
```js
scene = new THREE.Scene();
```
### ライト
3D空間の光源のこと。様々な種類がある。詳しくはドキュメントへ
```js
let Light = new THREE.DirectionalLight(0xffffff, 1);
dirLight.position.set(x,y,z);
scene.add(Light);
```
### レンダラー
ブラウザに描画する。描画範囲などを決める。
```js
renderer = new THREE.WebGLRenderer({antialias: true});
renderer.setPixelRatio(ratio);
renderer.setSize(width, height);
```
### 配置
HTMLに配置します。タグだったりに配置します。
```js
div.appendChild(renderer.domElement);
```
### オブジェクトの配置
物体を配置します。すでにあるモデルを配置するもよし、ジオメトリとマテリアルを指定してメッシュを配置するもよしです。
