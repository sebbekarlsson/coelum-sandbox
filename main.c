#include <coelum/main.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <coelum/constants.h>
#include <coelum/input.h>
#include <coelum/scene.h>
#include <coelum/actor.h>


extern theatre_T* THEATRE;
extern keyboard_state_T* KEYBOARD_STATE;


#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif

float i = 0.0f;


void custom_tick(actor_T* self)
{
    self->ry += 0.01f;
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

scene_T* init_scene_main()
{
    // creating a scene                          tick        draw
    scene_T* s = scene_constructor(init_scene(), (void*) 0, (void*) 0);
    s->bg_r = 154;
    s->bg_g = 55;
    s->bg_g = 200;

    // creating an actor
    actor_T* a = actor_constructor(
        init_actor(),
        0.0f,
        0.0f,
        0.0f,
        custom_tick,
        (void*) 0, // draw method
        "cherry"
    );
    a->width = 32;
    a->height = 32;
    a->texture = get_texture("cherry.png", GL_RGBA)->renderable_texture;

    ((state_T*)s)->camera->z += 90;
   
    // adding the actor to the scene 
    dynamic_list_append(((state_T*)s)->actors, a);

    return s;
} 

int main(int argc, char* argv[])
{
    coelum_init();

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    return coelum_main(argc, argv);
}
