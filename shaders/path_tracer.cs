#version 450

const float PI = 3.14159265358979323846f;
const float FLOAT_INF = 1e20f;

layout(local_size_x = 16, local_size_y = 16, local_size_z = 1) in;
layout(rgba32f, binding = 0) uniform image2D texture0;

uniform ivec2 resolution = ivec2(1920,1080);//ivec2(800, 600);

float Max(vec3 v) {
    return max(v.x, max(v.y, v.z));
}

vec3 Saturate(vec3 v) {
    return clamp(v, 0.0f, 1.0f);
}

uint Hash(uint key) {
	key = (key ^ 61u) ^ (key >> 16u);
	key = key + (key << 3u);
	key = key ^ (key >> 4u);
	key = key * 0x27D4EB2Du;
	key = key ^ (key >> 15u);
	return key;
}

float RandFloat(inout uint state) {
	return float(state) * uintBitsToFloat(0x2F800000u);
}

uint RandUint(inout uint state) {
	state ^= (state << 13u);
	state ^= (state >> 17u);
	state ^= (state << 5u);

    return state;
}

float Rand(inout uint state) {
    return RandFloat(RandUint(state));
}

vec3 CosineWeightedHemisphereSample(float u1, float u2) {
	float cosTheta = sqrt(1.0f - u1);
	float sinTheta = sqrt(u1);
	float phi = 2.0f * PI * u2;

	return vec3(cos(phi) * sinTheta, sin(phi) * sinTheta, cosTheta);
}

float Reflectance0(float n1, float n2) {
	float sqrt_R0 = (n1 - n2) / (n1 + n2);
	return sqrt_R0 * sqrt_R0;
}

float SchlickReflectance(float n1, float n2, float c) {
	float R0 = Reflectance0(n1, n2);
	return R0 + (1.0f - R0) * c * c * c * c * c;
}

vec3 IdealSpecularReflect(vec3 direction, vec3 normal) {
	return reflect(direction, normal);
}

vec3 IdealSpecularTransmit(vec3 direction, vec3 normal, float n_out, float n_in,
                           out float pr, inout uint state) {

	vec3 d_Re = IdealSpecularReflect(direction, normal);

	bool out_to_in = (0.0f > dot(normal, direction));
	vec3 nl = out_to_in ? normal : -normal;
	float nn = out_to_in ? n_out / n_in : n_in / n_out;
	float cosTheta = dot(direction, nl);
	float cos2Phi = 1.0f - nn * nn * (1.0f - cosTheta * cosTheta);

    if (0.0f > cos2Phi) {
        pr = 1.0f;
        return d_Re;
    }

    vec3 d_Tr = normalize(nn * direction - nl * (nn * cosTheta + sqrt(cos2Phi)));
    float c = 1.0f - (out_to_in ? -cosTheta : dot(d_Tr, normal));

    float Re = SchlickReflectance(n_out, n_in, c);
    float p_Re = 0.25f + 0.5f * Re;

    if (Rand(state) < p_Re) {
        pr = (Re / p_Re);
        return d_Re;
    }
    else {
        float Tr = 1.0f - Re;
        float p_Tr = 1.0f - p_Re;
        pr = (Tr / p_Tr);
        return d_Tr;
    }
}

struct Ray {
    vec3 origin;
   	vec3 direction;
	float tmin;
    float tmax;
	uint depth;
};

vec3 EvaluateRay(Ray ray, float t) {
    return ray.origin + ray.direction * t;
}

const uint REFLECTION_DIFFUSE    = 1u;
const uint REFLECTION_SPECULAR   = 0u;
const uint REFLECTION_REFRACTIVE = 2u;

const float EPSILON = 1e-2f;

struct Sphere {
    float radius;
	vec3  position;
	vec3  emission;
	vec3  color;
	uint  reflection_type;
};

struct Triangle{
    vec3 vertices[3];
    vec3 normal;
    vec3  emission;
    vec3  color;
    uint  reflection_type;
};

layout (std430, binding=1) buffer vertex_buffer { vec4 vertices[]; };

struct HitInfo
{
    float dist;
    vec3 position;
    vec3 normal;
    float curr_ior;
    bool transmitted;
    vec3  emission;
    vec3  color;
    uint  reflection_type;
    Ray ray;
    int obj_index;
};

