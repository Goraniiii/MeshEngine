# MeshEngine

3D 메쉬 자료구조를 직접 구현하고, 다양한 알고리즘을 적용하는 C++/OpenGL 기반 프로젝트입니다.

## Key Features
1. Half-Edge Data Structure
1. Mesh Smoothing Algorithm
	- Laplacian Smoothing
	- Taubin Smoothing
1. Gaussian Curvature Analysis
	- Heatmap Visualization
1. OpenGL Rendering

## Tech Stack
- Language: C++
- Graphics API: OpenGL
- Math Library: Eigen
- Window/Loader: GLFW, GLAD

## Project Structure
- `/src` : source code
	- `/mesh` : mesh, half-edge
	- `/viewer` : renderer, shader, camera
- `/shaders` : GLSL shader files(`.vert`, `.flag`)
- `/assets/mesh` : `.obj` for test