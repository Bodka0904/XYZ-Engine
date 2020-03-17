#type compute
#version 430
//Size of compute shader local work group - x=32, y=32, z=1(default)
layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

const int c_MaxParticles = 10000;

struct ParticleVertex
{
	vec4 color;
	vec2 position;
	vec2 texCoord;
	vec2 size;
	float angle;
	float alignment;
};
struct ParticleData
{
	vec4 colorBegin;
	vec4 colorEnd;
	vec2 velocity;
	float sizeBegin;
	float sizeEnd;
	float lifeTime;
	float timeAlive;
	float alignment1;
	float alignment2;
};

layout(std430, binding = 0) buffer
buffer_InPos
{
	ParticleVertex InVertex[];
};

layout(std430, binding = 1) buffer
buffer_InData
{
	ParticleData InData[];
};

uniform float u_Time;
uniform float u_Speed;


float rand(vec2 co)
{
	return fract(sin(dot(co.xy, vec2(12.9898, 78.233))) * 43758.5453) * 1000.0;
}

vec4 ChangeColorOverLife(ParticleData data)
{
	return mix(data.colorBegin, data.colorEnd, data.timeAlive / data.lifeTime);
}
vec2 ChangeSizeOverLife(ParticleData data)
{
	vec2 begin = vec2(data.sizeBegin, data.sizeBegin);
	vec2 end = vec2(data.sizeEnd, data.sizeEnd);
	return mix(begin,end, data.timeAlive / data.lifeTime);
}
void main(void)
{
	uint index = gl_GlobalInvocationID.x + gl_GlobalInvocationID.y * gl_NumWorkGroups.x * gl_WorkGroupSize.x;

	if (index > c_MaxParticles)
		return;

	ParticleVertex pVertex = InVertex[index];
	ParticleData pData = InData[index];

	pVertex.position.x += (pData.velocity.x * u_Speed) * u_Time;
	pVertex.position.y += (pData.velocity.y * u_Speed) * u_Time; 
	pVertex.angle += u_Time * 100;
	pVertex.color = ChangeColorOverLife(pData);
	pVertex.size = ChangeSizeOverLife(pData);

	pData.timeAlive += u_Time;

	if (pData.timeAlive > pData.lifeTime)
	{
		pData.timeAlive = 0;
		pVertex.position = vec2(0, 0);
	}

	InVertex[index] = pVertex;
	InData[index] = pData;
}