//Moller-Trumbore
HitInfo FindHit(Triangle triangle, Ray ray)
{
    float e = 1e-3;
    vec3 edge1 = triangle.vertices[1] - triangle.vertices[0];
    vec3 edge2 = triangle.vertices[2] - triangle.vertices[0];
    HitInfo obj_hit;
    vec3 h = cross(ray.direction, edge2);
    float a = dot(edge1, h);
    //no hit return empty
    if (a > -e && a < e){
        obj_hit.dist = -1;
        return obj_hit;
    }
    float f = 1.0 / a;
    vec3 s = ray.origin - triangle.vertices[0];
    float u = dot(s,h) * f;
    if (u < 0.0 || u > 1.0){
        obj_hit.dist = -2;
        return obj_hit;
    }
    vec3 q = cross(s, edge1);
    float v = dot(ray.direction, q) * f;
    if (v < 0.0 || u + v > 1.0)
    {
        obj_hit.dist = -3;
        return obj_hit;
    }

    float t = dot(edge2, q) * f;
    if (t <= e)
    {
        obj_hit.dist = -4;
        return obj_hit;
    }

    obj_hit.dist = t;
    obj_hit.position = ray.origin + ray.direction * obj_hit.dist;
    obj_hit.normal = normalize(cross(edge1, edge2));
    obj_hit.transmitted = false;
    obj_hit.emission = triangle.emission;
    obj_hit.color = triangle.color;
    obj_hit.reflection_type = triangle.reflection_type;
    obj_hit.ray = ray;

    return obj_hit;

}


bool Intersect(Sphere sphere, inout Ray ray) {
    vec3  op = sphere.position - ray.origin;
    float dop = dot(ray.direction, op);
    float D = dop * dop - dot(op, op) + sphere.radius * sphere.radius;

    if (D < 0.0f) {
        return false;
    }

    float sqrtD = sqrt(D);

    float tmin = dop - sqrtD;
    if (ray.tmin < tmin && tmin < ray.tmax) {
        ray.tmax = tmin;
        return true;
    }

    float tmax = dop + sqrtD;
    if (ray.tmin < tmax && tmax < ray.tmax) {
        ray.tmax = tmax;
        return true;
    }

    return false;
}

const vec3 EYE = vec3(0.0f, 0.0f, 295.6f);
const float FOV = 0.4135f;

const float SCENE_REFRACTIVE_INDEX_OUT = 1.0f;
const float SCENE_REFRACTIVE_INDEX_IN  = 1.5f;

const uint NUM_SPHERES = 9u;

Sphere spheres[NUM_SPHERES] = Sphere[](
    Sphere(1e4f, vec3(1e4f + 1.0f,  40.8f, 81.6f),  vec3(0.0f), vec3(0.75f,  0.25f,  0.25f), REFLECTION_DIFFUSE),
    Sphere(1e4f, vec3(-1e4f + 99.0f, 40.8f, 81.6f),  vec3(0.0f), vec3(0.25f,  0.25f,  0.75f), REFLECTION_DIFFUSE),
    Sphere(1e4f, vec3(50.0f, 40.8f, 1e4f),           vec3(0.0f), vec3(0.75f), REFLECTION_DIFFUSE),
    Sphere(1e4f, vec3(50.0f, 40.8f, -1e4f + 170.0f), vec3(0.0f), vec3(0.0f), REFLECTION_DIFFUSE),
    Sphere(1e4f, vec3(50.0f, 1e4f, 81.6f),           vec3(0.0f), vec3(0.75f), REFLECTION_DIFFUSE),
    Sphere(1e4f, vec3(50.0f, -1e4f + 81.6f, 81.6f),  vec3(0.0f), vec3(0.75f), REFLECTION_DIFFUSE),
    Sphere(16.5f, vec3(27.0f, 16.5f, 47.0f),         vec3(0.0f), vec3(0.999f), REFLECTION_SPECULAR),
    Sphere(16.5f, vec3(73.0f, 16.5f, 78.0f),         vec3(0.0f), vec3(0.999f), REFLECTION_REFRACTIVE),
    Sphere(600.0f, vec3(50.0f, 681.33f, 81.6f),      vec3(12.0f), vec3(0.0f), REFLECTION_DIFFUSE)
);

bool Intersect(inout Ray ray, out uint id) {
	bool hit = false;
	for (uint i = 0u; i < NUM_SPHERES; ++i) {
		if (Intersect(spheres[i], ray)) {
			hit = true;
			id = i;
		}
	}

	return hit;
}

