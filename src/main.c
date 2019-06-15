#include <coelum/main.h>
#include <coelum/scene.h>
#include <coelum/theatre.h>
#include <coelum/constants.h>
#include <coelum/input.h>
#include <coelum/scene.h>
#include <coelum/actor.h>
#include <coelum/utils.h>
#include <coelum/textures.h>
#include <coelum/draw_utils.h>
#include <coelum/scene_manager.h>
#include <coelum/actor_light.h>
#include <objload/obj.h>


extern theatre_T* THEATRE;
extern keyboard_state_T* KEYBOARD_STATE;
extern mouse_state_T* MOUSE_STATE;

actor_T* weapon;
obj_T* obj;
unsigned int tex;


#ifndef GL_RGBA
#define GL_RGBA 0x1908
#endif

#ifndef GL_RGB
#define GL_RGB 0x1907
#endif

float i = 0.0f;
float distance = 0.0f;


void custom_scene_tick(scene_T* self)
{
    state_T* state = (state_T*) self;
    state->camera->rx += MOUSE_STATE->dy * 0.25f;
    state->camera->ry += MOUSE_STATE->dx * 0.25f;

    if (KEYBOARD_STATE->keys[GLFW_KEY_W])
    {
        state->camera->x -= cos(glm_rad(state->camera->ry + 90.0f));
        state->camera->z += sin(glm_rad(state->camera->ry + 90.0f));
        distance += 0.3f;
    }

    if (KEYBOARD_STATE->keys[GLFW_KEY_S])
    {
        state->camera->x += cos(glm_rad(state->camera->ry + 90.0f));
        state->camera->z -= sin(glm_rad(state->camera->ry + 90.0f));
    }


    state->camera->y = -16 - (cos(distance) * 0.5f);

    weapon->x = -state->camera->x + (cos(glm_rad(state->camera->ry + 90.0f)) * 24.0f);
    weapon->y = state->camera->y + 18 + (tan(glm_rad(state->camera->rx)) * 24.0f);
    weapon->z = -state->camera->z - (sin(glm_rad(state->camera->ry + 90.0f)) * 24.0f);
    weapon->ry = state->camera->ry + 120.0f;

    weapon->rz = 90 + state->camera->rx;
}

void custom_actor_draw(actor_T* actor)
{
    actor_draw(actor);
    scene_T* scene = scene_manager_get_current_scene(THEATRE->scene_manager);
    state_T* state = (state_T*) scene;

    draw_3D_axis(
        actor->x, actor->y, actor->z,
        actor->width, actor->height, actor->width,
        state
    );

    draw_3D_model(obj, tex, 0.0f, 6.0f, 0.0f, 255.0f, 255.0f, 255.0f, state);
}

scene_T* init_scene_main()
{
    // creating a scene                          tick               draw       dimensions
    scene_T* s = scene_constructor(init_scene(), custom_scene_tick, (void*) 0, 3);
    s->bg_r = 154;
    s->bg_g = 55;
    s->bg_g = 200;
    ((state_T*) s)->lighting_enabled = 1;

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
                (void*) 0,
                (void*) 0,
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
        (void*)0, custom_actor_draw,
        "weapon"    
    );
    weapon->width = 16;
    weapon->height = 16;
    weapon->texture = get_texture("res/sword.gif", GL_RGBA)->renderable_texture;

    dynamic_list_append(((state_T*)s)->actors, weapon);

    actor_light_T* light = init_actor_light(
        0.0f, 0.0f, -12.0f,
        1.0f        
    );

    dynamic_list_append(((state_T*)s)->actors, light);

    return s;
} 

int main(int argc, char* argv[])
{

    coelum_init();
    
    obj_list_T* object_list = obj_load_from_file("res/block.obj");
    obj = object_list->objects[0];

    MOUSE_STATE->input_mode = GLFW_CURSOR_DISABLED;

    tex = get_texture("res/cobble.png", GL_RGBA)->renderable_texture;

    scene_manager_register_scene(THEATRE->scene_manager, (scene_T*) init_scene_main());

    int status = coelum_main(argc, argv);

    obj_list_free(object_list);

    return status;
}
