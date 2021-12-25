#include "rain.h"
#include "entitysystem.h"
#include "map.h"
#include "common.h"
#include "network.h"
#include "player.h"
#include "particle.h"

struct entity_system raindrops;
int rcolor[3] = {153, 216, 255};

static bool rain_render_single(void* obj, void* user) {
	struct RainDrop* drop = (struct RainDrop*)obj;

	glColor3ub(rcolor[0], rcolor[1], rcolor[2]);
	glBegin(GL_LINES);
		glVertex3f(drop->x, drop->y, drop->z);
		glVertex3f(drop->x, drop->y+0.5, drop->z);
	glEnd();

	return false;
}

static bool rain_update_single(void* obj, void* user) {
	if(!network_connected)
		return true;

	struct RainDrop* drop = (struct RainDrop*)obj;

	if (drop->y < drop->next_hit){
		particle_create(rgb(rcolor[0],rcolor[1],rcolor[2]),
						drop->x, drop->y+2, drop->z, 2.5F,
						1.0F, 3, 0.07F, 0.1F);

		drop->x=(players[local_player_id].pos.x+64)+(rand()%128)+(-128);
		drop->y=rand()%65;
		drop->z=(players[local_player_id].pos.z+64)+(rand()%128)+(-128);
		drop->next_hit = map_height_at(drop->x, drop->z);
	}

	drop->y-=0.3;

	return false;
}

void rain_render() {
	entitysys_iterate(&raindrops, NULL, rain_render_single);
}

void rain_update(float dt) {
	entitysys_iterate(&raindrops, &dt, rain_update_single);
}

void add_raindrop(float x, float y, float z) {
	entitysys_add(&raindrops, 
				  &(struct RainDrop) {
			  		  .x = x,
			  		  .y = y,
			  		  .z = z,
			  		  .next_hit = map_height_at(x,z)
				  });
}

void rain_init() {
	entitysys_create(&raindrops, sizeof(struct RainDrop), 1200);
}