vec3 CalculateRadiance(Ray ray, inout uint state) {
    Ray  r = Ray(ray.origin, ray.direction, ray.tmin, ray.tmax, ray.depth);

    vec3 L = vec3(0.0f);
    vec3 F = vec3(1.0f);

    float size = 500.0f; // Adjust the size as needed
    Triangle triangles[12];
    // Front-facing triangles
    triangles[0] = Triangle(
        vec3[3](vec3(size, -size, size), vec3(size, size, size), vec3(-size, -size, size)),vec3(0.0f,0.0f,0.0f),
        vec3(0.0f),
        vec3(1,0,0),
        1u);
    triangles[11] = Triangle(
        vec3[3](vec3(-size, size, size), vec3(-size, -size, size), vec3(size, size, size)),vec3(0.0f,0.0f,0.0f),
        vec3(0.0f),
        vec3(1,0,0),
        1u);

    // Back-facing triangles
    triangles[2] = Triangle(
        vec3[3](vec3(size, size, -size), vec3(size, -size, -size), vec3(-size, size, -size)),vec3(0.0f,0.0f,0.0f),
        vec3(0.0f),
        vec3(1,0,0),
        1u);
    triangles[3] = Triangle(
        vec3[3](vec3(-size, -size, -size), vec3(-size, size, -size), vec3(size, -size, -size)),vec3(0.0f,0.0f,0.0f),
        vec3(10.0f),
        vec3(0),
        1u);

    // Top-facing triangles
    triangles[4] = Triangle(
        vec3[3](vec3(-size, size, size), vec3(-size, size, -size), vec3(size, size, size)),vec3(0.0f,0.0f,0.0f),
        vec3(10.0f),
        vec3(0),
        1u);
    triangles[5] = Triangle(
        vec3[3](vec3(size, size, -size), vec3(size, size, size), vec3(-size, size, -size)),vec3(0.0f,0.0f,0.0f),
        vec3(0.0f),
        vec3(0),
        1u);

    // Bottom-facing triangles
    triangles[6] = Triangle(
        vec3[3](vec3(-size, -size, -size), vec3(-size, -size, size), vec3(size, -size, -size)),vec3(0.0f,0.0f,0.0f),
        vec3(10.0f),
        vec3(0),
        1u);
    triangles[7] = Triangle(
        vec3[3](vec3(size, -size, size), vec3(size, -size, -size), vec3(-size, -size, size)),vec3(0.0f,0.0f,0.0f),
        vec3(10.0f),
        vec3(0),
        1u);

    // Left-facing triangles
    triangles[8] = Triangle(
        vec3[3](vec3(-size, size, size), vec3(-size, -size, -size), vec3(-size, size, -size)),vec3(0.0f,0.0f,0.0f),
        vec3(0.0f),
        vec3(1,0,0),
         1u);
    triangles[9] = Triangle(
        vec3[3](vec3(0, 50, -450), vec3(0, 0, -450), vec3(50, 0, -450)),vec3(0.0f,0.0f,0.0f),
        vec3(0.0f),
        vec3(0,1,1),
1u);

    // Right-facing triangles
    triangles[10] = Triangle(
        vec3[3](vec3(0, 50, -450), vec3(50, 50, -450), vec3(50, 0, -450)),vec3(0.0f,0.0f,0.0f),
        vec3(0),
        vec3(0,1,0),
1u);
    triangles[1] = Triangle(
        vec3[3](vec3(8, 8, 0), vec3(8, 2, -400), vec3(-8, 8, -400)),vec3(0.0f,0.0f,0.0f),
        vec3(0),
        vec3(0,1,0),
1u);

//        triangles[1].vertices = vec3[3](vertices[0].xyz, vertices[1].xyz, vertices[2].xyz);

//     vec3 v = vertices[3];
//
//     triangles[1].vertices[0] = vec3(0, 0, -350);
//     triangles[1].vertices[1] = vec3(100, 0, -350);
//     triangles[1].vertices[2] = vec3(0, 100, -350);

    while (true){
        int i = 0;
        HitInfo info;
        HitInfo min_info;
        min_info.dist = 999999999;
        while(i < 12){
            info =  FindHit(triangles[i], r);
            if (info.dist > 0){
                if (info.dist < min_info.dist){
                min_info = info;
                }
            }
            i+= 1;
        }
        info = min_info;
        if ((info.dist < 0.0f) || (info.dist > 999999997)){
            return vec3(0);
        }
        L += F * info.emission;
        F *= info.color;
        if (4u < r.depth){
                    float continue_probability = Max(info.color);
                    if (Rand(state) >= continue_probability){
                        return L;
                    }
                    F /= continue_probability;
                }
        vec3 n  = info.normal;
        vec3 p = info.position;
                switch (info.reflection_type) {

                    case REFLECTION_SPECULAR: {
                        vec3 d = IdealSpecularReflect(r.direction, n);
                        r = Ray(p, d, EPSILON, FLOAT_INF, r.depth + 1u);
                        break;
                    }

                    case REFLECTION_REFRACTIVE: {
                        float pr;
                        vec3 d = IdealSpecularTransmit(r.direction, n, SCENE_REFRACTIVE_INDEX_OUT, SCENE_REFRACTIVE_INDEX_IN, pr, state);
                        F *= pr;
                        r = Ray(p, d, EPSILON, FLOAT_INF, r.depth + 1u);
                        break;
                    }

                    default: {
                        vec3 w = (0.0f > dot(n, r.direction)) ? n : -n;
                        vec3 u = normalize(cross(((0.1f < abs(w.x)) ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f)), w));
                        vec3 v = cross(w, u);
                        vec3 sample_d = CosineWeightedHemisphereSample(Rand(state), Rand(state));
                        vec3 d = normalize(sample_d.x * u + sample_d.y * v + sample_d.z * w);
                        r = Ray(p, d, EPSILON, FLOAT_INF, r.depth + 1u);
                        break;
                    }
                }
//         L += F * info.emission;
//         if (info.emission > 1.0f){
//         return L;}
//         return L;


//         F *= info.color;
//         return L;
//         if (4u < r.depth){
//             float continue_probability = Max(info.color);
//             if (Rand(state) >= continue_probability){
//                 return L;
//             }
//             F /= continue_probability;
//         }
//         vec3 d = IdealSpecularReflect(r.direction, info.normal);
//         r = Ray(info.position, d, EPSILON, FLOAT_INF, r.depth + 1u);
    }
