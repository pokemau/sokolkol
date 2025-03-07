#define SOKOL_IMPL
#define SOKOL_GLCORE

#include "sokol_app.h"
#include "sokol_gfx.h"
#include "sokol_log.h"
#include "sokol_glue.h"
#include "cglm/cglm.h"
#include "cglm/mat4.h"


#include <stdint.h>

#include "../shd.h"


static struct {
    float rx, ry;
    sg_pipeline pip;
    sg_bindings bind;
    sg_pass_action pass_action;
} state;

static void init(void) {

    sg_setup(&(sg_desc){
        .environment = sglue_environment(),
        .logger.func = slog_func,
    });


    /* cube vertex buffer */
    float vertices[] = {
        -1.0, -1.0, -1.0,   1.0, 0.0, 0.0, 1.0,
         1.0, -1.0, -1.0,   1.0, 0.0, 0.0, 1.0,
         1.0,  1.0, -1.0,   1.0, 0.0, 0.0, 1.0,
        -1.0,  1.0, -1.0,   1.0, 0.0, 0.0, 1.0,

        -1.0, -1.0,  1.0,   0.0, 1.0, 0.0, 1.0,
         1.0, -1.0,  1.0,   0.0, 1.0, 0.0, 1.0,
         1.0,  1.0,  1.0,   0.0, 1.0, 0.0, 1.0,
        -1.0,  1.0,  1.0,   0.0, 1.0, 0.0, 1.0,

        -1.0, -1.0, -1.0,   0.0, 0.0, 1.0, 1.0,
        -1.0,  1.0, -1.0,   0.0, 0.0, 1.0, 1.0,
        -1.0,  1.0,  1.0,   0.0, 0.0, 1.0, 1.0,
        -1.0, -1.0,  1.0,   0.0, 0.0, 1.0, 1.0,

        1.0, -1.0, -1.0,    1.0, 0.5, 0.0, 1.0,
        1.0,  1.0, -1.0,    1.0, 0.5, 0.0, 1.0,
        1.0,  1.0,  1.0,    1.0, 0.5, 0.0, 1.0,
        1.0, -1.0,  1.0,    1.0, 0.5, 0.0, 1.0,

        -1.0, -1.0, -1.0,   0.0, 0.5, 1.0, 1.0,
        -1.0, -1.0,  1.0,   0.0, 0.5, 1.0, 1.0,
         1.0, -1.0,  1.0,   0.0, 0.5, 1.0, 1.0,
         1.0, -1.0, -1.0,   0.0, 0.5, 1.0, 1.0,

        -1.0,  1.0, -1.0,   1.0, 0.0, 0.5, 1.0,
        -1.0,  1.0,  1.0,   1.0, 0.0, 0.5, 1.0,
         1.0,  1.0,  1.0,   1.0, 0.0, 0.5, 1.0,
         1.0,  1.0, -1.0,   1.0, 0.0, 0.5, 1.0
    };

    sg_shader shd = sg_make_shader(quad_shader_desc(sg_query_backend()));

    sg_buffer vbuf = sg_make_buffer(&(sg_buffer_desc){
        .data = SG_RANGE(vertices),
        .label = "cube-vertices"
    });

    uint16_t indices[] = {
        0, 1, 2,  0, 2, 3,
        6, 5, 4,  7, 6, 4,
        8, 9, 10,  8, 10, 11,
        14, 13, 12,  15, 14, 12,
        16, 17, 18,  16, 18, 19,
        22, 21, 20,  23, 22, 20
    };

    sg_buffer ibuf = sg_make_buffer(&(sg_buffer_desc){
        .type = SG_BUFFERTYPE_INDEXBUFFER,
        .data = SG_RANGE(indices),
        .label = "cube-indices"
    });

    state.pip = sg_make_pipeline(&(sg_pipeline_desc){
        .layout = {
            .buffers[0].stride = 28,
            .attrs = {
                [ATTR_quad_position].format = SG_VERTEXFORMAT_FLOAT3,
                [ATTR_quad_color0].format = SG_VERTEXFORMAT_FLOAT4,
            },

        },
        .shader = shd,
        .index_type = SG_INDEXTYPE_UINT16,
        .cull_mode = SG_CULLMODE_BACK,
        .depth = {
            .write_enabled = true,
            .compare = SG_COMPAREFUNC_LESS_EQUAL
        },
        .label = "cube-pipeline"
    });


    state.bind = (sg_bindings) {
        .vertex_buffers[0] = vbuf,
        .index_buffer = ibuf
    };
}

static void frame(void) {

   // vs_params_t vs_params;
    const float w = sapp_widthf();
    const float h = sapp_heightf();
    const float t = (float)(sapp_frame_duration() * 60.0);
    mat4 proj, view, view_proj;
    glm_perspective(60.0f, w/h, 0.01f, 10.0f, proj);
    glm_lookat((vec3){0.0,1.5,6.0f}, (vec3){0,0,0},(vec3){0,1.0f,0}, view);
    glm_mat4_mul(proj, view, view_proj);
    state.rx += 1.0f * t; state.ry += 2.0f * t;

    mat4 rxm, rym, model;
    glm_rotate(rxm, state.rx, (vec3){1,0,0});
    glm_rotate(rym, state.ry, (vec3){0,1,0});
    glm_mat4_mul(rxm, rym, model);
    glm_mat4_mul(view_proj, model, vs_params.mvp);

//    sg_begin_pass(&(sg_pass){
//        .action = state.pass_action,
//        .swapchain = sglue_swapchain()
//    });
    sg_begin_pass(&(sg_pass){
        .action = {
                .colors[0] = {
                        .load_action = SG_LOADACTION_CLEAR,
                        .clear_value = { 0.25f, 0.5f, 0.75f, 1.0f }
                    },
            },
        .swapchain = sglue_swapchain()
    });
    sg_apply_pipeline(state.pip);
    sg_apply_bindings(&state.bind);
    sg_apply_uniforms(UB_vs_params, &SG_RANGE(vs_params));
    sg_draw(0, 36, 1);

//    sg_apply_pipeline(state.pip);
//    sg_apply_bindings(&state.bind);
//    sg_draw(0, 36, 1);
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    sg_shutdown();
}

void event(const sapp_event *e) {

    if (e->type == SAPP_EVENTTYPE_KEY_DOWN) {

    }
}

sapp_desc sokol_main(int argc, char *argv[]) {
    return (sapp_desc){.init_cb = init,
                       .frame_cb = frame,
                       .cleanup_cb = cleanup,
                       .event_cb = event,
                       .width = 800,
                       .height = 500,
                       .window_title = "Sokolkol",
                       .logger.func = slog_func,
                       .icon.sokol_default = true};
}
