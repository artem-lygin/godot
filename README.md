# Demo of advanced syntax highlighting

**This a fork of [Godot Engine](https://godotengine.org).** This fork is used to demonstrate syntax highlighting feature for GDScript in Godot Engine, allowing different font styles (Bold, Italic) for various syntax elements in the script editor. This code does not ment to be used in real project.

[![Video demonstration](https://img.youtube.com/vi/pkU5qcE-Os8/maxresdefault.jpg)](https://www.youtube.com/watch?v=pkU5qcE-Os8)

## Engine Changes Documentation

### 1. Core Engine Modifications

#### `scene/resources/syntax_highlighter.h` & `scene/resources/syntax_highlighter.cpp`
The `CodeHighlighter` class has been extended to support font styling alongside color highlighting for keywords, member keywords, and color regions.

**New member variables:**
- `keyword_styles`: Dictionary mapping keywords to their font style (int).
- `member_keyword_styles`: Dictionary mapping member keywords to their font style (int).
- `ColorRegion::font_style`: int field added to the `ColorRegion` struct.

**New methods:**
- `add_keyword_style(keyword, font_style)` / `remove_keyword_style` / `has_keyword_style` / `get_keyword_style` / `set_keyword_styles` / `clear_keyword_styles`
- Equivalent methods for `member_keyword_styles`.
- `add_color_region(...)`: accepts an optional `p_font_style` argument (default 0) to define the font style for the region.

**Logic updates:**
- `_get_line_syntax_highlighting_impl`: per-column Dictionary entries now include `"bold": true` and/or `"italic": true` keys alongside `"color"`.

#### `scene/gui/text_edit.h` & `scene/gui/text_edit.cpp`
The `TextEdit` control has been updated to render text using the font styles provided by the syntax highlighter.

**Rendering:**
- During text shaping, `TextEdit` reads `"bold"`/`"italic"` keys from the syntax highlighting Dictionary and selects `font_bold` or `font_italic` from the theme for that text segment.
- `ThemeCache` exposes `font_bold` and `font_italic` entries (bound via `BIND_THEME_ITEM`).

**Re-shaping fix:**
- `set_syntax_highlighter()` now forces re-shaping of all text lines when the highlighter changes. This ensures bold/italic font choices are applied immediately, fixing a regression introduced when the upstream refactor changed the order in which the syntax highlighter is assigned relative to the node entering the scene tree.

### 2. GDScript Module Modifications

#### `modules/gdscript/editor/gdscript_highlighter.cpp` & `.h`
The GDScript syntax highlighter leverages the new font-style infrastructure to expose per-token styling via editor settings.

**Per-token font style settings** (all under `text_editor/theme/highlighting/`):
- `symbol_font_style`, `keyword_font_style`, `control_flow_keyword_font_style`
- `base_type_font_style`, `engine_type_font_style`, `user_type_font_style`
- `comment_font_style`, `doc_comment_font_style`, `string_font_style`
- `function_font_style`, `member_variable_font_style`, `number_font_style`
- `text_font_style`
- `gdscript/node_path_font_style`, `gdscript/node_reference_font_style`
- `gdscript/annotation_font_style`, `gdscript/string_name_font_style`

Each setting accepts `0 = Normal`, `1 = Bold`, `2 = Italic`.

**NodePath highlighting:** NodePath literals (`^"..."`, `^'...'`) are correctly styled across their full length, including the leading `^` and the quoted string.

### 3. Editor Settings & Theme

#### `editor/settings/editor_settings.cpp`
- Registers all `*_font_style` settings listed above with `PROPERTY_HINT_ENUM` (`"Normal,Bold,Italic"`), default `0`.
- Registers `interface/editor/fonts/code_font_bold` and `interface/editor/fonts/code_font_italic` for selecting the bold/italic monospace font files.

#### `editor/themes/editor_fonts.cpp`
- Loads `mono_bold_fc` and `mono_italic_fc` font face configurations.
- Registers them as `source_bold` and `source_italic` in the `EditorFonts` theme.

#### `editor/themes/editor_theme_manager.cpp`
- Sets `font_bold` and `font_italic` on the `CodeEdit` theme type from `source_bold`/`source_italic`, making them available to `TextEdit` for bold/italic rendering.

## Font Style Bitmask

| Value | Meaning |
|-------|---------|
| 0 | Normal |
| 1 | Bold |
| 2 | Italic |
| 3 | Bold + Italic |

## Summary

These changes add per-token font styling (Bold/Italic) to the Godot script editor's GDScript syntax highlighting. Each syntax element (keywords, strings, comments, types, NodePaths, etc.) can be independently configured to appear in Normal, Bold, or Italic weight via Editor Settings. The implementation is generic — `CodeHighlighter` and `TextEdit` changes are language-agnostic and available to any syntax highlighter or plugin.