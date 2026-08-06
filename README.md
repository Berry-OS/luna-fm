**🌙 Luna Files**

A lightweight Linux file manager built with the single-header **Luna UI** engine. Luna Files combines familiar PCManFM-style workflows with a polished macOS-inspired interface, smooth grid and list views, and a CSS-driven appearance.

[![Sponsor](https://img.shields.io/badge/Sponsor%20this%20project-%E2%9D%A4%EF%B8%8F-white?logo=githubsponsors&logoColor=EA4AAA&labelColor=EA4AAA)](https://github.com/sponsors/yui0)

![Luna Files grid view](docs/screenshots/luna-files-overview.jpg)
![Luna Files list view](docs/screenshots/luna-files-documents.jpg)
![Luna Files dark appearance settings](docs/screenshots/luna-files-settings-dark.jpg)

---

## ✨ Highlights

- Familiar sidebar shortcuts for Home, Desktop, Documents, Downloads, Pictures, Music, Videos, Trash, and the filesystem root
- Back, forward, up, reload, location entry, and live folder search
- Responsive icon and list views with hidden-file toggling and name, date, or size sorting
- Single, `Ctrl` multi-selection, and `Shift` range selection
- Double-click or `Enter` to open files and folders, plus a right-click context menu
- New-folder, rename, properties, Trash, and permanent-delete workflows
- File clipboard with `Ctrl+C`, `Ctrl+X`, and `Ctrl+V`
- Light and dark themes, accent colors, font controls, display density, and animation settings
- Optional Wayland and X11 clipboard interoperability

## 🛠️ Build

### Debian / Ubuntu

```bash
sudo apt install build-essential pkg-config libglfw3-dev libgl1-mesa-dev
make
```

The project includes `luna-ui.h`. Keep the following supporting single-header libraries in the same directory:

```text
stb_truetype.h
stb_image.h
stb_image_write.h
cssparser.h
```

## 🚀 Run

Open the home directory:

```bash
./luna-fm
```

Open a specific folder:

```bash
./luna-fm /path/to/folder
```

For richer file clipboard integration on Wayland:

```bash
sudo apt install wl-clipboard
```

For X11:

```bash
sudo apt install xclip
```

Luna Files still provides its internal clipboard and GLFW text clipboard when these optional tools are unavailable.

## ⌨️ Keyboard Shortcuts

| Shortcut | Action |
|---|---|
| `Enter` | Open the selected item |
| `Backspace` | Go back |
| `F2` | Rename |
| `Delete` | Move to Trash |
| `Ctrl+A` | Select all |
| `Ctrl+C` | Copy |
| `Ctrl+X` | Cut |
| `Ctrl+V` | Paste |
| `Ctrl+L` | Focus the location bar |
| `Esc` | Close the active menu or dialog |

## 🎨 Luna UI

The interface is rendered by `luna-ui.h`, a single-header HTML/CSS-to-OpenGL engine. Layout, controls, shadows, rounded surfaces, themes, and interaction states are CSS-driven rather than drawn as a traditional immediate-mode interface.

## ⚡ Implementation Notes

Luna Files creates 384 reusable display slots at startup. Folder navigation, searching, sorting, and view changes update slot text and classes with `luna_set_text()` instead of reparsing the complete HTML document. This keeps routine directory refreshes lightweight while retaining a fully styled UI.

## 📄 License

Luna Files and Luna UI are distributed under the Mozilla Public License 2.0.
