#version 330 core
out vec4 FragColor;

in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

#define MAX_LIGHTS 100

layout(std140) uniform LightData {
    vec3 lightPositions[MAX_LIGHTS];
    vec3 lightColors[MAX_LIGHTS];
    int numLights;
};

uniform vec3 camPos;
uniform vec4 baseColorFactor;
uniform float metallicFactor;
uniform float roughnessFactor;
uniform vec3 lightPositions[1];
uniform vec3 lightColors[1];
uniform sampler2D baseColorTexture;

const float PI = 3.14159265359;
float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}


// Schlick's approximation for geometric self-shadowing
float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;
    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;
    return nom / denom;
}

// Smith's method combining shadowing and masking
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
    return ggx1 * ggx2;
}


vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}


void main() {
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);
    vec4 baseColor = texture(baseColorTexture, TexCoords) * baseColorFactor;

    vec3 Lo = vec3(0.0);
    
    for(int i = 0; i < 4; i++) {
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;
        
        float NDF = DistributionGGX(N, H, roughnessFactor);
        float G = GeometrySmith(N, V, L, roughnessFactor);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), mix(vec3(0.04), baseColor.rgb, metallicFactor));

        vec3 nominator = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular = nominator / max(denominator, 0.001);

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallicFactor;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * baseColor.rgb / PI + specular) * radiance * NdotL;
    }

    vec3 ambient = vec3(0.03) * baseColor.rgb;
    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2)); // Gamma correction

    FragColor = vec4(color, baseColor.a);



}