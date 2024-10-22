# Ray Tracing
Here's an example image that I generated using the script :

![image_2024-10-22_161622759](https://github.com/user-attachments/assets/571f7e56-7a2f-4b73-ba9c-cec9103d375f)

# About This Project
This project is a simple yet powerful ray tracing engine written in C++. It demonstrates various features of ray tracing, including :

- **Anti-Aliasing (MSAA)**: To produce smoother images by reducing the jagged edges.

- **Positionable Camera**: Allows the camera to be moved and positioned freely within the scene.

- **Defocus Blur**: Simulates the effect of a camera lens, adding a realistic blur to out-of-focus areas.

# Project Structure
**Ray Tracing Engine**: The core logic for ray tracing, including the main rendering loop in Ray_Tracing.cpp.

**Camera**: Defined in camera.h, it handles the positioning and configuration of the camera.

**Materials**: Various materials like Lambertian, Metal, and Dielectric are defined in material.h.

**Hittable Objects**: Spheres that can be hit by rays, defined in hittable.h and sphere.h.

**Utilities**: Helper functions and classes, including vector operations in vec3.h and color handling in color.h.
