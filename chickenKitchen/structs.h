#pragma once

struct moved 
{
	GLfloat rot, dist, height;
} 
viewed = {0, -2, 15};

struct point_struct 
{
	GLfloat x, y, z;
} 
point = {0.f, 0.f, 0.f};

struct angle_struct 
{
	GLfloat x, y, z;
} 
angle = {0.f, 0.f, 0.f};

struct room_struct 
{
	GLfloat scale_x, scale_y, scale_z;
} 
room_s = {};

struct table_struct 
{
	GLfloat x, y, z, scale_x, scale_y, scale_z;
} 
table_s = {};

struct bot_struct 
{
	GLfloat x, y, z, start_height;
} 
bot_s = {};

struct progress_counter_struct 
{
	GLint progress, chicken_speed;
} 
pcs = {65000, 1};

struct scenario 
{
	GLint rozruch_kury, uprawa, mieszanie, pieczenie, wyciag_kromki_na_patelnie, smazenie, podrzut, podanie_podanie_na_talerzu, koniec;
} 
scenario = {0, 720, 1440, 2160, 2880, 3140, 4680, 5400, 5760}; 

