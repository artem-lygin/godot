# Demo of advanced syntax highlighting

**This a fork of [Godot Engine](https://godotengine.org).** This fork is used to demonstrate syntax highlighting feature for GDScript in Godot Engine, allowing different font styles (Bold, Italic) for various syntax elements in the script editor. This code does not ment to be used in real project.

[![Video demonstration](https://img.youtube.com/vi/pkU5qcE-Os8/maxresdefault.jpg)](https://www.youtube.com/watch?v=pkU5qcE-Os8)

## Engine Changes Documentation
This document outlines the modifications made to the Godot Engine core by [Gemini 3 Pro](https://gemini.google.com).

### 1. Core Engine Modifications
#### `scene/resources/syntax_highlighter.h` & `scene/resources/syntax_highlighter.cpp`
The `CodeHighlighter` class has been extended to support font styling alongside color highlighting for keywords, member keywords, and color regions.

##### New Member Variables:

- `keyword_styles`: A Dictionary mapping keywords to their font style (int).
- `member_keyword_styles`: A Dictionary mapping member keywords to their font style (int).
- `ColorRegion` struct now includes an `int font_style` member.

##### New Methods:

- `add_keyword_style(keyword, font_style)`: Assigns a specific font style to a keyword.
- `remove_keyword_style(keyword)`: Removes the font style for a keyword.
- `has_keyword_style(keyword)`: Checks if a keyword has a font style assigned.
- `get_keyword_style(keyword)`: Retrieves the font style for a keyword.
  - `set_keyword_styles(styles)`: Batch sets keyword styles.
- `clear_keyword_styles()`: Clears all keyword styles.
- Equivalent methods for `member_keyword_styles` (e.g., `add_member_keyword_style`, `get_member_keyword_style`).
- `add_color_region(...)`: Now accepts an optional `p_font_style` argument (defaulting to 0) to define the font style for the region.

##### Logic Updates:

- `_get_line_syntax_highlighting`: Updated to return a Dictionary that includes font_style information in the column-indexed data. The return format for each column entry now includes the color and the font style.

#### `scene/gui/text_edit.h` & `scene/gui/text_edit.cpp`
The `TextEdit` control has been updated to render text using the specific font styles provided by the syntax highlighter.

##### Rendering Logic:
- The `TextEdit` now queries the new `font_style` data from the syntax highlighting result.
- When drawing text, it checks the returned `font_style` (Bold, Italic, Bold-Italic) and selects the appropriate font variation (`font_bold`, `font_italic`, etc.) from the theme to render the text segment.

##### API:
- Exposed rendering capability for mixed font styles within the same line.

### 2. GDScript Module Modifications
#### `modules/gdscript/editor/gdscript_highlighter.cpp` & `modules/gdscript/editor/gdscript_highlighter.h`
The GDScript highlighter has been specifically updated to leverage the new core capabilities, primarily to fix and enhance NodePath highlighting.

##### NodePath Highlighting:
- Logic was added (or modified) to identifying NodePath literals (starting with `^` and potentially quoted).
- Explicitly assigns the "NodePath" font style (likely bold or a specific variation) to the entire NodePath literal, including the caret `^` and the path string. This resolves the issue where the string part was falling back to the generic string style.
- Ensures that NodePaths are visually distinct from regular strings.

### 3. Editor Settings & Theme
#### `editor/settings/editor_settings.cpp` & `editor/themes/editor_theme_manager.cpp`
##### Font Style Registration:
- Updates were made to ensure that the Bold and Italic font variations are correctly registered and accessible to the `TextEdit` via the editor theme.
- This guarantees that when the highlighter requests a "Bold" style, the editor actually renders it using the bold font variant.

## Summary of Impact
These changes allow for richer syntax highlighting in the Godot script editor. As a direct result, NodePaths in GDScript are now consistently styled (e.g., in bold) across their entire length, improving code readability and visual consistency. The changes in `CodeHighlighter` and `TextEdit` are generic, meaning other languages or plugins can also leverage this new font styling capability.