//     while (true) {
//         uint id;
//         if (!Intersect(r, id)) {
//             return L;
//         }
//
//         Sphere sphere = spheres[id];
//         vec3 p = EvaluateRay(r, r.tmax);
//         vec3 n = normalize(p - sphere.position);
//
//         L += F * sphere.emission;
//         F *= sphere.color;
//
//         if (4u < r.depth) {
//             float continue_probability = Max(sphere.color);
//             if (Rand(state) >= continue_probability) {
//                 return L;
//             }
//             F /= continue_probability;
//         }
//
//         switch (sphere.reflection_type) {
//
//             case REFLECTION_SPECULAR: {
//                 vec3 d = IdealSpecularReflect(r.direction, n);
//                 r = Ray(p, d, EPSILON, FLOAT_INF, r.depth + 1u);
//                 break;
//             }
//
//             case REFLECTION_REFRACTIVE: {
//                 float pr;
//                 vec3 d = IdealSpecularTransmit(r.direction, n, SCENE_REFRACTIVE_INDEX_OUT, SCENE_REFRACTIVE_INDEX_IN, pr, state);
//                 F *= pr;
//                 r = Ray(p, d, EPSILON, FLOAT_INF, r.depth + 1u);
//                 break;
//             }
//
//             default: {
//                 vec3 w = (0.0f > dot(n, r.direction)) ? n : -n;
//                 vec3 u = normalize(cross(((0.1f < abs(w.x)) ? vec3(0.0f, 1.0f, 0.0f) : vec3(1.0f, 0.0f, 0.0f)), w));
//                 vec3 v = cross(w, u);
//                 vec3 sample_d = CosineWeightedHemisphereSample(Rand(state), Rand(state));
//                 vec3 d = normalize(sample_d.x * u + sample_d.y * v + sample_d.z * w);
//                 r = Ray(p, d, EPSILON, FLOAT_INF, r.depth + 1u);
//                 break;
//             }
//         }
//     }
}

vec3 CalculateRadiance(vec2 fragCoord, inout uint state) {
	vec3  camera_direction = normalize(vec3(0.0f, 0.0f, -1.0f));
	vec3  camera_x = vec3(resolution.x * FOV / resolution.y, 0.0f, 0.0f);
	vec3  camera_y = normalize(cross(camera_x, camera_direction)) * FOV;

    vec2  u2 = vec2(Rand(state), Rand(state));
    vec2  cs = (fragCoord + u2) / resolution.xy - vec2(0.5f);
    vec3  d = cs.x * camera_x + cs.y * camera_y + camera_direction;
    return CalculateRadiance(Ray(EYE + d * 130.0f, normalize(d), EPSILON, FLOAT_INF, 0u), state);
}

layout (location = 0) uniform float time;
layout (location = 1) uniform int frame;

void main() {
    ivec2 fragCoord = ivec2(gl_GlobalInvocationID.xy);

    uint  index = uint(fragCoord.y * resolution.x + fragCoord.x);
    uint  key = index ^ floatBitsToUint(time);
    uint  state = Hash(key);

    vec3  hdr = CalculateRadiance(fragCoord, state);
    vec3  mean = imageLoad(texture0, fragCoord).xyz;
    mean += (hdr - mean) / float(frame + 1);

    vec4 color = vec4(mean, 1.0f);
    imageStore(texture0, fragCoord, color);
}
