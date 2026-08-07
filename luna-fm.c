/* luna-fm — thin launcher for the reusable Luna standard file dialog. */
#define _GNU_SOURCE
#define LUNA_UI_MAX_ELEMENTS 2048
#define LUNA_UI_MAX_RULES 900
#define LUNA_UI_IMPLEMENTATION
#include "luna-ui.h"
#define LUNA_WINDOW_IMPLEMENTATION
#define LUNA_WINDOW_FILE_DIALOG_IMPLEMENTATION
#include "luna-window.h"

#include <stdio.h>
#include <string.h>

int main(int argc, char** argv) {
    LunaFileDialogConfig config;
    LunaFileDialogResult result;
    memset(&config, 0, sizeof(config));
    config.mode = LUNA_FILE_DIALOG_MANAGER;
    config.title = "Luna Files";
    config.width = 700;
    config.height = 460;

    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "--open")) config.mode = LUNA_FILE_DIALOG_OPEN_FILE;
        else if (!strcmp(argv[i], "--open-multiple")) config.mode = LUNA_FILE_DIALOG_OPEN_FILES;
        else if (!strcmp(argv[i], "--folder")) config.mode = LUNA_FILE_DIALOG_SELECT_FOLDER;
        else if (!strcmp(argv[i], "--save")) config.mode = LUNA_FILE_DIALOG_SAVE_FILE;
        else if (!strcmp(argv[i], "--client-chrome")) config.client_chrome = 1;
        else if (!strcmp(argv[i], "--name") && i + 1 < argc) config.suggested_name = argv[++i];
        else if (!strcmp(argv[i], "--filter-name") && i + 1 < argc) config.filter_name = argv[++i];
        else if (!strcmp(argv[i], "--filter") && i + 1 < argc) config.filter_patterns = argv[++i];
        else config.initial_path = argv[i];
    }

    int rc = luna_file_dialog_run(&config, &result);
    if (rc != 0) return rc;
    if (config.mode == LUNA_FILE_DIALOG_MANAGER) return 0;
    if (!result.accepted) return 1;
    for (int i = 0; i < result.count; ++i) puts(result.paths[i]);
    return 0;
}
