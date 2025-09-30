#pragma once

#ifndef WLR_USE_UNSTABLE
    #define WLR_USE_UNSTABLE 1
#endif

/**
 * This file is used to put all wlroots headers needed in the Wayfire API
 * in an extern "C" block because wlroots headers are not always compatible
 * with C++.
 */
extern "C"
{
    struct wlr_backend;
    struct wlr_renderer;
    struct wlr_seat;
    struct wlr_cursor;
    struct wlr_data_device_manager;
    struct wlr_data_control_manager_v1;
    struct wlr_gamma_control_manager_v1;
    struct wlr_xdg_output_manager_v1;
    struct wlr_export_dmabuf_manager_v1;
    struct wlr_server_decoration_manager;
    struct wlr_input_inhibit_manager;
    struct wlr_idle_inhibit_manager_v1;
    struct wlr_xdg_decoration_manager_v1;
    struct wlr_virtual_keyboard_manager_v1;
    struct wlr_virtual_pointer_manager_v1;
    struct wlr_idle_notifier_v1;
    struct wlr_screencopy_manager_v1;
    struct wlr_ext_foreign_toplevel_list_v1;
    struct wlr_ext_image_copy_capture_manager_v1;
    struct wlr_ext_output_image_capture_source_manager_v1;
    struct wlr_foreign_toplevel_manager_v1;
    struct wlr_pointer_gestures_v1;
    struct wlr_relative_pointer_manager_v1;
    struct wlr_pointer_constraints_v1;
    struct wlr_tablet_manager_v2;
    struct wlr_input_method_manager_v2;
    struct wlr_text_input_manager_v3;
    struct wlr_presentation;
    struct wlr_primary_selection_v1_device_manager;
    struct wlr_drm_lease_v1_manager;
    struct wlr_session_lock_manager_v1;

    struct wlr_xdg_foreign_v1;
    struct wlr_xdg_foreign_v2;
    struct wlr_xdg_foreign_registry;

    struct wlr_pointer_axis_event;
    struct wlr_pointer_motion_event;
    struct wlr_output_layout;
    struct wlr_surface;
    struct wlr_texture;
    struct wlr_viewporter;

    struct wlr_ext_data_control_manager_v1;

#include <wlr/types/wlr_input_device.h>
#include <wlr/types/wlr_pointer.h>
#include <wlr/types/wlr_keyboard.h>
#include <wlr/types/wlr_touch.h>
#define static
#include <wlr/types/wlr_output.h>
#undef static
#include <wlr/backend/session.h>
#include <wlr/util/box.h>
#include <wlr/util/edges.h>
#include <wayland-server.h>
#include <wlr/config.h>

    static constexpr uint32_t WLR_KEY_PRESSED  = WL_KEYBOARD_KEY_STATE_PRESSED;
    static constexpr uint32_t WLR_KEY_RELEASED = WL_KEYBOARD_KEY_STATE_RELEASED;

    struct mwlr_keyboard_modifiers_event
    {
        uint32_t time_msec;
    };

    enum wlr_text_input_v3_features {
        WLR_TEXT_INPUT_V3_FEATURE_SURROUNDING_TEXT = 1 << 0,
        WLR_TEXT_INPUT_V3_FEATURE_CONTENT_TYPE = 1 << 1,
        WLR_TEXT_INPUT_V3_FEATURE_CURSOR_RECTANGLE = 1 << 2,
    };

    struct text_input_state_base_t {
        struct {
            char *text;
            uint32_t cursor;
            uint32_t anchor;
        } surrounding;

        uint32_t text_change_cause;

        struct {
            uint32_t hint;
            uint32_t purpose;
        } content_type;


        uint32_t features;

        struct wlr_box cursor_rectangle;
    };

    struct text_input_v1_state: text_input_state_base_t {
        uint32_t features = (uint32_t)0; // unsuppported in text-input-v1
        uint32_t text_change_cause = (uint32_t)0; // unsuppported in text-input-v1
    };

    struct text_input_v3_state: text_input_state_base_t {
    };

    struct text_input_base_t {
        uint32_t version;
        struct wlr_seat *seat;
        struct wl_resource *resource;
        struct wlr_surface *focused_surface;
        struct text_input_state_base_t pending;
        struct text_input_state_base_t current;

        uint32_t current_serial;
        bool pending_enabled;
        bool current_enabled;

        struct wl_list link;

        struct wl_listener surface_destroy;
        struct wl_listener seat_destroy;

        struct {
            struct wl_signal enable;
            struct wl_signal commit;
            struct wl_signal disable;
            struct wl_signal destroy;
        } events;
    };

    struct wf_text_input_v1 : text_input_base_t {
        uint32_t version = (uint32_t)1;
        struct text_input_v1_state pending;
        struct text_input_v1_state current;
    };

    struct wlr_text_input_v3 : text_input_base_t {
        uint32_t version = (uint32_t)3;
        struct text_input_v3_state pending;
        struct text_input_v3_state current;

        uint32_t active_features;
    };

    struct text_input_manager_v1 {
        struct wl_global *global;
        struct wl_list text_inputs;

        struct wl_listener display_destroy;

        struct {
            struct wl_signal text_input; // struct wf_text_input_v1
            struct wl_signal destroy; // struct input_method_manager_v1
        } events;
    };

    struct text_input_manager_v1 *text_input_manager_v1_create(
        struct wl_display *wl_display);

    struct wlr_text_input_manager_v3 {
        struct wl_global *global;
        struct wl_list text_inputs; // struct wlr_text_input_v3.resource.link

        struct wl_listener display_destroy;

        struct {
            struct wl_signal text_input; // struct wlr_text_input_v3
            struct wl_signal destroy; // struct wlr_input_method_manager_v3
        } events;
    };

    struct wlr_text_input_manager_v3 *wlr_text_input_manager_v3_create(
        struct wl_display *wl_display);

    // Sends enter to the surface and saves it
    void wlr_text_input_v3_send_enter(struct wlr_text_input_v3 *text_input,
        struct wlr_surface *wlr_surface);
    // Sends leave to the currently focused surface and clears it
    void wlr_text_input_v3_send_leave(struct wlr_text_input_v3 *text_input);
    void wlr_text_input_v3_send_preedit_string(struct wlr_text_input_v3 *text_input,
        const char *text, int32_t cursor_begin, int32_t cursor_end);
    void wlr_text_input_v3_send_commit_string(struct wlr_text_input_v3 *text_input,
        const char *text);
    void wlr_text_input_v3_send_delete_surrounding_text(
        struct wlr_text_input_v3 *text_input, uint32_t before_length,
        uint32_t after_length);
    void wlr_text_input_v3_send_done(struct wlr_text_input_v3 *text_input);

}
