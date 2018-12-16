in float mode;
in float dist;
in vec2 texCoord;
in vec3 lightDir,normal,halfVector;
in vec4 diffuse,ambient,ambientGlobal;
uniform sampler2D texture;

void DirectionLight();   //方向光
void PointLight();       //点光源
void SpotLight();        //聚光灯

void main()
{
    if(mode==1){
      DirectionLight();
    }
    else if(mode==2){
      PointLight();
    }
    else if(mode==3){
      SpotLight();
    }
} 

void DirectionLight(){

    vec3 ct,cf;
    vec3 n,halfV;
    vec4 texel;
    float NdotL,NdotHV;
    float at,af;

    texel = texture2D(texture,texCoord);
    ct = texel.rgb;
    at = texel.a;
    cf = ambientGlobal.rgb;
    af = diffuse.a;

    n = normalize(normal);
    NdotL = max(dot(n,lightDir),0.0);

    if (NdotL > 0.0) {
        cf += NdotL * diffuse.rgb + ambient.rgb;

        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV),0.0);
        cf += gl_FrontMaterial.specular.rgb * gl_LightSource[0].specular.rgb *
          pow(NdotHV,gl_FrontMaterial.shininess);
    }
    gl_FragColor = vec4(ct * cf, at * af);
}

void PointLight(){

    vec3 ct,cf;
    vec3 n,halfV;
    vec4 texel;
    float NdotL,NdotHV;
    float at,af;
    float att;

    texel = texture2D(texture,texCoord);
    ct = texel.rgb;
    at = texel.a;
    cf = ambientGlobal.rgb;
    af = diffuse.a;

    n = normalize(normal);
    NdotL = max(dot(n,normalize(lightDir)),0.0);
    
    
    if (NdotL > 0.0) {
        att = 1.0 / (gl_LightSource[0].constantAttenuation +
          gl_LightSource[0].linearAttenuation * dist +
          gl_LightSource[0].quadraticAttenuation * dist * dist);
        cf += att * (NdotL * diffuse.rgb + ambient.rgb);

        halfV = normalize(halfVector);
        NdotHV = max(dot(n,halfV),0.0);
        cf += att * gl_FrontMaterial.specular.rgb * gl_LightSource[0].specular.rgb *
          pow(NdotHV,gl_FrontMaterial.shininess);
    }
    gl_FragColor = vec4(ct * cf, at * af);
}

void SpotLight(){
     
     vec3 ct,cf;
     vec3 n,halfV;
     vec4 texel;
     float NdotL,NdotHV;
     float at,af;
     float att,spotEffect;
 
     texel = texture2D(texture,texCoord);
     ct = texel.rgb;
     at = texel.a;
     cf = ambientGlobal.rgb;
     af = diffuse.a;
 
     n = normalize(normal);
     NdotL = max(dot(n,normalize(lightDir)),0.0);

     if (NdotL > 0.0){ 
       spotEffect = dot(normalize(gl_LightSource[0].spotDirection), normalize(-lightDir));
       if(spotEffect > gl_LightSource[0].spotCosCutoff)
       {  
         spotEffect = pow(spotEffect, gl_LightSource[0].spotExponent);
         att = spotEffect / (gl_LightSource[0].constantAttenuation +
           gl_LightSource[0].linearAttenuation * dist +
           gl_LightSource[0].quadraticAttenuation * dist * dist);
         cf += att * (NdotL * diffuse.rgb + ambient.rgb); 

         halfV = normalize(halfVector);
         NdotHV = max(dot(n,halfV),0.0);
         cf += att * gl_FrontMaterial.specular.rgb *
         gl_LightSource[0].specular.rgb *pow(NdotHV,gl_FrontMaterial.shininess);
       }
     }
     gl_FragColor = vec4(ct * cf, at * af);
}