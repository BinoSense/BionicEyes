#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 color;
layout(location = 2) in vec3 normal;
//layout(location = 1) in vec3 a_color;

uniform mat4 projection;
uniform mat4 normalMatrix;
uniform mat4 worldMatrix;

uniform mat4 matLeftPitch;
uniform mat4 matLeftYaw;
uniform mat4 matLeftRoll;
uniform mat4 matRightPitch;
uniform mat4 matRightYaw;
uniform mat4 matRightRoll;

uniform int label;
uniform float leftRight;
uniform float shift;

layout(location = 0) out vec4 vpos;
layout(location = 1) out vec4 nvec;
layout(location = 2) out vec3 crgb;
layout(location = 3) out vec3 N;
layout(location = 4) out vec3 I;


void main(void)
{

//float shift = 38.5;
vec4 P = vec4(0,0,0,1.0);	
vec4  NN = vec4(0,0,0,1.0);
//if (leftRight<0.5){
    float sgn = sign(0.5f-leftRight);
    mat4 matPitch, matYaw,matRoll;
    if (sgn<0){
        matPitch=matLeftPitch;
        matYaw=matLeftYaw;
        matRoll=matLeftRoll;
    }
    else {
        matPitch=matRightPitch;
        matYaw=matRightYaw;
        matRoll=matRightRoll;
    }

    switch (label) {
    case 0:
        crgb = vec3(0.0,0.0,0.95);        
        P = matYaw*vec4(sgn*pos.x,pos.y,pos.z,1.0);	   
        break;
    case 1:
        crgb = vec3(0.95,0.0,0.0);                
        P = matYaw*matPitch*vec4(sgn*pos.x,pos.y,pos.z,1.0);	
        break;
    case 2: 
        if (pos.y *pos.x>0) 
            crgb = vec3(0.0,0.95,0.0);        
        else 
            crgb = vec3(0.05,0.05,0.05);        
        P = matYaw*matPitch*matRoll* vec4(sgn*pos.x,pos.y,pos.z,1.0);	    
        break;
}

    


    // if (label >0.5 && label < 1.5)
    // {        
    //     crgb = vec3(0.95,0.0,0.0);                
    //     P = matYaw*matPitch*vec4(sgn*pos.x,pos.y,pos.z,1.0);	    
        
    // }
    // if (label >1.5 ){
    //     if (pos.y *pos.x>0) 
    //            crgb = vec3(0.0,0.95,0.0);        
    //            else 
    //            crgb = vec3(0.05,0.05,0.05);        
    //     P = matYaw*matPitch*matRoll* vec4(sgn*pos.x,pos.y,pos.z,1.0);	    
        
    // }   
    // if(label <0.5) {
    //     crgb = vec3(0.0,0.0,0.95);        
    //     P = matYaw*vec4(sgn*pos.x,pos.y,pos.z,1.0);	    
    //     //P.x+=sgn*shift;
    // }   

    P.x+=sgn*shift;
    NN=sgn*normalMatrix* vec4(sgn*normal.x,normal.y,normal.z,0.0);  
    
    I  = P.xyz - vec3 (0,0,0);
    //vec4  NN  =normalMatrix * vec4(normal,1.0);
    N  = NN.xyz;	
    
    gl_Position = projection*worldMatrix*P;    
    //vpos = P; // set the output variable to a dark-red color
    vpos = projection*P; // set the output variable to a dark-red color
    nvec= vec4(normal,1.0);
}