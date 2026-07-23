#version 410 core

out vec4 FragColor;

// Uniforms for camera manipulation
uniform vec3 u_CameraPos;
uniform vec3 u_CameraDir;
uniform vec3 u_CameraUp;
uniform vec2 u_Resolution;

// 3D Texture where each voxel holds structural data (0 = empty, >0 = solid)
uniform sampler3D u_VoxelGrid; 

const vec3 GRID_SIZE = vec3(256.0, 256.0, 256.0); // Adjust based on your texture resolution

// Structure to hold ray hit data
struct RayHit {
    bool hit;
    vec3 hitPos;
    vec3 normal;
    vec4 voxelColor;
};

// Amanatides & Woo DDA Algorithm for Voxel Traversal
RayHit voxelTraversal(vec3 rayOrigin, vec3 rayDir) {
    RayHit result;
    result.hit = false;

    // Early exit if the ray starts outside the grid boundaries (simple bounding box check)
    // For production, you would calculate entry/exit intersections here.

    // Current voxel grid coordinates
    vec3 mapPos = floor(rayOrigin);

    // Direction variables for step size and direction
    vec3 deltaDist = abs(vec3(length(rayDir)) / (rayDir + 1e-6));
    vec3 rayStep = sign(rayDir);
    vec3 sideDist;

    // Calculate initial distance to the first voxel boundaries
    if (rayDir.x < 0.0) { sideDist.x = (rayOrigin.x - mapPos.x) * deltaDist.x; }
    else { sideDist.x = (mapPos.x + 1.0 - rayOrigin.x) * deltaDist.x; }

    if (rayDir.y < 0.0) { sideDist.y = (rayOrigin.y - mapPos.y) * deltaDist.y; }
    else { sideDist.y = (mapPos.y + 1.0 - rayOrigin.y) * deltaDist.y; }

    if (rayDir.z < 0.0) { sideDist.z = (rayOrigin.z - mapPos.z) * deltaDist.z; }
    else { sideDist.z = (mapPos.z + 1.0 - rayOrigin.z) * deltaDist.z; }

    vec3 mask = vec3(0.0);

    // Maximum steps to prevent infinite loops
    for (int i = 0; i < 256; i++) {
        // Check if map position is within the bounds of our 3D texture data
        if (mapPos.x >= 0.0 && mapPos.x < GRID_SIZE.x &&
            mapPos.y >= 0.0 && mapPos.y < GRID_SIZE.y &&
            mapPos.z >= 0.0 && mapPos.z < GRID_SIZE.z) {
            
            // Normalize coordinates for sampling the 3D texture (0.0 to 1.0)
            vec3 texCoord = (mapPos + vec3(0.5)) / GRID_SIZE;
            vec4 voxel = texture(u_VoxelGrid, texCoord);

            if (voxel.a > 0.0) { // Found a solid voxel!
                result.hit = true;
                result.voxelColor = voxel;
                result.normal = -rayStep * mask; // Normal vector of the hit surface
                return result;
            }
        }

        // Advance to the next voxel using the closest boundary axis
        if (sideDist.x < sideDist.y) {
            if (sideDist.x < sideDist.z) {
                sideDist.x += deltaDist.x;
                mapPos.x += rayStep.x;
                mask = vec3(1.0, 0.0, 0.0);
            } else {
                sideDist.z += deltaDist.z;
                mapPos.z += rayStep.z;
                mask = vec3(0.0, 0.0, 1.0);
            }
        } else {
            if (sideDist.y < sideDist.z) {
                sideDist.y += deltaDist.y;
                mapPos.y += rayStep.y;
                mask = vec3(0.0, 1.0, 0.0);
            } else {
                sideDist.z += deltaDist.z;
                mapPos.z += rayStep.z;
                mask = vec3(0.0, 0.0, 1.0);
            }
        }
    }
    return result;
}

void main() {
    // Convert screen coordinates to Normalized Device Coordinates (-1 to 1)
    vec2 uv = (gl_FragCoord.xy / u_Resolution) * 2.0 - 1.0;
    float aspect = u_Resolution.x / u_Resolution.y;
    uv.x *= aspect;

    // Generate Ray Origin & Ray Direction using a simple camera matrix
    vec3 rayOrigin = u_CameraPos;
    vec3 right = normalize(cross(u_CameraDir, u_CameraUp));
    vec3 up = normalize(cross(right, u_CameraDir));
    vec3 rayDir = normalize(u_CameraDir + uv.x * right + uv.y * up);

    // Execute Traversal
    RayHit hitInfo = voxelTraversal(rayOrigin, rayDir);

    if (hitInfo.hit) {
        // Very basic lighting calculation (Lambertian Diffuse Directional Light)
        vec3 lightDir = normalize(vec3(0.5, 1.0, 0.3));
        float diffuse = max(dot(hitInfo.normal, lightDir), 0.2); // 0.2 ambient floor
        
        FragColor = vec4(hitInfo.voxelColor.rgb * diffuse, 1.0);
    } else {
        // Simple background sky color gradient
        FragColor = vec4(0.4, 0.6, 0.9, 1.0) * (0.5 + 0.5 * rayDir.y);
    }
}