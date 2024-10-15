![image]()
![alt text](https://github.com/milesrichmond/opengl-learning/blob/main/demos/readmeImage.png?raw=true)

### Overview
This project is aiming to be a physics simulation, but the actual scope of this is unclear currently. I've always wanted to make some type of simulation like this, but had always struggled with the visualization. 

### Current features:
- Import & render 3D models
- Camera is user controlled
- Read options and scenes from JSON files

### TODO list:
- [HIGH] Correctly attract objects with gravity (In testing)
- [HIGH] Correct collision detection & bounces (In testing)
- [MED] Object rotation 
- [LOW] Object bounds for more complex shapes
- [LOWEST] Optimization (this will always be here and is not high priority):
    - Reduce n^2 cost of gravitational acceleration

### Important background
This is a side project that I have been working on occasionally and progress is slow. It was initially based on the amazing tutorial by Joey DeVries, https://learnopengl.com. There may be remanants of things that directly match their source code from when I was learning the basics, but I am working through to redesign and trim things down to what I actually need for this project. **Things are not yet properly cited**, but I want to get to a more finished project. I will try and cite things as I move foreward, but when I reach a stopping point I will go back and fix things. Additionally, I am learning bits of C++ with this project, there **WILL** be bad practices, but I will do my best to avoid them.

*I had compiled this on macos (arm64) so it may not build for other platforms.*