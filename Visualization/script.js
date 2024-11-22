//参考 https://zenn.dev/sdkfz181tiger/books/735e854bee9fc9/viewer/

import * as THREE from "three";
import { GLTFLoader } from "GLTFLoader";

const CANDATA = "./cansat-3dData/cansatafull.gltf"
const W_WIDTH  = window.innerWidth; // ブラウザの横サイズ
const W_HEIGHT = window.innerHeight;// ブラウザの縦サイズ
const W_ASPECT = window.innerWidth / window.innerHeight;// アスペクト比
const W_RATIO  = window.devicePixelRatio;// ピクセル比
let camera, scene, renderer, cube, model;// カメラ、シーン、レンダラー、立方体

window.onload = ()=>{
	//カメラ
	camera = new THREE.PerspectiveCamera(60, W_ASPECT, 0.1, 1000);
	camera.position.set(0, 5, 0);
	//シーン
	scene = new THREE.Scene();
	scene.background = new THREE.Color('#00bfff');
	//ヘルパー
	let gridhelper = new THREE.GridHelper();
	scene.add(gridhelper);
	//ライト
	// let ambLight = new THREE.AmbientLight(0x333333);
	// scene.add(ambLight);
	let dirLight = new THREE.DirectionalLight(0xffffff, 1);
	dirLight.position.set(20,20,20);
	scene.add(dirLight);
	//レンダラー
	renderer = new THREE.WebGLRenderer({antialias: true});
	renderer.setPixelRatio(W_RATIO);// ピクセル比
	renderer.setSize(W_WIDTH, W_HEIGHT);
	//配置
	let div = document.getElementById("three");
	div.appendChild(renderer.domElement);

	//テスト
	// const geometry = new THREE.BoxGeometry(10,10,10);
	// const material = new THREE.MeshBasicMaterial({color:0x00ff00});
	// cube = new THREE.Mesh(geometry, material);
	// scene.add(cube);
	
	//配置
    const loader = new GLTFLoader();
    loader.load(CANDATA, function (gltf) {
        model = gltf.scene;
        scene.add(model);

        model.scale.set(20, 20, 20);  // サイズ調整
		model.position.set(0, 0, 0);
    }, undefined, function (error) {
        console.error('Model loading error:', error);
    });
	//アニメーション
	animate();
}

function animate(){
	//読み込み待ち
	if (model) {
		// model.rotation.x += 0.01;
		// model.rotation.y += 0.01;
	}
	camera.lookAt(new THREE.Vector3(0, 0, 0));
	renderer.render(scene, camera);// レンダリング
	requestAnimationFrame(animate);// 更新
}
