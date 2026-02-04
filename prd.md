PRD: Rich Syntax Highlighting & Font Variations in Script Editor

Metadata

Details

Title

Rich Syntax Highlighting & Font Variations

Target Version

Godot 4.x

Status

Draft

Author

Artem Lygin

Related Proposal

syntax_highlighting_fonts.md

1. Executive Summary

This document defines the requirements for extending the Godot Editor's syntax highlighting system. The goal is to move beyond color-only highlighting by introducing font style variations (Bold and Italic). This involves updates to the Editor Settings to allow configuring variant font files, updates to the Theme System to allow assigning styles to syntax tokens, and updates to the SyntaxHighlighter API to support these new styles programmatically.

2. Problem Statement

Currently, the Godot Script Editor (CodeEdit) and the SyntaxHighlighter API restrict syntax highlighting to color changes only.

Visual Hierarchy: Developers cannot use font weight (Bold) or slant (Italic) to distinguish between keywords, types, and documentation, which is a standard feature in modern IDEs (VS Code, JetBrains).

Accessibility: Users with color vision deficiencies have limited ways to distinguish code semantics.

API Limitation: The _get_line_syntax_highlighting method only processes the "color" key in its return dictionary.

3. Goals & User Stories

3.1 Goals

Enable users to provide specific font files for Bold and Italic code variants in Editor Settings.

Enable users to configure default highlighting styles (e.g., "Make Keywords Bold") via the Editor Theme settings.

Enable plugins to programmatically apply Bold/Italic styles via SyntaxHighlighter.

3.2 User Stories

As a User, I want to load a specific "FiraCode-Bold.ttf" for my bold code font so that it matches my main code font perfectly.

As a User, I want to set "Engine Types" (like Vector2, Node) to display in Bold and "Comments" in Italic via the Editor Settings > Text Editor > Highlighting menu.

As a Plugin Developer, I want to write a highlighter that detects specific custom annotations and renders them in Bold without forcing the user to change their global theme.

4. Functional Requirements

4.1 Editor Settings Updates

The Interface > Editor section must be expanded to support font families.

Setting Path

Type

Description

Default Behavior

interface/editor/code_font

String (File)

(Existing) The primary font for code.

System Monospace

interface/editor/code_font_bold

String (File)

(New) The font used when Bold style is requested.

Falls back to code_font if empty.

interface/editor/code_font_italic

String (File)

(New) The font used when Italic style is requested.

Falls back to code_font if empty.

Fallback Logic: If code_font_bold is not provided, the engine should attempt to synthetic embolden code_font or render code_font as is. It must not crash or render missing glyphs.

4.2 Theme & Highlighting Settings

The syntax highlighting editor (Editor Settings > Text Editor > Highlighting) must be updated. Currently, it maps token names to Colors. It must now map token names to a structure containing both Color and Font Style.

New Theme Properties: For every existing color property (e.g., symbol_color), a corresponding font style property must be added.

Existing Property

New Property

Type

Options

Default

.../highlighting/symbol_color

.../highlighting/symbol_font_style

Enum

Default, Bold, Italic

Default

.../highlighting/keyword_color

.../highlighting/keyword_font_style

Enum

Default, Bold, Italic

Default

.../highlighting/comment_color

.../highlighting/comment_font_style

Enum

Default, Bold, Italic

Default

(All other tokens)

..._font_style

Enum

...

Default

UI Requirement:

The "Highlighting" tab in Editor Settings needs a UI overhaul.

Instead of just a Color Picker next to the token name, each row should have:

Color Picker (Existing)

Dropdown Menu: [ Normal | Bold | Italic ]

4.3 SyntaxHighlighter API Updates

The SyntaxHighlighter class must update its dictionary validation to accept font keys.

Method: _get_line_syntax_highlighting(line: int) -> Dictionary

Return Dictionary Structure:

Key: int (Column index)

Value: Dictionary (Attributes)

Supported Attributes:

Key

Type

Description

"color"

Color

(Existing) Text color.

"bold"

bool

(New) If true, use code_font_bold.

"italic"

bool

(New) If true, use code_font_italic.

Note: Using booleans allows for potential future combination (Bold + Italic) if a code_font_bold_italic setting is added later, though mutually exclusive Enums are also acceptable for V1.

5. Technical Implementation Strategy

5.1 Core Rendering (TextEdit / CodeEdit)

Current State: TextEdit likely holds a single Ref<Font> for drawing code.

Required Change: TextEdit must hold references to 3 fonts:

font_normal

font_bold

font_italic

Drawing Loop: In the _draw_line method (or equivalent caching mechanism), when iterating the syntax highlighting map:

Check syntax_map[column].has("bold").

Select the corresponding RID or Ref<Font>.

Pass this font to draw_chars.

5.2 GDScript Syntax Highlighter (GDScriptSyntaxHighlighter)

The built-in highlighter for GDScript must be updated to read the new Theme Properties defined in Section 4.2.

When parsing keywords (e.g., func), it should query EDITOR_GET("text_editor/theme/highlighting/keyword_font_style") and apply the result to the returned dictionary.

6. Migration & Backward Compatibility

Settings: Old editor_settings-4.tres files will lack the new _font_style entries. The system must default these to 0 (Normal) to ensure the editor looks identical to previous versions upon first launch.

Plugins: Existing plugins returning only "color" dicts will continue to work exactly as before (Font Style defaults to Normal). This is a non-breaking change.

7. Future Considerations (Out of Scope for V1)

Bold + Italic: Supporting a 4th font file for simultaneous Bold and Italic.

Font Features: Exposing OpenType features (ligatures) control via syntax highlighting.

Text Decoration: Underline, Strikethrough via syntax highlighting.