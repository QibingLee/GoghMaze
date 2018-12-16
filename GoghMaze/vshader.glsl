in vec3 vNormal;
in vec4 vPosition;
in vec2 vTexCoord;
out float mode;
out float dist;
out vec2 texCoord;
out vec3 lightDir,normal,halfVector;
out vec4 diffuse,ambient,ambientGlobal;

void main()
{
        texCoord = vTexCoord;
        gl_Position = gl_ModelViewProjectionMatrix * vPosition;
        normal = normalize(gl_NormalMatrix * vNormal);
        halfVector = normalize(gl_LightSource[0].halfVector.xyz);
        diffuse =  gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
        ambient =  gl_FrontMaterial.ambient * gl_LightSource[0].ambient;
        ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;

        if(gl_LightSource[0].position.w ==0){
           lightDir = normalize(vec3(gl_LightSource[0].position));
           mode=1;
        }
        else if(gl_LightSource[0].spotDirection.x==0 && gl_LightSource[0].spotDirection.y==0
          &&gl_LightSource[0].spotDirection.z==0){
           vec3 dic = vec3(gl_LightSource[0].position-gl_Position);
           lightDir = normalize(dic);
           dist = length(dic);
           mode=2;
        }
        else{
           vec3 dic = vec3(gl_LightSource[0].position-gl_Position);
           lightDir = normalize(dic);
           dist = length(dic);
           mode=3;
        }

} 
