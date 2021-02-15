#version 330 core

in vec3 nrm;
in vec3 pos;

layout(location = 0) out vec4 fragColor;

uniform struct LightInfo {
  vec4 Position;  // Light position in eye coords.
  vec3 L;  // D,S intensity
  vec3 La; // amb 
} Light;

uniform struct MaterialInfo {
  vec3 Ka;            // Ambient reflectivity
  vec3 Kd;            // Diffuse reflectivity
  vec3 Ks;            // Specular reflectivity
  float Shininess;    // Specular shininess factor
} Material;

vec3 blinnPhong( vec3 position, vec3 n ) {  
  vec3 ambient = Light.La * Material.Ka;
  vec3 s = normalize( Light.Position.xyz - position );
  float sDotN = max( dot(s,n), 0.0 );
  vec3 diffuse = Material.Kd * sDotN;
  vec3 spec = vec3(0.0);
  if( sDotN > 0.0 ) {
    vec3 v = normalize(-position.xyz);
    vec3 h = normalize( v + s );
    spec = Material.Ks *
            pow( max( dot(h,n), 0.0 ), Material.Shininess );
  }
  return ambient + Light.L * (diffuse + spec);
}


void main()
{
	fragColor = vec4(blinnPhong(pos, nrm), 1.0);
}
