import "./style.css";
import * as THREE from "three";
import { GLTFLoader } from "three/examples/jsm/loaders/GLTFLoader";
import { OrbitControls } from "three/examples/jsm/controls/OrbitControls.js";
import * as dat from "dat.gui";

// Loading
const textureLoader = new THREE.TextureLoader();

// Debug
const gui = new dat.GUI();

// Canvas
const canvas = document.querySelector("canvas.webgl");

// Scene
const scene = new THREE.Scene();

// animation
let mixer;
let clock = new THREE.Clock();
const actions = [];

// pingu load
const loader = new GLTFLoader();
const url = "/textures/pingu.glb";
let model = null;

loader.load(
    url,
    function (gltf) {
        model = gltf.scene;
        model.position.set(0, -0.5, 0);
        scene.add(model);

        //ANIMATION
        mixer = new THREE.AnimationMixer(model);
        gltf.animations.forEach((animation) => {
            actions.push(mixer.clipAction(animation).play());
        });

        tick();
    },
    function (error) {
        console.log("An error happened");
        console.log(error);
    }
);

// Lights 1
const pointLight = new THREE.PointLight(0xffffff, 0.1);
pointLight.position.x = 2;
pointLight.position.y = 3;
pointLight.position.z = 4;
scene.add(pointLight);

// Lights 2
const pointLight2 = new THREE.PointLight(0xffffff, 2);
pointLight2.position.set(-1.86, 1, -1.65);
pointLight2.intensity = 7;

scene.add(pointLight2);

// Lights 3
const pointLight3 = new THREE.PointLight(0xe1ff, 2);
pointLight3.position.set(2.13, -3, -1.98);
pointLight3.intensity = 6.8;

scene.add(pointLight3);

/**
 * Sizes
 */
const sizes = {
    width: window.innerWidth,
    height: window.innerHeight,
};

window.addEventListener("resize", () => {
    // Update sizes
    sizes.width = window.innerWidth;
    sizes.height = window.innerHeight;

    // Update camera
    camera.aspect = sizes.width / sizes.height;
    camera.updateProjectionMatrix();

    // Update renderer
    renderer.setSize(sizes.width, sizes.height);
    renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));
});

// Base camera
const camera = new THREE.PerspectiveCamera(
    75,
    sizes.width / sizes.height,
    0.1,
    100
);
camera.position.x = 0;
camera.position.y = 0;
camera.position.z = 2;
scene.add(camera);

// Controls
const controls = new OrbitControls(camera, canvas);
controls.enableDamping = true;

/**
 * Renderer
 */
const renderer = new THREE.WebGLRenderer({
    canvas: canvas,
    alpha: true,
    antialias: true,
});
renderer.setSize(sizes.width, sizes.height);
renderer.setPixelRatio(Math.min(window.devicePixelRatio, 2));

/**
 * Animate
 */

const tick = () => {
    const delta = clock.getDelta();
    mixer.update(delta);

    // Update Orbital Controls
    controls.update();

    // Render
    renderer.render(scene, camera);

    // Call tick again on the next frame
    window.requestAnimationFrame(tick);
};