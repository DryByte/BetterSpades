
struct RainDrop {
	float x, y, z;
	float next_hit;
};

void rain_render(void);
void rain_update(float dt);
void add_raindrop(float x, float y, float z);
void rain_init(void);