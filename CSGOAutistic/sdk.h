#pragma once

#include <cstdint>
#include <iostream>

namespace valve
{
	namespace sdk
	{
		class PlayerEntity;
		class Bone;

		struct Vector3
		{
			float x;
			float y;
			float z;

			Vector3& operator +(Vector3 const& rhs)
			{
				x += rhs.x;
				y += rhs.y;
				z += rhs.z;
				return *this;
			}
		};

		struct Color
		{
			std::uint32_t r;
			std::uint32_t g;
			std::uint32_t b;
			std::uint32_t a;
		};

		struct Vector2
		{
			std::uint32_t x;
			std::uint32_t y;
		};

		typedef float ViewMatrix[16];

		typedef struct EntityList_
		{
			PlayerEntity *entity;
			char pad_0[0xC];
		} *EntityList;

		class PlayerEntity
		{
		public:
			char pad_0x0000[0xEC]; //0x0000
			std::uint32_t is_dormant; //0x00EC 
			char pad_0x00F0[0x4]; //0x00F0
			std::int32_t team_number; //0x00F4 
			char pad_0x00F8[0x8]; //0x00F8
			std::int32_t health; //0x0100 
			char pad_0x0104[0x4]; //0x0104
			valve::sdk::Vector3 vec_view_offset; //0x108
			valve::sdk::Vector3 move_direction; //0x0114 
			char pad_0x0120[0xC]; //0x0120
			valve::sdk::Vector3 view_angles; //0x012C 
			valve::sdk::Vector3 origin; //0x0138
			char pad_0x144[0x2564]; // 0x0144
			float *bone_matrix; // 0x26A8
			char pad_0x26AC[0x980]; // 0x26AC
			valve::sdk::Vector3 aim_punch_angle; // 0x302C
			valve::sdk::Vector3 aim_punch_angle_velocity; // 0x3038
		}; //Size=0x3044

		class Bone
		{
		public:
			char pad_0x0000[0xC]; //0x0000
			float x; //0x000C 
			char pad_0x0010[0xC]; //0x0010
			float y; //0x001C 
			char pad_0x0020[0xC]; //0x0020
			float z; //0x002C 
		};

		typedef Bone bone_matrix[93];

		//struct matrix3x4_t
		//{
		//	matrix3x4_t() {}
		//	matrix3x4_t(
		//		float m00, float m01, float m02, float m03,
		//		float m10, float m11, float m12, float m13,
		//		float m20, float m21, float m22, float m23)
		//	{
		//		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		//		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		//		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
		//	}

		//	float m_flMatVal[3][4];
		//};

	}
}