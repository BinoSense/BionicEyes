#version 460 core
layout(location = 0)  in vec4 vpos;
layout(location = 1) in vec4 nvec;
layout(location = 2) in vec3 crgb;
layout(location = 3) in vec3 N;
layout(location = 4) in vec3 I;

out vec4 FragColor;
uniform mat4 projection;

uniform float edgefalloff;
uniform float intensity;
uniform float ambient;


void main(void)
{
	vec3 normalCs = normalize(cross(dFdx(vpos.xyz), dFdy(vpos.xyz)));
    //float edgefalloff=0.1f;
    //float intensity=2.0f;
    //float ambient=0.2f;
    //float opac = dot(normalize(N), normalize(I));
    
    float opac = dot(normalize(-normalCs), normalize(-I));
    opac = abs(opac);
    opac = ambient + intensity*(1.0-pow(opac, edgefalloff));
    //opac = 1.0 - opac;
    
    gl_FragColor.xyz =  opac *crgb;
    gl_FragColor.a = opac;
    //FragColor=vec4(1,0,0,0.5);
    //FragColor = VertexColor;
    // vec3 n = normalize(nvec.xyz);
	// vec4 diffuse = vec4(0.0);
	// vec4 specular = vec4(0.0);
    // vec4 lightPos=projection*vec4(60,60,60,1);
	
	// // the material properties are embedded in the shader (for now)
	// vec4 mat_ambient = vec4(1, 1, 1, 1.0);
	// vec4 mat_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	// vec4 mat_specular = vec4(1.0, 1.0, 1.0, 1.0);
	
	// // ambient term
	// vec4 ambient = mat_ambient * 0.15;
	
	// // diffuse color
	// vec4 kd = mat_diffuse *0.5;
	
	// // specular color
	// vec4 ks = mat_specular * 0.35;
	
	// // diffuse term
	// vec3 lightDir = normalize(lightPos.xyz - vpos.xyz);
	// float NdotL = dot(n, lightDir);
	
	// if (NdotL > 0.0)
	// 	diffuse = kd * NdotL;
	
	// // specular term
	// vec3 rVector = normalize(2.0 * n * dot(n, lightDir) - lightDir);
	// vec3 viewVector = normalize(-vpos.xyz);
	// float RdotV = dot(rVector, viewVector);
	
	// if (RdotV > 0.0)
	// 	specular = ks * pow(RdotV, 10.0f);

	// FragColor = ambient + diffuse + specular;
    
}