#pragma once

#include <cstdint>
#include <iostream>

namespace valve
{
	namespace sdk
	{
		class PlayerEntity;

		typedef struct
		{
			float x;
			float y;
			float z;
		} Vector3;

		typedef struct
		{
			std::uint32_t r;
			std::uint32_t g;
			std::uint32_t b;
			std::uint32_t a;
		} Color;

		typedef struct
		{
			std::uint32_t x;
			std::uint32_t y;
		} Vector2;

		typedef struct
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
			char pad_0x0104[0x10]; //0x0104
			valve::sdk::Vector3 move_direction; //0x0114 
			char pad_0x0120[0xC]; //0x0120
			valve::sdk::Vector3 view_angles; //0x012C 
			valve::sdk::Vector3 origin; //0x0138
		}; //Size=0x0144


	}
}