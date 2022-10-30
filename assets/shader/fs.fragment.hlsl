#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 ourTexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // mix函数需要接受两个值作为参数，并对它们根据第三个参数进行线性插值
    // 第三参数为0, 返回第一个值; 为1, 返回第二个值;
    // 0.2表示 第一个的80% 第二个的20% 混合
    FragColor = mix(texture(texture1, ourTexCoord), texture(texture2, ourTexCoord), 0.2);
}