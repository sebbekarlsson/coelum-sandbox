#include <coelum/main.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <coelum/constants.h>
#include <coelum/input.h>
#include <coelum/scene.h>
#include <coelum/actor.h>
#include <coelum/utils.h>


extern theatre_T* THEATRE;
extern keyboard_state_T* KEYBOARD_STATE;
extern mouse_state_T* MOUSE_STATE;

actor_T* weapon;


#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif

#ifndef GL_RGB
#define GL_RGB 0x1907
#endif

float i = 0.0f;
float distance = 0.0f;


void custom_tick(actor_T* self)
{
    return;
    if (self->y < 480.0f - 32)
    {
        self->dy += 0.1f;
    }
    else
    {
        if (self->dy > 1.6f)
            self->dy -= self->dy * 1.9f;
        else
            self->dy = 0.0f;

        self->y = 480.0f - 32;

        if (KEYBOARD_STATE->keys[GLFW_KEY_SPACE])
            self->dy -= 1.9f;
    }

    self->y += self->dy;
}

void custom_scene_tick(scene_T* self)
{
    state_T* state = (state_T*) self;
    state->camera->rx += MOUSE_STATE->dy * 0.25f;
    state->camera->ry += MOUSE_STATE->dx * 0.25f;

    if (KEYBOARD_STATE->keys[GLFW_KEY_W])
    {
        state->camera->x -= cos(to_radians(state->camera->ry + 90.0f));
        state->camera->z += sin(to_radians(state->camera->ry + 90.0f));
        distance += 0.3f;
    }

    if (KEYBOARD_STATE->keys[GLFW_KEY_S])
    {
        state->camera->x += cos(to_radians(state->camera->ry + 90.0f));
        state->camera->z -= sin(to_radians(state->camera->ry + 90.0f));
    }


    state->camera->y = -16 - (cos(distance) * 0.5f);

    weapon->x = -state->camera->x + (cos(to_radians(state->camera->ry + 90.0f)) * 24.0f);
    weapon->y = state->camera->y + 16;
    weapon->z = -state->camera->z - (sin(to_radians(state->camera->ry + 90.0f)) * 24.0f);
    weapon->ry = state->camera->ry + 120.0f;
    weapon->rx = state->camera->rx + 90.0f;
    weapon->rz = state->camera->rx + 90.0f;

    //weapon->x -= ;
    //weapon->z += ;
}

scene_T* init_scene_main()
{
    // creating a scene                          tick               draw       dimensions
    scene_T* s = scene_constructor(init_scene(), custom_scene_tick, (void*) 0, 3);
    s->bg_r = 154;
    s->bg_g = 55;
    s->bg_g = 200;

    for (int x = 0; x < 16; x++)
    {
        for (int z = 0; z < 24; z++)
        {
            // creating an actor
            actor_T* a = actor_constructor(
                init_actor(),
                (-x * 32) + ((16 * 32) / 2),
                0.0f,
                -z * 32,
                custom_tick,
                (void*) 0, // draw method
                "grass"
            );
            a->width = 32;
            a->height = 32;
            a->rx = 90.0f;
            a->texture = get_texture("res/grass.png", GL_RGB)->renderable_texture;
           
            // adding the actor to the scene 
            dynamic_list_append(((state_T*)s)->actors, a);
        }
    }

    ((state_T*)s)->camera->z = 32;
    ((state_T*)s)->camera->y = -16;

    weapon = actor_constructor(
        init_actor(),
        0.0f, 0.0f, 0.0f,
        (void*)0, (void*)0,
        "weapon"    
    );
    weapon->width = 16;
    weapon->height = 16;
    weapon->texture = get_texture("res/sword.gif", GL_RGBA)->renderable_texture;

    dynamic_list_append(((state_T*)s)->actors, weapon);

    return s;
} 

int main(int argc, char* argv[])
{
    coelum_init();

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    return coelum_main(argc, argv);
}
